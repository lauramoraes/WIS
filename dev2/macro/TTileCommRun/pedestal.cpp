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
#include "TWISMacroResult.h"
#include "wis2Tilecomm.h" 

const Int_t MAXEVT_FOR_2D=100000;

		/*********Some definitions**************/
const TString defaultNtuplesDir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
const TString outputDir = "/afs/cern.ch/user/t/tilecom/public/www/WIS/results/team5_local/pedestal/";
//const TString standOutputDir = "/afs/cern.ch/user/t/tilebeam/scratch0/Commissioning/results/team5/pedestal/";
const TString outputWebDir = "http://tcws.web.cern.ch/tcws/WIS/results/team5_local/pedestal/";

		/***************************************/
void pedplots(TString runno, TString filename, TString module, TWISMacroResult & results)
{

  Double_t maxhi = 2.5;
  Double_t maxlo = 1.2;

  Double_t maxhiRMS = 3.5;
  Double_t maxloRMS = 2.2;

  TString defdir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
  TString deffile = "tiletb_"+runno+"_Ped.0.root";
 
  if(filename == ""){
    filename = defdir+deffile;
  }

  TFile *f = TFile::Open(filename);
  TTree *t = (TTree*)f->Get("TileRec/h1000");

  const Int_t nsamp = 7;

  Int_t samphi[48][nsamp], samplo[48][nsamp];
  TString varhi = "Sample"+module+"hi";
  TString varlo = "Sample"+module+"lo";
  t->SetBranchAddress(varhi, &samphi);
  t->SetBranchAddress(varlo, &samplo);

  Int_t nevt = t->GetEntries();

  Int_t nevt_for_2D = (nevt>MAXEVT_FOR_2D) ? MAXEVT_FOR_2D : nevt;

  TH1F *hhfhi[48], *hhflo[48], *hlfhi[48], *hlflo[48];

  TString nhfhi, thfhi, nhflo, thflo, nlfhi, tlfhi, nlflo, tlflo;


  /********** PedExpert **********/
  /*Int_t samphiExp[48][7], samploExp[48][7];
  TString varhiExp = "Sample"+module+"hi";
  TString varloExp = "Sample"+module+"lo";
  t->SetBranchAddress(varhiExp, &samphi);
  t->SetBranchAddress(varloExp, &samplo);
  */
  TH1F *hhi[48], *hlo[48];
  TH2F *chi[48], *clo[48];
  TString hnamehi, htitlehi, hnamelo, htitlelo;
  TString cnamehi, ctitlehi, cnamelo, ctitlelo;
  /*******************************/

  Int_t i, j, k;
  for(j = 0; j < 48; j++){

    nhfhi = "hfhi";
    nhfhi += j;
    thfhi = "HiFreqHi ";
    thfhi += j;
    hhfhi[j]=new TH1F(nhfhi, thfhi, 70, 0, 5.0);

    nhflo = "hflo";
    nhflo += j;
    thflo = "HiFreqLo ";
    thflo += j;
    hhflo[j]=new TH1F(nhflo, thflo, 70, 0, 5.0);

    //////////////////////
    // CHANGE: These histograms will no longer be needed.
    ///////////////////////
    /*nlfhi = "lfhi";
    nlfhi += j;
    tlfhi = "LoFreqHi ";
    tlfhi += j;
    hlfhi[j]=new TH1F(nlfhi, tlfhi, 100, 0, 99);

    nlflo = "lflo";
    nlflo += j;
    tlflo = "LoFreqLo ";
    tlflo += j;
    hlflo[j]=new TH1F(nlflo, tlflo, 100, 0, 99);*/


    /********** PedExpert **********/
    hnamehi = "pedhi";
    hnamehi += j;
    htitlehi="Pedestal,hi,ch";
    htitlehi += j;
    hhi[j]=new TH1F(hnamehi, htitlehi, 100, 0, 100);

    hnamelo = "pedlo";
    hnamelo += j;
    htitlelo="Pedestal,lo,ch";
    htitlelo += j;
    hlo[j]=new TH1F(hnamelo, htitlelo, 100, 0, 100);

    cnamehi = "pedtimehi";
    cnamehi += j;
    ctitlehi="PedTime,hi,ch";
    ctitlehi += j;
    chi[j]=new TH2F(cnamehi, ctitlehi, nevt_for_2D/10, 0, nevt_for_2D, 100, 0, 100);

    cnamelo = "pedtimelo";
    cnamelo += j;
    ctitlelo="PedTime,lo,ch";
    ctitlelo += j;
    clo[j]=new TH2F(cnamelo, ctitlelo, nevt_for_2D/10, 0, nevt_for_2D, 100, 0, 100);
    /*******************************/
  }

  Double_t evtmeanhi, evtmeanlo;
  Double_t evtrmshi, evtrmslo;

  ////////////
  // CHANGE: New variables defined
  ///////////////
  Double_t lfsumhi[48]={0}, lfsumlo[48]={0}, lfsqsumhi[48]={0}, lfsqsumlo[48]={0};

  for(i=0;i<nevt;i++)
  {
    t->GetEntry(i);

    for(j=0;j<48;j++)
    {
      evtmeanhi = 0.0;
      evtmeanlo = 0.0;
      evtrmshi = 0.0;
      evtrmslo = 0.0;

      for(k=0;k<nsamp;k++){
        evtmeanhi += (Double_t) samphi[j][k]/nsamp;
        evtmeanlo += (Double_t) samplo[j][k]/nsamp;
      }

      for(k=0;k<nsamp;k++){
        evtrmshi += (samphi[j][k]-evtmeanhi)*(samphi[j][k]-evtmeanhi)/(nsamp-1);
        evtrmslo += (samplo[j][k]-evtmeanlo)*(samplo[j][k]-evtmeanlo)/(nsamp-1);
      }

      evtrmshi = sqrt(evtrmshi);
      evtrmslo = sqrt(evtrmslo);

      hhfhi[j]->Fill(evtrmshi);
      hhflo[j]->Fill(evtrmslo);

      ////////////////
      // CHANGE: Don't need to fill lf histograms and do part of low-frequency RMS calculation
      /////////////
      //hlfhi[j]->Fill(samphi[j][0]);
      //hlflo[j]->Fill(samplo[j][0]);

      lfsumhi[j] += samphi[j][0];
      lfsqsumhi[j] += samphi[j][0]*samphi[j][0];
      lfsumlo[j] += samplo[j][0];
      lfsqsumlo[j] += samplo[j][0]*samplo[j][0];

      /********** PedExpert **********/
      if (i % 20 == 0 && i < MAXEVT_FOR_2D)
      {
        for(k = 0; k < nsamp; k++)
        {
          hhi[j]->Fill(samphi[j][k]);
          hlo[j]->Fill(samplo[j][k]);
          chi[j]->Fill(i, samphi[j][k]);
          clo[j]->Fill(i, samplo[j][k]);
        }
      }
      /*******************************/

    }
  }

  Double_t chan[48], meanhi[48], rmshi[48], rmshfhi[48];
  Double_t meanlo[48], rmslo[48], rmshflo[48];
  Double_t avglo = 0.0, avghi = 0.0;
  vector<Int_t> bad;

  Double_t badRMShiX[48], badRMSloX[48], badRMShfhiX[48], badRMShfloX[48];
  Double_t badRMShiY[48], badRMSloY[48], badRMShfhiY[48], badRMShfloY[48];
  Int_t NbadRMShi = 0, NbadRMSlo = 0, NbadRMShfhi = 0, NbadRMShflo = 0;

  for(j=0;j<48;j++){
    chan[j]=j;

    ///////////////
    //CHANGE: low-frequency RMS calculations for high and low gain are different.
    ///////////
    meanhi[j]=(Double_t)(lfsumhi[j]/((Double_t)nevt));
    rmshi[j]=sqrt(lfsqsumhi[j]/((Double_t)nevt) - (meanhi[j]*meanhi[j]));

    //meanhi[j]=hlfhi[j]->GetMean();
    //rmshi[j]=hlfhi[j]->GetRMS();
    if (rmshi[j] > maxhiRMS)
    {
    	//test->SetPoint(j, j, rmshi[j]);
    	badRMShiX[NbadRMShi] = j;
    	badRMShiY[NbadRMShi] = rmshi[j];
    	NbadRMShi++;
    }

    rmshfhi[j]=hhfhi[j]->GetMean();
    if (rmshfhi[j] > maxhi)
    {
    	badRMShfhiX[NbadRMShfhi] = j;
    	badRMShfhiY[NbadRMShfhi] = rmshfhi[j];
    	NbadRMShfhi++;
    }	

    meanlo[j]= lfsumlo[j]/((Double_t)nevt);
    rmslo[j]=sqrt(lfsqsumlo[j]/((Double_t)nevt) - (meanlo[j]*meanlo[j]));
    //meanlo[j]=hlflo[j]->GetMean();
    //rmslo[j]=hlflo[j]->GetRMS();
    if (rmslo[j] > maxloRMS)
    {
    	badRMSloX[NbadRMSlo] = j;
    	badRMSloY[NbadRMSlo] = rmslo[j];
    	NbadRMSlo++;
    }	

    rmshflo[j]=hhflo[j]->GetMean();
    if (rmshflo[j] > maxlo)
    {
    	badRMShfloX[NbadRMShflo] = j;
    	badRMShfloY[NbadRMShflo] = rmshflo[j];
    	NbadRMShflo++;
    }

    if(j != 31 && j != 32 && j != 43){
      avglo += rmshflo[j]/45;
      avghi += rmshfhi[j]/45;
    }
    if(rmshflo[j] > maxlo || rmshfhi[j] > maxhi || rmslo[j] > maxloRMS || rmshi[j] > maxhiRMS)
      bad.push_back(j);

  }

  TLine *lhigain = new TLine(0, maxhi, 50, maxhi);
  lhigain->SetLineColor(2);
  TLine *llogain = new TLine(0, maxlo, 50, maxlo);
  llogain->SetLineColor(2);

  TLine *lhigainRMS = new TLine(0, maxhiRMS, 50, maxhiRMS);
  lhigainRMS->SetLineColor(2);
  TLine *llogainRMS = new TLine(0, maxloRMS, 50, maxloRMS);
  llogainRMS->SetLineColor(2);

  TPolyMarker *badRMShi = new TPolyMarker(NbadRMShi, badRMShiX, badRMShiY, "p");
  badRMShi->SetMarkerColor(2);
  badRMShi->SetMarkerStyle(21);

  TPolyMarker *badRMSlo = new TPolyMarker(NbadRMSlo, badRMSloX, badRMSloY, "p");
  badRMSlo->SetMarkerColor(2);
  badRMSlo->SetMarkerStyle(21);

  TPolyMarker *badRMShfhi = new TPolyMarker(NbadRMShfhi, badRMShfhiX, badRMShfhiY, "p");
  badRMShfhi->SetMarkerColor(2);
  badRMShfhi->SetMarkerStyle(21);

  TPolyMarker *badRMShflo = new TPolyMarker(NbadRMShflo, badRMShfloX, badRMShfloY, "p");
  badRMShflo->SetMarkerColor(2);
  badRMShflo->SetMarkerStyle(21);

  TGraph *grmeanhi = new TGraph(48, chan, meanhi);
  grmeanhi->SetMarkerStyle(21);
  grmeanhi->SetTitle("Mean pedestal, high gain");
  grmeanhi->GetXaxis()->SetTitle("Channel");
  TGraph *grrmshi = new TGraph(48, chan, rmshi);
  grrmshi->SetMarkerStyle(21);
  grrmshi->SetTitle("RMS pedestal, high gain");
  grrmshi->GetXaxis()->SetTitle("Channel");
  TGraph *grrmshfhi = new TGraph(48, chan, rmshfhi);
  grrmshfhi->SetMarkerStyle(21);
  grrmshfhi->SetTitle("Mean high-frequency RMS, high gain");
  grrmshfhi->GetXaxis()->SetTitle("Channel");

  TGraph *grmeanlo = new TGraph(48, chan, meanlo);
  grmeanlo->SetMarkerStyle(21);
  grmeanlo->SetTitle("Mean pedestal, low gain");
  grmeanlo->GetXaxis()->SetTitle("Channel");
  TGraph *grrmslo = new TGraph(48, chan, rmslo);
  grrmslo->SetMarkerStyle(21);
  grrmslo->SetTitle("RMS pedestal, low gain");
  grrmslo->GetXaxis()->SetTitle("Channel");
  TGraph *grrmshflo = new TGraph(48, chan, rmshflo);
  grrmshflo->SetMarkerStyle(21);
  grrmshflo->SetTitle("Mean high-frequency RMS, low gain");
  grrmshflo->GetXaxis()->SetTitle("Channel");

  TCanvas *cped = new TCanvas("cped", "Pedestal", 700,700);
  cped->Divide(2,3);
  cped->cd(1);
  grmeanhi->Draw("AP");
  cped->cd(2);
  grmeanlo->Draw("AP");
  cped->cd(3);
  grrmshi->Draw("AP");
  lhigainRMS->Draw();
  badRMShi->Draw();
  cped->cd(4);
  grrmslo->Draw("AP");
  llogainRMS->Draw();
  badRMSlo->Draw();
  cped->cd(5);
  grrmshfhi->Draw("AP");
  lhigain->Draw();
  badRMShfhi->Draw();
  cped->cd(6);
  grrmshflo->Draw("AP");
  llogain->Draw();
  badRMShflo->Draw();

  //cout<<"Average high gain high-frequency RMS is "<<avghi<<"<br />"<<endl;
  results.addValue("Average high gain high-frequency RMS", avghi);
  //cout<<"Average low gain high-frequency RMS is "<<avglo<<"<br />"<<endl;
  results.addValue("Average low gain high-frequency RMS", avglo);
  //cout<<"Number of bad channels is "<<bad.size()<<"<br />"<<endl;
  results.addValue("Number of bad channels", bad.size());
  cout.precision(3);
  TString table;
  if(bad.size() > 0)
  {
    //cout<<"Channel"<<'\t'<<"Low gain"<<'\t'<<"High gain"<<"<br />"<<endl;
    table  = "<table class=\"main\" xmlns=\"http://www.w3.org/1999/xhtml\">";
    table += "<tr><th>Channel</th><th>Low gain RMS</th><th>High gain RMS</th></tr>";

    for(k=0;k<bad.size();k++)
    {
      Int_t ch = bad.at(k);
      //cout<<ch<<"\t" <<rmshflo[ch]<<"\t\t"<<rmshfhi[ch]<<"<br />"<<endl;
      table += "<tr><td>"; table += ch; table += "</td><td>"; table += rmshflo[ch]; table += "</td><td>";
      table += rmshfhi[ch]; table +="</td></tr>";
    }

    table += "</table>";
    results.addTable("Bad channels", table);
  }

  TString moduleDir = "LB" + module + "/";
  
  TString filenameps = outputDir + moduleDir +"r"+runno+"_"+module+"_Ped_v2.ps";
  TString filenamepng =outputDir + moduleDir +"r"+runno+"_"+module+"_Ped_v2.png";
  cped->Print(filenameps);
  cped->Print(filenamepng);

  TString plotFilename= outputWebDir + moduleDir + "r"+runno+"_"+module+"_Ped_v2.png";
  TString plotPsFilename= outputWebDir + moduleDir + "r"+runno+"_"+module+"_Ped_v2.ps";

  TString completeModuleName = "LB" + module;
  wis2Tilecomm(runno, completeModuleName, plotFilename.Data(), results);
  wis2Tilecomm(runno, completeModuleName, plotPsFilename.Data(), results);

  TString linkTitle = "Send results to QC Sheet";
  TString linkHref = "http://atlasmonitor.web.cern.ch/atlasmonitor/saveValuesExample.jsp?module=";
  linkHref += "LB" + module + "&run=" + runno + "&runType=Ped" + "&qcSheetType=team5&__avgNoiseHi=";
  linkHref += avghi;
  linkHref += "&__avgNoiseLo=";
  linkHref += avglo;
  linkHref += "&__badChannels=";
  linkHref += bad.size();
  linkHref += "&_tbadChannelsTable=";
  linkHref += table;
  //results.addLink(linkTitle, linkHref);
  TString URL = linkHref;
  results.addURLToPage(URL);


  /********** PedExpert **********/
  TCanvas *c1 = new TCanvas("c1", "PedHi,1-24", 900, 900);
  c1->Divide(4,6);
  TCanvas *c5 = new TCanvas("c5", "PedHi,25-48", 900, 900);
  c5->Divide(4,6);

  TCanvas *c2 = new TCanvas("c2", "PedLo,1-24", 900, 900);
  c2->Divide(4,6);
  TCanvas *c6 = new TCanvas("c6", "PedLo,25-48",900,900);
  c6->Divide(4,6);

  TCanvas *c3 = new TCanvas("c3", "PedTimeHi,1-24", 900, 900);
  c3->Divide(4,6);
  TCanvas *c7 = new TCanvas("c7", "PedTimeHi,25-48",900,900);
  c7->Divide(4,6);

  TCanvas *c4 = new TCanvas("c4", "PedTimeLo,1-24", 900, 900);
  c4->Divide(4,6);
  TCanvas *c8 = new TCanvas("c8", "PedTimeLo,25-48",900,900);
  c8->Divide(4,6);


  for(j=0;j<24;j++){
    c1->cd(j+1);
    gPad->SetLogy();
    hhi[j]->Draw();
    c5->cd(j+1);
    gPad->SetLogy();
    hhi[j+24]->Draw();

    c2->cd(j+1);
    gPad->SetLogy();
    hlo[j]->Draw();
    c6->cd(j+1);
    gPad->SetLogy();
    hlo[j+24]->Draw();

    c3->cd(j+1);
    chi[j]->Draw();
    c7->cd(j+1);
    chi[j+24]->Draw();

    c4->cd(j+1);
    clo[j]->Draw();
    c8->cd(j+1);
    clo[j+24]->Draw();

  }

  // Saving plots to disk
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedHi1-24.png";
  c1->Print(filenamepng);
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedHi25-48.png";
  c5->Print(filenamepng);
  
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedLo1-24.png";
  c2->Print(filenamepng);
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedLo25-48.png";
  c6->Print(filenamepng);
  
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedTimeHi1-24.png";
  c3->Print(filenamepng);
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedTimeHi25-48.png";
  c7->Print(filenamepng);
  
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedTimeLo1-24.png";
  c4->Print(filenamepng);
  filenamepng = outputDir + "expert/" + moduleDir + "r" + runno + "_" + module + "_Ped_Expert_PedTimeLo25-48.png";
  c8->Print(filenamepng);
  
  completeModuleName = "LB" + module;

  // Saving plots to Results Database (TileComm Analysis)
  TString plot1 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedHi1-24.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot1.Data(), results);

  TString plot2 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedHi25-48.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot2.Data(), results);

  TString plot3 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedLo1-24.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot3.Data(), results);

  TString plot4 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedLo25-48.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot4.Data(), results);

  TString plot5 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedTimeHi1-24.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot5.Data(), results);

  TString plot6 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedTimeHi25-48.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot6.Data(), results);

  TString plot7 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedTimeLo1-24.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot7.Data(), results);

  TString plot8 = outputWebDir + "expert/" + moduleDir + "r"+runno+"_"+module+"_Ped_Expert_PedTimeLo25-48.png";
  wis2Tilecomm(runno.Data(), completeModuleName.Data(), plot8.Data(), results);

  
  // Showing links in macro results page
  results.addValue("Pedestal Expert Plots", "");
  results.addLink("High Gain Channels 1-24", plot1);
  results.addLink("High Gain Channels 25-48", plot2);
  results.addLink("Low Gain Channels 1-24", plot3);
  results.addLink("Low Gain Channels 25-48", plot4);
  results.addLink("Time High Gain Channels 1-24", plot5);
  results.addLink("Time High Gain Channels 25-48", plot6);
  results.addLink("Time Low Gain Channels 1-24", plot7);
  results.addLink("Time Low Gain Channels 25-48", plot8);

  /*******************************/

}

