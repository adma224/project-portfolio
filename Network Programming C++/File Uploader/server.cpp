#include "server.h"

int main(int argc, char** argv){
 	cout << "Starting server\n";
	if (argc != 2)
		err_sys("Usage: ./myserver <port-number>");
	string port(argv[1]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		err_sys("Invalid port number");

	int sockfd, respfd, n;
	struct sockaddr_in servaddr, clianddr;
	char buffer[MAXLINE+1];

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		err_sys("Socket Error");

	cout << "Socket num" << to_string(sockfd) << "\n";
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(stoi(port,nullptr,10));

	char *ip = inet_ntoa(servaddr.sin_addr);
	cout << "Starting server at "<< ip <<":" <<port<<"...\n";

	if((bind(sockfd, (SA*) &servaddr, sizeof(servaddr)))<0)
		err_sys("Bind Error");
	if((listen(sockfd,10))<0)
		err_sys("Listen Error");
	for (;;){
		respfd = accept(sockfd, (SA*) NULL, NULL);
		cout << "Packet arrived\n";
		string line;
		getline(respfd, line);
		ifstream ifs;
		ifs.open (line);
		while ((n = read(respfd, buffer, MAXLINE))>0) {
			buffer[n] = 0;
			out << buffer;
			if(n<MAXLINE) break;
		}
	}
	return EXIT_SUCCESS;
}

int err_sys(const char* x) { 
	cerr << x << "\n";
    return EXIT_FAILURE;
}
