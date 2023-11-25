#include "shell_client.h"

int main(int argc, char** argv){
	/*
		Check program arguments
	*/
	if (argc < 3)
		return err_sys("Usage: ./myclient <IP-addr> <port-number>");
	string port(argv[2]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		return err_sys("Invalid port number");
	/*
		Set up socket.
	*/
	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[MAXLINE+1];
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		return err_sys("Socket Error");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	string IP(argv[1]);
	//cout << IP << ":" << port << "\n";
	if (inet_pton(AF_INET, &IP[0], &servaddr.sin_addr) <= 0)
		return err_sys("inet_pton error");
	if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr))<0)
		return err_sys("Connect Error");
	/*
		Socket timeout of 3 seconds in case of unavailable server
	*/
	struct timeval tv;
	tv.tv_sec = 120;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	/*
		Handle input commands from cin using readline
	*/
	int n;
	for (;;){
		char* line = readline("client$ ");
        if (send(sockfd, line, strlen(line), 0)<0)
        	return err_sys("send() failed");
        if (strlen(line)==0)
        	continue;
        /*
			Exit command quits in both server and client to avoid problems.
        */
        if (!strcmp(line, "exit"))
        	break;
        bzero(buffer, MAXLINE);
    	while ((n = recv(sockfd, buffer, MAXLINE,0))>=0) {
    		if (n < 0) {
    			return err_sys("Client timeout");
    		}
			buffer[n] = 0;
			cout << buffer;
			if(n < MAXLINE) {
				break;
			}
		}
		cout << endl;
	}
	close(sockfd);
	//cout << "Disconnected from server." << endl;
	return EXIT_SUCCESS;
}