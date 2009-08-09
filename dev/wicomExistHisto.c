#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <rfio_api.h>
#include <stage_api.h>
#include <serrno.h>
#include <libgen.h>
	#include<time.h>
#include "wisFunctions.h"
#include "wisConsts.h"

int main(int argc, char *argv[])
{
	int retcode;
	struct stat statbuf;
	struct tm *timeinfo;
	char runNumber[MAX_LENGTH_STRING + 1], fileType[MAX_LENGTH_STRING + 1], runType[MAX_LENGTH_STRING + 1];
	char basename[MAX_LENGTH_FILENAME + 1], filename[MAX_LENGTH_FILENAME + 1], prefix[MAX_LENGTH_STRING + 1];
	strcpy(runNumber, argv[1]);
	strcpy(runType, argv[2]);
	
	snprintf(basename, MAX_LENGTH_FILENAME, "tilemon_%s_%s.0.root", runNumber, runType);
	
	// Look for file in CASTOR directory
	snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s", CASTOR_COMM_FILES, basename);
	retcode = rfio_stat(filename, &statbuf);
	if (retcode<0)
		exit(0);
	
	timeinfo= localtime (&(statbuf.st_ctime));
	printf("%02i/%02i/%04i&nbsp;%02i:%02i:%02i", timeinfo->tm_mday, timeinfo->tm_mon+1,  timeinfo->tm_year+1900,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
	
	exit(1);
}
