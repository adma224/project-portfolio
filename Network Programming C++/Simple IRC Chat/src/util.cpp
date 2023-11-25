#include "util.h"

extern string sys_name;
extern bool DEBUG;

int err_sys(const char* x) {
	cerr << endl;
	perror(x);
	return errno;
}

void send_irc(int sockfd, const char* msg){
	string response = ":"+sys_name+" "+msg;
	write(sockfd, response.c_str(), response.size());
}

wordvec parse_irc(const char* buffer){
	wordvec tokens;
	if (buffer[0]==':'){
		string line(&buffer[1]);

		size_t find = line.find_first_of(':');

		string prefix = (find!=string::npos) ? 
			line.substr(0,find) : line;
		string suffix = (find!=string::npos) ?
			line.substr(find+1) : "";

		tokens.push_back(prefix);
		tokens.push_back(suffix);
	}
	return tokens;
}
	
wordvec split(const char* str, const char* delim){
	string line(str);
    wordvec words;

    regex reg(delim);
    sregex_token_iterator 
    	begin(line.begin(), line.end(), reg, -1), end;

    copy(begin, end, back_inserter(words));

    return words;
}
