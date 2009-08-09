#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wisConsts.h"

unsigned int entities(char *, int, char *);

int main(int argc, char *argv[])
{
	char filename[SIZE], tmpFilename[SIZE], command[10*SIZE], s[SIZE];
	FILE *file, *tmpFile;
	
	//setreuid(TILEBEAM_UID, TILEBEAM_UID);
//	s//etregid(TILEBEAM_GID, TILEBEAM_GID);
	
	snprintf(filename, SIZE, "%s", argv[1]);
	
	snprintf(tmpFilename, SIZE, "%s.tmp", filename);
	file = fopen(filename, "r");
	tmpFile = fopen(tmpFilename, "w");
	
	fgets(s, SIZE, file);
//	while (!strstr(s, "<?xml") && !feof(file))
		fgets(s, SIZE, file);

	while (!feof(file))
	{
		if (!strstr(s, "Info in <TCanvas::Print>") && !strstr(s, "SysError in <TRFIOFile::TRFIOFile>") && !strstr(s, "Error in <TTree") && !strstr(s, "Error in <TFile::ReadBuffer>") && !strstr(s,"Error in <TFile::Init>"))
			fputs(s, tmpFile);
		fgets(s, SIZE, file);
	}
	
	snprintf(command, SIZE, "rm -f %s", filename);
	system(command);
	
	snprintf(command, SIZE, "mv %s %s", tmpFilename, filename);
	system(command);
	
	snprintf(command, SIZE, "rm -f %s", tmpFilename);
	system(command);
	
	exit(OK);
}
