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
#include <iostream>
#include <vector>
//#include <stdlib.h>

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
const TString outputDir = "/home/httpd/html/tile/info/lps/WIS/results/team5/";
const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/";
//const TString outputWebDir = "http://tileinfo.web.cern.ch/tileinfo/lps/WIS/results/team5/";
const TString outputWebDir = "http://cern.ch/TileCommWebSystem/WIS/results/team5/";
const TString outputDirPerModule = "/work/commissioning/summary/";


void Initialize(const char *runNumber, const char *runType, const char *treeName = "TileRec/h1000", const char *filename = "");

void checkCRC_DMUMask_BCID();

void buildHTMLTable(TString &table, const vector< vector<Int_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows);

void buildHTMLTable(TString &table, Int_t *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows);


//==== Global Variables ====
TChain *fChain;
TString fRunNumber;
TString fRunType;
Int_t  fCurrentTree;
Long64_t numEvents;
Int_t gainsNumber;

// histos
TH1F *histoPulseHeight[Nmodules][Nchannels][Ngains];
TH1F *histoBitStat[Nmodules][Nchannels][Ngains];

// stores available modules in this ntuple (0 to 64)
TH1F *histoModules;

TH2F *histoPulseShape[Nmodules][Nchannels][Ngains];

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
Int_t SlinkCRC[Nmodules][Ngains][2];
Int_t DMUMask[Nmodules][Ngains][2];
Int_t BCID[Nmodules][Ngains][NtileDMUs];

// Branchs
TBranch *b_EvTime;
TBranch *b_Run;
TBranch *b_EVT;
TBranch *b_Trig;
TBranch *b_Cispar;
TBranch *b_SlinkCRC[Nmodules][Ngains];
TBranch *b_DMUMask[Nmodules][Ngains];
TBranch *b_BCID[Nmodules][Ngains];

Int_t NSlinkErrors[Nmodules][Ngains];
Int_t NDMUMaskErrors[Nmodules][Ngains];
Int_t NBCIDErrors[Ngains][Nmodules][NtileDMUs];

vector< vector<Int_t> > SlinkErrors;
vector< vector<Int_t> > DMUMaskErrors;
vector< vector< vector<Int_t> > > BCIDErrors;

TWISMacroResult results;


