#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/usr/include/mysql/mysql.h"
#include "TWISMacroResult.h"

   /* C to MySQL Program */
int wis2Tilecomm(const char* runNumber, const char* moduleName, const char* filename, TWISMacroResult & macroResults)
{
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char localhost[100], user[25], password[25], db1[100];
	char idatlasId[4], testsId[4], command[1024], tempModuleName[4];
	char runId[10], tempFilename[1024], shortFilename[1024];
	char partsShortFilename[20][1024];
	char errorMsg[1024];
	int existPlot = 0, plotTitleId;
	
	int index, index2=0, index3=0;
	int index4;
	/***********************************************************
	 **		Connecting to MySQL                       ***
	 **********************************************************/
	
	if(!mysql_init(&mysql))
	{
		sprintf(errorMsg, "MySQL Error 1: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		return(0);
   	}
	//DEFINING PARAMETERS
	strcpy(user, "lodi");
	strcpy(password, "cOAnAd26");
	strcpy(localhost, "atlasdev1");
	strcpy(db1, "tbanalysis");

	if(!mysql_real_connect(&mysql, localhost, user, password, db1, 0, NULL, 0))
	{
		sprintf(errorMsg, "MySQL Error 2: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		return(1);
	}
	/******************CONNECTED************************************/

	/***********************************************************
         **   CHECKING IF PLOT ALREADY EXISTS ON TILECOMM DB      **
         **********************************************************/


	for(index=0; filename[strlen(filename)-(index+1)]!= '/'; index++)
		tempFilename[index]= filename[strlen(filename)-(index+1)];
	tempFilename[index]='\0';

	for(index=0; index< strlen(tempFilename); index++)
		shortFilename[index]= tempFilename[strlen(tempFilename)-(index+1)];
	shortFilename[index]='\0';
	
	sprintf(command, "SELECT * from tcaPlot where name=\"%s\"", shortFilename);
	if(!mysql_query(&mysql, command))
	{
		result = mysql_store_result(&mysql);
		if(mysql_fetch_row(result))//This plot already exists in Tilecomm DB.
		{
			sprintf(errorMsg, "WIS to TileComm Warning: This plot already exists in Tilecomm database -> '%s'", shortFilename);
			//macroResults.addWarning(errorMsg);
			existPlot = 1; // plot will be updated, instead of inserted
			//mysql_close(&mysql);
			//return(1);
		}
		mysql_free_result(result);
	}
	else
	{// if the query wasn't successful. 
		sprintf(errorMsg, "MySQL Error 3: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		mysql_close(&mysql);
		return(1);
	}

	/**********************************************************
	**	       TAKING RUNID FROM TCAPLOT TABLE               **
	**********************************************************/
	
	for(index=0; index<3 ;index++)
		tempModuleName[index]=moduleName[index];
	tempModuleName[index]='\0';


	sprintf(command, "SELECT * from idatlas where code=\"%s\"", tempModuleName);
	if(!mysql_query(&mysql, command))
	{
		result = mysql_store_result(&mysql);
		if((row = mysql_fetch_row(result)))
		{
			strcpy(idatlasId, row[0]); // taking the module name id
			mysql_free_result(result);
		}
		else// Wrong module name.
		{
			//printf("Erro \n");
			sprintf(errorMsg, "WIS to TileComm Error: Wrong module name -> '%s'", moduleName);
			macroResults.addError(errorMsg);
			mysql_close(&mysql);
			return(1);
		}
	}
	else // if the query wasn't successful.
	{
		sprintf(errorMsg, "MySQL Error 4: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		mysql_close(&mysql);
		return(1);
	}
	for(index=0; index<2 ;index++)
		tempModuleName[index]=moduleName[strlen(moduleName)+(index-2)];
	tempModuleName[index]='\0';
	

	sprintf(command, "SELECT id from testsID where testID=\"%s\"", tempModuleName);
	
	if(!mysql_query(&mysql, command))
	{
		result = mysql_store_result(&mysql);
		if((row = mysql_fetch_row(result)))
		{
			strcpy(testsId, row[0]); // taking the test name id
			mysql_free_result(result);
		}
		else// Wrong module name.
		{
			sprintf(errorMsg, "WIS to TileComm Error: Wrong module name -> '%s'", moduleName);
			macroResults.addError(errorMsg);
			mysql_close(&mysql);
			return(1);
		}
	}
	else // if the query wasn't successful.
	{
		sprintf(errorMsg, "MySQL Error 5: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		mysql_close(&mysql);
		return(1);
	}
	sprintf(command, "SELECT id from tcaRun where (idAtlasId =\"%s\" AND idTestId=\"%s\" AND runNumber=\"%s\")", idatlasId, testsId, runNumber);
	if(!mysql_query(&mysql, command))
	{
		result = mysql_store_result(&mysql);
		if((row = mysql_fetch_row(result)))
		{
			
			strcpy(runId, row[0]); // taking the Run id
			mysql_free_result(result);
		}
		else// Wrong run number.
		{
			sprintf(errorMsg, "WIS to TileComm Warning: Inexistent run number in TileComm DB -> '%s'. Plot cannot be uploaded to the database", runNumber);
			macroResults.addWarning(errorMsg);
			mysql_close(&mysql);
			return(1);
		}
	}
	else // if the query wasn't successful.
	{
		sprintf(errorMsg, "MySQL Error 6: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		mysql_close(&mysql);
		return(1);
	}
	/*************************************************************************/
	/*************************************************************************
	**     INSERTING DATA IN TILECOMM DB						**
	*************************************************************************/
	
	for(index=0; shortFilename[index]!= '.'; index++)
	{
		if(shortFilename[index]!='_')
		{
			partsShortFilename[index2][index3]=shortFilename[index];
			index3++;
		}
		else
		{
			partsShortFilename[index2][index3]='\0';
			index2++;
			index3=0;
		}
	}

	partsShortFilename[index2][index3]='\0';
	
	switch(index2)
	{
		case 2:
		if (strcmp(partsShortFilename[2],"MonoCis")==0)
			plotTitleId = 6;
		else if (strcmp(partsShortFilename[2],"MonoCIS")==0)
			plotTitleId = 6;
		else if (strcmp(partsShortFilename[2],"PED")==0)
			plotTitleId = 1;
		else if (strcmp(partsShortFilename[2],"Ped")==0)
			plotTitleId = 1;
		else if (strcmp(partsShortFilename[2],"ped")==0)
			plotTitleId = 1;
		else if (strcmp(partsShortFilename[2],"baseline")==0)
			plotTitleId = 2;
		else if (strcmp(partsShortFilename[2],"DeadChannels")==0)
			plotTitleId = 3;
		else if (strcmp(partsShortFilename[2],"PulseHeight")==0)
			plotTitleId = 11;
		else if (strcmp(partsShortFilename[2],"CIS")==0)
			plotTitleId = 12;
		else
			plotTitleId = 13;
		break;
		case 3:
		if (strcmp(partsShortFilename[3],"PulseHeight")==0)
		{
			if (strcmp(partsShortFilename[2],"lo")==0)
				plotTitleId = 5;
			else if (strcmp(partsShortFilename[2],"hi")==0)
				plotTitleId = 4;
		}	
	
		else if (strcmp(partsShortFilename[2],"calib")==0)
			plotTitleId = 12;
		break;

		case 4:
		if (strcmp(partsShortFilename[0],"corr")==0)
		{
			if (strcmp(partsShortFilename[4],"lo")==0)
				plotTitleId = 8;
			else if (strcmp(partsShortFilename[4],"hi")==0)
				plotTitleId = 7;
		}
		else if (strcmp(partsShortFilename[0],"cova")==0)
		{
			if (strcmp(partsShortFilename[4],"lo")==0)
				plotTitleId = 10;
			else if (strcmp(partsShortFilename[4],"hi")==0)
				plotTitleId = 9;
		}
		else if (strcmp(partsShortFilename[2],"ped")==0)
				plotTitleId = 13;
		else if (strcmp(partsShortFilename[4],"linearity")==0)
				plotTitleId = 13;
	
		break;
		default:
			plotTitleId = 13;
	}
			
	if (!existPlot)
		sprintf(command, "insert into tcaPlot (name, runId, url, titleId) values (\"%s\",\"%s\", \"%s\", \"%i\")" , shortFilename, runId, filename, plotTitleId);
	else // Update the plot. We are only inserting the URL.
		sprintf(command, "update tcaPlot set url=\"%s\" where name=\"%s\" and runId=\"%s\"" , filename, shortFilename, runId);
	if(mysql_query(&mysql, command))
	{
		sprintf(errorMsg, "MySQL Error 7: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		mysql_close(&mysql);
		return(1);
	}
	mysql_close(&mysql);
	return(0);
}

