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
#include <TGraph.h>
#include <TLeafI.h>
#include <TLine.h>
//#include <stdlib.h>

//#include "TTileCommMonoGainRun.h"
#include "TWISMacroResult.h"
#include "wis2Tilecomm.h"

using namespace std;

//TROOT root("TTileCommRun", "Analyze TileCal Commissioning runs");

//==== some definitions ====
const Int_t Nmodules  = 128;
const Int_t Nchannels = 48;
const Int_t NPMTs     = 48;
const Int_t Nsamples  = 7;
const Int_t Ngains    = 2;
const Int_t Nbarrels  = 4;
const Int_t NtileDMUs = 16;
const TString defaultNtuplesDir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
const TString outputDir = "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5_local/";
//const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/";
const TString outputWebDir = "http://tcws.web.cern.ch/tcws/WIS/results/team5_local/";


void Initialize(const char *runNumber, const char *runType, const char *treeName = "TileRec/h1000", const char *filename = "");

void Baseline();


//==== Global Variables ====
TChain *fChain;
TString fRunNumber;
TString fRunType;
Long64_t numEvents;
Int_t gainsNumber;

// histos
//TH1F *histoBitStat[Nmodules][Nchannels][Ngains];

// stores available modules in this ntuple (0 to 64)
TH1F *histoModules;

bool moduleInNtuple[Nmodules];

// Names
TString moduleName[Nmodules];
TString gainName[Ngains];
TString channelName[Nchannels];
TString PMTName[Nchannels];
TString barrelName[Nbarrels];
TString tileDMUName[NtileDMUs];

// Leaves
Int_t EvTime;
Int_t Run;
Int_t EVT;
Int_t Trig;
Int_t Cispar[16];

// Branchs
TBranch *b_EvTime;
TBranch *b_Run;
TBranch *b_EVT;
TBranch *b_Trig;
TBranch *b_Cispar;

// Leaves
Float_t Efit[Nmodules][NPMTs];
Int_t Sample[Nmodules][Nchannels][9];

// Branchs
TBranch *b_Efit[Nmodules];
TBranch *b_Sample[Nmodules];

vector< vector<Float_t> > PMTEnergies;

TWISMacroResult results;


