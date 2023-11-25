#include "util.h"

#include <stdio.h>
#include <readline/readline.h>
#include <map>
#include <unordered_map>

extern int sockfd;	// Server socket
extern int respfd;	// Chat Socket
extern int listfd; 	// Listening Socket
extern int chatfd;	// Active Socket variable
extern int state;	// State of the client

extern bool DEBUG;

// Command-to-Function map
using command_fn	= void (*)(const char* line);
using command_hash	= unordered_map<string,command_fn>;

command_fn find_command_fn (const string& command);

void server_connect(char* id);
void print_stream();

void fn_info (const char* line);
void fn_wait (const char* line);
void fn_chat (const char* line);
void fn_quit (const char* line);
void fn_null (const char* line);


command_hash cmd_hash {
	{"/list"	, fn_info },
	{"/wait"	, fn_wait },
	{"/connect"	, fn_chat },
	{"/quit"	, fn_quit },
};

// Signal Handlers
void sigint(int signal);
void sigpipe(int signal);

