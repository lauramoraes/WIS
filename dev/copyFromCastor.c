#include <mysql.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <rfio_api.h>
#include <stage_api.h>
#include <time.h>
#include <libgen.h>

#include "wisConsts.h"

int main(int argc, char *argv[])
{
   char query[SIZE], path[MAX_LENGTH_FILENAME], runType[MAX_LENGTH_STRING], command[12*MAX_LENGTH_COMMAND];
   char files[40][MAX_LENGTH_FILENAME];
   int f = 0; // files counter
   MYSQL *mysql;
   MYSQL_RES *result;
   MYSQL_ROW row;
   int i, retCode;
   struct stat statbuf;
   long int localsize;
   FILE *cron;
   time_t tm;

   setreuid(TILEBEAM_UID, TILEBEAM_UID);
   setregid(TILEBEAM_GID, TILEBEAM_GID);

   // Appends to file 'crontimes' the time of last execution of this program
   cron = fopen("crontimes", "a");
   tm = time(NULL);
   fprintf(cron, "%s\n", ctime(&tm));
   fclose(cron);

   if (argc > 1)
   {
      if (strcmp(argv[1], "v2") == 0)
      {
         setenv("RFIO_USE_CASTOR_V2", "YES", 1);
         setenv("STAGE_HOST", "castoratlas", 1);
         setenv("STAGE_SVCCLASS", "default", 1);
      }
      else
      if (strcmp(argv[1], "v1") == 0)
      {
         unsetenv("RFIO_USE_CASTOR_V2");
         setenv("STAGE_HOST", "stageatlas", 1);
         unsetenv("STAGE_SVCCLASS");
      }
      else
         setenv("STAGE_HOST", argv[1], 1);
   }

   if (getenv("RFIO_USE_CASTOR_V2") != NULL && strcmp(getenv("RFIO_USE_CASTOR_V2"), "YES") == 0)
      printf("\nUsing CASTOR2");

   printf("\nSTAGE_HOST = %s\n", getenv("STAGE_HOST"));

   // Initiates a MySQL connection to the localhost
   mysql = mysql_init(mysql);
   if (!mysql)
   {
      printf("Insufficient memory initializing DB connection.\n");
      exit(1);
   }
   if (!mysql_real_connect(mysql, "localhost", "tilecal", "", "tile", 0, NULL, 0))
   {
      fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
      exit(1);
   }

   // Executes a MySQL query to take last 60 runs
   strcpy(query, "SELECT run, type from comminfo ORDER BY date DESC limit 0,65");
   if (mysql_query(mysql, query))
   {
      fprintf(stderr, "Failed to execute to query: Error: %s\n", mysql_error(mysql));
      exit(1);
   }
   result = mysql_store_result(mysql);
   if (!result) {
      fprintf(stderr, "Failed to execute to query: Error: %s\n", mysql_error(mysql));
      exit(1);
   }

   // Gets files from the last 40 runs from CASTOR
   i = 0;
   printf("\n");
   while ((row = mysql_fetch_row(result)) && i < 40)
   {
      strcpy(runType, row[1]);
      if (strcmp(runType, "MonoCIS") == 0)
         strcpy(runType, "MonoCis");
      else
      if (strcmp(runType, "LED") == 0)
         strcpy(runType, "Led");

      snprintf(path, MAX_LENGTH_FILENAME, "%s/tiletb_%s_%s.0.root", LOCAL_COMM_FILES, row[0], runType);
      retCode = rfio_stat(path, &statbuf);
      if (retCode < 0)
      {
         //printf("Run %s does not exist in local directory!\n", row[0]);
         snprintf(path, MAX_LENGTH_FILENAME, "%s/tiletb_%s_%s.0.root", CASTOR_COMM_FILES, row[0], runType);
         retCode = rfio_stat(path, &statbuf);
         if (retCode < 0)
            printf("Ntuple for run %s does not exist in castor directory!\n", row[0]);
         else
         {
            snprintf(files[f], MAX_LENGTH_COMMAND, "%s", path);
            f++;
         }
      }
      else
      {
         // Verify whether the files in CASTOR and in local directory have the same size
         localsize = statbuf.st_size;
         snprintf(path, MAX_LENGTH_FILENAME, "%s/tiletb_%s_%s.0.root", CASTOR_COMM_FILES, row[0], runType);
         retCode = rfio_stat(path, &statbuf);
         if ((retCode >= 0) && (localsize != statbuf.st_size))
         {
            printf("Different file sizes for run %s!\n", row[0]);
            snprintf(path, MAX_LENGTH_FILENAME, "%s/tiletb_%s_%s.0.root", LOCAL_COMM_FILES, row[0], runType);
            snprintf(command, MAX_LENGTH_COMMAND, "rm -f %s" , path);
            system(command);

            snprintf(path, MAX_LENGTH_FILENAME, "%s/tiletb_%s_%s.0.root", CASTOR_COMM_FILES, row[0], runType);
            snprintf(files[f], MAX_LENGTH_COMMAND, "%s", path);
            f++;
         }
      }
      i++;
   }

   do
   {

      strcpy(runType, row[1]);
      if (strcmp(runType, "MonoCIS") == 0)
         strcpy(runType, "MonoCis");
      else
      if (strcmp(runType, "LED") == 0)
         strcpy(runType, "Led");

      snprintf(path, MAX_LENGTH_FILENAME, "%s/tiletb_%s_%s.0.root", LOCAL_COMM_FILES, row[0], runType);
      retCode = rfio_stat(path, &statbuf);
      if (retCode == 0)
      {
         snprintf(command, MAX_LENGTH_COMMAND, "rm -f %s", path);
         system(command);
         printf("File '%s' removed!\n", path);
      }
      i++;

   } while ((row = mysql_fetch_row(result)) && i < 65);

   mysql_free_result(result);
   mysql_close(mysql);

   strcpy(command, "stager_get");
   for (i = 0; i < f; i++)
   {
      strcat(command, " -M "); strcat(command, files[i]);
   }
   strcat(command, " > /dev/null");
   system(command);

   for (i = 0; i < f; i++)
   {
      snprintf(command, MAX_LENGTH_COMMAND, "rfcp %s %s/temp", files[i], LOCAL_COMM_FILES);
      system(command);
      snprintf(command, MAX_LENGTH_COMMAND, "mv %s/temp/%s %s", LOCAL_COMM_FILES, basename(files[i]), LOCAL_COMM_FILES);
      system(command);
   }

   exit(OK);
}

