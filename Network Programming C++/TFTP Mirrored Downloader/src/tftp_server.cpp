#include "tftp_server.h"

mutex mtx;

int main(int argc, char** argv){
	/*
		Initiate listening socket and bind it
	*/
	if(argc!=2) err_sys("Usage: ./myserver <port-number>");
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	string port(argv[1]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		err_sys("Invalid port number");
	cout << "Starting server at port " << argv[1] << "\n";
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	bind(sockfd, (SA *)&servaddr,sizeof(servaddr));
	/*
		Run the TFTP server
	*/
	run_server(sockfd);
	
}

void run_server(int sockfd){
	socklen_t len;
	struct sockaddr_in source_addr;
	socklen_t clilen = sizeof(source_addr);
	/*
		Wait packets and create threads to handle incoming requests
	*/
	for ( ; ; ) {
		try{
		len = clilen;
		bzero(&source_addr, sizeof(source_addr));
		char buffer[MAXLINE+1];
		recvfrom(sockfd, buffer, MAXLINE, 0, (SA*) &source_addr, &len);
		string IP = inet_ntoa(source_addr.sin_addr);
		unsigned long port = source_addr.sin_port;
		cout << "Packet received from " << IP << ":" << to_string(port) << "\n";
		//handle_request(buffer, IP, port);
		thread server_response(handle_request, buffer, IP, port);
		server_response.detach();
		} catch (int e){
			err_sys("Server had to abort");
		}
	}
}

void handle_request(char buffer[MAXLINE+1], string IP, unsigned long port){
	/*
		Use a mutex to avoid data races in buffer
	*/
	mtx.lock();
	char mesg[MAXLINE+1];
	memcpy(mesg, buffer, MAXLINE);
	mtx.unlock();	
	/* 
		Open new port and connect through UDP with client
	*/
	struct sockaddr_in servaddr, clianddr;
	bzero(&servaddr,sizeof(servaddr));
	bzero(&clianddr, sizeof(clianddr));
	servaddr.sin_port = htons(SERV_PORT);
	clianddr.sin_port = port;
	servaddr.sin_family = AF_INET;
	clianddr.sin_family = AF_INET;
	clianddr.sin_port = port;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, &IP[0], &clianddr.sin_addr);
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	bind(sockfd, (SA*) &servaddr, sizeof(servaddr));
	send_ack(sockfd, 0, (SA*) &clianddr, sizeof(clianddr));
	unsigned short* opcode = reinterpret_cast<unsigned short*>(mesg);
	connect(sockfd, (SA*) &clianddr, sizeof(clianddr));

	if (*opcode == 1){
		/*
			Handle RRQ request
		*/
		int path_length = strlen(mesg+2);
		char filename[path_length+1];
		strncpy(filename, mesg+2, path_length+1);
		//cout << "RRQ: " << filename << "\n";
		FILE* infile;
		infile = fopen(filename,"r");
		if (infile){
			char* mode = mesg+path_length+3;
			if (mode[0]=='F'){
				if ((send_size(infile, sockfd))<0){
					dg_err(2, sockfd);
				}
			} else if(mode[0]=='C'){
				if (send_file_chunk(infile,mode,sockfd)<0){
					dg_err(2, sockfd);
				}
			} else dg_err(0, sockfd);
			fclose(infile);
		} else dg_err(1, sockfd);
	} else dg_err(0, sockfd);
	close(sockfd);
}

int send_file_chunk(FILE* infile, char* message, int sockfd){
	/*
		Determine offset and chunk size
	*/

	unsigned short* offset    = reinterpret_cast<unsigned short*>(message+1);
	unsigned long* chunk_size = reinterpret_cast<unsigned long*>(message+3);
	//cout << "Sending chunk " << *offset << " : "<<*chunk_size<< " bytes\n";
	/*
		Prepare message
	*/
	char mesg[516];
	unsigned short opcode = 3;
	unsigned short block = 0;

	/*
		Get starting byte and ending byte
	*/
	fseek(infile, 0, SEEK_END);
	int size = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	int byte = (*offset)*(*chunk_size+1);
	fseek(infile, byte, SEEK_CUR);
	char* buffer = mesg+4;
	int to_read = *chunk_size+1;
	int limit = (int)(*offset+1)*(*chunk_size+1);
	//int slice =0;
	do{
		memcpy(mesg,reinterpret_cast<void*>(&opcode),2);
		memcpy(mesg+2,reinterpret_cast<void*>(&block),2); 
		min(to_read, 512);
		fread(buffer, min(to_read, 512), 1, infile);
		byte = ftell(infile);
		if (tftp_send(sockfd, mesg, min(to_read, 512)+4)<0) return -1;
		//cout << "Chunk: "<< *offset << ". Slice: " << slice++ << ". Left to Read:" << to_read << "B\n";
		to_read = min(limit-byte, size-byte);
	} while(to_read > 0);

	return 0;
}

int send_size(FILE* infile, int sockfd){

	
	/*
		Prepare tftp DATA message
	*/
	fseek(infile, 0, SEEK_END);
	unsigned long file_size = ftell(infile);
	char buffer[sizeof(unsigned short)*2+sizeof(unsigned long)];
	unsigned short opcode_out = 3;
	unsigned short block_num  = 1;
	memcpy(buffer,   reinterpret_cast<void*>(&opcode_out),sizeof(unsigned short));
	memcpy(buffer+2, reinterpret_cast<void*>(&block_num), sizeof(unsigned short));
	memcpy(buffer+4, reinterpret_cast<void*>(&file_size), sizeof(unsigned long));
	int buff_size = sizeof(unsigned short)*2+sizeof(unsigned long);
	//cout << "Filesize resquest: "<< file_size<<"\n";
	tftp_send(sockfd, buffer, buff_size);
	
	return 1;
}

int dg_err(unsigned short err_code, int sockfd){
	/*
		Prepare the tftp packet with the error code
	*/
	//cerr << "TFTP Error: " << to_string(err_code) << "\n";
	char buffer[MAXLINE];
	unsigned short error = 5;
	memcpy(buffer, reinterpret_cast<void*>(error), 2);
	memcpy(buffer+2, reinterpret_cast<void*>(err_code), 2);
	write(sockfd, buffer, 4);
	return 1;
}