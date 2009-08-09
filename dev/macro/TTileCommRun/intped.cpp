#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <ctype.h>
#include <TROOT.h>
#include <TUnixSystem.h>
#include <TChain.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TString.h>
#include "TObjArray.h"
#include "TObjString.h"
#include <TH2.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLeafI.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPolyMarker.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TGraph.h>

#include "TWISMacroResult.h"
#include "wisIntegrator2Tilecomm.h"

#define outputDir  "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5/integrator/"
#define standOutputDir  "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/integrator/"
#define outputWebDir  "http://tcws.web.cern.ch/tcws/WIS/results/team5/integrator/"

using namespace std;

void ntwrite(const char* filename)
{
	ifstream in;
	TString outfilename= "data/" + TString(filename);
	outfilename += ".root";
	TString infilename = "data/" + TString(filename);
	
	in.open(infilename);
	if(!in.is_open())
	{
		cout<<"Warning: could not open file" << infilename<< endl;
		cout<<"Ntuple is not recreated!"<<endl;
		return;
	}
	TFile *f = new TFile(outfilename,"RECREATE");
	TTree int1("int1","");
	Int_t x[50], time,pmtsum;
	Int_t pmt[48];

	cout<<"...creating ntuple"<<endl; 
	int1.Branch("pmt",&pmt,"pmt[48]/I");   
	int1.Branch("pmtsum",&pmtsum,"pmtsum/I");
	int1.Branch("time",&time,"time/I");
	
	int kk=0;
	while (true) 
	{
		pmtsum = 0; 
		for (int i=0; i<48; i++)
		{
			in >> x[i];
			pmt[i]=x[i];
			pmtsum += x[i];
		}
		time=kk++;
		int1.Fill();
		if (!in.good()) break;
	}
	int1.Write();
	f->Write();
	in.close();
}
void canvas_style(TCanvas *c,
			float left_margin=0.15,
			float right_margin=0.05,
			float top_margin=0.05,
			float bottom_margin=0.15,
			int canvas_color=0,
			int frame_color=0) 
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

TString TStrTok(TString str, int i)
{
	TObjArray *identifier;
	identifier = str.Tokenize("_.");
	int j= 0;
	if(i < identifier->GetEntriesFast())
        {
                TObjString* oStringIdentifier;
		TIter iIdentifier(identifier);
		while(oStringIdentifier=(TObjString*)iIdentifier())
		{
			if(i == j)
			{
				return(TString(oStringIdentifier->GetString().Data()));
				delete [] identifier;
			}
			j++;
		}
		delete [] identifier;
	}
	return 0;
}

