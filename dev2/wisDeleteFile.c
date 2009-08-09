#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#include "wisConsts.h"
#include "wisFunctions.h"

int main(int argc, char *argv[])
{
  int retCode;
  
 // setreuid (TILEBEAM_UID, TILEBEAM_UID);
 // setregid (TILEBEAM_GID, TILEBEAM_GID);

  retCode = wisDeleteFile(argv[1]);
  exit(retCode);
}

