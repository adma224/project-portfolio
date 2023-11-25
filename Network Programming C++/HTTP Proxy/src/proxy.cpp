#include "proxy.h"

ofstream out_log;
map<string,int> site_map;
int sockfd;

int main(int argc, char** argv){
	if(argc!=3){
		err_sys("Usage: ./proxy <listen-port> <forbidden-sites-file>");
		return EXIT_FAILURE;
	}
	/*
		Get forbidden Sites File
	*/
	site_map = forbidden_sites(argv[2]);
	/*
		Initiate listening socket and bind it
	*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	string port(argv[1]);
	if (!all_of(port.begin(), port.end(),::isdigit))
		return err_sys("Invalid port number");
	servaddr.sin_port = htons(stoi(port,nullptr,10));
	if (bind(sockfd, (SA *)&servaddr,sizeof(servaddr))<0)
		return err_sys("bind() failed");
	if (listen(sockfd, 100)<0)
		return err_sys("listen() failed");
	/*
		Run the TFTP server
	*/
	out_log.open("access.log", ofstream::app);
	cout << "Starting server at port " << argv[1] << "\n";
	run_proxy(sockfd);
}

void run_proxy(int sockfd){
	int respfd;
	signal(SIGINT, interrupt);
	/*
		Wait packets and create threads to handle incoming requests
	*/
	for ( ; ; ) {
		try{
		if ((respfd = accept(sockfd, NULL, NULL))<0)
			throw err_sys("accept() failed");

		thread server_response(handle_connection, respfd);
		server_response.detach();

		} catch (int e){
			err_sys("Listening failed");
		}
	}
}

void handle_connection(int respfd){
	map<string, string> message;
	char serv[] = "80";
	char buffer[MAXLINE+1];
	socklen_t len;
	struct sockaddr_in host_addr;
	socklen_t clilen = sizeof(host_addr);
	/*
		Start proxy until either side sends a disconnect message
	*/
	int n;
	int sockfd;
	/*
		Sets timeout for client socket
	*/
	struct timeval tv;
	tv.tv_sec = 120;
	tv.tv_usec = 0;
	setsockopt(respfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	/*
		System Time
	*/
    time_t current_time;
    current_time = time(NULL);
    if (current_time == ((time_t)-1)){
        err_sys("Failure to obtain the current time");
    }
	try{
		for ( ; ; ){
			/*
				Parse incomming client HTTP request
			*/
			if ((n = read (respfd, buffer, MAXLINE)) <= 0) break;
			message.clear();
			message = parse_request(buffer);
			string host 	= message["host"];
			string request 	= message["request"];
			string line 	= message["line"];
			/*
				Check for possible errors (See proxy.h - HTTP Error Messages)
			*/
			if (message.empty() || (host.length() == 0)){
				write(respfd, br, strlen(br));
				break;
			}
			if (!(request!="GET") && !(request!="CONNECT")){
				write(respfd, fr, strlen(fr));
				break;
			}
			if (site_map[host]){
				write(respfd, fh, strlen(fh));
				break;
			}
			/*
				Get server info and start TCP connection, check if succesful
			*/
			len = clilen;
			bzero(&host_addr, len);
			if (getpeername(sockfd, (SA*) &host_addr, &len)<0){
				if ((sockfd = tcp_connect(message["host"].c_str(), serv))<0){
					write(respfd, nf, strlen(nf));
					break;
				}
			}
			if (getpeername(sockfd, (SA*) &host_addr, &len)<0) throw errno;
			/*
				Get client info for log entry
			*/
			len = clilen;
			bzero(&host_addr, len);
			if (getpeername(respfd, (SA*) &host_addr, &len)<0) throw errno;
			if ((n = write(sockfd, buffer, MAXLINE)) <= 0) break;
			/*
				Receive server response and redirect to client, prints log entry
			*/
			bool first = true;

			while ((n = read (sockfd, buffer, MAXLINE)) > 0) {
				if (first) {
					message = parse_request(buffer);
					//<date-format> <client-ip> <request-line> <host> <http-status-code> <object-bytes>
					ctime(&current_time);
					string time = ctime(&current_time);
					time.pop_back();
					out_log	<< time << " "
							<< inet_ntoa(host_addr.sin_addr) << ":" << to_string(host_addr.sin_port) 
							<< " \"" << line   << "\" " << host  << " "
							<< message["code"] << " "   << message["size"]  << endl << flush;
					first = false;
				}
				
				if ((n = write(respfd, buffer, MAXLINE)) < 0) {
					close(sockfd);
					close(respfd);
				} if (n < MAXLINE) break;
			}
			if (n==0) break;
			/*
				Loops again if connection is still alive
			*/
		}
	} catch(int exception){
		/*
			Unexpected error handling, sends appropiate response to client
		*/
		err_sys("Proxy failed");
		write(respfd, er, strlen(er));
	}
	close(sockfd);
	close(respfd);
}


/*
	Code for tcp_connect() was taken from the course book: 
			UNIX® Network Programming Volume 1, Third Edition, Pg 407
*/
int tcp_connect(const char *host, const char* serv){
	int sockfd, n;
	struct addrinfo hints, *res, *ressave;
	/*
		Only connects to HTTP ports on server
	*/
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((n = getaddrinfo (host, serv, &hints, &res))<0)
		return -1;
	ressave = res;
	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue;
		if (connect (sockfd, res->ai_addr, res->ai_addrlen)==0)
			break;
		close(sockfd);
	} while ((res = res->ai_next) != NULL);
	if (res == NULL)
		err_sys ("tcp_connect error");

	freeaddrinfo(ressave);
	/*
		Sets a timeout of 2 min
	*/
	struct timeval tv;
	tv.tv_sec = 120;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	return(sockfd);
}

