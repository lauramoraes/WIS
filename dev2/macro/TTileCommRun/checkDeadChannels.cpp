#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cassert>
#include <ctype.h>
#include <TROOT.h>
#include <TUnixSystem.h>
#include <TChain.h>
#include <TLine.h>
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
#include <vector>
//#include <stdlib.h>
#include "wis2Tilecomm.h"
//#include "TTileCommRun.h"
#include "TWISMacroResult.h"

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

void DeadChannels();

void buildHTMLTable(TString &table, const vector< vector<Float_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows, Bool_t *selectedCols, Float_t warningLevel);


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
		results.addError("Usage:\n    ./checkDeadChannels <runNumber> <runType> [<filename>]");
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

	results.setMacroName("Dead Channels");
	results.setParameter("Run Number", runNumber);
	results.setParameter("Run Type", runType);
	results.setParameter("Number of Events", numEvents);

	DeadChannels();
	
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
		cout << "Class constructor error: Runtype should be mono gain: MonoCis, Led, or Phys" << endl;
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


void DeadChannels()
{
	Int_t iEvt, iModule, iPMT;
	Float_t warningLevel = 0.8;
	
	gStyle->SetOptStat(0);

	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
				
			for (iPMT = 0; iPMT < NPMTs; iPMT++)
			{
				if (iPMT == 31 || iPMT == 32 || iPMT == 43) // 3 empty PMT/channels: 32, 33 and 44
					continue;
				//cout << Efit[iModule][iPMT]/float(numEvents) << " ";
				//hchannel.Fill(iPMT + 1, Efit[iModule][iPMT]/float(numEvents));
				PMTEnergies.at(iPMT).at(iModule) += Efit[iModule][iPMT]/float(numEvents);
			}
		}

	}

	TCanvas c1("c1", "", 600, 600);

//	cout << "<table><tr><td valign=\"top\">" << endl;
//	cout << "<ul>" << endl;
	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		if (!moduleInNtuple[iModule])
			continue;
			
		TString title = "Energy Module " + moduleName[iModule] + " " + fRunType + " Run " + fRunNumber;
		TH1F hchannel("hchannel", title.Data(), NPMTs, 1, NPMTs + 1);
		hchannel.SetXTitle("PMT number");
		hchannel.SetYTitle("Average energy per event (pC)");
		hchannel.SetFillColor(kBlue);
		hchannel.SetMarkerStyle(21);    

		Int_t totalWarnings = 0;
		
		for (iPMT = 0; iPMT < NPMTs; iPMT++)
		{
			if (iPMT == 31 || iPMT == 32 || iPMT == 43) // 3 empty PMT/channels: 32, 33 and 44
				continue;
			//cout << Efit[iModule][iPMT]/float(numEvents) << " ";
			hchannel.Fill(iPMT + 1, PMTEnergies.at(iPMT).at(iModule));
			if (PMTEnergies.at(iPMT).at(iModule) < warningLevel)
				totalWarnings++;

		}
		
		Char_t text[50];
		hchannel.Draw("P");
		TLine warningLine(0, warningLevel, float(NPMTs), warningLevel);
		warningLine.SetLineColor(kRed);
		warningLine.SetLineWidth(3);
		warningLine.Draw();
		
		TString outFilePS, outFilePNG;
		TString moduleDir = "LB" + moduleName[iModule] + "/";
		if (strcmp(fRunType, "Led") == 0)
		{
			outFilePS  = outputDir + "LED/" + moduleDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.ps";
			outFilePNG = outputDir + "LED/" + moduleDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.png";
		}
		else
		if (strcmp(fRunType, "MonoCis") == 0)
		{
			outFilePS  = outputDir + "MonoCis/" + moduleDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.ps";
			outFilePNG = outputDir + "MonoCis/" + moduleDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.png";
		}
		
		c1.SaveAs(outFilePS.Data());
		c1.SaveAs(outFilePNG.Data());
		
		//printf("\tNumber of channels with low intensity %d\n", totalWarnings);
		
		TString plotTitle = ""; plotTitle += totalWarnings; plotTitle += " PMTs with low intensity in module " + moduleName[iModule];
		
		TString plotFilename, plotPsFilename;
		if (strcmp(fRunType, "Led") == 0)
		{
			plotFilename = outputWebDir + "LED/" + moduleDir + basename(outFilePNG.Data());
			plotPsFilename = outputWebDir + "LED/" + moduleDir + basename(outFilePS.Data());
		}
		else
		if (strcmp(fRunType, "MonoCis") == 0)
		{
			plotFilename = outputWebDir + "MonoCis/" + moduleDir + basename(outFilePNG.Data());
			plotPsFilename = outputWebDir + "MonoCis/" + moduleDir + basename(outFilePS.Data());
		}
		
		TString module = "LB" + moduleName[iModule];
		wis2Tilecomm(fRunNumber.Data(), module.Data(), plotFilename.Data(), results);
		wis2Tilecomm(fRunNumber.Data(), module.Data(), plotPsFilename.Data(), results);
		results.addPlot(plotTitle, plotFilename);
		
	}

//	cout << "</ul>" << endl;
//	cout << "</td><td valign=\"top\">" << endl;

	TString rowsTitle = "Module";
	TString colsTitle = "PMT";
	TString table;
//	cout << "<b>Average energy per event:</b><br />" << endl;
	buildHTMLTable(table, PMTEnergies, (const Int_t)NPMTs, (const Int_t)Nmodules, (const TString)colsTitle, (const TString)rowsTitle, (TString*)PMTName, (TString*)moduleName, NULL, moduleInNtuple, warningLevel);
//	cout << table << endl;
	results.addTable("Average energy per event:", table);

//	cout << "</td></tr></table><br />" << endl;

}


void buildHTMLTable(TString &table, const vector< vector<Float_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows, Bool_t *selectedCols, Float_t warningLevel)
{
	Int_t i, j;
	
	//table  = "<table class=\"main\">\n<tr><th></th>";
	table  = "<table class=\"main\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<tr><th></th>";
	table += "<th colspan=\"";
	table += ncols;
	table += "\">";
	table += colsTitle;
	table += "</th></tr>\n";
	table += "<tr><th>";
	table += rowsTitle;
	table += "</th>";
/*
	cout << (matrix + (48*ncols) + 0) << endl;
	cout << &NSlinkErrors[48][0] << endl;
*/
	for (j = 0; j < ncols; j++)
	{
		//if (j == 31 || j == 32 || j == 43) // 3 empty PMT/channels: 32, 33 and 44
		//	continue;

		if (selectedCols && !selectedCols[j])
			continue;

		table += "<th>";
		table += colNames[j];
		table += "</th>";
	}
	table += "</tr>\n";

	for (i = 0; i < nrows; i++)
	{
		if (selectedRows && !selectedRows[i])
			continue;

		if (i == 31 || i == 32 || i == 43) // 3 empty PMT/channels: 32, 33 and 44
			continue;
		
		table += "<tr><th>";
		table += rowNames[i];
		table += "</th>";
		for (j = 0; j < ncols; j++)
		{
			if (selectedCols && !selectedCols[j])
				continue;

			//if (j == 31 || j == 32 || j == 43) // 3 empty PMT/channels: 32, 33 and 44
			//	continue;

			table += "<td align=\"right\">";
			if (matrix.at(i).at(j) < warningLevel)
			{
				table += "<font color=\"red\"><b>";
				table += matrix.at(i).at(j);
				table += "</b></font>";
			}
			else
				table += matrix.at(i).at(j);
			table += "</td>";
		}
		table += "</tr>\n";

	}
	table += "</table>\n";
}

