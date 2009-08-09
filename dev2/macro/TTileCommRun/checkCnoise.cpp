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
//#include <stdlib.h>

#include "TTree.h"
#include "TH2D.h"
#include "TString.h"

#include "wisMacroConsts.h"
#include "TWISMacroResult.h"
#include "wis2Tilecomm.h"

#define outputDir  "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5_local/"
//#define standOutputDir  "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/"
#define outputWebDir  "http://tcws.web.cern.ch/tcws/WIS/results/team5_local/"
using namespace std;

void canvas_style(TCanvas *c,
			float left_margin=0.15,
			float right_margin=0.15,
			float top_margin=0.10,
			float bottom_margin=0.15,
			int canvas_color=0,
			int frame_color=0);
void cnoise(const Int_t runnum, Int_t module, Int_t is_A, Long64_t nmax, TString filename);

//TROOT root("TTileCommRun", "Analyze TileCal Commissioning runs");

TWISMacroResult results;


int main(int argc, char *argv[])
{
	//TTileCommMonoGainRun *Run;
	TString runNumber, runType, filename, module;
	Int_t barrelCode, retCode, maxEvents;
	struct stat statBuf;
	
	//cout << getenv("PATH") << endl;
	//cout << getenv("ROOTSYS") << endl;
	
	if (argc < 5)
	{
		//cout << "Usage:"<< endl << "    ./checkCnoise <runNumber> <runType> <module> <maxEvents> [<filename>]" << endl;
		results.addError("Usage:\n    ./checkCnoise <runNumber> <runType> <module> <maxEvents> [<filename>]");
		return 0;
	}
	
	runNumber = argv[1];
	runType = argv[2];
	module = argv[3];
	//cout << module << ":" << module.Length() << endl;
	if (module.Length() > 3)
	{
		module[0] = module[2];
		module[1] = module[3];
		module[2] = module[4];
		module.Resize(3);
	}
	module.ToUpper();
	//cout << module << ":" << module.Length() << endl;

	//cout << "Module: " << module << endl;
	maxEvents = atoi(argv[4]);
	if (barrelCode = 0)
		barrelCode = 1;
	else
	if (barrelCode == 1)
		barrelCode = 0;

	if (argc > 5)
		filename = argv[5];
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
	
	//Run = new TTileCommMonoGainRun(runNumber.Data(), runType.Data(), barrelCode, "TileRec/h1000", filename.Data());
	/*
	if (argc > 5)
	{
		Run->Show(atoi(argv[5]));
		exit(0);
	}
	*/
	//Long64_t numEvents = Run->GetChain()->GetEntries();
/*	
	cout << "<html><head>" << endl;
	cout << "	<link href=\"../css/standard.css\" type=\"text/css\" rel=\"stylesheet\" />" << endl;
	cout << "</head><body>" << endl;
	cout << endl << "<br /><b>Run Number: " << runNumber << "</b><br />" << endl << endl;
	cout << endl << "<b>Maximum number of events: " << maxEvents << "</b><br />" << endl << endl;
*/
	TString moduleName = "LB" + module;
	results.setMacroName("Coherent Noise");
	results.setParameter("Run Number", runNumber);
	results.setParameter("Run Type", runType);
	results.setParameter("Module Name", moduleName);
	results.setParameter("Maximum number of events", maxEvents);

	
	if (module[0] == 'A' || module[0] == 'a')
		barrelCode = 1;
	else
		barrelCode = 0;
	char moduleNumber[3];
	moduleNumber[0] = module[1];
	moduleNumber[1] = module[2];
	moduleNumber[2] = '\0';
	cnoise((Int_t)atoi(runNumber), (Int_t)atoi(moduleNumber), barrelCode, maxEvents, filename);
	//cout << "</body></html>" << endl;

	//delete Run;
	results.out();
	
	return 0;
}


void canvas_style(TCanvas *c, float left_margin, float right_margin, float top_margin, float bottom_margin, int canvas_color, int frame_color)
{
  c->SetLeftMargin(left_margin);
  c->SetRightMargin(right_margin);
  c->SetTopMargin(top_margin);
  c->SetBottomMargin(bottom_margin);
  c->SetFillColor(canvas_color);
  c->SetFrameFillColor(frame_color);
  
  c->SetBorderMode(0);
  c->SetBorderSize(1);
  c->SetFrameBorderMode(0);
}


