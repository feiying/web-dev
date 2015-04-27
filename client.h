#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE	80
#define	SERV_PORT	8000

struct command_line{
	char serveraddr[MAXLINE];
	char dir_path[MAXLINE];
	char file_path[MAXLINE];
};
typedef struct command_line* comdl;

