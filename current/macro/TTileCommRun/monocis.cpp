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
#include <TLeafI.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPolyMarker.h>
#include <TGraph.h>
//#include <stdlib.h>
#include "wis2Tilecomm.h"
#include "TWISMacroResult.h"

///Modified RMS and Mean Code
////
////by Alexis Kalogeropoulos

/*********Some definitions**************/
const Int_t MAXEVT_FOR_2D=100000;
const TString defaultNtuplesDir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
const TString outputDir = "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5_local/MonoCis/";
//const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/MonoCis/";
const TString outputWebDir = "http://tcws.web.cern.ch/tcws/WIS/results/team5_local/MonoCis/";
/***************************************/

void monplots(TString runno, TString filename, TString module, TWISMacroResult &results)
{

	Double_t maxenerms = 1.5;
	Double_t maxpedrms = 0.8;

	TString defdir = "castor/cern.ch/user/t/tilebeam/commissioning/";
	TString deffile ="tiletb_" +runno+ "_MonoCis.0.root";

	if(filename == "")
		filename = defdir+deffile;
	TFile *f = TFile::Open(filename);
	TTree *t = (TTree*)f->Get("TileRec/h1000");

	Float_t efit[48], pedfit[48];
	Int_t cispar[16];

	TString vare = "Efit"+module;
	TString varp = "Pedfit"+module;
	TString varc = "Cispar";
	t->SetBranchAddress(vare, &efit);
	t->SetBranchAddress(varp, &pedfit);
	t->SetBranchAddress(varc, &cispar);

	Int_t nevt = t->GetEntries();
	Int_t nevt_for_2D = (nevt>MAXEVT_FOR_2D) ? MAXEVT_FOR_2D : nevt;
	Int_t i, j, k;

	/********** MonExpert **********/
	Int_t samp[48][9];
	TString var = "Sample"+module;
	t->SetBranchAddress(var, &samp);

	TH1F *h[48];
	TH2F *c[48];
	TString hname, htitle;
	TString cname, ctitle;
	/*******************************/

	t->GetEntry(100);
	Double_t charge = 2*4.096*cispar[6]*cispar[7]/1023;
	Double_t uphist = charge +20.0;
	Double_t minmean = charge - 10.0;

	TH1F *hene[48], *hped[48];
	TString nene, tene, nped, tped;

	for(j=0;j<48;j++)
	{
		nene = "ene";
		nene += j;
		tene = "Energy ";
		tene += j;
		hene[j]=new TH1F(nene, tene, 100, 0, uphist);

		nped = "ped";
		nped += j;
		tped = "Pedestal ";
		tped += j;
		hped[j]=new TH1F(nped, tped, 100, 0, 100);
		/********** MonExpert **********/
		hname = "Amp";
		hname += j;
		htitle="Sample 3, ch";
		htitle += j;
		h[j]=new TH1F(hname, htitle, 100, 0, 200);

		cname = "amptime";
		cname += j;
		ctitle="Sample3vsTime,ch";
		ctitle += j;
		c[j]=new TH2F(cname, ctitle, nevt_for_2D/10, 0, nevt_for_2D, 100, 0, 200);
		/*******************************/
	}


	Double_t sumene[48]={0}, sqsumene[48]={0}, sumped[48]={0}, sqsumped[48]={0};
	Double_t chan[48]={0}, meanene[48]={0}, rmsene[48]={0}, meanped[48]={0}, rmsped[48]={0};
	vector<Int_t> bad;
	for(Int_t i=0;i<nevt;i++)
	{
		t->GetEntry(i);

		for(j=0;j<48;j++)
		{

			sumene[j] += efit[j];
			sqsumene[j] += (efit[j]*efit[j]);
			sumped[j] += pedfit[j];
			sqsumped[j] += (pedfit[j]*pedfit[j]);
			/********** MonExpert **********/
			if (i % 20 == 0 && i < MAXEVT_FOR_2D)
			{
				h[j]->Fill(samp[j][3]);
				c[j]->Fill(i, samp[j][3]);
			}
			/*******************************/
		}
	}

	Double_t badMeanEneX[48]={0}, badRMSEneX[48]={0}, badPedRMSX[48]={0};
	Double_t badMeanEneY[48]={0}, badRMSEneY[48]={0}, badPedRMSY[48]={0};
	Int_t NbadMeanEne = 0, NbadRMSEne = 0, NbadPedRMS = 0;

	for(j=0;j<48;j++)
	{
		chan[j]=j;
		if(j !=31 && j!=32 && j!=43)
		{   
			meanene[j]=(Double_t) sumene[j]/nevt; 
			if (meanene[j] < minmean)
			{
				badMeanEneX[NbadMeanEne] = j;
				badMeanEneY[NbadMeanEne] = meanene[j];
				NbadMeanEne++;
			}
			rmsene[j]= sqrt((Double_t) sqsumene[j]/nevt-(meanene[j]*meanene[j]));
			if (rmsene[j] > maxenerms)
			{
				badRMSEneX[NbadRMSEne] = j;
				badRMSEneY[NbadRMSEne] = rmsene[j];
				NbadRMSEne++;
			}
			meanped[j]=(Double_t) sumped[j]/nevt;
			rmsped[j]= sqrt((Double_t) sqsumped[j]/nevt-(meanped[j]*meanped[j]));
			if (rmsped[j] > maxpedrms)
			{
				badPedRMSX[NbadPedRMS] = j;
				badPedRMSY[NbadPedRMS] = rmsped[j];
				NbadPedRMS++;
			}
			if ((minmean > meanene[j]) || ( rmsene[j] > maxenerms) || ( rmsped[j] > maxpedrms))
				bad.push_back(j);
		}
	}
	TLine *lmean = new TLine(0, minmean, 50, minmean);
	lmean->SetLineColor(2);
	TLine *lenerms = new TLine(0, maxenerms, 50, maxenerms);
	lenerms->SetLineColor(2);
	TLine *lpedrms = new TLine(0, maxpedrms, 50, maxpedrms);
	lpedrms->SetLineColor(2);

	TPolyMarker *badMeanEne = new TPolyMarker(NbadMeanEne, badMeanEneX, badMeanEneY, "p");
	badMeanEne->SetMarkerColor(2);
	badMeanEne->SetMarkerStyle(21);

	TPolyMarker *badRMSEne = new TPolyMarker(NbadRMSEne, badRMSEneX, badRMSEneY, "p");
	badRMSEne->SetMarkerColor(2);
	badRMSEne->SetMarkerStyle(21);

	TPolyMarker *badPedRMS = new TPolyMarker(NbadPedRMS, badPedRMSX, badPedRMSY, "p");
	badPedRMS->SetMarkerColor(2);
	badPedRMS->SetMarkerStyle(21);

	TGraph *genemean = new TGraph(48, chan, meanene);
	genemean->SetMarkerStyle(21);
	genemean->SetTitle("Mean Energy");
	genemean->GetXaxis()->SetTitle("Channel");
	TGraph *generms = new TGraph(48, chan, rmsene);
	generms->SetMarkerStyle(21);
	generms->SetTitle("RMS Energy");
	generms->GetXaxis()->SetTitle("Channel");
	TGraph *gpedmean = new TGraph(48, chan, meanped);
	gpedmean->SetMarkerStyle(21);
	gpedmean->SetTitle("Mean Pedestal");
	gpedmean->GetXaxis()->SetTitle("Channel");
	TGraph *gpedrms = new TGraph(48, chan, rmsped);
	gpedrms->SetMarkerStyle(21);
	gpedrms->SetTitle("RMS Pedestal");
	gpedrms->GetXaxis()->SetTitle("Channel");

	TCanvas *cmon = new TCanvas("cmon", "MonoCis", 700, 500);
	cmon->Divide(2,2);
	cmon->cd(1);
	genemean->Draw("AP");
	lmean->Draw();
	cmon->cd(2);
	generms->Draw("AP");
	lenerms->Draw();
	cmon->cd(3);
	gpedmean->Draw("AP");
	cmon->cd(4);
	gpedrms->Draw("AP");
	lpedrms->Draw();


	cout.precision(3);
	results.addValue("Number of bad channels", bad.size());

	if(bad.size() > 0)
	{
		//cout<<"Channel"<<'\t'<<"Mean energy"<<'\t'<<"RMS energy"<<'\t'<<"RMS Monocis"<<"<br />"<<endl;
		TString table;
		table  = "<table class=\"main\" xmlns=\"http://www.w3.org/1999/xhtml\">\n";
		table += "<tr><th>Channel</th><th>Mean energy</th><th>RMS energy</th><th>RMS Monocis</th></tr>\n";

		for(i=0;i<bad.size();i++)
		{
			Int_t ch = bad.at(i);
			//cout<<ch<<"\t"<<meanene[i]<<"\t\t"<<rmsene[i]<<"\t\t"<<rmsped[i]<<"<br />"<<endl;
			table += "<tr><td>"; table += ch; table += "</td><td>"; table += meanene[i]; table += "</td><td>";
			table += rmsene[i]; table +="</td><td>"; table += rmsped[i]; table +="</td></tr>\n";
		}

		table += "</table>\n";
		results.addTable("Bad channels", table);
	}

	TString moduleDir = "LB" + module + "/";

	TString filenameps = outputDir + moduleDir +"r"+runno+"_"+module+"_MonoCis.ps";
	TString filenamepng = outputDir + moduleDir +"r"+runno+"_"+module+"_MonoCis.png";
	cmon->Print(filenameps);
	cmon->Print(filenamepng);

	TString plotFilename = outputWebDir + moduleDir + "r"+runno+"_"+module+"_MonoCis.png";
	TString plotPsFilename = outputWebDir + moduleDir + "r"+runno+"_"+module+"_MonoCis.ps";

	TString completeModuleName = "LB" + module;
	wis2Tilecomm(runno.Data(), completeModuleName.Data(), plotFilename.Data(), results);
	wis2Tilecomm(runno.Data(), completeModuleName.Data(), plotPsFilename.Data(), results);

	TString linkTitle = "Send results to QC Sheet";
	TString linkHref = "http://atlasmonitor.web.cern.ch/atlasmonitor/saveValuesExample.jsp?module=";
	linkHref += "LB" + module + "&run=" + runno + "&badChannels=";
	linkHref += bad.size();
	//results.addLink(linkTitle, linkHref);

	results.addPlot("", plotFilename);


	/********** MonExpert **********/

	TCanvas *c1 = new TCanvas("c1", "Amp,1-24", 900, 900);
	c1->Divide(4,6);
	TCanvas *c5 = new TCanvas("c5", "Amp,25-48", 900, 900);
	c5->Divide(4,6);

	TCanvas *c3 = new TCanvas("c3", "AmpTime,1-24", 900, 900);
	c3->Divide(4,6);
	TCanvas *c7 = new TCanvas("c7", "AmpTime,25-48",900,900);
	c7->Divide(4,6);

	for(j = 0; j < 24; j++)
	{
		c1->cd(j+1);
		gPad->SetLogy();
		h[j]->Draw();
		c5->cd(j+1);
		gPad->SetLogy();
		h[j+24]->Draw();

		c3->cd(j+1);
		c[j]->Draw();
		c7->cd(j+1);
		c[j+24]->Draw();

	}

	// Saving plots to disk
	filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_MonoCis_Expert_Amp1-24.png";
	c1->Print(filenamepng);
	filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_MonoCis_Expert_Amp25-48.png";
	c5->Print(filenamepng);
	filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_MonoCis_Expert_AmpTime1-24.png";
	c3->Print(filenamepng);
	filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_MonoCis_Expert_AmpTime25-48.png";
	c7->Print(filenamepng);

	completeModuleName = "LB" + module;

	// Saving plots to Results Database (TileComm Analysis)
	TString plot1 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_MonoCis_Expert_Amp1-24.png";
	wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot1.Data(), results);

	TString plot2 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_MonoCis_Expert_Amp25-48.png";
	wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot2.Data(), results);

	TString plot3 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_MonoCis_Expert_AmpTime1-24.png";
	wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot3.Data(), results);

	TString plot4 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_MonoCis_Expert_AmpTime25-48.png";
	wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot4.Data(), results);


	// Showing links in macro results page
	results.addValue("MonoCis Expert Plots", "");
	results.addLink("Amp Channels 1-24", plot1);
	results.addLink("Amp Channels 25-48", plot2);
	results.addLink("AmpTime Channels 1-24", plot3);
	results.addLink("AmpTime Channels 25-48", plot4);

	/*******************************/
}
