#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <sstream>
#include <cassert>
#include <ctype.h>
#include <TROOT.h>
#include <TUnixSystem.h>
#include <TChain.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TString.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TFile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <shift.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLeafI.h>
#include <TLine.h>
//#include <stdlib.h>
#include "TWISMacroResult.h"


using namespace std;

		/*********Some definitions**************/
const TString defaultNtuplesDir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
const TString outputDir = "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5_local/";
//const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/";
const TString outputWebDir = "http://tcws.web.cern.ch/tcws/WIS/results/team5_local/";
		/***************************************/


		/***********Analysis Functions(MACROS)**********/
void monplots(TString runno, TString filename, TString module, TWISMacroResult & results);
void pedplots(TString runno, TString filename, TString module, TWISMacroResult & results);
		/***********************************************/

TWISMacroResult results;


int main(int argc, char *argv[])
{
	TString runNumber, runType, filename, moduleName;
	TString webFileDir, filenameps, filenamepng;
	Int_t retCode;
	struct stat statBuf;

	if (argc < 4)
	{
		//cout << "Usage:"<< endl << "    ./analiseRuns <runNumber> <runType> <moduleName> [<filename>]" << endl;
		//cout << "runType -> Use \"Ped\" for pedestal runs or \"MonoCis\" for monocis runs" << endl;
		results.addError("Usage:\n    ./analiseRuns <runNumber> <runType> <moduleName> [<filename>]\nrunType -> Use \"Ped\" for pedestal runs or \"MonoCis\" for monocis runs");
		return 0;
	}
	
	runNumber = argv[1];
	runType = argv[2];
	if (strlen(argv[3]) < 5)
	{
		moduleName = "LB";
		moduleName += argv[3];
	}
	else
		moduleName= argv[3];
	moduleName.ToUpper();

	if((runType.CompareTo("Ped")) &&  (runType.CompareTo("MonoCis")))
	{
		//cout << "runType -> Use \"Ped\" for pedestal runs or \"MonoCis\" for monocis runs"<< endl;
		results.addError("runType -> Use \"Ped\" for pedestal runs or \"MonoCis\" for monocis runs");
		return 0;
	}

	if (argc == 4)
	{
		filename  = "tiletb_";
		filename += runNumber;
		filename += "_";
		filename += runType;
		filename += ".0.root";
		TString dir = "/work/tiletransfer/wis/";
		TString aux = dir + filename;
		retCode = stat((char*)aux.Data(), &statBuf);
		if (retCode < 0)
		{
			dir = "/castor/cern.ch/user/t/tilebeam/commissioning/";
			aux = dir + filename;
			retCode = rfio_stat((char*)aux.Data(), &statBuf);
			if (retCode < 0)
			{
				//cout << "<h3>Ntuple does not exist in CASTOR default directory.</h3>" << endl;
				results.addError("Ntuple does not exist in CASTOR default directory.");
				return 0;
			}
			else
				filename = "rfio:" + aux;
		}
		else
		{
			filename = aux;
		}
	}
	else
		filename = argv[4];
/*
	cout << "<html><head>" << endl;
	cout << "	<link href=\"../css/standard.css\" type=\"text/css\" rel=\"stylesheet\" />" << endl;
	cout << "<title>WIS - Check Run Data</title>" <<endl;
	cout << "</head><body>" << endl;
	cout << endl << "<br /><b>Run Number:  " << runNumber << "</b><br />" << endl << endl;
	cout << endl << "<b>Module Name: " << moduleName  << "</b><br />" << endl << endl;
	cout << endl << "<b>Run Type:    " << runType  << "</b><br /><br />" << endl << endl;
*/
	results.setMacroName("TileC Step");
	results.setMacroVersion("2.0");
	results.setParameter("Run Number", runNumber);
	results.setParameter("Module Name", moduleName);
	results.setParameter("Run Type", runType);

/********************Analysis Functions************************/
/************This functions will generate Plots****************/

	TString module; module.Resize(3); module[0] = moduleName[2]; module[1] = moduleName[3]; module[2] = moduleName[4];
	if(!(runType.CompareTo("Ped")))//Pedestal runs
	{
		pedplots(runNumber, filename, module, results);
		webFileDir = outputWebDir +"pedestal/LB" + module + "/r" + runNumber+"_"+module+"_Ped_v2.png";
		//cout << "<img src=\"" << webFileDir<< "\" /><br /><br />" << endl;
		results.addPlot("", webFileDir);
	}
	else if(!(runType.CompareTo("MonoCis")))//Monocis Runs
	{
		monplots(runNumber, filename, module, results);
		/*webFileDir = outputWebDir +"MonoCis/LB" + moduleName + "/r"+  runNumber+"_"+moduleName+"_MonoCis.png";
		//cout << "<img src=\"" << webFileDir<< "\" /><br /><br />" << endl;
		results.addPlot("", webFileDir);*/
	}
		

/**********************************************************/

//	cout << "</body></html>" << endl;

	results.out();
	
	return 0;
}

