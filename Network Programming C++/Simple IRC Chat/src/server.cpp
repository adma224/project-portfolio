#include "server.h"

client_map user_info;
string sys_name = "simple_chat";
bool DEBUG = false;

int main(int argc, char** argv){
	if(argc<2){
		err_sys("Usage: ./server <port-number>");
		return EXIT_FAILURE;
	}
	/*
		Set debugging variable
	*/
	if (argc==3 && strcmp(argv[2],"-@")==0){
		DEBUG = true;
		cout << "DEBUG mode." << endl;
	}
	/*
		Initiate listening socket and bind it
	*/
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	string port(argv[1]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		return err_sys("Invalid port number");
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	if (bind(sockfd, (SA *)&servaddr,sizeof(servaddr))<0)
		return err_sys("bind() failed");
	if (listen(sockfd, 100)<0)
		return err_sys("listen() failed");
	
	cout << "Starting server at port " << argv[1] <<" pid:"<<getpid() <<endl;
	run_server(sockfd);
}

void run_server(int sockfd){
	int respfd;
	/*
		Wait packets and create threads to handle incoming requests
	*/
	try{
		for (;;) {
			
			if ((respfd = accept(sockfd, NULL, NULL))<0)
				throw errno;

			if (DEBUG)
				cout << "Connection received" << endl;

			thread server_response(serve_client, respfd);
			server_response.detach();
		}
	} catch (int e){
		err_sys("Listening failed");
	}
	close(sockfd);
}

void serve_client(int respfd){
	char buffer[MAXLINE+1];
	string id = "";
	for (;;){
		try{
			bzero(buffer, MAXLINE);
			if (read(respfd, buffer, MAXLINE)<=0){
				if (DEBUG) 
					err_sys("What happened");
				fn_quit(respfd, id.c_str(), "/quit");
				break;
			}
			/*
				Split buffer into prefix and suffix
			*/
			wordvec tokens = parse_irc(buffer);

			if (tokens.size()!=2)
				continue;

			string prefix = tokens[0];
			string suffix = tokens[1];

			/*
				Check if message is a command, if not it silently ignores it
			*/
			if (suffix[0] != '/'){
				if (DEBUG)
					cout << "Not a command" << endl;
				continue;
			}
			wordvec words = split(suffix.c_str(), " ");
			string cmd = words[0];
			if (DEBUG)
				cout << "Command received: " << cmd << endl;
			/*
				Map the selected command with a specific function
			*/
			command_fn fn = find_command_fn (cmd);
			fn (respfd, prefix.c_str(), suffix.c_str());
			/*
				Keep track of user ID in case of an abrupt disconnect
			*/
			if (id == "" && cmd == "/nick" && words.size()>1)
				id = words[1];
		} catch(int e){
			if (DEBUG) err_sys("Connection Error");
		}
	}
}

command_fn find_command_fn (const string& cmd) {
	/*
		Use command_hash map to map the selected command to a function
	*/
	const auto result = cmd_hash.find (cmd);
	if (result == cmd_hash.end()) {
		return cmd_err;
	}
	return result->second;
}

void fn_nick (int respfd, const char* prefix, const char* suffix){
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	/*
		Check if ID already exists
	*/
	string id(prefix);
	id.erase(remove(id.begin(), id.end(), ' '), id.end());
	if (id.empty()) return;

	const auto find = user_info.find(id);
	if (find != user_info.end()) {
		string err_nick = "433 " + id + " :Client ID is already taken";
		if (DEBUG)
			cout << "-433 ERR_NICKNAMEINUSE " << id << endl;
		send_irc(respfd, err_nick.c_str());
		close(respfd);
		throw 1;
	}
	/*
		Create new user_info map entry
	*/
	struct data entry;
	entry.socket	= respfd;
	entry.state		= INFO; 
	entry.wait_port	= UNDEFINED;

	user_info[id] = make_shared<struct data>(entry);

	string user = "USER " + id;
	send_irc(respfd, user.c_str());

	if (DEBUG){
		for(auto it = user_info.begin(); it != user_info.end(); ++it){
			cout << it->second->socket << "_\t_" << it->second->state 
				<< "_\t_" << it->second->wait_port <<"_\t_"<<it->first<<"_" <<endl;
		}
	}
}

void fn_info (int respfd, const char* prefix, const char* suffix){
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	/*
		Send all clients with the state WAIT
	*/
	int i = 1;
	string id(prefix);
	id.erase(remove(id.begin(), id.end(), ' '), id.end());
	string pre = "371 " + id + " :";
	string list = pre;
	for(auto it = user_info.begin(); it != user_info.end(); ++it){
		if (it->second->state == WAIT){
			string item = to_string(i++) + ") " + it->first+ "\n";
			if ((list.size()+item.size())>=MAXLINE){
				send_irc(respfd, item.c_str());
				list = pre;
				if (DEBUG)
					cout << "RPL_INFO " << list << endl;
			}
			list += item;
		}
	}
	send_irc(respfd, list.c_str());
	if (DEBUG)
		cout << "RPL_INFO " << list << endl;
	/*
		Send a RPL_ENDOFINFO message
	*/
	string done = "374 " + id + " :";
	if (DEBUG)
		cout << "RPL_ENDOFINFO " << done << endl;
	send_irc(respfd, done.c_str());
}

void fn_wait (int respfd, const char* prefix, const char* suffix){
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	/*
		Check that a valid port number was provided
	*/
	char line[strlen(suffix)];
	strcpy(line, suffix);

	wordvec words = split(suffix, " ");
	string port;
	if (words.size()>1){
		port = words[1];
		if (port =="/drop" || !all_of(port.begin(), port.end(),::isdigit))
			return;
	} else {
		char buffer[MAXLINE+1];
		bzero(buffer, MAXLINE);
		read(respfd, buffer, MAXLINE);
		wordvec tokens = parse_irc(buffer);
		port = tokens[1];
	}
	/*
		Store the port number and change the state to WAIT
	*/
	string id(prefix);
	id.erase(remove(id.begin(), id.end(), ' '), id.end());
	data_ptr ptr	= user_info[id];
	if (!ptr)
		return;
	if (!all_of(port.begin(), port.end(),::isdigit))
		return;
	ptr->state		= WAIT;
	ptr->wait_port	= stoi(port);

	if (DEBUG){
		for(auto it = user_info.begin(); it != user_info.end(); ++it){
			cout << it->second->socket << "_\t_" << it->second->state 
				<< "_\t_" << it->second->wait_port <<"_\t_"<<it->first<<"_" <<endl;
		}
	}
}

void fn_chat (int respfd, const char* prefix, const char* suffix){
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	/*
		Check if connect request is viable
	*/
	wordvec tokens = split(suffix, " ");
	if(tokens.size()<2)
		return;
	string id = tokens[1];
	data_ptr ptr	= user_info[id];
	if (!ptr || ptr->state != WAIT){
		string err_401 = "401 "+id + " :No such nickname";
		send_irc(respfd,err_401.c_str());
		if (DEBUG)
			cout << "-401 ERR_NOSUCHNICK " << id << endl;
		return;
	}
	/*
		Extract and send IP and port of corresponding host
	*/
	int socket = ptr->socket;
	struct sockaddr_in host_addr;
	socklen_t len = sizeof(host_addr);
	getpeername(socket, (SA*) &host_addr, &len);
	string IP	= inet_ntoa(host_addr.sin_addr);
	string port	= to_string(ptr->wait_port);
	string response = ":" + IP + " " + port;
	send_irc(respfd, response.c_str());
	/*
		Change the state of both hosts to CHAT
	*/
	string _id(prefix);
	_id.erase(remove(_id.begin(), _id.end(), ' '), _id.end());
	data_ptr _ptr = user_info[_id];
	_ptr->state = CHAT;
	ptr->state = CHAT;
}

void fn_quit (int respfd, const char* prefix, const char* suffix){
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	/*
		Free resources used in the communication
	*/
	string id(prefix);
	id.erase(remove(id.begin(), id.end(), ' '), id.end());
	user_info.erase (id);
	close(respfd);
	respfd = -1;
}

void fn_drop (int respfd, const char* prefix, const char* suffix){
	/*
		Function called when a client ends a state of WAIT or CHAT
	*/
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	string id(prefix);
	id.erase(remove(id.begin(), id.end(), ' '), id.end());
	data_ptr ptr	= user_info[id];
	if (!ptr)
		return;
	ptr->state		= INFO;
	ptr->wait_port	= UNDEFINED;
	if (DEBUG){
		cout << "After dropping " << prefix << endl;
		for(auto it = user_info.begin(); it != user_info.end(); ++it){
			cout << it->second->socket << "_\t_" << it->second->state 
				<< "_\t_" << it->second->wait_port <<"_\t_"<<it->first<<"_" <<endl;
		}
	}
}

void cmd_err (int respfd, const char* prefix, const char* suffix){
	if (DEBUG)
		cout << "_sfd_" << respfd << "_pre_" << prefix << "_suf_" << suffix << endl;
	/*
		Default function that responds for an unknown command
	*/
	char line[strlen(suffix)];
	strcpy(line, suffix);

	char* token = strtok(line, " ");

	string cmd;

	if(token == nullptr)
		cmd = "";
	else 
		cmd = token;

	string err = "421 " + cmd + " :Command " + cmd + " not recognized";

	if (DEBUG){
		cout << "-421 ERR_UNKNOWNCOMMAND " << cmd << endl;
		send_irc(respfd, err.c_str());
	}	
}

