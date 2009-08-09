
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TString.h"
#include "wisMacroConsts.h"

using namespace std;

const TString defaultNtuplesDir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
const TString outputDir = "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5/";
const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/";
const TString outputWebDir = "http://tcws.web.cern.ch/tcws/WIS/results/team5/";

void canvas_style(TCanvas *c,
		  float left_margin=0.15,
		  float right_margin=0.15,
		  float top_margin=0.10,
		  float bottom_margin=0.15,
		  int canvas_color=0,
		  int frame_color=0){
  
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
/*
  char  correpslofile2[200];
  char  corrpnglofile2[200];
  char  correpshifile2[200];
  char  corrpnghifile2[200];
  char  covaepslofile2[200];
  char  covapnglofile2[200];
  char  covaepshifile2[200];
  char  covapnghifile2[200];
*/
  TString moduleName;
  if(is_A){
    moduleName = "LBA";
    moduleName += module;
    sprintf(correpslofile,"%s/coherence/corr_efit_LBA%i_%i_lo.eps", outputDir, module,runnum);  
    sprintf(corrpnglofile,"%s/coherence/corr_efit_LBA%i_%i_lo.png", outputDir, module,runnum);  
    sprintf(correpshifile,"%s/coherence/corr_efit_LBA%i_%i_hi.eps", outputDir, module,runnum);  
    sprintf(corrpnghifile,"%s/coherence/corr_efit_LBA%i_%i_hi.png", outputDir, module,runnum);  
    sprintf(covaepslofile,"%s/coherence/cova_efit_LBA%i_%i_lo.eps", outputDir, module,runnum);  
    sprintf(covapnglofile,"%s/coherence/cova_efit_LBA%i_%i_lo.png", outputDir, module,runnum);  
    sprintf(covaepshifile,"%s/coherence/cova_efit_LBA%i_%i_hi.eps", outputDir, module,runnum);
    sprintf(covapnghifile,"%s/coherence/cova_efit_LBA%i_%i_hi.png", outputDir, module,runnum); 
/*
    sprintf(correpslofile2,"%s/coherence/corr_efit_LBA%i_%i_lo.eps", standOutputDir, module,runnum);
    sprintf(corrpnglofile2,"%s/coherence/corr_efit_LBA%i_%i_lo.png", standOutputDir, module,runnum);
    sprintf(correpshifile2,"%s/coherence/corr_efit_LBA%i_%i_hi.eps", standOutputDir, module,runnum);
    sprintf(corrpnghifile2,"%s/coherence/corr_efit_LBA%i_%i_hi.png", standOutputDir, module,runnum);  
    sprintf(covaepslofile2,"%s/coherence/cova_efit_LBA%i_%i_lo.eps", standOutputDir, module,runnum);  
    sprintf(covapnglofile2,"%s/coherence/cova_efit_LBA%i_%i_lo.png", standOutputDir, module,runnum);  
    sprintf(covaepshifile2,"%s/coherence/cova_efit_LBA%i_%i_hi.eps", standOutputDir, module,runnum);  
    sprintf(covapnghifile2,"%s/coherence/cova_efit_LBA%i_%i_hi.png", standOutputDir, module,runnum); */
  }else{
    moduleName = "LBC";
    moduleName += module;
    sprintf(correpslofile,"%s/coherence/corr_efit_LBC%i_%i_lo.eps", outputDir, module,runnum);  
    sprintf(corrpnglofile,"%s/coherence/corr_efit_LBC%i_%i_lo.png", outputDir, module,runnum);  
    sprintf(correpshifile,"%s/coherence/corr_efit_LBC%i_%i_hi.eps", outputDir, module,runnum);  
    sprintf(corrpnghifile,"%s/coherence/corr_efit_LBC%i_%i_hi.png", outputDir, module,runnum);  
    sprintf(covaepslofile,"%s/coherence/cova_efit_LBC%i_%i_lo.eps", outputDir, module,runnum);  
    sprintf(covapnglofile,"%s/coherence/cova_efit_LBC%i_%i_lo.png", outputDir, module,runnum);  
    sprintf(covaepshifile,"%s/coherence/cova_efit_LBC%i_%i_hi.eps", outputDir, module,runnum);  
    sprintf(covapnghifile,"%s/coherence/cova_efit_LBC%i_%i_hi.png", outputDir, module,runnum); 
/*
    sprintf(correpslofile2,"%s/coherence/corr_efit_LBC%i_%i_lo.eps", standOutputDir, module,runnum);  
    sprintf(corrpnglofile2,"%s/coherence/corr_efit_LBC%i_%i_lo.png", standOutputDir, module,runnum);  
    sprintf(correpshifile2,"%s/coherence/corr_efit_LBC%i_%i_hi.eps", standOutputDir, module,runnum);  
    sprintf(corrpnghifile2,"%s/coherence/corr_efit_LBC%i_%i_hi.png", standOutputDir, module,runnum);  
    sprintf(covaepslofile2,"%s/coherence/cova_efit_LBC%i_%i_lo.eps", standOutputDir, module,runnum);  
    sprintf(covapnglofile2,"%s/coherence/cova_efit_LBC%i_%i_lo.png", standOutputDir, module,runnum);  
    sprintf(covaepshifile2,"%s/coherence/cova_efit_LBC%i_%i_hi.eps", standOutputDir, module,runnum);  
    sprintf(covapnghifile2,"%s/coherence/cova_efit_LBC%i_%i_hi.png", standOutputDir, module,runnum); */
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
  //c1->Print(covaepshifile2,"eps");
  //c1->Print(covapnghifile2,"png");

  h_corr_hi->Draw("colz");
  c1->Print(correpshifile,"eps");
  c1->Print(corrpnghifile,"png");
  //c1->Print(correpshifile2,"eps");
  //c1->Print(corrpnghifile2,"png");

  h_cova_lo->Draw("colz");
  c1->Print(covaepslofile,"eps");
  c1->Print(covapnglofile,"png");
  //c1->Print(covaepslofile2,"eps");
  //c1->Print(covapnglofile2,"png");

  h_corr_lo->Draw("colz");
  c1->Print(correpslofile,"eps");
  c1->Print(corrpnglofile,"png");
  //c1->Print(correpslofile2,"eps");
  //c1->Print(corrpnglofile2,"png");


  printf("\n<br /><b>Number of entries: %i (should be 1980)</b><br /><br />\n ",nsum);
  printf("<b>Average correlation: High Gain: %f, Low Gain: %f</b><br /><br />\n ",av_corr_hi,av_corr_lo );
  if (av_corr_hi > 0.15)  printf("<font color=\"red\">Warning:</font> average correlation too large for high gain!<br/>\n");
  if (av_corr_lo > 0.15)  printf("<font color=\"red\">Warning:</font> average correlation too large for low gain!<br />\n");
  printf("   ");
  printf("   ");

  TString resultsDir = WEB_SERVER;
  resultsDir += "/";
  resultsDir += SYSTEM_HOME;
  resultsDir += "/results/team5/coherence/";
  cout << "<ul>" << endl;
  cout << "<li>Correlation " << moduleName << " Low Gain<br />" << endl;
  cout << "<img src=\"" << resultsDir << basename(corrpnglofile) << "\" /></li><br />" << endl;
  cout << "<li>Correlation " << moduleName << " High Gain<br />" << endl;
  cout << "<img src=\"" << resultsDir << basename(corrpnghifile) << "\" /></li><br />" << endl;
  cout << "<li>Covariance " << moduleName << " Low Gain<br />" << endl;
  cout << "<img src=\"" << resultsDir << basename(covapnglofile) << "\" /></li><br />" << endl;
  cout << "<li>Covariance " << moduleName << " High Gain<br />" << endl;
  cout << "<img src=\"" << resultsDir << basename(covapnghifile) << "\" /></li><br />" << endl;
  cout << "</ul>" << endl;
  

  return;
  
}

