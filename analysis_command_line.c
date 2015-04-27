#include "client.h"

int analysis_command_line(char *argv1, char *argv2, comdl cl)
{
	struct stat buf;
	char *p, *ptr;

	if (lstat(argv1, &buf) < 0){
		if ((p = strchr(argv1, ':')) != 0){
			strcpy(cl->file_path, p+1);
			strncpy(cl->serveraddr, argv1, p-argv1);
			cl->serveraddr[p-argv1] = '\0'; 
			printf("***************** %s\n", cl->serveraddr);
			printf("***************** %s\n", cl->file_path);
			printf("***************** %s\n", cl->dir_path);
			if (lstat(cl->file_path, &buf) < 0){
				perror("error");
				exit(1);
			}
			if(S_ISREG(buf.st_mode)){
				if (!strcmp(cl->serveraddr, "127.0.0.1")){
					printf("****************** serveraddr is right\n");
					if (lstat(argv2, &buf) < 0){
						perror("error");
						exit(1);
					}else{
						strcpy(cl->dir_path, argv2);
						printf("****************** \n");
						if (S_ISDIR(buf.st_mode)){
							printf("****************** %s\n", cl->dir_path);
							return 1;
						}
						exit(0);
					}
				}else{
					perror("server ip error");
					exit(1);
				}
			}
		}
	}

	else if(S_ISREG(buf.st_mode)){
		printf("*********************** file******************\n");
		ptr = "regular";
		strcpy(cl->file_path, argv1);
		if ((p = strchr(argv2, ':')) != 0){
			printf("********************* : *************\n");
			strcpy(cl->dir_path, p+1);
			strncpy(cl->serveraddr, argv2, p-argv2);
			if (!strcmp(cl->serveraddr, "127.0.0.1")){
				if (lstat(cl->dir_path, &buf) < 0){
					perror("error");
					exit(1);
				}else{
					if (S_ISDIR(buf.st_mode))
						return 2;
				}
			}
		}
	}
	exit(0);
}
