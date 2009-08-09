#include <sys/types.h>
#include <sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <rfio_api.h>
#include <time.h>

#include "wisFunctions.h"
#include "wisConsts.h"

int main(int argc, char *argv[])
{
	int retcode;
	struct stat statbuf;
	char runNumber[MAX_LENGTH_STRING + 1], runType[MAX_LENGTH_STRING + 1];
	char filename[MAX_LENGTH_FILENAME + 1]; 
	char moduleName[MAX_LENGTH_STRING + 1];
	unsigned i, found=0;
	 struct tm *timeinfo;
	time_t firstDate, lastDate=0;
	firstDate=time(NULL);
        strcpy(runNumber, argv[1]);
        strcpy(runType, argv[2]);

	for(i=1; i< 65; i++)
	{ 
		
		sprintf(moduleName, "LBA%02i", i);
		snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s/tiletb_%s_%s.%s.0.root",CASTOR_COMM_FILES, moduleName, runNumber, runType, moduleName);
		retcode = rfio_stat(filename, &statbuf);
		if(retcode<0)
			continue;
		found=1;
		if(firstDate>statbuf.st_ctime)
			firstDate=statbuf.st_ctime;
		if(lastDate<statbuf.st_ctime)
			lastDate=statbuf.st_ctime;
	}
	for(i=1; i< 65; i++)
	{ 
		
		sprintf(moduleName, "LBC%02i", i);
		snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s/tiletb_%s_%s.%s.0.root",CASTOR_COMM_FILES, moduleName, runNumber, runType, moduleName);
		retcode = rfio_stat(filename, &statbuf);
		if(retcode<0)
			continue;
		found=1;
		if(firstDate>statbuf.st_ctime)
			firstDate=statbuf.st_ctime;
		if(lastDate<statbuf.st_ctime)
			lastDate=statbuf.st_ctime;
	}
/*	for(i=1; i< 65; i++)
	{ 
		
		sprintf(moduleName, "EBA%02i", i);
		snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s/tiletb_%s_%s.%s.0.root",CASTOR_COMM_FILES, moduleName, runNumber, runType, moduleName);
		retcode = rfio_stat(filename, &statbuf);
		if(retcode<0)
			continue;
		if(firstDate>statbuf.st_ctime)
			firstDate=statbuf.st_ctime;
		if(lastDate<statbuf.st_ctime)
			lastDate=statbuf.st_ctime;
	}
	for(i=1; i< 65; i++)
	{ 
		
		sprintf(moduleName, "EBC%02i", i);
		snprintf(filename, MAX_LENGTH_FILENAME, "%s/%s/tiletb_%s_%s.%s.0.root",CASTOR_COMM_FILES, moduleName, runNumber, runType, moduleName);
		retcode = rfio_stat(filename, &statbuf);
		if(retcode<0)
			continue;
		if(firstDate>statbuf.st_ctime)
			firstDate=statbuf.st_ctime;
		if(lastDate<statbuf.st_ctime)
			lastDate=statbuf.st_ctime;
	}
*/
	if(found)
	{
		printf("<font face=\"arial\" size=\"2\"><table>\n\t<tr>\n\t\t<th><font face=\"arial\" size=\"2\">First Created</font></th>\n\t\t<td>");
		timeinfo= localtime (&(firstDate));
		printf("<font face=\"arial\" size=\"2\">%02i/%02i/%04i&nbsp;%02i:%02i:%02i </font>\n", timeinfo->tm_mday, timeinfo->tm_mon+1,  timeinfo->tm_year+1900,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
		printf("</td>\n\t</tr>\n\t<tr>\n\t\t<th><font face=\"arial\" size=\"2\">Last Created</font></th>\n\t\t<td>");
		timeinfo= localtime (&(lastDate));
		printf("<font face=\"arial\" size=\"2\">%02i/%02i/%04i&nbsp;%02i:%02i:%02i\n</font>", timeinfo->tm_mday, timeinfo->tm_mon+1,  timeinfo->tm_year+1900,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
		printf("</td>\n\t</tr>\n</table></font>\n");
	}
	exit(OK);

}
