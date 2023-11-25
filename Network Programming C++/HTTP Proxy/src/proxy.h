#include "unp.h" 
#include <cerrno>
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <pthread.h>
#include <map>
#include <regex>
#include <time.h>

using namespace std;

// Functions
void run_proxy(int sockfd);										// runs proxy until interrupt							
void handle_connection(int respfd);								// handles http request
int tcp_connect(const char *host, const char* serv);			// DNS lookup
map<string,int> forbidden_sites(char* filename);				// Extracts forbidden sites
map<string,string> parse_request(char* message);				// Parses HTTP messages

// System Error Handling
void interrupt(int signal);										// Interrupt handling
int err_sys(const char* x);										// Default System Error

// HTTP Error Messages
const char br[] = "HTTP/1.1 400 Bad Request \r\n\r\n";			 // bad request
const char fh[] = "HTTP/1.1 403 Forbidden Url\r\n\r\n";			 // forbidden host
const char nf[] = "HTTP/1.1 404 Not Found \r\n\r\n"; 			 // host not found
const char fr[] = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";	 // forbidden request
const char er[] = "HTTP/1.1 500 Internal Server Error \r\n\r\n"; // proxy error

// Log File
extern ofstream out_log;

// Forbidden Sites
extern map<string,int> site_map;
