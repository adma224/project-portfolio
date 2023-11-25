#include "client.h"

int sockfd, respfd, listfd, chatfd;
int state;
string sys_name = "", peer_name = "";
mutex mtx;

bool DEBUG = false;

int main(int argc, char** argv){
	/*
		Check Arguments
	*/
	if (argc < 4)
		return err_sys("Usage: ./client <IP-addr> <port-number> <id>");

	string port(argv[2]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		return err_sys("Invalid port number");

	if (argc == 5 && (strcmp(argv[4], "-@")==0)){
		cout << "DEBUG mode." << endl;
		DEBUG = true;
	}
	/*
		Set up socket
	*/
	struct sockaddr_in servaddr;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		return err_sys("Socket Error");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	string IP(argv[1]);

	if (DEBUG)
		cout <<"Connecting to " <<IP<<":" << port <<" pid:" << getpid() <<endl;

	if (inet_pton(AF_INET, &IP[0], &servaddr.sin_addr) <= 0)
		return err_sys("inet_pton error");
	if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr))<0)
		return err_sys("Connect Error");

	server_connect(argv[3]);
}

void server_connect(char* id){
	/*
		Override interrupt signals
	*/
	signal(SIGINT, 	sigint);
	signal(SIGPIPE,	sigpipe);
	//rl_callback_handler_install(prompt, (rl_vcpfunc_t*) &my_rlhandler);
	/*
		Send ID to server and check if it is available
	*/
	char buffer[MAXLINE+1];
	bzero(buffer, MAXLINE);
	sys_name = id;
	string nick = string()+":/nick " + id;
	if (DEBUG)
		cout << "-" << nick << endl; 
	send_irc(sockfd,nick.c_str());
	read(sockfd, buffer, MAXLINE);
	if (strstr(buffer, "433")) {
		wordvec tokens = parse_irc(buffer);
		cout << tokens[1] << endl;
		return;
	}
	/*
		Set the state and the out socket
	*/
	chatfd = sockfd;
	state = INFO;
	strcat(id, "> ");
	/*
		Run chat terminal, sending all lines and detecting when a command is typed
	*/
	int flags = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
	for (;;){
		/*
			Sends line to selected chatfd
		*/
		mtx.lock();
		mtx.unlock();
		char* line = readline(id);


		if (!strlen(line))
			continue;
		char message[] = ":";
		strcat(message, line);

		if (strcmp(line,"/quit")==0){
			if (state!=CHAT) send_irc(chatfd, message);
			return;
		}

		if (state == WAIT){
			cout << "Cannot send messages while waiting. Pres Ctrl+C to end wait." << endl;
			continue;
		}
		
		send_irc(chatfd, message);
		string suffix(line);

		if (!suffix.empty() && suffix[0] == '/'){
			/*
				Start a new thread to execute command
			*/
			wordvec words = split(suffix.c_str(), " ");

			if (words[0]=="/quit")
				fn_quit("");

			else if(state == INFO){
				string param = (words.size()>1) ? words[1] : "";
				command_fn fn = find_command_fn (words[0]);
				if (words[0]=="/list")
					fn("");
				else {
					thread cmd(fn, param.c_str());
					cmd.detach();
				}
			}
		} else{
			if (DEBUG)
				cout << "Not a command" << endl;
		}
	}
}

void sigint(int signal){
	/*
		Handle signal differently for each state
	*/
	mtx.lock();
	cout << endl;
	if (DEBUG)
		cout << " SIGINT caught " << signal << " in state " << state<<endl;
	switch(state){
		case INFO:
			fn_quit("");
		break;
		case WAIT:
			shutdown(listfd,SHUT_RDWR);
			close (listfd);
			listfd = -1;
			cout << "Stopped waiting." <<endl;
		break;
		case CHAT:
			shutdown(respfd, SHUT_RDWR);
			close (respfd);
			respfd = -1;
			cout << endl << "Left conversation with " << peer_name <<"."<<endl;
		break;
		default:
			if(DEBUG){
				cout << "Unknown state when calling /quit" << endl;
				exit(EXIT_FAILURE);
			}
	}
	/*
		Return to info state
	*/
	chatfd = sockfd;
	state = INFO;
	cout << sys_name << "> ";
	peer_name="";
	mtx.unlock();
	send_irc(sockfd, ":/drop");
}

