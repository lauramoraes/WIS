#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "wisConsts.h"

int main (int argc, char** argv, char** envp)
{
	int retCode;
	int i;
	int fd;
	int rv;
	pid_t pID = fork();
	pid_t pID2;
	FILE* pidFile;


	switch(pID)
	{
		case -1:
			printf("Fork ERROR -> abort");
			retCode = -1;
			break;
		case 0: //child-> run macro
			
			pID2= fork();
			switch(pID2)
			{
				case -1:
					printf("Fork ERROR -> abort");
					retCode = -1;
					break;
				case 0:
		
					for(i=0; argv[i+2]!=NULL; i++) argv[i]=argv[i+1];
					i--;

					fd = open(argv[i],O_WRONLY);
					if(fd== -1)
					{
						printf("%s doesn't open\n", argv[i]);
						return (-1);
					}
					retCode = close(2);
					if (retCode == -1)
					{
						printf("fclose error2: %s\n", strerror(errno));
						exit(0);
					}
					retCode = dup2(fd, 2);
					if (retCode == -1)
					{
						printf("dup2 error2: %s\n", strerror(errno));
						exit(0);
					}

					retCode = close(1);
					if (retCode == -1)
					{
						printf("fclose error1: %s\n", strerror(errno));
						exit(0);
					}
					retCode = dup2(fd, 1);
					if (retCode == -1)
					{
						printf("dup2 error1: %s\n", strerror(errno));
						exit(0);
					}
					argv[i]=NULL;

					retCode = execve(argv[0], argv, envp);
					if (retCode == -1)
						printf("execve error: %s\n", strerror(errno));
					close(1); close(2); close(fd);
					break;
				default:
					wait(&rv);
					for(i=0; argv[i+1]!=NULL; i++);
					remove(argv[i]);
					break;
			}
			break;
		default:
			for(i=0; argv[i+1]!=NULL; i++);
			pidFile = fopen(argv[i], "w");
			if(pidFile)
				fprintf(pidFile, "%d", pID+1);
			else return(-1);
	}
	return retCode;
}
