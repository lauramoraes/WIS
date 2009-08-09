#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "rfio_api.h"

int main( int argc, char *argv[] ) 
{
	DIR *pDIR;
	struct dirent *pDirEnt;
	unsigned i;
	char filename[1024];
	char runNumber[10];

	strcpy(runNumber, argv[1]);

	for(i=1; i<65; i++)
	{
		
		sprintf(filename, "/castor/cern.ch/user/t/tilebeam/commissioning/LBA%02i",i );
		pDIR = rfio_opendir(filename);
	
		if ( pDIR == NULL ) 
		{
			fprintf( stderr, "%s %d: opendir() failed (%s)\n",
				__FILE__, __LINE__, strerror( errno ));
			continue;
		}

		/* Get each directory entry from pDIR and print its name */

		pDirEnt = rfio_readdir( pDIR );
		while ( pDirEnt != NULL ) 
		{
			if(strstr(pDirEnt->d_name, runNumber))
				printf( "%s\n", pDirEnt->d_name );
			pDirEnt = rfio_readdir( pDIR );
		}
	
		/* Release the open directory */

		rfio_closedir( pDIR );
	}
	for(i=1; i<65; i++)
	{
		
		sprintf(filename, "/castor/cern.ch/user/t/tilebeam/commissioning/LBC%02i",i );
		pDIR = rfio_opendir(filename);
	
		if ( pDIR == NULL ) 
		{
			fprintf( stderr, "%s %d: opendir() failed (%s)\n",
				__FILE__, __LINE__, strerror( errno ));
			continue;
		}

		/* Get each directory entry from pDIR and print its name */

		pDirEnt = rfio_readdir( pDIR );
		while ( pDirEnt != NULL ) 
		{
			if(strstr(pDirEnt->d_name, runNumber))
				printf( "%s\n", pDirEnt->d_name );
			pDirEnt = rfio_readdir( pDIR );
		}
	
		/* Release the open directory */

		rfio_closedir( pDIR );
	}
	return 0;
}
