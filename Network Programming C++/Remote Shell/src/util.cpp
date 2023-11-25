#include "util.h"

int err_sys(const char* x) { 
	perror(x);
    return errno;
}