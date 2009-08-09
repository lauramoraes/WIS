#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wisConsts.h"
#include "wisFunctions.h"

int main(int argc, char *argv[])
{
	char runNumber[MAX_LENGTH_STRING + 1], runType[MAX_LENGTH_STRING + 1], module[MAX_LENGTH_STRING + 1], maxEvents[MAX_LENGTH_STRING + 1];
	char filename[MAX_LENGTH_FILENAME + 1], pidFilename[MAX_LENGTH_FILENAME + 1];
	char command[MAX_LENGTH_COMMAND + 1], execFilename[MAX_LENGTH_FILENAME + 1];
	char path[MAX_LENGTH_STRING + 1], ld_library_path[MAX_LENGTH_STRING + 1];
	char *argvRoot[6], *envRoot[4];
	FILE *fp;
	int retCode, i, fd;
	pid_t pid;

	//setreuid(TILEBEAM_UID, TILEBEAM_UID);
	//setregid(TILEBEAM_GID, TILEBEAM_GID);

	strcpy(filename, argv[1]);
	strcpy(pidFilename, argv[2]);
	strcpy(runNumber, argv[3]);
	strcpy(runType, argv[4]);
	strcpy(module, argv[5]);

	pid = fork();
	
	if (pid < 0 )
	{
		printf("error id=%d\n",pid);
		exit(1); 
	}
	else if (pid) // parent process
	{
		fp = fopen(pidFilename, "w");
		fprintf(fp, "%i", pid);
		fclose(fp);
		exit(OK);
	}
	else
	//if (pid == 0) // child process
	{
		//snprintf(command, MAX_LENGTH_COMMAND + 1, "./macro/rootinit.sh; root -b -l -q './macro/checkCRCandBCID.cpp(\"%s\", \"\", \"%s\", \"%s\")' >> ./%s", runNumber, runType, module, filename);
		//snprintf(command, MAX_LENGTH_COMMAND + 1, "ls -l >> ./%s", filename);
		//retCode = system(command);
		
	//	setreuid(TILEBEAM_UID, TILEBEAM_UID);
	//	setregid(TILEBEAM_GID, TILEBEAM_GID);
		
		setenv("ROOTSYS", ROOT_SYS, 1);
		snprintf(path, MAX_LENGTH_STRING + 1, "%s:%s", ROOT_PATH, getenv("PATH"));
		setenv("PATH", path, 1);
		snprintf(ld_library_path, MAX_LENGTH_STRING + 1, "%s/lib:%s", ROOT_SYS, getenv("LD_LIBRARY_PATH"));
		setenv("LD_LIBRARY_PATH", ld_library_path, 1);
		
		//int  execve(const  char  *filename,  char  *const  argv [], char *const envp[]);
		//snprintf(execFilename, MAX_LENGTH_FILENAME + 1, "%s/root", ROOT_PATH);
		snprintf(execFilename, MAX_LENGTH_FILENAME + 1, "./macro/TTileCommRun/checkCalibrationData");
		//printf("%s\n", execFilename);
		
		for (i = 0; i < 4; i++)
			argvRoot[i] = (char *)malloc(MAX_LENGTH_STRING + 1);
		argvRoot[4] = NULL;
		snprintf(argvRoot[0], MAX_LENGTH_STRING + 1, "%s", execFilename);
		//snprintf(argvRoot[1], MAX_LENGTH_STRING + 1, "-b");
		snprintf(argvRoot[1], MAX_LENGTH_STRING + 1, "%s", runNumber);
		//snprintf(argvRoot[2], MAX_LENGTH_STRING + 1, "-l");
		snprintf(argvRoot[2], MAX_LENGTH_STRING + 1, "%s", runType);
		//snprintf(argvRoot[3], MAX_LENGTH_STRING + 1, "-q");
		snprintf(argvRoot[3], MAX_LENGTH_STRING + 1, "%s", module);
		//snprintf(argvRoot[4], MAX_LENGTH_STRING + 1, "./macro/TTileCommRun/checkCnoise.C(\"%s\", \"\", \"%s\", \"%s\")", runNumber, runType, module);
		//for (i = 0; i < 4; i++)
			//printf("%s\n", argvRoot[i]);
	
		for (i = 0; i < 3; i++)
			envRoot[i] = (char *)malloc(MAX_LENGTH_STRING + 1);
		envRoot[3] = NULL;
		snprintf(envRoot[0], MAX_LENGTH_STRING + 1, "ROOTSYS=%s", ROOT_SYS);
		snprintf(envRoot[1], MAX_LENGTH_STRING + 1, "PATH=%s", path);
		snprintf(envRoot[2], MAX_LENGTH_STRING + 1, "LD_LIBRARY_PATH=%s", ld_library_path);
		//for (i = 0; i < 3; i++)
			//printf("%s\n", envRoot[i]);
		
		//printf("filename: %s\n", filename);
		fd = open(filename, O_WRONLY);
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

		//printf("Isto aqui foi escrito para onde???\nHein?!?!?! Tah no arquivo certo???\n");

		retCode = execve(execFilename, argvRoot, envRoot);
		if (retCode == -1)
			printf("execve error: %s\n", strerror(errno));

		close(1); close(2); close(fd);
		exit(OK);
	}

}
