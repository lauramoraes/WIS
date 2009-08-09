#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
using namespace std;
int main(void)
{
	
printf("%s%c%c\n",
"Content-Type:text/html;charset=iso-8859-1",13,10);	
	cout<<get_current_dir_name()<<endl;
	ofstream testefile("/afs/cern.ch/user/t/tilecom/public/www/WIS/dev/macro/TTileCommRun/tmp/testefile.log");
	testefile<<"Hello world!!Sobreescrito"<<endl;
    return EXIT_SUCCESS;
}
