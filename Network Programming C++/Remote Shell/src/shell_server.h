#include "unp.h"
#include "util.h"

using namespace std;
/*
	SIGINT exception handling to ensure proper termination of children processes.
*/
void interrupt(int signal);