int main(int argc, char *argv[])
{
//	TTileCommRun *Run;
	TString runNumber, runType, filename;
	Int_t barrelCode, retCode;
	struct stat statBuf;
	results.setMacroName("Check CRC, DMU Mask and BCID");
	//printf("entrou\n");
	//cout << getenv("PATH") << endl;
	//cout << getenv("ROOTSYS") << endl;
	
	if (argc < 3)
	{
		//cout << "Usage:"<< endl << "    ./checkCRC_DMU_BCID <runNumber> <runType> [<filename>]" << endl;
		results.addError("Usage:\n    ./checkCRC_DMU_BCID <runNumber> <runType> [<filename>]");
		results.out();
		return(0);
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
				cout << "<h3>Ntuple does not exist in CASTOR default directory.</h3>" << endl;
				results.addError("Ntuple does not exist in CASTOR default directory.");
				results.out();
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

	results.setMacroName("Check CRC, DMU Mask and BCID");
	results.setParameter("Run Number", runNumber);
	results.setParameter("Number of Events", numEvents);

	checkCRC_DMUMask_BCID();
	
	results.out();

	return 0;

}


void Initialize(const char *runNumber, const char *runType, const char *treeName, const char *filename)
{
	Int_t i, j;
	TString fileName;
	
	fChain = new TChain(treeName);
	//cout<<"Initialize"<<endl;
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
	//cout << "Passou daki" << endl;
	
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
		tileDMUName[i] = "";
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
	fCurrentTree = -1;
	fChain->SetMakeClass(1);

	//cout << "Passou daki2" << endl;
	fChain->SetBranchAddress("EvTime", &EvTime);
	fChain->SetBranchAddress("Run", &Run);
	fChain->SetBranchAddress("Evt", &EVT);
	fChain->SetBranchAddress("Trig", &Trig);
	fChain->SetBranchAddress("Cispar", Cispar);
	//cout << "Passou daki2.5" << endl;

	b_EvTime = fChain->GetBranch("EvTime");
	b_Run = fChain->GetBranch("Run");
	b_EVT = fChain->GetBranch("Evt");
	b_Trig = fChain->GetBranch("Trig");
	b_Cispar = fChain->GetBranch("Cispar");
//	cout << "Passou daki3" << endl;
//	cout.flush();

	for(Int_t iGain=0; iGain < gainsNumber; iGain++)
	{
		vector< vector<Int_t> > auxBCID;
		for(Int_t iModule = 0; iModule < Nmodules; iModule++)
		{
			vector<Int_t> auxBCID2;
			auxBCID.push_back(auxBCID2);
		}
		BCIDErrors.push_back(auxBCID);
	}
			
	TString branchName;
	for(Int_t iModule = 0; iModule < Nmodules; iModule++)
	{
		vector<Int_t> auxSlink;
		vector<Int_t> auxDMUMask;
		for(Int_t iGain=0; iGain < gainsNumber; iGain++)
		{	
			branchName = "SlinkCRC" + moduleName[iModule] + gainName[iGain];
			//cout << branchName.Data() << endl;
			fChain->SetBranchAddress(branchName, SlinkCRC[iModule][iGain]);
			//cout << "O erro ocorre na linha seguinte???" << endl;
			b_SlinkCRC[iModule][iGain] = fChain->GetBranch(branchName.Data());
			//cout << moduleName[iModule];
			if (!b_SlinkCRC[iModule][iGain])
				moduleInNtuple[iModule] = false;
			else
			{
				moduleInNtuple[iModule] = true;
				//cout << "InNtuple!" << moduleInNtuple[iModule] << endl;
			}
			
			branchName = "BCID" + moduleName[iModule] + gainName[iGain];
			fChain->SetBranchAddress(branchName.Data(), BCID[iModule][iGain]);
			b_BCID[iModule][iGain] = fChain->GetBranch(branchName.Data());
			
			branchName = "DMUMask" + moduleName[iModule] + gainName[iGain];
			fChain->SetBranchAddress(branchName.Data(), DMUMask[iModule][iGain]);
			b_DMUMask[iModule][iGain] = fChain->GetBranch(branchName.Data());
			
			NSlinkErrors[iModule][iGain] = 0; auxSlink.push_back(0);
			NDMUMaskErrors[iModule][iGain] = 0; auxDMUMask.push_back(0);
			for (Int_t iDig = 0; iDig < NtileDMUs; iDig++)
			{
				NBCIDErrors[iGain][iModule][iDig] = 0;
				BCIDErrors.at(iGain).at(iModule).push_back(0);
			}
		}
		SlinkErrors.push_back(auxSlink);
		DMUMaskErrors.push_back(auxDMUMask);
	}
	/***************************************************************************************/

	//cout << "Passou daki" << endl;
	numEvents = fChain->GetEntries();
//cout<<"end of Initialize"<<endl;
} // end of Initialize


void checkCRC_DMUMask_BCID()
{
	Int_t iEvt, iModule, iGain, iDig, totalCRCErrors = 0, totalDMUMaskErrors = 0;
	//Int_t beginEvt[numEvents], endEvt[endEvents];
	//Int_t b = 0, e = 0;
	//cout << "numEvents " << numEvents << "!" << endl;
	//cout<<"entrou checkCRC_DMUMask_BCID"<<endl;
	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
			
			for (iGain = 0; iGain < gainsNumber; iGain++)
			{
				if (SlinkCRC[iModule][iGain][0] != SlinkCRC[iModule][iGain][1])
				{
					SlinkErrors.at(iModule).at(iGain)++;
					//cout << SlinkErrors.at(iModule).at(iGain) << endl;
					NSlinkErrors[iModule][iGain]++;
					totalCRCErrors++;
				}
/*
				if ((DMUMask[iModule][iGain][0] != 65535) || (DMUMask[iModule][iGain][1] != 65535))
				{
					DMUMaskErrors.at(iModule).at(iGain)++;
					//cout << "DMU" << DMUMaskErrors.at(iModule).at(iGain) << endl;
					NDMUMaskErrors[iModule][iGain]++;
					//cout << "DMU" << NDMUMaskErrors[iModule][iGain] << endl;
					//cout << "Event" << iEvt << endl;
					totalDMUMaskErrors++;
				}
*/
				for (iDig = 0; iDig < NtileDMUs; iDig++)
				{
					if (BCID[iModule][iGain][iDig] != BCID[iModule][iGain][0])
					{
						NBCIDErrors[iGain][iModule][iDig]++;
						BCIDErrors.at(iGain).at(iModule).at(iDig)++;
					}
				}

			}
		}
	}
	//cout << "Got till entry " << iEvt << "!" << endl;

	TString rowsTitle = "Module";
	TString colsTitle = "Gain";
	if (gainsNumber == 1)
		colsTitle = "";
	TString table;
