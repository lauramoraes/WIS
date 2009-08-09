#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <shift.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>
#include <signal.h>
#include <ctype.h>

#include "wisConsts.h"
#include "wisFunctions.h"


int wisDeleteFile(char *file)
{
    int retCode;
    char command[MAX_LENGTH_COMMAND + 1];

    setreuid(TILEBEAM_UID, TILEBEAM_UID);
    setregid(TILEBEAM_GID, TILEBEAM_GID);

    snprintf(command, SIZE, "rm -f %s", file);
    //printf("%s", command);
    retCode = system(command);
    //printf("%d", retCode);

    return(retCode);
}



/* ctbFileExists(filename) */
int ctbFileExists(char *filename)
{
   struct stat statbuf;
   int retCode;

   retCode = rfio_stat(filename, &statbuf);
   if (retCode < 0)
      return(1);

  return (OK);

} /* ctbFileExists(filename) */


/*ctbStageFile(run, monthYear, onlineoffline)*/
int ctbStageFile(char *filename)
{
  char command [SIZE];
  int  retCode;

  snprintf(command, SIZE, "/usr/local/bin/stagein -h stageatlas -M %s", filename);
  retCode = system(command);

  return(retCode);

} /* ctbStageFile(filename) */


/* ctbCopyFile(filename, copyTo) */
int ctbCopyFile(char *filename, char *copyTo)
{
    FILE *fp = 0, *fout = 0;
    int size, size_read, count = 0;
    long long bufferSize = 0;
    char path[SIZE], *buffer = 0;
    struct stat statbuf;

    if (strstr(filename, "/castor"))
    {
      //snprintf(command, MAX_LENGTH_COMMAND, "%s %s %s", RFCP, argv[1], argv[2]);//, basename(argv[1]));
      snprintf(path, SIZE, "%s/%s", copyTo, basename(filename));
      //printf("%s - %s<br>\n", argv[1], path);
      rfio_stat(filename, &statbuf);
      size = statbuf.st_size;
      fp = rfio_fopen(filename, "r");
      fout = rfio_fopen(path, "w");
      bufferSize = 1024*1024*1024;
      buffer = (char *) malloc(bufferSize);
      while (buffer == 0 && bufferSize != 1) {
         bufferSize = bufferSize/1024;
         buffer = (char *) malloc(bufferSize);
      }
      while (!rfio_feof(fp) && fp != 0 && fout != 0) {
         size_read = rfio_fread(buffer, 1, bufferSize, fp);
         if (size_read > 0)
            rfio_fwrite(buffer, 1, size_read, fout);

         count += size_read;
      }
      printf("<input type=\"hidden\" name=\"filesize\" value=\"%d %d\">\n", count, size);
      rfio_fclose(fp);
      rfio_fclose(fout);
    }
    //else
      //snprintf(command, MAX_LENGTH_COMMAND, "ln -s %s %s", argv[1], argv[2]);
   //retCode = system(command);

/*   if (retCode == 256)
      exit(RFCP_256_ERROR);
    else
    if (retCode == 7168)
      exit(RFCP_7168_ERROR);
    else
    if (retCode != 0)
      exit(RFCP_ERROR);*/
    //system(command);
  return (OK);

} /* ctbCopyFile(filename, copyTo) */



void ctbSendEmailTo(char *to, char *from, char *mailSubject, char *textMailMessage, char *htmlMailMessage)
{
   FILE *p;

   p = popen("/usr/sbin/sendmail -t","w");

   fprintf(p, "From: %s\n", from);
   fprintf(p, "To: %s\n", to);
   fprintf(p, "Subject: %s\n", mailSubject);

   fprintf(p, "MIME-Version: 1.0\n");
   fprintf(p, "X-Mozilla-Status2: 00000000\n");
   fprintf(p, "Content-Type: multipart/alternative;");
   fprintf(p, "boundary=\"----=_NextPart_000_002C_01BFABBF.4A7D6BA0\"\n");
   fprintf(p, "------=_NextPart_000_002C_01BFABBF.4A7D6BA0\n");
   fprintf(p, "Content-Type: text/plain;");
   fprintf(p, "charset=US-ASCII\n");
   fprintf(p, "Content-Transfer-Encoding: 8bit\n\n");
   fprintf(p, textMailMessage);
   fprintf(p, "------=_NextPart_000_002C_01BFABBF.4A7D6BA0\n");
   fprintf(p, "Content-Type: text/html;");
   fprintf(p, "charset=US-ASCII\n");
   fprintf(p, "Content-Transfer-Encoding: quoted-printable\n\n");
   fprintf(p, htmlMailMessage);
   fprintf(p, "\n");
   fprintf(p, "------=_NextPart_000_002C_01BFABBF.4A7D6BA0--\n");

   pclose(p);
}

void ctbTouchFile(char *filename)
{
   char command[SIZE];

   snprintf(command, SIZE, "touch %s", filename);
   system(command);
}

int ctbIsAllDigit(char *str)
{
    int i;

    for (i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
            return(0);
    }
    return(1);
}

