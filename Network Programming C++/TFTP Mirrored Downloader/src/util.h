
#include "unp.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <pthread.h>
#include <tuple>

using namespace std;

// Sends multiplexed tftp packets and implements retransmitions

int err_sys(const char* x);

int tftp_send(int sockfd, char* buffer, uint16_t   msg_size);

int tftp_read(int sockfd, char* buffer, unsigned long buff_size);

int send_ack(int sockfd, unsigned short block, SA* destaddr, socklen_t len);

int tftp_sendto(int sockfd, char* buffer, int size, SA* servaddr, socklen_t len);

