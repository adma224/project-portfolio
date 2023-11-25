// $Id: cix.cpp,v 1.9 2019-04-05 15:04:28-07 - - $

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"
#include "util.h"

logstream outlog (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"rm"  , cix_command::RM  },
   {"put"  , cix_command::PUT  },
   {"get"  , cix_command::GET  },
};

static const char help[] = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cix_help() {
   cout << help;
}
using wordvec = vector<string>;
wordvec split (const string& line, const string& delimiters) {
   wordvec words;
   size_t end = 0;

   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output wordvec.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   return words;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   outlog << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   outlog << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      outlog << "sent LS, server did not return LSOUT" << endl;
      outlog << "server returned " << header << endl;
   }else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      outlog << "received " << host_nbytes << " bytes" << endl;
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   }
}


void cix_put(client_socket& server, string filename){
   cix_header header;
   strcpy(header.filename,filename.c_str());
   ifstream i_file(filename,ios::binary);
   if(i_file.good()){
      header.command = cix_command::PUT;
      i_file.seekg(0,i_file.end);
      int size = i_file.tellg();
      i_file.seekg(0,i_file.beg); 
      auto buffer = make_unique<char[]> (ntohl(header.nbytes) + 1);
      header.nbytes=size;
      i_file.read(buffer.get(),size);
      outlog << "sending header " << header << endl;
      send_packet(server,&header,sizeof header);
      if(size>0){
         outlog << "sending content for " << filename << endl;
         send_packet(server,buffer.get(),size);
      }
      recv_packet(server,&header,sizeof header);
      outlog << "received header " << header << endl;
      i_file.close();
   } else{
         outlog <<filename<< ": file not found" << endl;
   }
   switch(header.command){
      case cix_command::ACK:
         outlog <<"sent "<< ntohl(header.nbytes)<<" bytes"<<endl;
         break;
      case cix_command::NAK:
         outlog << "sent PUT, server did not return ACK" << endl;
         outlog << "server returned " << header << endl;
         break;
      case cix_command::ERROR:
         outlog << "invalid header: " << header << endl;
         break;
      default:
         break;
   }
}

void cix_get(client_socket& server, string filename){
   cix_header header;
   header.command = cix_command::GET;
   memset (header.filename, 0, FILENAME_SIZE);
   strcpy(header.filename,filename.c_str());
   outlog << "sending header " << header << endl;
   send_packet(server,&header,sizeof header);
   recv_packet(server,&header,sizeof header);
   outlog << "received header " << header << endl;
   switch(header.command){
      default:
         break;
      case cix_command::FILEOUT:
         {
         ofstream o_file(filename,ios::binary);
         auto buffer = make_unique<char[]> (ntohl(header.nbytes) + 1);
         outlog <<"received "<<ntohl(header.nbytes)<<" bytes"<<endl;
         recv_packet(server,buffer.get(),header.nbytes);
         o_file.write(buffer.get(),header.nbytes);
         o_file.close();
         }
         break;
      case cix_command::NAK:
         outlog << "sent GET, server did not return FILEOUT" << endl;
         outlog << "server returned " << header << endl;
         break;
      case cix_command::ERROR:
         outlog << "invalid header: " << header << endl;
         break;

   }
}

void cix_rm(client_socket& server, string filename){
   cix_header header;
   header.command = cix_command::RM;
   header.nbytes = 0;
   memset (header.filename, 0, FILENAME_SIZE);
   strcpy(header.filename,filename.c_str());
   outlog << "sending header " << header << endl;
   send_packet(server,&header,sizeof header);
   recv_packet(server,&header,sizeof header);
   outlog << "received header " << header << endl;
   switch(header.command){
      case cix_command::ACK:
         outlog << "file " << filename << " removed" << endl;
         break;
      case cix_command::NAK:
         outlog << "sent RM, server did not return ACK" << endl;
         outlog << "server returned " << header << endl;
         break;
      case cix_command::ERROR:
         outlog << "invalid header: " << header << endl;
         break;
      default:
         break;
   }
}

void usage() {
   cerr << "Usage: " << outlog.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   outlog.execname (basename (argv[0]));
   outlog << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   outlog << to_string (hostinfo()) << endl;
   try {
      outlog << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      outlog << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         if(line=="") continue;
         outlog << "command " << line << endl;
         wordvec words = split (line, " \t");
         const auto& itor = command_map.find (words[0]);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               break;
            case cix_command::LS:
               cix_ls (server);
               break;
            case cix_command::GET:
               if(words.size()==2) cix_get (server,words[1]);
               break;
            case cix_command::PUT:
               if(words.size()==2) cix_put (server,words[1]);
               break;
            case cix_command::RM:
               if(words.size()==2) cix_rm (server,words[1]);
               break;

            default:
               outlog << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      outlog << error.what() << endl;
   }catch (cix_exit& error) {
      outlog << "caught cix_exit" << endl;
   }
   outlog << "finishing" << endl;
   return 0;
}