//void cnoise(const char * filename, Int_t module, Int_t is_A, Long64_t nmax)
void cnoise(const Int_t runnum, Int_t module, Int_t is_A, Long64_t nmax, TString filename)
{

  TString  inputfile;
  //char  logfile[200];
  char  evarlo[50];
  char  evarhi[50];
  //sprintf(logfile,"%s.log", filename);
  //sprintf(inputfile,"rfio:/castor/cern.ch/user/t/tilebeam/cosmics/sep05/%s.root", filename);  
  if(filename == ""){
    inputfile = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/tiletb_";
    inputfile += runnum;
    inputfile += "_Ped.0.root";}
  else(inputfile = filename);
  if(is_A){
    sprintf(evarlo,"EfitA%ilo", module);
    sprintf(evarhi,"EfitA%ihi", module);
  }else{
    sprintf(evarlo,"EfitC%ilo", module);
    sprintf(evarhi,"EfitC%ihi", module);
  }


  //printf("\n  ---  opening file ---   \n");
  TFile *f = (TFile*) TFile::Open(inputfile);
  TTree *tree=0;
  tree = (TTree *) f->Get("TileRec/h1000");
  if (tree == 0) return;

  //printf("\n  ---  processing tree ---   \n\n");

  Long64_t nentries = tree->GetEntriesFast();
  Long64_t Nmax = TMath::Min(nmax,nentries);
  //Int_t nbytes = 0;
  //int nb = 0;
  

  Int_t           EvTime;
  Int_t           Run;
  Int_t           Evt;
  Float_t         Efitlo[48];
  Float_t         Efithi[48];
  
  tree->SetBranchAddress("EvTime",&EvTime);
  tree->SetBranchAddress("Run",&Run);
  tree->SetBranchAddress("Evt",&Evt);
  tree->SetBranchAddress(evarlo,Efitlo);
  tree->SetBranchAddress(evarhi,Efithi);
  
  
  TH2D *h_cova_hi  = new TH2D("h_cova_hi", "covariance",48,0,48,48,0,48);    
  TH2D *h_corr_hi  = new TH2D("h_corr_hi", "correlation",48,0,48,48,0,48);  
  
  TH2D *h_cova_lo  = new TH2D("h_cova_lo", "covariance",48,0,48,48,0,48);    
  TH2D *h_corr_lo  = new TH2D("h_corr_lo", "correlation",48,0,48,48,0,48);  
  

  Float_t mean_hi[48][48],cova_hi[48][48],corr_hi[48][48];
  Float_t pmtmean_hi[48],pmtrms_hi[48],pmtrmser_hi[48], npmt[48],npmter[48];    
  Float_t mean_lo[48][48],cova_lo[48][48],corr_lo[48][48];
  Float_t pmtmean_lo[48],pmtrms_lo[48],pmtrmser_lo[48];
  Float_t av_corr_hi  = 0.0;
  Float_t sum_corr_hi = 0.0;
  Int_t   nsum        = 0;
  Float_t av_corr_lo  = 0.0;
  Float_t sum_corr_lo = 0;  


  for (int i=0;i<48;i++){
    pmtmean_hi[i]=0.;
    pmtmean_lo[i]=0.;
    pmtrms_hi[i]=0.;
    pmtrms_lo[i]=0.;
    pmtrmser_hi[i]=0.;
    pmtrmser_lo[i]=0.;
    npmt[i]=i;
    npmter[i]=0;
    for (int j=0;j<48;j++){
      mean_hi[i][j]=0.;
      cova_hi[i][j]=0.;
      corr_hi[i][j]=0.;
      mean_lo[i][j]=0.;
      cova_lo[i][j]=0.;
      corr_lo[i][j]=0.;
    }      
  }
  

  //////////////
  //EVENT LOOP//
  //////////////
  
  
  for (Long64_t jentry=0; jentry<Nmax;jentry++) {
    tree->GetEntry(jentry); 
    
    Float_t hifac = 1023*64/800;
    Float_t lofac = 1023/800;  
    
    for (int i=0;i<48;i++){
      pmtmean_hi[i]+=Efithi[i]*hifac;
      pmtmean_lo[i]+=Efitlo[i]*lofac;
      
      for (int j=0;j<48;j++){
	mean_hi[i][j]+=Efithi[i]*Efithi[j]*hifac*hifac;
	mean_lo[i][j]+=Efitlo[i]*Efitlo[j]*lofac*lofac;
      }
    }      
  }//event loop
  
  for (int i=0;i<48;i++){
    pmtmean_hi[i] =  pmtmean_hi[i]/(Float_t)Nmax; 
    pmtmean_lo[i] =  pmtmean_hi[i]/(Float_t)Nmax; 
    for (int j=0;j<48;j++){
      mean_hi[i][j] = mean_hi[i][j]/(Float_t)Nmax;
      mean_lo[i][j] = mean_lo[i][j]/(Float_t)Nmax;
    }
  }
  
  for (int i=0;i<48;i++){
    for (int j=0;j<48;j++){
      cova_hi[i][j]=mean_hi[i][j]-pmtmean_hi[i]*pmtmean_hi[j];
      cova_lo[i][j]=mean_lo[i][j]-pmtmean_lo[i]*pmtmean_lo[j]; 
    }
    pmtrms_hi[i]=sqrt(cova_hi[i][i]);
    pmtrms_lo[i]=sqrt(cova_lo[i][i]);
  }
  
  for (int i=0;i<48;i++){
    for (int j=0;j<48;j++){
      if(pmtrms_hi[i]==0. ||pmtrms_hi[j]==0.||i==31||i==32||i==43||j==31||j==32||j==43){
	corr_hi[i][j]=0.;
	corr_lo[i][j]=0.;
      }else{
	corr_hi[i][j]=cova_hi[i][j]/pmtrms_hi[i]/pmtrms_hi[j];
	corr_lo[i][j]=cova_lo[i][j]/pmtrms_lo[i]/pmtrms_lo[j];
	if (i!=j){
	  nsum++;
	  sum_corr_hi+=corr_hi[i][j];
	  sum_corr_lo+=corr_lo[i][j];
	}
      }
      h_cova_hi->Fill(i,j,cova_hi[i][j]);
      h_corr_hi->Fill(i,j,corr_hi[i][j]);
      h_cova_lo->Fill(i,j,cova_lo[i][j]);
      h_corr_lo->Fill(i,j,corr_lo[i][j]);
    }
  }
  av_corr_hi  = sum_corr_hi/nsum;
  av_corr_lo  = sum_corr_lo/nsum;  


  char  correpslofile[200];
  char  corrpnglofile[200];
  char  correpshifile[200];
  char  corrpnghifile[200];
  char  covaepslofile[200];
  char  covapnglofile[200];
  char  covaepshifile[200];
  char  covapnghifile[200];

  TString moduleName;
  if(is_A){
    moduleName = "LBA";
    moduleName += module;
    sprintf(correpslofile,"%scoherence/%s/corr_efit_LBA%i_%i_lo.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(corrpnglofile,"%scoherence/%s/corr_efit_LBA%i_%i_lo.png", outputDir, moduleName.Data(), module,runnum);  
    sprintf(correpshifile,"%scoherence/%s/corr_efit_LBA%i_%i_hi.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(corrpnghifile,"%scoherence/%s/corr_efit_LBA%i_%i_hi.png", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covaepslofile,"%scoherence/%s/cova_efit_LBA%i_%i_lo.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covapnglofile,"%scoherence/%s/cova_efit_LBA%i_%i_lo.png", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covaepshifile,"%scoherence/%s/cova_efit_LBA%i_%i_hi.eps", outputDir, moduleName.Data(), module,runnum);
    sprintf(covapnghifile,"%scoherence/%s/cova_efit_LBA%i_%i_hi.png", outputDir, moduleName.Data(), module,runnum); 

  }else{
    moduleName = "LBC";
    moduleName += module;
    sprintf(correpslofile,"%scoherence/%s/corr_efit_LBC%i_%i_lo.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(corrpnglofile,"%scoherence/%s/corr_efit_LBC%i_%i_lo.png", outputDir, moduleName.Data(), module,runnum);  
    sprintf(correpshifile,"%scoherence/%s/corr_efit_LBC%i_%i_hi.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(corrpnghifile,"%scoherence/%s/corr_efit_LBC%i_%i_hi.png", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covaepslofile,"%scoherence/%s/cova_efit_LBC%i_%i_lo.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covapnglofile,"%scoherence/%s/cova_efit_LBC%i_%i_lo.png", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covaepshifile,"%scoherence/%s/cova_efit_LBC%i_%i_hi.eps", outputDir, moduleName.Data(), module,runnum);  
    sprintf(covapnghifile,"%scoherence/%s/cova_efit_LBC%i_%i_hi.png", outputDir, moduleName.Data(), module,runnum); 
  }
  
  
  TCanvas *c1 = new TCanvas("c1", "Coh noise", 800,500);
  canvas_style(c1);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(1);
  h_corr_hi->SetStats(0);
  h_cova_hi->SetStats(0);
  h_corr_lo->SetStats(0);
  h_cova_lo->SetStats(0);
  
  h_cova_hi->Draw("colz");
  c1->Print(covaepshifile,"eps");
  c1->Print(covapnghifile,"png");

  h_corr_hi->Draw("colz");
  c1->Print(correpshifile,"eps");
  c1->Print(corrpnghifile,"png");

  h_cova_lo->Draw("colz");
  c1->Print(covaepslofile,"eps");
  c1->Print(covapnglofile,"png");

  h_corr_lo->Draw("colz");
  c1->Print(correpslofile,"eps");
  c1->Print(corrpnglofile,"png");


  //printf("\n<br /><b>Number of entries: %i (should be 1980)</b><br /><br />\n ",nsum);
  results.addValue("Number of entries (should be 1980)", nsum);
  //printf("<b>Average correlation: High Gain: %f, Low Gain: %f</b><br /><br />\n ",av_corr_hi,av_corr_lo );
  results.addValue("Average correlation, High Gain", av_corr_hi);
  results.addValue("Average correlation, Low Gain", av_corr_lo);

  TString URL = "http://atlasmonitor.web.cern.ch/atlasmonitor/saveValuesExample.jsp?module=";
  URL += moduleName + "&run=";
  URL += runnum;
  URL += "&runType=Ped&qcSheetType=team5&__avgCorrHi=";
  URL += av_corr_hi;
  URL += "&__avgCorrLo=";
  URL += av_corr_lo;
  results.addURLToPage(URL);

  if (av_corr_hi > 0.15)
    //printf("<font color=\"red\">Warning:</font> average correlation too large for high gain!<br/>\n");
    results.addWarning("Average correlation too large for high gain!");
  
  if (av_corr_lo > 0.15)
    //printf("<font color=\"red\">Warning:</font> average correlation too large for low gain!<br />\n");
    results.addWarning("Average correlation too large for low gain!");
    
  //printf("   ");
  //printf("   ");

  TString resultsDir = outputWebDir;
  resultsDir += "coherence/";
  resultsDir += moduleName;
  resultsDir += "/";
  //cout << "<ul>" << endl;
  TString runNumber = ""; runNumber += runnum;
  

  TString plotTitle, plotFilename;
  
  //cout << "<li>Correlation " << moduleName << " Low Gain<br />" << endl;
  //cout << "<img src=\"" << resultsDir << basename(corrpnglofile) << "\" /></li><br />" << endl;
  plotTitle = "Correlation " + moduleName + " Low Gain";
  plotFilename = resultsDir + basename(corrpnglofile);
  results.addPlot(plotTitle, plotFilename);
  
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);
  plotFilename = resultsDir + basename(correpslofile);
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);

  //cout << "<li>Correlation " << moduleName << " High Gain<br />" << endl;
  //cout << "<img src=\"" << resultsDir << basename(corrpnghifile) << "\" /></li><br />" << endl;
  plotTitle = "Correlation " + moduleName + " High Gain";
  plotFilename = resultsDir + basename(corrpnghifile);
  results.addPlot(plotTitle, plotFilename);
  
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);
  plotFilename = resultsDir + basename(correpshifile);
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);

  //cout << "<li>Covariance " << moduleName << " Low Gain<br />" << endl;
  //cout << "<img src=\"" << resultsDir << basename(covapnglofile) << "\" /></li><br />" << endl;
  plotTitle = "Covariance " + moduleName + " Low Gain";
  plotFilename = resultsDir + basename(covapnglofile);
  results.addPlot(plotTitle, plotFilename);
  
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);
  plotFilename = resultsDir + basename(covaepslofile);
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);

  //cout << "<li>Covariance " << moduleName << " High Gain<br />" << endl;
  //cout << "<img src=\"" << resultsDir << basename(covapnghifile) << "\" /></li><br />" << endl;
  plotTitle = "Covariance " + moduleName + " High Gain";
  plotFilename = resultsDir + basename(covapnghifile);
  results.addPlot(plotTitle, plotFilename);
  
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);
  plotFilename = resultsDir + basename(covaepshifile);
  wis2Tilecomm(runNumber.Data(), moduleName.Data(), plotFilename.Data(), results);

  //cout << "</ul>" << endl;

  return;
  
}

