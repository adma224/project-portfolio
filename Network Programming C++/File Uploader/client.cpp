#include "client.h"

int main(int argc, char** argv){

	if(argc<4) 
		err_sys("Usage: ./myclient <IP-address> <port-num> <files...>");
	string port(argv[2]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		err_sys("Invalid port number");

	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[MAXLINE+1];
	buffer[0] = 0;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		err_sys("Socket Error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	string IP(argv[1]);

	cout << IP << ":" << port << "\n";

	if (inet_pton(AF_INET, &IP[0], &servaddr.sin_addr) <= 0)
		return err_sys("inet_pton error");

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr))<0)
		return err_sys("connect error");
	int byte;
	for(int i = 3; i<argc; i++){
		FILE* infile = fopen(argv[i],"r");
		fopen(argv[i],"r");
		cout << "Uploading " << argv[i] << "\n";
		if(infile){
			int length = strlen(argv[i]);
			strncpy(buffer, argv[i], length);
			buffer[length] = '\r';
			buffer[length+1] = '\n';
			write(sockfd, argv[i]+"\r\n", strlen(argv[i]));
			int size = fseek(infile, 0, SEEK_END);
			fseek(infile, 0, SEEK_SET);
		 	do{
		 		byte = fread(buffer, 1, MAXLINE, infile);
		 		cout << "Sending\n" << buffer << "\n";
		 		cout << "Byte: " << byte << "\n";
		 		buffer[byte] = 0;
			  	if (write(sockfd, buffer, MAXLINE) == -1)
			    	err_sys("Error sending file");
			  	bzero(buffer, MAXLINE);
			} while(byte < size);
			
		} else err_sys("File not found");
	}
	return EXIT_SUCCESS;
}

int err_sys(const char* x) { 
	cerr << x << "\n";
    return EXIT_FAILURE;
}