#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE		80
#define SERV_PORT	8000

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr, cliaddr;
	struct stat bbuf;
	socklen_t cliaddr_len;
	int listenfd, connfd, fp;
	char buf[MAXLINE];
	int i, n;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);

	printf("Accepting connetions...\n");
	while (1){
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
		bzero(&buf, sizeof(buf));
		read(connfd, buf, MAXLINE);

		lstat(buf, &bbuf);

		if(buf[strlen(buf)-1] =='/')
			buf[strlen(buf) -1] = '\0';

		/***************** judgement the file **********************/
		if(S_ISREG(bbuf.st_mode)){
			fp = open(buf, O_RDONLY);
			if (fp < 0){
				perror("fail to open\n");
				exit(1);
			}
			char buf1[] = "open ok";
			write(connfd, buf1, sizeof(buf1));
			read(connfd, buf, MAXLINE);/*******/
			n = 0;
			while((n = read(fp, buf, MAXLINE)) != 0)
				write(connfd, buf, n);
			close(fp);
		}

		/******************** judgement the directory *****************/
		if(S_ISDIR(bbuf.st_mode)){
			char p[] = "directory";
			char buf2[MAXLINE];
			write(connfd, p, strlen(p));
			n = 0;
			char *p1;
			if((n = read(connfd, buf2, MAXLINE)) != 0)
				strcat(buf, buf2);
			else{
				perror("error");
				exit(1);
			}
			fp = open(buf, O_WRONLY|O_CREAT|O_TRUNC);
			char buf3[] = "ok";
			write(connfd, "ok", strlen(buf3));
			while((n = read(connfd, buf, MAXLINE)) != 0)
				write(fp, buf, n);
			close(fp);
		}
		close(connfd);
	}
	return 0;
}
