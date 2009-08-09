#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <cstdio>
#include <cstdlib>

#include "/usr/include/mysql/mysql.h"

using namespace std;

int main (int argc, char** argv)
{
	DIR *pDir, *pDir2;
	struct dirent *pent, *pent2;
	string directory;
	string localhost, user, password, db1, errorMsg;
        MYSQL mysql;
        MYSQL_RES *result;
        MYSQL_ROW row;
	int i=0;
	pDir = opendir(".");

	localhost = "atlasdev1";
	password =  "cOAnAd26";
	user = "lodi";
	db1 = "tbanalysis";

	cout<<"connecting to MySql"<<endl;
	if(!mysql_init(&mysql))
	{
		cout<< "MySQL Error 1:"<< mysql_error(&mysql)<<endl;
		return(1);
	}
	if(!mysql_real_connect(&mysql, localhost.c_str(), user.c_str(), password.c_str(), db1.c_str(), 0, NULL, 0))
	{
		cout<< "MySQL Error 1:"<< mysql_error(&mysql)<<endl;
		return(1);
	}

	cout<<"connected to MySql"<<endl;
	if(!pDir)
	{
		cout<<"opendir failure"<<endl;
		exit (1);
	}
	while((pent = readdir(pDir)))
	{
		if(pent->d_name[0]=='2' and strlen(pent->d_name)==8)
		{
			cout<<pent->d_name<<endl;
			directory= "./" + string(pent->d_name);
			pDir2 = opendir( directory.c_str());
			if(!pDir2)
			{
				cout<<"opendir failure"<<endl;
				exit (1);
			}
			while((pent2 = readdir(pDir2)))
			{
				if(strstr(pent2->d_name, "root"))
				{
					i++;
					cout<<"\t"<<pent2->d_name<<endl;
				}
			}
			closedir(pDir2);

		}
	}
	closedir(pDir);
	cout<<i<<endl;
	exit (0);
}

