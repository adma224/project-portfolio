#include "unp.h"

#include <cerrno>
#include <cstdio>
#include <string>

#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <pthread.h>
#include <vector>
#include <regex>
#include <mutex>

#include <iostream>

using namespace std;

#define INFO 0
#define WAIT 1
#define CHAT 2

#define UNDEFINED 0

using wordvec = vector<string>;

int err_sys(const char* x);

void send_irc(int sockfd, const char* msg);

wordvec parse_irc(const char* buffer);

wordvec split(const char* str, const char* delim);

