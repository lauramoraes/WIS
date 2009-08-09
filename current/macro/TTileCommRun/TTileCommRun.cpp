#define TTileCommRun_cxx

#include "TTileCommRun.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TLeafI.h>

#ifdef TTileCommRun_cxx

TTileCommRun::TTileCommRun(const char *runNumber, const char *runType, const char *treeName, const char *filename)
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
	
	AddFile(fileName.Data());
	
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

	numEvents = fChain->GetEntries();

} // end of TTileCommRun::TTileCommRun


void TTileCommRun::AddFile(const char *filename)
{
	//cout << filename << endl;
	fChain->Add(filename);
}


Long64_t TTileCommRun::GetNumEvents()
{
	return numEvents;
}


void TTileCommRun::checkCRC_DMUMask_BCID()
{
	Int_t iEvt, iModule, iGain, iDig, totalCRCErrors = 0, totalDMUMaskErrors = 0;
	//Int_t beginEvt[numEvents], endEvt[endEvents];
	//Int_t b = 0, e = 0;
	//cout << "numEvents " << numEvents << "!" << endl;
	
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

				if ((DMUMask[iModule][iGain][0] != 65535) || (DMUMask[iModule][iGain][1] != 65535))
				{
					DMUMaskErrors.at(iModule).at(iGain)++;
					//cout << "DMU" << DMUMaskErrors.at(iModule).at(iGain) << endl;
					NDMUMaskErrors[iModule][iGain]++;
					//cout << "DMU" << NDMUMaskErrors[iModule][iGain] << endl;
					//cout << "Event" << iEvt << endl;
					totalDMUMaskErrors++;
				}

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
	cout << "<b>CRC Errors:</b><br />" << endl;
	//buildHTMLTable(table, (Int_t*)NSlinkErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
	buildHTMLTable(table, SlinkErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
	cout << table << endl;

	cout << "<br /><b>DMU Mask Errors:</b><br />" << endl;
	//buildHTMLTable(table, (Int_t*)NDMUMaskErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
	buildHTMLTable(table, DMUMaskErrors, (const Int_t)Nmodules, (const Int_t)gainsNumber, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)gainName, moduleInNtuple);
	cout << table << endl;

	if (gainsNumber == 1)
	{
		colsTitle = "Tile DMUs";
		cout << "<br /><b>BCID Errors</b><br />" << endl;
		buildHTMLTable(table, BCIDErrors.at(0), (const Int_t)Nmodules, (const Int_t)NtileDMUs, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)tileDMUName, moduleInNtuple);
		cout << table << endl;
	}
	else
	{
		colsTitle = "Tile DMUs";
		cout << "<br /><b>BCID Errors / Low Gain:</b><br />" << endl;
		buildHTMLTable(table, BCIDErrors.at(0), (const Int_t)Nmodules, (const Int_t)NtileDMUs, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)tileDMUName, moduleInNtuple);
		cout << table << endl;
	
		cout << "<br /><b>BCID Errors / High Gain:</b><br />" << endl;
		buildHTMLTable(table, BCIDErrors.at(1), (const Int_t)Nmodules, (const Int_t)NtileDMUs, (const TString)rowsTitle, (const TString)colsTitle, (TString*)moduleName, (TString*)tileDMUName, moduleInNtuple);
		cout << table << endl;
	}
}


void TTileCommRun::buildHTMLTable(TString &table, Int_t *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows)
{
	Int_t i, j;
	
	table  = "<table class=\"main\">\n<tr><th></th>";
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


void TTileCommRun::buildHTMLTable(TString &table, const vector< vector<Int_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows)
{
	Int_t i, j;
	
	table  = "<table class=\"main\">\n<tr><th></th>";
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


TTileCommRun::~TTileCommRun()
{
	if (!fChain)
		return;

	delete fChain;
	//delete fChain->GetCurrentFile();
	//delete outputFile;

	//delete histoBitStat;
	//delete histoPulseHeight;
	//delete histoPulseShape;
}


Int_t TTileCommRun::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain)
		return 0;
	return fChain->GetEntry(entry);
}


TChain *TTileCommRun::GetChain()
{
	return fChain;
}


Int_t **TTileCommRun::GetSlinkErrorsMatrix()
{
	return (Int_t **)NSlinkErrors;
}


Int_t **TTileCommRun::GetDMUMaskErrorsMatrix()
{
	return (Int_t **)NDMUMaskErrors;
}


Int_t ***TTileCommRun::GetBCIDErrorsMatrix()
{
	return (Int_t ***)NBCIDErrors;
}


vector< vector<Int_t> > TTileCommRun::GetSlinkErrorsArray()
{
	return SlinkErrors;
}


vector< vector<Int_t> > TTileCommRun::GetDMUMaskErrorsArray()
{
	return DMUMaskErrors;
}


vector< vector< vector<Int_t> > > TTileCommRun::GetBCIDErrorsArray()
{
	return BCIDErrors;
}


void TTileCommRun::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}


Int_t TTileCommRun::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


const char *TTileCommRun::BarrelName(Int_t barrel)
{
  return (const char*)barrelName[barrel];
}

#endif // #ifdef TTileCommRun_cxx

