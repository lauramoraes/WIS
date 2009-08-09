#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "wisConsts.h"
#include "wisFunctions.h"

int main(int argc, char *argv[])
{
	int retCode;
	char command[MAX_LENGTH_STRING + 1];

	setreuid(TILEBEAM_UID, TILEBEAM_UID);
	setregid(TILEBEAM_GID, TILEBEAM_GID);

	retCode = wisDeleteFile(argv[2]);

	snprintf(command, MAX_LENGTH_STRING, "wget \"%s\" -O %s", argv[1], argv[2]);
	//printf("Command: %s<br />\n", command);
	retCode = system(command);

	exit(OK);

}
