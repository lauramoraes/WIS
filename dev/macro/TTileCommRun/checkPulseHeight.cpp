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

#include "wisMacroFunctions.h"
#include "TWISMacroResult.h"
//#include "wis2Tilecomm.h"

using namespace std;

//==== some definitions ====
const Int_t Nmodules  = 128;
const Int_t Nbarrels  = 4;
const TString defaultNtuplesDir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
const TString outputDir = "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/temp/team5/";
const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/temp/team5/";
const TString outputWebDir = "http://tcws.web.cern.ch/tcws/WIS/results/temp/team5/";

//TROOT root("TTileCommRun", "Analyze TileCal Commissioning runs");

TWISMacroResult results;


int main(int argc, char *argv[])
{
	TString runNumber, runType, filename;
	Int_t barrelCode, retCode;
	Int_t iModule;
	TCanvas *chi, *clo;
	struct stat statBuf;

	//cout << getenv("PATH") << endl;
	//cout << getenv("ROOTSYS") << endl;

	if (argc < 3)
	{
		//cout << "Usage:"<< endl << "    ./checkPulseHeight <runNumber> <runType> [<filename>]" << endl;
		results.addError("Usage:\n    ./checkPulseHeight <runNumber> <runType> [<filename>]");
		return 0;
	}

	runNumber = argv[1];
	runType = argv[2];

	if (argc > 3)
		filename = argv[3];
	else
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
	//cout<<filename<<endl;
	TChain *t = new TChain("TileRec/h1000");
	t->Add(filename);

	Int_t nevt = t->GetEntries();

	results.setMacroName("Pulse Height");
	results.setParameter("Run Number", runNumber);
	results.setParameter("Number of Events", nevt);

/**********************************************************/

	Int_t i, j, k;

    /******************* Set names ********************/
	TString moduleName[Nmodules];
	TString barrelName[Nbarrels];

	// barrel names
	barrelName[0] = "A";
	barrelName[1] = "C";
	barrelName[2] = "EBA";
	barrelName[3] = "EBC";

	// module names
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 64; j++)
		{
			moduleName[i*64 + j] = barrelName[i];
			moduleName[i*64 + j] += (j + 1);
		}
	}
	/**************************************************/

	Int_t SampleHi[Nmodules][48][7];
	Int_t SampleLo[Nmodules][48][7];
	TBranch *b_SampleHi[Nmodules];
	TBranch *b_SampleLo[Nmodules];
	bool moduleInNtuple[Nmodules];

	TString branchName;
	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		branchName = "Sample" + moduleName[iModule] + "hi";
		t->SetBranchAddress(branchName, SampleHi[iModule]);
		b_SampleHi[iModule] = t->GetBranch(branchName.Data());

		branchName = "Sample" + moduleName[iModule] + "lo";
		t->SetBranchAddress(branchName, SampleLo[iModule]);
		b_SampleLo[iModule] = t->GetBranch(branchName.Data());

		if (!b_SampleHi[iModule])
			moduleInNtuple[iModule] = false;
		else
			moduleInNtuple[iModule] = true;
	}

	TH1F *hhi[Nmodules][48];
	TH1F *hlo[Nmodules][48];
	TString hnamehi, htitlehi, hnamelo, htitlelo;

	for (j = 0; j < 48; j++)
	{
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
	
			hnamehi = "chhi";
			hnamehi +=  j;
			hnamehi += moduleName[iModule];
			htitlehi = "Chanhi ";
			htitlehi += j;
			htitlehi += " " + moduleName[iModule];
	
			hhi[iModule][j]=new TH1F(hnamehi, htitlehi, 1024, 0, 1023);
	
			hnamelo = "chlo";
			hnamelo += j;
			hnamelo += moduleName[iModule];
			htitlelo = "Chanlo ";
			htitlelo += j;
			htitlelo += " " + moduleName[iModule];
		
			hlo[iModule][j]=new TH1F(hnamelo, htitlelo, 1024, 0, 1023);
		}
	}

	Int_t minevt = 16000;
	for (i = 16000; i < nevt; i++)
	{
		t->GetEntry(i);

		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;

			for (j = 0; j < 48; j++)
			{
				for (k = 0; k < 7; k++)
				{
					hhi[iModule][j]->Fill(SampleHi[iModule][j][k]);
					hlo[iModule][j]->Fill(SampleLo[iModule][j][k]);
				}
			}
		}
	}
	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		if (!moduleInNtuple[iModule])
			continue;

		chi = new TCanvas("chi", "High Gain", 900, 900);
		chi->Divide(6,8);

		clo = new TCanvas("clo", "Low Gain", 900, 900);
		clo->Divide(6,8);

		for(j = 0; j < 48; j++)
		{
			chi->cd(j+1);
			gPad->SetLogy();
			hhi[iModule][j]->Draw();
			clo->cd(j+1);
			gPad->SetLogy();
			hlo[iModule][j]->Draw();
		}

		TString outFilePShi = outputDir + "CIS/LB" + moduleName[iModule] + "/r" + runNumber + "_" + "LB" + moduleName[iModule] + "_hi_PulseHeight.ps";
		TString outFilePSlo = outputDir + "CIS/LB" + moduleName[iModule] + "/r" + runNumber + "_" + "LB" + moduleName[iModule] + "_lo_PulseHeight.ps";
		TString outFilePNGhi= outputDir + "CIS/LB" + moduleName[iModule] + "/r" + runNumber + "_" + "LB" + moduleName[iModule] + "_hi_PulseHeight.png";
		TString outFilePNGlo= outputDir + "CIS/LB" + moduleName[iModule] + "/r" + runNumber + "_" + "LB" + moduleName[iModule] + "_lo_PulseHeight.png";

		chi->Print(outFilePShi);
		clo->Print(outFilePSlo);

		chi->Print(outFilePNGhi);
		clo->Print(outFilePNGlo);
		
		TString plotTitle;
		TString plotFilename;

		TString completeModuleName = "LB" + moduleName[iModule];

		plotTitle = "Module LB" + moduleName[iModule] + " High Gain:";
		plotFilename = outputWebDir + "CIS/" + completeModuleName + "/" + basename(outFilePNGhi.Data());
		results.addPlot(plotTitle, plotFilename);

	//	wis2Tilecomm(runNumber.Data(), completeModuleName.Data(), plotFilename.Data(), results);
		plotFilename = outputWebDir + "CIS/" + completeModuleName + "/" + basename(outFilePShi.Data());
	//	wis2Tilecomm(runNumber.Data(), completeModuleName.Data(), plotFilename.Data(), results);

	//	plotTitle = "Module LB" + moduleName[iModule] + " Low Gain:";
		plotFilename = outputWebDir + "CIS/" + completeModuleName + "/" + basename(outFilePNGlo.Data());
	//	results.addPlot(plotTitle, plotFilename);

	//	wis2Tilecomm(runNumber.Data(), completeModuleName.Data(), plotFilename.Data(), results);
		plotFilename = outputWebDir + "CIS/" + completeModuleName + "/" + basename(outFilePSlo.Data());
	//	wis2Tilecomm(runNumber.Data(), completeModuleName.Data(), plotFilename.Data(), results);

		delete chi;
		delete clo;
	}

/**********************************************************/

	results.out();
	
	return 0;
}
