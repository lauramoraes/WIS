#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "TTileCommRun.h"

using namespace std;

void createLinkPerModule(const char *filename, const char *moduleName)
{
	TString command = "ln -s ";
	command += filename;
	command += " ";
	command += outputDirPerModule;
	command += "LB";
	command += moduleName;
	command += "/";
	command += basename(filename);
	system(command);
	return;
}

