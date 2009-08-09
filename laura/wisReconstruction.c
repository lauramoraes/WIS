#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wisConsts.h"
#include "wisFunctions.h"

void handleError(int errNo);

int main(int argc, char *argv[])
{
	char runNumber[MAX_LENGTH_STRING + 1], runType[MAX_LENGTH_STRING + 1], maxEvents[MAX_LENGTH_STRING + 1], version[MAX_LENGTH_STRING + 1];
	char outputDir[MAX_LENGTH_STRING + 1], jobOptions[MAX_LENGTH_STRING + 1], filename[MAX_LENGTH_FILENAME + 1];
	char castor[MAX_LENGTH_STRING + 1], castorDirectory[MAX_LENGTH_FILENAME + 1];
	char command[MAX_LENGTH_COMMAND + 1], line[MAX_LENGTH_STRING + 1], aux[MAX_LENGTH_STRING + 1];
	char histo[MAX_LENGTH_FILENAME + 1], pool[MAX_LENGTH_FILENAME + 1], ntuple[MAX_LENGTH_FILENAME + 1];
	FILE *fp;
	int write, retCode;

	setreuid(TILEBEAM_UID, TILEBEAM_UID);
	setregid(TILEBEAM_GID, TILEBEAM_GID);

	strcpy(runNumber, argv[1]);
	strcpy(runType, argv[2]);
	strcpy(maxEvents, argv[3]);
	strcpy(version, argv[4]);
	strcpy(outputDir, argv[5]);
	strcpy(jobOptions, argv[6]);
	strcpy(castor, argv[7]);
	strcpy(castorDirectory, argv[8]);
	strcpy(filename, argv[9]);

	//printf("Reconstructing run %s of type %s. Results to file %s. This will take several minutes...<br />\n", runNumber, runType, filename);

	if (strcmp(castor, "yes") == 0)
		strcpy(castor, "-cl");
	else 
		strcpy(castor, "-c");

	wisDeleteFile(filename);

	snprintf(command, MAX_LENGTH_COMMAND + 1, "./offlineShift.sh %s %s %s %s %s %s %s %s > %s", castor, castorDirectory, runNumber, runType, maxEvents, version, outputDir, jobOptions, filename);
	//printf("Command: '%s'<br />\n", command);
	retCode = system(command);
	//printf("retCode = '%d'<br />\n", retCode);

	fp = fopen(filename, "r");
	if (!fp)
	{
		printf("Error trying to open temporary file!<br />\n");
		exit(1);
	}

	write = 0;
	while((fgets(line, MAX_LENGTH_STRING, fp)) != NULL)
	{

		if (strstr(line, "Copying"))
		{
			write = 1;
			if (!strstr(line, "Results To Castor"))
			{
				strcpy(aux, line);
				strtok(aux, " ");
				strcpy(filename, strtok(NULL, " "));
			}
		}

		if (write)
			printf("%s<br />\n", line);

		if (strstr(filename, "histo"))
			strcpy(histo, filename);
		else
		if (strstr(filename, "Data"))
			strcpy(pool, filename);
		else
		if (strstr(filename, "tiletb"))
			strcpy(ntuple, filename);

		memset(line, '\0', MAX_LENGTH_STRING + 1);

	} //while

	printf("<br /><b>Links for reconstructed files:</b><br />\n");
	printf("<a href=\"%s/Reco-%s-%s.%s/%s\">Histo File</a><br />\n", "/tileinfo/tiletransfer/wis", runNumber, runType, version, histo);
	printf("<a href=\"%s/Reco-%s-%s.%s/%s\">POOL File</a><br />\n", "/tileinfo/tiletransfer/wis", runNumber, runType, version, pool);
	printf("<a href=\"%s/Reco-%s-%s.%s/%s\">Ntuple</a><br />\n", "/tileinfo/tiletransfer/wis", runNumber, runType, version, ntuple);

	exit(OK);

}

void handleError(int errNo)
{
	if (errNo < 0)
	{
		printf("Error: %s<br />\n", strerror(errno));
		exit(0);
	}
}

