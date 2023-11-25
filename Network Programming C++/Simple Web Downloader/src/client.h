#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include 	<arpa/inet.h>
#include 	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include 	<iostream>
#include  	<vector>
#include 	<regex>

#define	MAXLINE		1024	/* max text line length */
#define	SA	struct sockaddr