map<string,int> forbidden_sites(char* filename){
	ifstream ifs;
	ifs.open (filename);
	map<string,int> sites;
	if(ifs.good()){
		/*
			Traverse through lines in the forbidden sites file and save each site
		*/
		string line;
		while(getline(ifs, line)){
			if (line.size()>0){
				line.erase(remove(line.begin(), line.end(), '\r'), line.end());
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				sites.insert(pair<string,int>(line,1));
			}
		}
	}
	return sites;
}

void interrupt(int signal){
	out_log.close();
	cerr << " Exiting program with signal " << signal << endl;
	shutdown(sockfd, 2);
	close(sockfd);
	exit(0);
}

int err_sys(const char* x) { 
	perror(x);
    return errno;
}

map<string,string> parse_request(char* buffer){
	map<string,string> params;
	string message(buffer);
    istringstream msg(message);
    string line;
    regex reg {R"( )"};
   	while (getline(msg, line) && line.size()!=0) {
   		/*
			Uses regular expressions to break line into words, and removes \r characters
   		*/
   		line.erase(remove(line.begin(), line.end(), '\r'), line.end());
		sregex_token_iterator p(line.begin(), line.end(), reg, -1);
		sregex_token_iterator end;
		while (p != end) {
			if (*p=="HTTP/1.1") {
				if (++p!=end) params.insert(pair<string,string>("code",*p));
				else continue;
				if (++p!=end) {
					string msg = "";
					while(p!=end){
						msg += *p++;
					}
					params.insert(pair<string,string>("msg",msg));
				}
				continue;
			} else{
				if (*p=="GET") {
					params.insert(pair<string,string>("line",line));
					params.insert(pair<string,string>("request",*p));
				}	
				else if (*p=="CONNECT")	{
					params.insert(pair<string,string>("line",line));
					params.insert(pair<string,string>("request",*p));
				}		
				if (*p=="Host:") 			if (++p!=end) params.insert(pair<string,string>("host",*p));
				if (*p=="Content-Length:") 	if (++p!=end) params.insert(pair<string,string>("size",*p));
				*p++;
			}
		}
    }
	return params;
}