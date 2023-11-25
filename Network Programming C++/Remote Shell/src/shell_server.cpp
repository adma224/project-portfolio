#include "shell_server.h"

int main(int argc, char** argv){
	/*
		Check program arguments.
	*/
	if (argc < 2)
		return err_sys("Usage: ./myserver <port-number>");
	string port(argv[1]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		return err_sys("Invalid port number");
	/*
		Set up listening socket.
	*/
	int sockfd, respfd;
	struct sockaddr_in servaddr;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		return err_sys("Socket Error");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	if((bind(sockfd, (SA*) &servaddr, sizeof(servaddr)))<0)
		return err_sys("Bind Error");
	if((listen(sockfd,10))<0)
		return err_sys("Listen Error");
	cout << "Server online at port=" << port <<". " <<  endl;
	char buffer[MAXLINE+1];
	pid_t childpid;
	/*
		Set up SIGINT exception handling.
	*/
	signal(SIGINT, interrupt);

	for (;;){
		struct sockaddr_in clianddr;
		socklen_t addr_size;
		int n;
		if ((respfd = accept(sockfd, (struct sockaddr*)&clianddr, &addr_size))<0)
			return err_sys("accept() failed");
		//cout << "Connection accepted from "
				//<< inet_ntoa(clianddr.sin_addr)<< ":" << ntohs(clianddr.sin_port) << endl;
		/*
			Fork process with new respfd socket
		*/
		if((childpid = fork()) == 0){
			/*
				Set up a session timeout of 2 mins.
			*/
			close(sockfd);
			struct timeval tv;
			tv.tv_sec = 300;
			tv.tv_usec = 0;
			setsockopt(respfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
			for(;;){
				if ((n = recv(respfd, buffer, MAXLINE, 0))<0){
					cerr << "Server Timeout: 2 min have passed." << endl;
					break;
				}
				string command(buffer);
				if (n<0){
					err_sys("recv() failed");
					break;
				}
				/*
					Handle socket flushing from an abruptly closed client socket.
				*/
				if (!command.size()){
					cerr << "Client disconnected abruptly." << endl;
					break;
				}
				//cout << "Client: " << command << endl;
				if(command == "exit"){
					break;
				}else{
					/*
						Use popen to communicate with system through input commands.
					*/
					command += " 2>&1";
					//cout << command;
					FILE* sysin;
					if (!(sysin = popen(command.c_str(),"r"))){
						err_sys("popen() failed");
						break;
					}
					int first = 1;
					
					while (fgets(buffer, MAXLINE, sysin) || first){
						first = 0;
						cout << buffer;
				        if (send(respfd, buffer, strlen(buffer), 0)<0){
				        	err_sys("send() failed");
				        	break;
				        }
				        bzero(buffer, MAXLINE);
				    }
				    pclose(sysin);
				}
				bzero(buffer, MAXLINE);
			}
			close(respfd);
			//cout << "Disconnected from " << inet_ntoa(clianddr.sin_addr) << ':'<< ntohs(clianddr.sin_port) << " at process "<<getpid()<< endl;
			return EXIT_SUCCESS;
		} else {
			/*
				Debugging setup to keep track of running or terminated children.
			*/
			pid_t pid;
			int stat;
			while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
				cout << "Terminated process id:" << pid << endl;
		}
	}
	close (respfd);
	return EXIT_SUCCESS;
}
/*
	SIGINT exception handling now waits for children to also quit with the same signal before exiting.
*/
void interrupt(int signal){
	wait(NULL);
	cerr << "Received signal="<<signal << ". Exiting Process ID=" << getpid() <<"." <<endl;
	exit(0);
}