

#include "util.h"
/*
	A new version of err_sys
*/
int err_sys(const char* x) { 
	cerr << "Error: " << x << "\n";
    return EXIT_FAILURE;
}
/*
	Reliably send a packet using ACKs and timeouts
*/
int tftp_send(int sockfd, char* buffer, uint16_t msg_size){
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	int n;
	for (int i=0;i<3;i++){
		write(sockfd, buffer, msg_size);
		n = read(sockfd, buffer, msg_size);
		if (n<0){
			//cerr <<strerror(errno) << "\n";
		} else{
			unsigned short* opcode = reinterpret_cast<unsigned short*>(buffer);
			if (n>=2 && *opcode == 4){
				cout << "Received ACK.\n";
				return n;	
			}
			if(*opcode == 5) return -1;
		}
	}
	return -1;
}
/*
	Complement function that timeouts and sends ACK when packet is received
*/
int tftp_read(int sockfd, char* buffer, unsigned long buff_size){
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	int n;
	struct sockaddr_in destaddr;
	socklen_t clilen = sizeof(destaddr);
	socklen_t len;
	len = clilen;
	bzero(&destaddr, sizeof(destaddr));
	n = recvfrom(sockfd, buffer, buff_size, 0, (SA*) &destaddr, &len);
	//cout << "Received packet.\n";
	if (n<0){
		//cerr <<strerror(errno) << "\n";
	} else{
		unsigned short* opcode = reinterpret_cast<unsigned short*>(buffer);
		if (n>=2 && *opcode == 3) {
			unsigned short* block = reinterpret_cast<unsigned short*>(buffer+2);
			send_ack(sockfd, *block, (SA*) &destaddr, len);
			return n;
		}else if (*opcode == 5){
			return -1;
		} 
	}
	return -1;
}
int send_ack(int sockfd, unsigned short block, SA* destaddr, socklen_t len){
	//cout << "Sending ACK\n";
	char ack[4];
	unsigned short ack_code = 4;
	memcpy(ack, reinterpret_cast<void*>(&ack_code), 2);
	memcpy(ack+2, reinterpret_cast<void*>(&block), 2);
	sendto(sockfd, ack, 4, 0, destaddr, len);
	return 1;
}

int tftp_sendto(int sockfd, char* buffer, int size, SA* servaddr, socklen_t clilen){
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	int n;
	socklen_t len;
	for (int i = 0; i<3; i++){
		len = clilen;
		sendto(sockfd, buffer, size, 0, servaddr, len);
		n = recv(sockfd, buffer, size, 0);
		if (n<0){
			//cerr <<strerror(errno) << "\n";
		} else{
			unsigned short* opcode = reinterpret_cast<unsigned short*>(buffer);
			if (n>=2 && *opcode == 4) return n;
			if (*opcode == 5) return -1;
		}
	}
	return -1;
}