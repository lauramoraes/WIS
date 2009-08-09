#ifndef TTileCommMonoGainRun_h
#define TTileCommMonoGainRun_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <vector>
#include "TTileCommRun.h"

using namespace std;

class TTileCommMonoGainRun : public TTileCommRun
{
	// Leaves
	Float_t Efit[Nmodules][NPMTs];
	Int_t Sample[Nmodules][Nchannels][9];

	// Branchs
	TBranch *b_Efit[Nmodules];
	TBranch *b_Sample[Nmodules];
	
	vector< vector<Float_t> > PMTEnergies;

public:

	TTileCommMonoGainRun(const char *runNumber, const char *runType, const char *treeName = "TileRec/h1000", const char *filename = "");
	void DeadChannels();
	void buildHTMLTable(TString &table, const vector< vector<Float_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows, Bool_t *selectedCols, Float_t warningLevel);
	void Baseline();
	virtual ~TTileCommMonoGainRun() { };
};

#endif