//	cout << "<b>CRC Errors:</b><br />" << endl;
	//buildHTMLTable(table, (Int_t*)NSlinkErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
	buildHTMLTable(table, SlinkErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
//	cout << table << endl;
	results.addTable("CRC Errors:", table);

//	cout << "<br /><b>DMU Mask Errors:</b><br />" << endl;
	//buildHTMLTable(table, (Int_t*)NDMUMaskErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
//	buildHTMLTable(table, DMUMaskErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
//	cout << table << endl;
//	results.addTable("DMU Mask Errors:", table);
	
	if (gainsNumber == 1)
	{
		colsTitle = "Tile DMUs";
//		cout << "<br /><b>BCID Errors</b><br />" << endl;
		buildHTMLTable(table, BCIDErrors.at(0), (const Int_t)Nmodules, (const Int_t)NtileDMUs, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)tileDMUName, moduleInNtuple);
//		cout << table << endl;
		results.addTable("BCID Errors:", table);
	}
	else
	{
		colsTitle = "Tile DMUs";
//		cout << "<br /><b>BCID Errors / Low Gain:</b><br />" << endl;
		buildHTMLTable(table, BCIDErrors.at(0), (const Int_t)Nmodules, (const Int_t)NtileDMUs, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)tileDMUName, moduleInNtuple);
//		cout << table << endl;
		results.addTable("BCID Errors / Low Gain:", table);
	
//		cout << "<br /><b>BCID Errors / High Gain:</b><br />" << endl;
		buildHTMLTable(table, BCIDErrors.at(1), (const Int_t)Nmodules, (const Int_t)NtileDMUs, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)tileDMUName, moduleInNtuple);
//		cout << table << endl;
		results.addTable("BCID Errors / High Gain:", table);
	}
}


void buildHTMLTable(TString &table, Int_t *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows)
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
		table += "<th>";
		table += colNames[j];
		table += "</th>";
	}
	table += "</tr>\n";

	for (i = 0; i < nrows; i++)
	{
		if (selectedRows && !selectedRows[i])
			continue;
		
		table += "<tr><th>";
		table += rowNames[i];
		table += "</th>";
		for (j = 0; j < ncols; j++)
		{
			table += "<td align=\"center\">";
			table += *(matrix + (i*ncols) + j);
			table += "</td>";
		}
		table += "</tr>\n";

	}
	table += "</table>\n";
}


void buildHTMLTable(TString &table, const vector< vector<Int_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows)
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
		table += "<th>";
		table += colNames[j];
		table += "</th>";
	}
	table += "</tr>\n";

	for (i = 0; i < nrows; i++)
	{
		if (selectedRows && !selectedRows[i])
			continue;
		
		table += "<tr><th>";
		table += rowNames[i];
		table += "</th>";
		for (j = 0; j < ncols; j++)
		{
			table += "<td align=\"center\">";
			table += matrix.at(i).at(j);
			table += "</td>";
		}
		table += "</tr>\n";

	}
	table += "</table>\n";
}