int main(int argc, char *argv[])
{
	//TTileCommMonoGainRun *Run;
	TString runNumber, runType, filename;
	Int_t barrelCode, retCode;
	struct stat statBuf;
	
	//cout << getenv("PATH") << endl;
	//cout << getenv("ROOTSYS") << endl;
	
	if (argc < 3)
	{
		//cout << "Usage:"<< endl << "    ./checkDeadChannels <runNumber> <runType> [<filename>]" << endl;
		results.addError("Usage:\n    ./checkBaseline <runNumber> <runType> [<filename>]");
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
	
	//cout << filename << endl;
	
	Initialize(runNumber.Data(), runType.Data(), "TileRec/h1000", filename.Data());
/*	
	if (argc > 4)
	{
		Run->Show(atoi(argv[4]));
		exit(0);
	}
	
	Long64_t numEvents = Run->GetChain()->GetEntries();
*/	

	results.setMacroName("Baseline");
	results.setParameter("Run Number", runNumber);
	results.setParameter("Run Type", runType);
	results.setParameter("Number of Events", numEvents);

	Baseline();
	
	results.out();
	
	return 0;
}


void Initialize(const char *runNumber, const char *runType, const char *treeName, const char *filename)
{
	Int_t i, j;
	TString fileName;
	
	fChain = new TChain(treeName);
	
	fRunNumber = runNumber;
	fRunType = runType;
	
	if (strcmp(runType, "MonoCis") == 0 || strcmp(runType, "Phys") == 0 || strcmp(runType, "Led") == 0)
		gainsNumber = 1;
	else
		gainsNumber = 2;

	if (strcmp(filename, "") == 0)
	{
		fileName =  defaultNtuplesDir;
		fileName += "tiletb_";
		fileName += runNumber;
		fileName += "_";
		fileName += runType;
		fileName += ".0.root";
	}
	else
	if (strcmp(filename, "local") == 0)
	{
		fileName =  "/work/tiletransfer/wis/";
		fileName += "tiletb_";
		fileName += runNumber;
		fileName += "_";
		fileName += runType;
		fileName += ".0.root";
	}
	else
		fileName = filename;
	//cout << fileName << endl;
	
	fChain->Add(fileName.Data());
	
	/******************* Set names ********************/
	// barrel names
	barrelName[0] = "A";
	barrelName[1] = "C";
	barrelName[2] = "EBA";
	barrelName[3] = "EBC";

	// module names
	for(i = 0; i < 2; i++)
		for(j = 0; j < 64; j++)
		{
			moduleName[i*64 + j] = barrelName[i];
			moduleName[i*64 + j] += (j + 1);
			moduleInNtuple[i*64 + j] = true;
			//cout << "Module nr = " <<  i*64 + j << "  and module name is = " << moduleName[i*64 + j] << endl;
		}

	// TileDMU names
	for(i = 0; i < NtileDMUs; i++)
	{ 
		tileDMUName[i] = "TileDMU ";
		tileDMUName[i] += (i + 1);
	} 


	// channel names
	for(i = 0; i < Nchannels; i++)
	{
		channelName[i] = "Channel ";
		channelName[i] += (i + 1);
	}

	// PMT names
	for(i = 0; i < NPMTs; i++)
	{
		PMTName[i] = "PMT ";
		PMTName[i] += (i + 1);
	}

	// gain names
	if (strcmp(runType, "MonoCis") == 0 || strcmp(runType, "Phys") == 0 || strcmp(runType, "Led") == 0)
	{
		gainName[0] = "";
		gainName[1] = "";
	}
	else
	{
		gainName[0] = "lo";
		gainName[1] = "hi";
	}
	/**************************************************/

	/********************* Set branch addresses and get branch pointers ********************/
	//fCurrentTree = -1;
	//fChain->SetMakeClass(1);

	fChain->SetBranchAddress("EvTime", &EvTime);
	fChain->SetBranchAddress("Run", &Run);
	fChain->SetBranchAddress("Evt", &EVT);
	fChain->SetBranchAddress("Trig", &Trig);
	fChain->SetBranchAddress("Cispar", Cispar);

	b_EvTime = fChain->GetBranch("EvTime");
	b_Run = fChain->GetBranch("Run");
	b_EVT = fChain->GetBranch("Evt");
	b_Trig = fChain->GetBranch("Trig");
	b_Cispar = fChain->GetBranch("Cispar");


	if (strcmp(runType, "MonoCis") != 0 && strcmp(runType, "Led") != 0 && strcmp(runType, "Phys") != 0)
	{
		//cout << "Error: Runtype should be mono gain: MonoCis, Led, or Phys" << endl;
		results.addError("Error: Runtype should be mono gain: MonoCis, Led, or Phys");
		exit(0);
	}
	
	// Set branch addresses and get branch pointers
	TString branchName;
	for(Int_t iModule = 0; iModule < Nmodules; iModule++)
	{
		if (moduleInNtuple[iModule])
		{
			branchName = "Efit" + moduleName[iModule];
			fChain->SetBranchAddress(branchName, Efit[iModule]);
			b_Efit[iModule] = fChain->GetBranch(branchName.Data());
			if (!b_Efit[iModule])
				moduleInNtuple[iModule] = false;
			else
			{
				moduleInNtuple[iModule] = true;
				//cout << "InNtuple!" << moduleInNtuple[iModule] << endl;
			}

			branchName = "Sample" + moduleName[iModule];
			fChain->SetBranchAddress(branchName, Sample[iModule]);
			b_Sample[iModule] = fChain->GetBranch(branchName.Data());
		}
	}

	for(Int_t iPMT = 0; iPMT < NPMTs; iPMT++)
	{

		vector<Float_t> auxPMT;
		for(Int_t iModule = 0; iModule < Nmodules; iModule++)
			auxPMT.push_back(0.0);

		PMTEnergies.push_back(auxPMT);
	}

	numEvents = fChain->GetEntries();

} // end of Initialize


void Baseline()
{
	Int_t iEvt, iModule, j;
	Float_t warningLevel = 0.8;
	
	Double_t mean[Nmodules][48]={0}, rms[Nmodules][48]={0}, chan[Nmodules][48];

	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
				
			for(j = 0; j < 48; j++)
				mean[iModule][j] += (Double_t)Sample[iModule][j][0]/(Double_t)numEvents;
		}
	}
