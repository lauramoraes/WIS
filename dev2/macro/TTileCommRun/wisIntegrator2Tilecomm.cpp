#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/usr/include/mysql/mysql.h"
#include "TWISMacroResult.h"

/* C to MySQL Program */
int wisIntegrator2Tilecomm(const char* plotFilename, char const *filename, TWISMacroResult & macroResults)
{
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char localhost[100], user[25], password[25], db1[100];
	char command[1024];
	char errorMsg[1024];
	int existPlot = 0, id, idplot;

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

	sprintf(command, "SELECT id from tcaIntegratorPlot where url=\"%s\"", plotFilename);
	if(!mysql_query(&mysql, command))
	{
		result = mysql_store_result(&mysql);
		if((row= mysql_fetch_row(result)))//This plot already exists in Tilecomm DB.
		{
			existPlot = 1; // plot will be updated, instead of inserted
			idplot =strtol(row[0],NULL,10);
		}
		else
		{
			sprintf(command, "select id from wisIntegratorMacro where filename=\"%s.root\"" , filename);
			if(!mysql_query(&mysql, command))
			{	
				result = mysql_store_result(&mysql);
				if((row = mysql_fetch_row(result)))//taking id test
					id =strtol(row[0],NULL,10);
				else
				{					
                			sprintf(errorMsg, "MySQL Error 2: %s", mysql_error(&mysql));
                			macroResults.addError(errorMsg);
					return(0);
				}
			}
		}	
	}
	else
	{// if the query wasn't successful. 
		sprintf(errorMsg, "MySQL Error 3: %s", mysql_error(&mysql));
		macroResults.addError(errorMsg);
		mysql_close(&mysql);
		return(1);
	}

	/**********************************************************
	 **     INSERTING DATA IN TILECOMM DB						**
	 *************************************************************************/


	if (!existPlot)
		sprintf(command, "insert into tcaIntegratorPlot (url, idtest) values (\"%s\",\"%i\")" , plotFilename, id);
	else // Update the plot. We are only inserting the URL.
		sprintf(command, "update tcaIntegratorPlot set url=\"%s\" where id=\"%i\"" , plotFilename, idplot);
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

