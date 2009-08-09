#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <rfio_api.h>
#include <stage_api.h>
#include <serrno.h>
#include <libgen.h>

#include "wisConsts.h"

int main(int argc, char *argv[])
{
	char runNumber[MAX_LENGTH_STRING + 1];
	char path[MAX_LENGTH_FILENAME + 1], filename[MAX_LENGTH_FILENAME + 1];
	char searchNtuple[MAX_LENGTH_STRING + 1], searchData[MAX_LENGTH_STRING + 1];
	DIR *directory;
	int pool, ntuple;
	struct dirent *entry;
	
	strcpy(runNumber, argv[1]);
	snprintf(path, MAX_LENGTH_STRING, "/castor/cern.ch/user/t/tilebeam/commissioning");
	snprintf(searchNtuple, MAX_LENGTH_STRING, "tiletb_%s", runNumber);
	snprintf(searchData, MAX_LENGTH_STRING, "%s.root", runNumber);
	
	directory = rfio_opendir(path);
	if (directory == NULL)
	{
		printf("Error: %s\n", sstrerror(serrno));
		exit(4);
	}

	pool = 0;
	ntuple = 0;
	while ((entry = rfio_readdir(directory)) != NULL)
	{
		snprintf(filename, MAX_LENGTH_FILENAME, "%s", entry->d_name);
		if (strstr(filename, searchNtuple) != NULL)
			ntuple++;
		if (strstr(filename, searchData) != NULL)
			pool++;
	}
	
	rfio_closedir(directory);
	
	if (pool && ntuple)
		exit(1);
	if (ntuple)
		exit(2);
	if (pool)
		exit(3);

	exit(0);
}
