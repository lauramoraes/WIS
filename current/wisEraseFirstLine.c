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

 //  setreuid(TILEBEAM_UID, TILEBEAM_UID);
  // setregid(TILEBEAM_GID, TILEBEAM_GID);

   snprintf(filename, SIZE, "%s", argv[1]);

   snprintf(tmpFilename, SIZE, "%s.tmp", filename);
   file = fopen(filename, "r");
   tmpFile = fopen(tmpFilename, "w");

   fgets(s, SIZE, file);
   while(!feof(file))
   {
      fgets(s, SIZE, file);
      fputs(s, tmpFile);
   }

   snprintf(command, SIZE, "rm -f %s", filename);
   system(command);

   snprintf(command, SIZE, "mv %s %s", tmpFilename, filename);
   system(command);

   snprintf(command, SIZE, "rm -f %s", tmpFilename);
   system(command);

   exit(OK);
}
