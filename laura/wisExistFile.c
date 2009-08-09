#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <rfio_api.h>
#include <stage_api.h>
#include <serrno.h>
#include <libgen.h>

#include "wisFunctions.h"
#include "wisConsts.h"

int main(int argc, char *argv[])
{
	char runNumber[MAX_LENGTH_STRING + 1], fileType[MAX_LENGTH_STRING + 1], runType[MAX_LENGTH_STRING + 1];
	char basename[MAX_LENGTH_FILENAME + 1], filename[MAX_LENGTH_FILENAME + 1], prefix[MAX_LENGTH_STRING + 1];
	
	strcpy(fileType, argv[1]);
	strcpy(runNumber, argv[2]);
	strcpy(runType, argv[3]);
	
	if (strcmp(fileType, "pool") == 0)
	{
		// POOL files for these types of run are no expected
		if (strcmp(runType, "Phys") == 0 || strcmp(runType, "Led") == 0)
			exit(0);
		
		if (strcmp(runType, "CIS") == 0)
			strcpy(prefix, "TileCISData_fit");
		else
		if (strcmp(runType, "MonoCis") == 0)
			strcpy(prefix, "TileMonoData_fit");
		else
		if (strcmp(runType, "Ped") == 0)
			strcpy(prefix, "TileNoiseData");
	}
	
	if (strcmp(fileType, "ntuple") == 0)
	{
		snprintf(basename, MAX_LENGTH_FILENAME, "tiletb_%s_%s.0.root", runNumber, runType);
		snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s", LOCAL_COMM_FILES, basename);
	}
	else
	if (strcmp(fileType, "pool") == 0)
	{
		snprintf(basename, MAX_LENGTH_FILENAME, "r%s_%s.0.root", runNumber, runType);
		snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s", LOCAL_COMM_POOL_FILES, basename);
		snprintf(basename, MAX_LENGTH_FILENAME, "%s_%s.root", prefix, runNumber); // for looking in CASTOR
	}

	// Look for file in the local directory
	if (ctbFileExists(filename) == OK)
		exit(1);
	
	// Look for file in CASTOR directory
	snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s", CASTOR_COMM_FILES, basename);
	//printf("%s\n", filename);
	if (ctbFileExists(filename) == OK)
		exit(2);

	exit(0);
}
