#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "wisConsts.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	struct stat *buf;
	//char command[100];

	//   setreuid(TILEBEAM_UID, TILEBEAM_UID);
	//   setregid(TILEBEAM_GID, TILEBEAM_GID);
	//fp = fopen(argv[1], "r");
	//if (fp) {
	//fclose(fp);
	//}
	if (stat(argv[1], buf) == 0) // file already exists...
		exit(1);

	fp = fopen(argv[1], "w");
	if (fp){
		fclose(fp);
		//snprintf(command, 100, "chmod 777 %s", argv[1]);
		//system(command);
		exit(OK);
	}
	printf("Do not work\n");
	exit(1);

}
