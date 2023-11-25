#include "unp.h"
#include "util.h"
#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>

using namespace std;

// Starts the infinite loop for the server
void run_server(int sockfd);

// Handles the request in a new thread according to the new message
void handle_request(char buffer[MAXLINE+1], string IP, unsigned long port);

// Sends the requested chunk to the client
int send_file_chunk(FILE* infile, char* message, int sockfd);

int send_size(FILE* infile, int socket);

int dg_err(unsigned short err_code, int sockfd);


