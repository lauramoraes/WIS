#ifndef TTileCommRun_h
#define TTileCommRun_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <vector>

using namespace std;

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
const TString outputWebDir = "http://tileinfo.web.cern.ch/tileinfo/lps/WIS/results/team5/";
const TString outputDirPerModule = "/work/commissioning/summary/";

class TTileCommRun
{

protected:
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

public:

	TTileCommRun(const char *runNumber, const char *runType, const char *treeName = "TileRec/h1000", const char *filename = "");

	void AddFile(const char *filename);
	virtual Int_t Cut(Long64_t entry);

	TChain *GetChain();
	Long64_t GetNumEvents();
	Int_t **GetSlinkErrorsMatrix();
	Int_t **GetDMUMaskErrorsMatrix();
	Int_t ***GetBCIDErrorsMatrix();
	vector< vector<Int_t> > GetSlinkErrorsArray();
	vector< vector<Int_t> > GetDMUMaskErrorsArray();
	vector< vector< vector<Int_t> > > GetBCIDErrorsArray();
	
	void checkCRC_DMUMask_BCID();

	virtual Int_t GetEntry(Long64_t entry);
	//virtual Long64_t LoadTree(Long64_t entry);
	//virtual void Loop();
	virtual void Show(Long64_t entry = -1);
	virtual ~TTileCommRun();
	
	const char *BarrelName(Int_t barrel);
	//void GetPulseShape(Int_t imodule, Int_t ichannel, Int_t igain, Int_t capacitor=100);

	void buildHTMLTable(TString &table, Int_t *matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows = NULL);
	void buildHTMLTable(TString &table, const vector< vector<Int_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows);

};

#endif
