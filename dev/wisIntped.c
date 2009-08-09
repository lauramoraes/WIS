#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "wisConsts.h"
#include "wisFunctions.h"

int main(int argc, char *argv[])
{
	char  htmlfilename[MAX_LENGTH_FILENAME + 1], filename[MAX_LENGTH_FILENAME + 1], pidFilename[MAX_LENGTH_FILENAME + 1];
	char command[MAX_LENGTH_COMMAND + 1], execFilename[MAX_LENGTH_FILENAME + 1];
	char path[MAX_LENGTH_STRING + 1], ld_library_path[MAX_LENGTH_STRING + 1];
	char *argvRoot[6], *envRoot[4];
	FILE *fp;
	int retCode, i, fd;
	pid_t pid;
	//setreuid(TILEBEAM_UID, TILEBEAM_UID);
	//setregid(TILEBEAM_GID, TILEBEAM_GID);

	if(argc < 4)
	{
		printf("Usage:  %s [filename] [pidFilename] [htmlfilename]\n", argv[0]);
		exit(0);
	}
	printf("aki\n");
	strcpy(filename, argv[1]);
	strcpy(pidFilename, argv[2]);
	strcpy(htmlfilename, argv[3]);

	pid = fork();
	
	if (pid < 0 )
	{
		printf("error id=%d\n",pid);
		exit(10); 
	}
	else if (pid) // parent process
	{
	//	int ret1, status1 ;
		fp = fopen(pidFilename, "w");
		fprintf(fp, "%i", pid);
		fclose(fp);
	//	ret1 = wait(&status1);
		exit(OK);
	}
	else
	{

	//	setreuid(TILEBEAM_UID, TILEBEAM_UID);
	//	setregid(TILEBEAM_GID, TILEBEAM_GID);
	
		setenv("ROOTSYS", ROOT_SYS_INTEGRATOR, 1);
		snprintf(path, MAX_LENGTH_STRING + 1, "%s:%s", ROOT_PATH_INTEGRATOR, getenv("PATH"));
		setenv("PATH", path, 1);
		snprintf(ld_library_path, MAX_LENGTH_STRING + 1, "%s/lib:%s", ROOT_SYS_INTEGRATOR, getenv("LD_LIBRARY_PATH"));
		setenv("LD_LIBRARY_PATH", ld_library_path, 1);
		
		snprintf(execFilename, MAX_LENGTH_FILENAME + 1, "./macro/TTileCommRun/intped");
		
		for (i = 0; i < 2; i++)
			argvRoot[i] = (char *)malloc(MAX_LENGTH_STRING + 1);
		argvRoot[2] = NULL;
		snprintf(argvRoot[0], MAX_LENGTH_STRING + 1, "%s", execFilename);
		snprintf(argvRoot[1], MAX_LENGTH_STRING + 1, "%s", filename);
		for (i = 0; i < 3; i++)
			envRoot[i] = (char *)malloc(MAX_LENGTH_STRING + 1);
		envRoot[3] = NULL;
		snprintf(envRoot[0], MAX_LENGTH_STRING + 1, "ROOTSYS=%s", ROOT_SYS_INTEGRATOR);
		snprintf(envRoot[1], MAX_LENGTH_STRING + 1, "PATH=%s", path);
		snprintf(envRoot[2], MAX_LENGTH_STRING + 1, "LD_LIBRARY_PATH=%s", ld_library_path);
		fd = open(htmlfilename, O_WRONLY);

		if (fd == -1)
		{
			printf("open error1: %s\n", strerror(errno));
			exit(0);
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
		printf("%s %s\n ", execFilename, argvRoot[1]);
		retCode = execve(execFilename, argvRoot, envRoot);
		if (retCode == -1)
			printf("execve error: %s\n", strerror(errno));

		close(1); close(2); close(fd);
		exit(OK);
	}

}
