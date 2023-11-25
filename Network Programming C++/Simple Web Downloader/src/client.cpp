#include "client.h"


using namespace std;

int err_sys(const char* x) { 
	perror(x);
	cerr << "Usage: ./myweb <web url> <IP address>:<port#>/<page i.e.index.html> <opt: -h>\n";
    return 1;
}

int main(int argc, char** argv)
{
	int sockfd, n;
	bool h_flag = false;
	struct sockaddr_in servaddr;
	char recvline[MAXLINE+1];
	vector<string> address;

	if (argc < 3) 
		return err_sys("Incorrect number of arguments");

	string split(argv[2]);

	if (argc == 4){
		if (strcmp(argv[3],"-h")==0) h_flag=true;
		else return err_sys("Incorrect option input");
	}

	string message = h_flag ? "HEAD" : "GET" ;

	regex reg {R"(:|/)"};  
	sregex_token_iterator p(split.begin(), split.end(), reg, -1);  
	sregex_token_iterator end; 

	while (p != end) 
		address.push_back(*p++); 
	
	if ((address.size()!=3)||(!all_of(address[1].begin(), address[1].end(),::isdigit)))
		return err_sys("Incorrect address provided");
	
	message = message + " /"+address[2]+" HTTP/1.1\r\nHost: " + argv[1]+"\r\n\r\n";

	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)	
		return err_sys("Socket error");

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(stoi(address[1],nullptr,10)); 

	string IP = address[0];
	if (inet_pton(AF_INET, &IP[0], &servaddr.sin_addr) <= 0)
		return err_sys("inet_pton error");

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr))<0)
		return err_sys("connect error");

	const char * request = message.c_str();

	FILE *out;
	out = h_flag ? stdout : fopen("output.dat","w");

	send(sockfd, request, strlen(request), 0);

	while ((n = read(sockfd, recvline, MAXLINE))>0) {
		recvline[n] = 0;
		if (fputs(recvline, out)==EOF){
			return err_sys("fputs_error");
		}
		if(n<MAXLINE) break;
	}

   	if(!h_flag) 
   		fclose(out);

	if (n<0)
		return err_sys("read_error");

	return 0;
}