void sigpipe(int signal){
	/*
		SIPIPE signal handler
	*/
    if (DEBUG)
    	cout << " SIGPIPE caught " << signal<<endl;
    cout << "Server disconnected abruptly." << endl;
    if (chatfd == sockfd)
    	fn_quit("");
}

command_fn find_command_fn (const string& cmd) {
	/*
		Check cmd_hash for corresponding irc function
	*/
	const auto result = cmd_hash.find (cmd);
	if (result == cmd_hash.end()) {
		return fn_null;
	}
	return result->second;
}

void fn_wait(const char* line){
	string wait_port = (strlen(line)) ? line : "0";
	try{
		/*
			Establish a listening socket
		*/
		state = WAIT;
		if ((listfd = socket(AF_INET, SOCK_STREAM, 0))<0){
			err_sys("socket() failed");
			throw 0;
		}
		struct sockaddr_in selfaddr;
		bzero(&selfaddr, sizeof(selfaddr));
		selfaddr.sin_family = AF_INET;
		selfaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (!all_of(wait_port.begin(), wait_port.end(),::isdigit)){
			err_sys("Invalid port number");
			throw 1;
		}
		selfaddr.sin_port = htons(stoi(wait_port,nullptr,10));
		if (bind(listfd, (SA *)&selfaddr,sizeof(selfaddr))<0){
			err_sys("bind() failed");
			throw 2;
		}
		/*
			Send port number to the server if selected at random
		*/
		if (wait_port == "0"){
			struct sockaddr_in randaddr;
			socklen_t rand_size = sizeof(randaddr);
			getsockname(listfd, (SA*) &randaddr, &rand_size);
			wait_port = to_string(ntohs(randaddr.sin_port));
			string msg = ":" + wait_port;
			send_irc(sockfd, msg.c_str());
		}
		if (listen(listfd, 100)<0){
			err_sys("listen() failed");
			throw 3;
		}
		if (DEBUG)
			cout << "Opening port " << wait_port << endl;
		if ((respfd = accept(listfd, NULL, NULL))<0){
			err_sys("Accept Failed");
			throw 4;
		}
		/*
			Receive peer nickname
		*/
		char buffer[MAXLINE+1];
		read(respfd, buffer, MAXLINE);

		wordvec tokens = parse_irc(buffer);
		string id = tokens[0];
		id.erase(remove(id.begin(), id.end(), ' '), id.end());
		/*
			Close listening socket
		*/
		peer_name = id;
		shutdown(listfd,SHUT_RDWR);
		close(listfd);
		chatfd = respfd;
		/*
			Create a concurrent thread that prints incoming messages
		*/
		thread irc_chat(print_stream);
		irc_chat.detach();
		/*
			Set CHAT state
		*/
		mtx.lock();
		cout <<endl <<"Connected to " << peer_name << "." << endl;
		fflush(stdout); 
		cout << sys_name << "> ";
		mtx.unlock();
		state = CHAT;
	} catch (int e){
		/*
			Catch an error in the connection and revert to INFO state
		*/
		state = INFO;
		if (e>1){
			shutdown(listfd,SHUT_RDWR);
			close(listfd);
		}
		if (DEBUG){
			cout << endl << "Thrown exception " << e << " when waiting."<< endl;
		}
		mtx.lock();
		cout << sys_name << "> ";
		fflush(stdout); 
		send_irc(sockfd, ":/drop");
		mtx.unlock();
	}
}