//	for (j = 0; j < 48; j++)
//		mean[j] = mean[j]/(Double_t)numEvents;

	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
				
			for (j = 0; j < 48; j++)
				rms[iModule][j] += TMath::Power((Sample[iModule][j][0] - mean[iModule][j]), 2)/numEvents;
		}
	}
//	for (j = 0; j < 48; j++)
//		rms[j] = rms[j]/numEvents;

	TString outdir = outputDir;
	TString typeDir;
	if (fRunType == "Led")
	{
		outdir += "LED/";
		typeDir = "LED/";
	}
	else
	if(fRunType == "MonoCis")
	{
		outdir += "MonoCis/";
		typeDir = "MonoCis/";
	}
	else
	if(fRunType == "Phys")
	{
		outdir += "physics/";
		typeDir = "physics/";
	}

	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		if (!moduleInNtuple[iModule])
			continue;
			
		for (j = 0; j < 48; j++)
		{
			rms[iModule][j] = TMath::Sqrt(rms[iModule][j]);
			chan[iModule][j] = j;
		}

		TString pref = "Run " + fRunNumber + " Mod LB" + moduleName[iModule] + ":";

		TGraph *grm = new TGraph(48, chan[iModule], mean[iModule]);
		grm->SetMarkerStyle(21);
		grm->SetTitle(pref + " Mean First Sample");
		grm->GetXaxis()->SetTitle("Channel number");
		grm->GetYaxis()->SetTitle("ADC counts");

		TGraph *grr = new TGraph(48, chan[iModule], rms[iModule]);
		grr->SetMarkerStyle(21);
		grr->SetTitle(pref + " RMS first Sample");
		grr->GetXaxis()->SetTitle("Channel number");
		grr->GetYaxis()->SetTitle("ADC counts");
		
		TCanvas *c1 = new TCanvas();
		c1->Divide(1, 2);
		c1->cd(1);
		grm->Draw("AP");
		c1->cd(2);
		grr->Draw("AP");
		
		TString moduleDir = "LB" + moduleName[iModule] + "/";
		
		TString outFilePS = outdir + moduleDir + "r" + fRunNumber +"_" + moduleName[iModule] + "_baseline.ps";
		TString outFilePNG = outdir + moduleDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_baseline.png";
		c1->Print(outFilePS);
		c1->Print(outFilePNG);
		/*
		outFilePS = standOutputDir + "r" + fRunNumber +"_" + moduleName[iModule] + "_baseline.ps";
		outFilePNG = standOutputDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_baseline.png";
		
		c1->Print(outFilePS);
		c1->Print(outFilePNG);
		*/

		TString plotTitle = "Module LB" + moduleName[iModule];
		TString plotFilename = outputWebDir + typeDir + moduleDir + basename(outFilePNG.Data());
		TString plotPsFilename = outputWebDir + typeDir + moduleDir + basename(outFilePS.Data());
		results.addPlot(plotTitle, plotFilename);

		TString module = "LB" + moduleName[iModule];
		wis2Tilecomm(fRunNumber.Data(), module.Data(), plotFilename.Data(), results);
		wis2Tilecomm(fRunNumber.Data(), module.Data(), plotPsFilename.Data(), results);
		
		delete c1;
		delete grm;
		delete grr;

	}

}
