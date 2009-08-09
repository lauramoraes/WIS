#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <rfio_api.h>
#include <stage_api.h>
#include <serrno.h>
#include <libgen.h>
#include <time.h>

#include "wisFunctions.h"
#include "wisConsts.h"

int main(int argc, char *argv[])
{
	int retcode;
	struct stat statbuf;
	char runNumber[MAX_LENGTH_STRING + 1], fileType[MAX_LENGTH_STRING + 1], runType[MAX_LENGTH_STRING + 1];
	char basename[MAX_LENGTH_FILENAME + 1], filename[MAX_LENGTH_FILENAME + 1], prefix[MAX_LENGTH_STRING + 1];
	char moduleName[MAX_LENGTH_STRING + 1];
	strcpy(fileType, argv[1]);
	strcpy(moduleName,  argv[2]);
	strcpy(runNumber, argv[3]);
	strcpy(runType, argv[4]);
	
	if (strcmp(fileType, "ntuple") == 0)
		snprintf(basename, MAX_LENGTH_FILENAME, "tiletb_%s_%s.%s.0.root", runNumber, runType, moduleName);
	else
		exit(0);
	// Look for file in CASTOR directory
	snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s/%s", CASTOR_COMM_FILES, moduleName, basename);
	retcode = rfio_stat(filename, &statbuf);
	if (retcode<0)
		exit(0);
	exit(2);
}