void fn_chat(const char* line){
	/*
		Request Server for IP address and check for error messages
	*/
	char buffer[MAXLINE+1];
	read(sockfd,buffer,MAXLINE);
	wordvec tokens = parse_irc(buffer);
	if (DEBUG)
		cout << buffer << endl;
	if (strstr(buffer, "401")) {
		wordvec tokens = parse_irc(buffer);
		mtx.lock();
		cout << tokens[1] << endl << sys_name << "> ";
		fflush(stdout); 
		mtx.unlock();
		return;
	}
	wordvec words = split(tokens[1].c_str()," ");
	if (words.size()!=2)
		return;
	try{
		/*
			Connect to peer
		*/
		string IP = words[0];
		string port = words[1];

		struct sockaddr_in peeraddr;
		if((respfd = socket(AF_INET, SOCK_STREAM, 0))<0){
			err_sys("Socket Error");
			throw 0;
		}
		bzero(&peeraddr, sizeof(peeraddr));
		peeraddr.sin_family = AF_INET;
		peeraddr.sin_port = htons(stoi(port,nullptr,10));

		if (DEBUG)
			cout <<"Connecting to " <<IP<<":" << port  <<endl;

		if (inet_pton(AF_INET, &IP[0], &peeraddr.sin_addr) <= 0){
			err_sys("inet_pton error");
			throw 1;
		}
		if (connect(respfd, (SA*) &peeraddr, sizeof(peeraddr))<0){
			err_sys("Connect Error");
			throw 2;
		}
		/*
			Send user ID to peer
		*/
		send_irc(respfd, ":");
		/*
			Set to CHAT state and begin a listening thread for printing messages
		*/
		state=CHAT;
		chatfd = respfd;
		peer_name = line;

		thread irc_chat(print_stream);
		irc_chat.detach();
		mtx.lock();
		cout <<endl <<"Connected to " << peer_name << "." << endl;
		mtx.unlock();
		cout << sys_name << "> ";
		fflush(stdout); 
	} catch(int e){
		/*
			Catch messages during connection
		*/
		state = INFO;
		shutdown(respfd,SHUT_RDWR);
		close(respfd);
		if (DEBUG){
			cout << endl << "Thrown exception " << e << " when connecting."<< endl;
			cout << sys_name << "> ";
			fflush(stdout); 	
		}
		send_irc(sockfd, ":/drop");
		peer_name = "";
	}
}

void fn_info (const char* line){
	/*
		Parse incomming INFO response messages
	*/
	if (DEBUG)
		cout << "fn_info called "<< line <<endl;	
	char buffer[MAXLINE+1];
	bzero(buffer, MAXLINE);
	while(read(sockfd, buffer, MAXLINE) > 0){
	   	wordvec tokens = split(buffer, ":");
	   	if (tokens.size()>2)
	   		cout << tokens[2];
	   	else break;
	   	
		if (DEBUG)
			cout << buffer << endl;
	   	if (strstr(buffer, "374")){
			break;
	   	}
	   	bzero(buffer, MAXLINE);
	}
}

void fn_quit(const char* line){
	/*
		Close all existing connections and free all space
	*/
	if (DEBUG){
		cout << "/quit called on state " << state << line<< endl ;
	}
	if (state == CHAT){
		shutdown(respfd, SHUT_RDWR);
		close(respfd);
	}
	else if (state == WAIT){
		shutdown(listfd, SHUT_RDWR);
		close(listfd);
	}
	shutdown(sockfd,SHUT_RDWR);
	close(sockfd);
	exit(EXIT_SUCCESS);
}

void fn_null (const char* line){
	/*
		Default unknown command
	*/
	mtx.lock();
	cout << endl<<"Command not recognized. "<<line<< endl;
	cout << sys_name << "> ";
	fflush(stdout);
	mtx.unlock();
}

void print_stream(){
	/*
		Print all incoming messages from respfd (peer socket)
	*/
	int n;
	char buffer[MAXLINE];
	if (DEBUG)
		cout << "Listening thorugh respfd " << respfd << endl;

	int flags = 1;
	setsockopt(respfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
	while((n=read(respfd, buffer, MAXLINE))>0){
		if (peer_name=="")
			break;
		mtx.lock();
		wordvec tokens = parse_irc(buffer);
		cout << endl << peer_name << ": "<< tokens[1] << endl;
		cout << sys_name << "> ";
		fflush(stdout); 
		bzero(buffer, MAXLINE);
		mtx.unlock();
	}
	if (peer_name=="") 
		return;
	/*
		Returns to INFO state
	*/
	mtx.lock();
	respfd = -1;
	cout << endl << "Left conversation with " << peer_name <<"."<<endl;
	peer_name = "";
	shutdown(respfd, SHUT_RDWR);
	close (respfd);
	respfd = -1;
	chatfd = sockfd;
	state = INFO;
	cout << sys_name << "> ";
	fflush(stdout); 
	mtx.unlock();
	send_irc(sockfd, ":/drop");
}
