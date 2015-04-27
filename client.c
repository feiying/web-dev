#include "client.h"
#include "get_filename.h"

int main(int argc, char*argv[])
{
	struct sockaddr_in servaddr;
	struct command_line cl_s;               
	comdl cl;                                /* cl is struct */
	char buf[MAXLINE];
	char *str;
	int sockfd, fp, n, num = 0;

	if (argc != 3){
		fputs("usage: ./client <dir or file> <dir or file>\n", stderr);
		exit(1);
	}
	memset(&cl_s, 0, sizeof(struct command_line));
	cl = &cl_s;

	/**************** analysis command line argument *********************/
	num = analysis_command_line(argv[1], argv[2], cl);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, cl_s.serveraddr, &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	/******  'num = 1' means copy the file from server to client ********/
	if (num == 1){
		write(sockfd, cl_s.dir_path, strlen(cl_s.dir_path));

		while(!(n = read(sockfd, buf, MAXLINE)))
			;
		if(strcmp(buf, "directory"))
			exit(1);
		char buf1[MAXLINE];
		get_filename(buf1, cl_s.file_path);
		write(sockfd, buf1, strlen(buf1)+1);

		fp = open(cl_s.file_path, O_RDONLY);
		if (fp < 0){
			perror("fail to open\n");
			exit(1);
		}

		n = 0;
		char buf2[MAXLINE];
		read(sockfd, buf2, MAXLINE);
		if (strcmp(buf2, "ok"))
			exit(1);
		while(n = read(fp, buf, MAXLINE)){
			write(sockfd, buf, n);
		}
		close(fp);
	}

	/******  'num = 2' means copy the file from client to server ********/
	if (num == 2){
		write(sockfd, cl_s.dir_path, strlen(cl_s.dir_path));
		read(sockfd, buf, MAXLINE);
		if (strcmp(buf, "directory")){
			perror("the file not exist in server");
			exit(1);
		}
		char buf3[MAXLINE];
		get_filename(buf3, cl_s.file_path);
		write(sockfd, buf3, strlen(buf3)+1);
		fp = open(cl_s.file_path, O_RDONLY);
		read(sockfd, buf, MAXLINE);
		if(!strcmp(buf, "ok")){
			perror("error");
			exit(1);
		}
		n = 0;
		while(n = read(fp, buf, MAXLINE))
			write(sockfd, buf, n);
		close(fp);
	}
	close(sockfd);
	return 0;
}