TString dateFormat(const char * date)
{
	TString aux;
	aux = date[0];
	aux += date[1];
	aux += date[2];
	aux +=date[3];
        aux += "/";
        aux += date[4];
	aux += date[5];
        aux += "/";
        aux += date[6];
	aux += date[7];
	return(aux);
}
TString hourFormat(const char * hour)
{
	TString aux;
        aux = hour[0];
	aux += hour[1];
        aux += ":";
        aux += hour[2];
	aux += hour[3];
	aux += ":00";
	return(aux);
}
void ntread( const char *filename, Int_t t_low, Int_t t_high, Int_t level, Int_t mincounts,Int_t maxcounts)
{
	bool is_ok =true;
	stringstream ss;
	TWISMacroResult result;	
	
	TWISMacroResult();
	result.setMacroName("Integrator Output");

	gStyle->SetPalette(1);
	TString moduleName = TStrTok(TString(filename), 2);
	
	TString date = TStrTok(TString(filename), 3);
	TString inputfile = "root://tilebeam@pcata007.cern.ch//data/commissioning/team4/integrator/offline/" + TString(moduleName)+"/"+TString(filename)+TString(".root");

	cout<<inputfile<<endl;

	//processing ntuple
	TFile* f = (TFile*) TFile::Open(inputfile);

	if(!f)
	{
		TString error= "SEVERE ERROR: could not open the root file " + inputfile;
		result.addError(error);
		result.out();
                return;
	}
	TTree *int1 = (TTree*)f->Get("int1");

	Int_t pmtsum, time;
	Double_t partsum[48], partrms[48], partrmser[48], partrms2[48],partrms2er[48] ;
	Double_t pmtrms[48], pmtrmser[48], pmtmean[48];
	Double_t totvar,sumvar,totrms,totrmser,rmscohpc,totcoh, cohfrac;
	Int_t pmt[48];

	Int_t nentries = (Int_t)int1->GetEntries();
	
	Int_t tHigh = nentries;
	Int_t tLow = 0;

	if (t_high != -1)
	{
		tHigh = t_high;
		tLow  = t_low;
	}

	Int_t pmtHigh = 1000;
	Int_t pmtLow = 0;

	if (maxcounts != -1)
		pmtHigh = maxcounts;
	if (mincounts != -1)
		pmtLow = mincounts;

	int1->SetBranchAddress("pmt",&pmt);
	int1->SetBranchAddress("time",&time);
	int1->SetBranchAddress("pmtsum",&pmtsum);

	Int_t Nstep  = pmtHigh-pmtLow+1;
	Int_t Ntstep = (tHigh-tLow)*180;
	if(Ntstep >2000) Ntstep = 2000;
	if(Ntstep <1000) Ntstep = 1000;
	int pmtmax = 45;
	
	TH1D *h_sumpmt = new TH1D("h_sumpmt","pmt sum ",Nstep*48,pmtLow*48, pmtHigh*48);
	TH2D *h_cova1 = new TH2D("h_cova1","covariance",48,0,48,48,0,48);
	TH2D *h_corr1 = new TH2D("h_corr1","correlation",48,0,48,48,0,48);
	TH2D *h_cova2 = new TH2D("h_cova2","covariance",48,0,48,48,0,48);
	TH2D *h_corr2 = new TH2D("h_corr2","correlation",48,0,48,48,0,48);

	TString temp1, temp2;
	
	TH1D* h_pmt[48];
	for(unsigned i =0; i<48; i++)
	{
		ss<<i;//int to TString
		temp1 = "h_pmt" + ss.str();
		temp2 = "pmt " + ss.str();
		temp2 += " signal";
		h_pmt[i] = new TH1D(temp1,temp2,Nstep,pmtLow, pmtHigh);
		ss.str("");
	}
	TH2D* h_pmt_time[48];
	for(unsigned i =0; i<48; i++)
	{
		ss<<i;//int to TString
		temp1 = "h_pmt_time" + ss.str();
		temp2 = "pmt " + ss.str();
		temp2 += " signal vs time";
		h_pmt_time[i] = new TH2D(temp1,temp2, Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
		ss.str("");
	}
	double mean[48][48];
	double cova[48][48];
	double corr[48][48];

	int nchan = 0;

	for (unsigned i=0;i<48;i++)
	{
		for (unsigned j=0;j<48;j++)
		{
			mean[i][j]=0;
			cova[i][j]=0;
			corr[i][j]=0;
		}
	}

	for (Int_t ne=0;ne<nentries;ne++) 
	{
		int1->GetEntry(ne);
		h_sumpmt->Fill(pmtsum);
		for (int i=0;i<48;i++)
		{
			h_pmt[i]->Fill(pmt[i]);
			if(level >0)
				for (int j=0;j<48;j++)
					mean[i][j]+=pmt[i]*pmt[j];
		}

	}// event loop

	for (int i=0;i<48;i++)
	{
		pmtmean[i]   = h_pmt[i]->GetMean(1);
		pmtrms[i]    = h_pmt[i]->GetRMS(1);
		pmtrmser[i]  = h_pmt[i]->GetRMSError(1);
		for (int j=0;j<48;j++)
			mean[i][j] = mean[i][j]/nentries;
	}
	totrms     = h_sumpmt->GetRMS(1);
    	totrmser   = h_sumpmt->GetRMSError(1);
	for (Int_t ne=0;ne<nentries;ne++) 
	{
		int1->GetEntry(ne);
		Double_t  t = ne/90.0;
		if(level > 3)
		{
			Int_t corr_pmt[10];
			for (int i=0;i<10;i++) 
			{
				corr_pmt[i]  = pmt[i]-int(pmtmean[i])+300-20*i;
				h_pmt_time[i]->Fill(t,corr_pmt[i]);
			}
		}
		else
		{
			for (int i=0;i<10;i++) 
				h_pmt_time[i]->Fill(t,pmt[i]);
		}
		for (int i=10;i<48;i++) 
			h_pmt_time[i]->Fill(t,pmt[i]);
	}// event loop

	double av_corr = 0.0;
	double av_cova = 0.0;
	int nme=0;

	for (unsigned i=0;i<48;i++)
	{
		for (unsigned j=0;j<48;j++)
		{
			if(i==31||i==32||i==43||j==31||j==32||j==43)
			{
				cova[i][j]=0;
				corr[i][j]=0;
			}
			else
			{
				cova[i][j]=mean[i][j]-pmtmean[i]*pmtmean[j];
				corr[i][j]=cova[i][j]/pmtrms[i]/pmtrms[j];
			}
			h_cova1->Fill(i,j,cova[i][j]);
			h_corr1->Fill(i,j,corr[i][j]);
			h_cova2->Fill(i,j,fabs(cova[i][j]));
			h_corr2->Fill(i,j,fabs(corr[i][j]));
			if (i!=j) 
			{
				av_corr += corr[i][j];
				av_cova += cova[i][j];
				nme++;
			}
		}
	}


	av_corr = av_corr/pmtmax/(pmtmax-1);
	av_cova = av_cova/pmtmax/(pmtmax-1);

	TCanvas *c0 = new TCanvas("c0", "PMT 10", 800,500);
	canvas_style(c0);
	c0->SetLeftMargin(0.12);
	c0->SetBottomMargin(0.15);
	gStyle->SetPadColor(2);
	gStyle->SetCanvasColor(10);
	gStyle->SetOptTitle(0);


	int n0 =0;
	int n500 = 0;
	int n700 = 0;
	int nnoise = 0;


	for (int i=0;i<48;i++)
	{
		if (pmtmean[i] <10)  n0++;
		if (pmtmean[i] >500) n500++;
		if (pmtmean[i] >700) n700++;
		if (pmtrms[i] > 2.0) nnoise ++;
	}

	if (n0>3 || n700 >0 || nnoise >0) 
		is_ok = false;

	Double_t npmt[48],npmter[48];
	Double_t av_rms = 0.;
	Double_t av_counts = 0.;
	sumvar =0;

	for (int i=0;i<48;i++) 
	{
		npmt[i] = i;
		npmter[i] = 0;
	}

	for (int i=0;i<48;i++)
	{
		av_rms +=  pmtrms[i];
		av_counts +=  pmtmean[i];
		sumvar += (pmtrms[i]*pmtrms[i]);
	}

	av_rms = av_rms/pmtmax;
	av_counts = av_counts/pmtmax;

	totvar = totrms*totrms;
	totcoh = fabs(totvar - sumvar);
	rmscohpc = sqrt(totcoh/pmtmax/(pmtmax-1));
	cohfrac = rmscohpc/av_rms/av_rms;

	TString rmsepsfile = TString(outputDir) + TString("rms/") + TString(filename) + TString("_rms.eps");
	TString rmspngfile = TString(outputDir) + TString("rms/") + TString(filename) + TString("_rms.png");
	TString rmsroofile = TString(outputDir) + TString("rms/") + TString(filename) + TString("_rms.root");	

	c0->SetGridx(1);
	c0->SetGridy(1);

	double rmsmax[48];
	double rmsmaxer[48];
	for (int i=0;i<48;i++)
	{
		rmsmax[i]   = 2.0;
		rmsmaxer[i] = 0.0;
	}
	TGraphErrors*  Gr_1 = new TGraphErrors(48, npmt, pmtrms, npmter, pmtrmser);
	TGraphErrors*  Gr_11 = new TGraphErrors(48, npmt, rmsmax, npmter, rmsmaxer); 
	TMultiGraph* MG_1 = new TMultiGraph();
	Gr_1->SetMarkerStyle(20);
	Gr_1->SetMarkerColor(38);
	Gr_1->SetMarkerSize(1.2);
	Gr_11->SetLineColor(46);
	Gr_11->SetLineWidth(2);
	MG_1->Add(Gr_1, "P");
	MG_1->Add(Gr_11,"L");
	MG_1->Draw("A"); 
	MG_1->GetYaxis()->SetTitle("rms (ADC counts)");
	MG_1->GetXaxis()->SetTitle("PMT");
	MG_1->GetYaxis()->SetTitleSize(0.05);
	MG_1->GetXaxis()->SetTitleSize(0.05);
	MG_1->GetXaxis()->SetTitleOffset(0.8);
	MG_1->GetYaxis()->SetLabelSize(0.05);
	MG_1->GetXaxis()->SetLabelSize(0.05);
	MG_1->GetYaxis()->SetRangeUser(0.1,4.9); 
	MG_1->GetXaxis()->SetRangeUser(-2,48); 
	MG_1->GetXaxis()->SetNdivisions(25); 

	c0->Print(rmsepsfile,"eps");
	c0->Print(rmspngfile,"png");
	TString rmsplotfile = TString(outputWebDir) + TString("rms/") + TString(filename) + TString("_rms.png");
	result.addPlot("rms x PMT", rmsplotfile);
	wisIntegrator2Tilecomm(rmsplotfile, filename, result);
	if(level >2)
	{
		result.addWarning("rms level is higher than 2: root file generated");
		c0->Print(rmsroofile,"root");
		TString rmsrootweblink = TString(outputWebDir) + TString("rms/") + TString(filename) + TString("_rms.root");
		result.addLink("root file", rmsrootweblink);
	}
//=======================================================================


	if (level >0)
	{

		TString pmt_rofile1 = TString(outputDir) + TString("exp/pmt_") + TString(filename) + TString("_1.root");
		TString pmt_rofile2 = TString(outputDir) + TString("exp/pmt_") + TString(filename) + TString("_2.root");
		TString pmt_rofile3 = TString(outputDir) + TString("exp/pmt_") + TString(filename) + TString("_3.root");
		TString pmt_rofile4 = TString(outputDir) + TString("exp/pmt_") + TString(filename) + TString("_4.root");
	
		TString file_open = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds.ps[");
		TString file_close = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds.ps]");
		TString file_write = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds.ps");
	
		TString file_write1 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds1.png");
		TString file_write2 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds2.png");
		TString file_write3 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds3.png");
		TString file_write4 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds4.png");
		TCanvas *c1 = new TCanvas("c1", "PMTs 1-12", 1);
		canvas_style(c1);
		c1->Divide(4,3);	
		TCanvas *c2 = new TCanvas("c2", "PMTs 13-24", 1);
		canvas_style(c2);
		c2->Divide(4,3);
		TCanvas *c3 = new TCanvas("c3", "PMTs 25-36", 1);
		canvas_style(c3);
		c3->Divide(4,3);
		TCanvas *c4 = new TCanvas("c4", "PMTs 37-48", 1);
		canvas_style(c4);
		c4->Divide(4,3);
	
		c1->Print(file_open);
	
		for (int i=1;i<13;i++)
		{
			c1->cd(i);
			h_pmt_time[i-1]->GetYaxis()->SetRangeUser(pmtmean[i-1]-20,pmtmean[i-1]+20);
			h_pmt_time[i-1]->Draw();
			c2->cd(i);
			h_pmt_time[i+11]->GetYaxis()->SetRangeUser(pmtmean[i+11]-20,pmtmean[i+11]+20);
			h_pmt_time[i+11]->Draw();
			c3->cd(i);
			h_pmt_time[i+23]->GetYaxis()->SetRangeUser(pmtmean[i+23]-20,pmtmean[i+23]+20);
			h_pmt_time[i+23]->Draw();
			c4->cd(i);
			h_pmt_time[i+35]->GetYaxis()->SetRangeUser(pmtmean[i+35]-20,pmtmean[i+35]+20);
			h_pmt_time[i+35]->Draw();
		}
	
	
		c1->Print(file_write);
		c1->Print(file_write1);
		c2->Print(file_write);
		c2->Print(file_write2);
		c3->Print(file_write);
		c3->Print(file_write3);
		c4->Print(file_write);
		c4->Print(file_write4);
	
		for (int i=1;i<13;i++)
		{
			c1->cd(i);
			h_pmt[i-1]->GetXaxis()->SetRangeUser(pmtmean[i-1]-20,pmtmean[i-1]+20);
			h_pmt[i-1]->Draw();
			c2->cd(i);
			h_pmt[i+11]->GetXaxis()->SetRangeUser(pmtmean[i+11]-20,pmtmean[i+11]+20);
			h_pmt[i+11]->Draw();
			c3->cd(i);
			h_pmt[i+23]->GetXaxis()->SetRangeUser(pmtmean[i+23]-20,pmtmean[i+23]+20);
			h_pmt[i+23]->Draw();
			c4->cd(i);
			h_pmt[i+35]->GetXaxis()->SetRangeUser(pmtmean[i+35]-20,pmtmean[i+35]+20);
			h_pmt[i+35]->Draw();
		}
	
		TString file_write5 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds5.png");
		TString file_write6 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds6.png");
		TString file_write7 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds7.png");
		TString file_write8 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds8.png");
		TString file_close1 = TString(outputDir) + TString("peds/") + TString(filename) + TString("_peds.png]");
		c1->Print(file_write5);
		c2->Print(file_write6);
		c3->Print(file_write7);
		c4->Print(file_write8);
		c4->Print(file_close1);
		c1->Print(file_write);
		c2->Print(file_write);
		c3->Print(file_write);
		c4->Print(file_write);
		c4->Print(file_close);
		
		file_write1 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds1.png");
		file_write2 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds2.png");
		file_write3 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds3.png");
		file_write4 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds4.png");
		file_write5 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds5.png");
		file_write6 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds6.png");
		file_write7 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds7.png");
		file_write8 = TString(outputWebDir) + TString("peds/") + TString(filename) + TString("_peds8.png");
		
		result.addPlot("PMTs 1-12",file_write1);
		result.addPlot("PMTs 13-24", file_write2 );
		result.addPlot("PMTs 25-36", file_write3 );
		result.addPlot("PMTs 37-48", file_write4 );
		result.addPlot("PMTs 1-12", file_write5 );
		result.addPlot("PMTs 13-24", file_write6 );
		result.addPlot("PMTs 25-36", file_write7 );
		result.addPlot("PMTs 37-48",file_write8  );
		
		wisIntegrator2Tilecomm(file_write1, filename, result);
		wisIntegrator2Tilecomm(file_write2, filename, result);
		wisIntegrator2Tilecomm(file_write3, filename, result);
		wisIntegrator2Tilecomm(file_write4, filename, result);
		wisIntegrator2Tilecomm(file_write5, filename, result);
		wisIntegrator2Tilecomm(file_write6, filename, result);
		wisIntegrator2Tilecomm(file_write7, filename, result);
		wisIntegrator2Tilecomm(file_write8, filename, result);
	}
//=====================================================================

	if (level >3)
	{
		TCanvas *c5 = new TCanvas("c5", "timelines", 800,500);
		canvas_style(c5);
		c5->SetGridx(1);
		TString  pmttallepsfile= TString(outputDir) + TString("exp/exp_pmttalltime_") + TString(filename) + TString(".eps");
		TString  pmttallroofile= TString(outputDir) + TString("exp/exp_pmttalltime_") + TString(filename) + TString(".root");
		TString  pmttallpngfile= TString(outputDir) + TString("exp/exp_pmttalltime_") + TString(filename) + TString(".png");

		h_pmt_time[0]->SetYTitle("ADC counts");
		h_pmt_time[0]->SetTitleSize(0.05,"X");
		h_pmt_time[0]->SetTitleSize(0.05,"Y");
		h_pmt_time[0]->SetLabelSize(0.05,"X");
		h_pmt_time[0]->SetLabelSize(0.05,"Y");
		h_pmt_time[0]->GetYaxis()->SetRangeUser(pmtLow,pmtHigh);
		h_pmt_time[0]->SetMarkerSize(0.4);
		h_pmt_time[0]->SetMarkerStyle(20);
		h_pmt_time[0]->SetMarkerColor(32);
		h_pmt_time[0]->Draw();
		for (int i=1;i<10;i++)
		{
			h_pmt_time[i]->SetMarkerSize(0.4);
			h_pmt_time[i]->SetMarkerStyle(20);
			h_pmt_time[i]->SetMarkerColor(32+2*i);
			h_pmt_time[i]->Draw("same");
		}

		c5->Print(pmttallepsfile,"eps");
		c5->Print(pmttallroofile,"root");
		c5->Print(pmttallpngfile,"png");
		pmttallpngfile= TString(outputWebDir) + TString("exp/exp_pmttalltime_") + TString(filename) + TString(".png");
		result.addPlot("ADC counts", pmttallpngfile);
		wisIntegrator2Tilecomm(pmttallpngfile, filename, result);
	}

	if (level > 0)
	{
		TString correpsfile = TString(outputDir) + TString("corr/")+ TString(filename)+ TString("_corr.eps");
		TString corrpngfile = TString(outputDir) + TString("corr/")+ TString(filename)+ TString("_corr.png");
		TString corrroofile = TString(outputDir) + TString("corr/")+ TString(filename)+ TString("_corr.root");
		TString covaepsfile = TString(outputDir) + TString("corr/")+ TString(filename)+ TString("_cova.eps");
		TString covapngfile = TString(outputDir) + TString("corr/")+ TString(filename)+ TString("_cova.png");
		TString covaroofile = TString(outputDir) + TString("corr/")+ TString(filename)+ TString("_cova.root");

		TCanvas *c6 = new TCanvas("c6", "Coh noise", 800,500);
		//canvas_style(c6);
		c6->SetLeftMargin(0.12);
		c6->SetBottomMargin(0.15);
		gStyle->SetPadColor(10);
		gStyle->SetCanvasColor(10);
		gStyle->SetOptTitle(0);
		h_corr1->SetStats(0);
		h_cova1->SetStats(0);
		h_corr2->SetStats(0);
		h_cova2->SetStats(0);
		h_corr1->Draw("colz");
		c6->Print(correpsfile,"eps");
		c6->Print(corrpngfile,"png");
		corrpngfile = TString(outputWebDir) + TString("corr/")+ TString(filename)+ TString("_corr.png");
		result.addPlot("Coh Noise (CORR)", corrpngfile);
		wisIntegrator2Tilecomm(corrpngfile, filename, result);

		h_cova1->Draw("colz");
		c6->Print(covaepsfile,"eps");
		c6->Print(covapngfile,"png");
		covapngfile = TString(outputWebDir) + TString("corr/")+ TString(filename)+ TString("_cova.png");
		result.addPlot("Coh Noise (COVA)", covapngfile);
		wisIntegrator2Tilecomm(covapngfile, filename, result);
	}
	if (n0>3||n500>0||nnoise >0) 
	{
		TString warning;
		if (n0>3)     
		{
			ss<<n0;
			warning= TString("** Severe: ") + ss.str()+ TString(" PMTs without contact (should be 3) **");
			result.addWarning(warning);
			ss.str("");
		}
		if (n500>0)
		{
			ss<<n0;
			warning= ss.str()+ TString(" PMTs with pedestal > 500 ADC counts  ** \n Please report ! If HV in ON: Check for light leaks/ leakage currents !");
			result.addWarning(warning);
			ss.str("");
		}
		if (n700>0)
		{
			ss<<n0;
			warning= TString("** Severe: ") + ss.str()+ TString(" PMTs with pedestal > 700 ADC counts ** \n Please report ! If HV in ON: Check for light leaks/ leakage currents ! \n  ->Check if problem persists after HV stabilization time !");
			result.addWarning(warning);
			ss.str("");
		}
		if (nnoise>0) 
		{
			ss<<nnoise;
			warning= TString("** Severe: ") + ss.str()+ TString(" PMTs with pedestal > 2 ADC counts ** n Please report ! If HV in ON: Check for light leaks/ leakage currents ! \n  ->Check if problem persists after HV stabilization time !");
			result.addWarning(warning);
			ss.str("");
		}
		TString valueWarning;
		for (int i=1;i<48;i++)
		{
			ss<<i;
			warning= "pmt " + ss.str();
			ss.str("");
			ss<< pmtmean[i];
			valueWarning = ss.str();
			valueWarning += " +- ";
			ss.str("");
			ss<< pmtrms[i];
			valueWarning += ss.str();
			result.addValue(warning, valueWarning);
			ss.str("");
		}
	}
	
	result.addValue("average ADC counts",av_counts);
	result.addValue("average rms", av_rms);
	result.addValue("average covariance", av_cova);
	result.addValue("average correlation", av_corr);
	
	
	if(av_corr > 0.10)
	{
		result.addWarning("... Severe Warning: average correlation larger than 10%"); 
		is_ok = false;
	}
	else if(av_corr > 0.05)
	{
		result.addWarning("... Warning: average correlation larger than 5%");
	}
	TString status;
	if (is_ok)
	{
		status="OK";
		result.addValue("Status", status);
	}
	else
	{
		status="NOT OK";
		result.addValue("Status", status);
	}
	
	

	/* LINK TO ATLASMONITOR*/
	TString hour = TStrTok(TString(filename), 4);
	TString fullDate = dateFormat(date) + TString(" ") + hourFormat(hour);
	TString linkHref = "http://atlasmonitor.web.cern.ch/atlasmonitor/saveValuesExample.jsp?module=";
	linkHref += moduleName+ TString("&__rDate=");
	linkHref += fullDate + TString("&qcSheetType=team5&__warnings=\"");
	linkHref += status;
	linkHref += "\"&__avgADCcount=";
	linkHref +=av_counts ;
	linkHref += "&__avgRMS=";
	linkHref +=  av_rms;
	linkHref += "&__avgCova=";
	linkHref += av_cova;
	linkHref += "&__avgCorr=";
	linkHref += av_corr;
	linkHref += "&runType=Integrator";
 	TString URL = linkHref;
 	result.addURLToPage(URL);

	
	result.out();
	delete h_sumpmt;
	delete h_cova1;
	delete h_corr1;
	delete h_cova2;
	delete h_corr2;
	for(unsigned i =0 ; i<48; i++)
	{
		delete h_pmt[i];
		h_pmt[i] = NULL;
	}
	for(unsigned i =0 ; i<48; i++)
	{
		delete h_pmt_time[i];
		h_pmt_time[i] = NULL;
	}
	
}

void intped_exp(const char * filename, Int_t t_low, Int_t t_high, Int_t level, Int_t mincounts,Int_t maxcounts ) 
{
	ntwrite(filename);
	ntread(filename,t_low,t_high, level,mincounts,maxcounts);
}

void intped(const char * filename) 
{
	intped_exp(filename,10,20,1,-1,-1);
}

int main (int argc, char** argv)
{
	if(argc != 2)
	{
		cout<<"Usage:"<< argv[0]<< " [ntuple filename] "<<endl;
		exit (0);
	}
	ntread(argv[1],5,15,1,-1,-1);
	exit(0);
}
