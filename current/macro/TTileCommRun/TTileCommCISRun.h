#ifndef TTileCommCISRun_h
#define TTileCommCISRun_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <vector>
#include "TTileCommRun.h"

using namespace std;

class TTileCommCISRun : public TTileCommRun
{

public:

	TTileCommCISRun(const char *runNumber, const char *runType, const char *treeName = "TileRec/h1000", const char *filename = "");
	void PulseHeight();
	virtual ~TTileCommCISRun() { };
};

#endif
