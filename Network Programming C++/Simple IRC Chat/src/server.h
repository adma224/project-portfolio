#include "util.h" 

#include <map>
#include <unordered_map>

// Client information struct

struct data {
   int socket;
   int state;
   int wait_port;
};

using data_ptr		= shared_ptr<struct data>;


// Command-to-Function map
using command_fn	= void (*)(int respfd, const char* prefix, const char* suffix);
using command_hash	= unordered_map<string,command_fn>;
using client_map	= map<string,data_ptr>;

extern bool DEBUG;

extern client_map user_info;

command_fn find_command_fn (const string& command);

void fn_nick (int respfd, const char* prefix, const char* suffix);
void fn_info (int respfd, const char* prefix, const char* suffix);
void fn_wait (int respfd, const char* prefix, const char* suffix);
void fn_chat (int respfd, const char* prefix, const char* suffix);
void fn_quit (int respfd, const char* prefix, const char* suffix);
void fn_drop (int respfd, const char* prefix, const char* suffix);
void cmd_err (int respfd, const char* prefix, const char* suffix);

command_hash cmd_hash {
	{"/nick"	, fn_nick},
	{"/list"	, fn_info},
	{"/connect" , fn_chat},
	{"/wait"	, fn_wait},
	{"/quit"	, fn_quit},
	{"/drop"	, fn_drop},
};


// Server primary functions
void run_server		(int sockfd);
void serve_client	(int respfd);












