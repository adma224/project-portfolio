// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $

#include <cstdlib>
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <regex>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

#define DISPLAY (key.empty()&&value.empty())
#define ERASE (!key.empty()&&value.empty())
#define SEARCH (key.empty()&&!value.empty())
#define INSERT (!key.empty()&&!value.empty())

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   str_str_map test;

   cout << string(argv[0]) << endl;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};


   int count =0;
   istream* in = &cin;
   ifstream inFile;
   regex inFile_regex {R"(\S+$)"};
   smatch inFile_matches;
   string fileName (argv[argc-1]);
   if(argc>1&&regex_search(fileName,inFile_matches,inFile_regex)){
      inFile.open(inFile_matches[0]);
      if(!inFile){
         complain() << "No such file or directory" << endl;
         exit(1);
      }
      in = &inFile;
   }
   for (;;) {
      string line;
      getline (*in, line);
      if (in->eof()) break;
      cout <<"-: "<< ++count <<": " << line << endl;
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         continue;
      }
      if (regex_search (line, result, key_value_regex)) {
         string key = result[1];
         string value = result[2];
         
         if(DISPLAY){
            for (str_str_map::iterator itor = test.begin();
               itor != test.end(); ++itor) {
               cout << *itor << endl;
            }
         } else if(ERASE){
            str_str_map::iterator itor = test.find(key);
            if(itor!=test.end()){
               test.erase(itor);
            }
         } else if(INSERT){
            str_str_pair pair (key,value);
            cout << pair << endl;
            test.insert(pair);
         } else if(SEARCH){
            str_str_map::iterator itor = test.begin();
            while(itor != test.end()){
               if(itor->second==result[2]) cout<< *itor<< endl;
               ++itor;
            }
         }
      }else if (regex_search (line, result, trimmed_regex)) {
         str_str_map::iterator itor;
         itor = test.find(result[1]);
         if(itor!=test.end()){
            cout << *itor << endl;
         } else{
            complain() << result[1] << ": key not found" << endl;
         }
      }else {
         assert (false and "This can not happen.");
      }
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
