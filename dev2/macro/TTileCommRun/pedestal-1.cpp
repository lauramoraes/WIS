void pedplots(TString runno, TString filename, TString module)
{

  Double_t maxhi = 2.5;
  Double_t maxlo = 1.2;

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

  TH1F *hhfhi[48], *hhflo[48], *hlfhi[48], *hlflo[48];

  TString nhfhi, thfhi, nhflo, thflo, nlfhi, tlfhi, nlflo, tlflo;

  Int_t i, j, k;
  for(j=0;j<48;j++){

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

    nlfhi = "lfhi";
    nlfhi += j;
    tlfhi = "LoFreqHi ";
    tlfhi += j;
    hlfhi[j]=new TH1F(nlfhi, tlfhi, 100, 0, 99);

    nlflo = "lflo";
    nlflo += j;
    tlflo = "LoFreqLo ";
    tlflo += j;
    hlflo[j]=new TH1F(nlflo, tlflo, 100, 0, 99);
  }

  Double_t evtmeanhi, evtmeanlo;
  Double_t evtrmshi, evtrmslo;

  for(i=0;i<nevt;i++){
    t->GetEntry(i);

    for(j=0;j<48;j++){
      evtmeanhi = 0.0;
      evtmeanlo = 0.0;
      evtrmshi = 0.0;
      evtrmslo = 0.0;

      for(k=0;k<nsamp;k++){
	evtmeanhi += (Double_t) samphi[j][k]/nsamp;
	evtmeanlo += (Double_t) samplo[j][k]/nsamp;
      }

      for(k=0;k<nsamp;k++){
	evtrmshi += (samphi[j][k]-evtmeanhi)**2/(nsamp-1);
	evtrmslo += (samplo[j][k]-evtmeanlo)**2/(nsamp-1);
      }

      evtrmshi = sqrt(evtrmshi);
      evtrmslo = sqrt(evtrmslo);

      hhfhi[j]->Fill(evtrmshi);
      hhflo[j]->Fill(evtrmslo);

      hlfhi[j]->Fill(samphi[j][0]);
      hlflo[j]->Fill(samplo[j][0]);
    }
  }

  Double_t chan[48], meanhi[48], rmshi[48], rmshfhi[48];
  Double_t meanlo[48], rmslo[48], rmshflo[48];
  Double_t avglo, avghi;
  vector<Int_t> bad;


  for(j=0;j<48;j++){
    chan[j]=j;

    meanhi[j]=hlfhi[j]->GetMean();
    rmshi[j]=hlfhi[j]->GetRMS();
    rmshfhi[j]=hhfhi[j]->GetMean();

    meanlo[j]=hlflo[j]->GetMean();
    rmslo[j]=hlflo[j]->GetRMS();
    rmshflo[j]=hhflo[j]->GetMean();

    if(j != 31 && j != 32 && j != 43){
      avglo += rmshflo[j]/45;
      avghi += rmshfhi[j]/45;
    }
    if(rmshflo[j] > maxlo || rmshfhi[j] > maxhi)
      bad.push_back(j);

  }

  TLine *lhigain = new TLine(0, maxhi, 50, maxhi);
  lhigain->SetLineColor(2);
  TLine *llogain = new TLine(0, maxlo, 50, maxlo);
  llogain->SetLineColor(2);

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

  TCanvas *c1 = new TCanvas("c1", "Pedestal", 700,700);
  c1->Divide(2,3);
  c1->cd(1);
  grmeanhi->Draw("AP");
  c1->cd(2);
  grmeanlo->Draw("AP");
  c1->cd(3);
  grrmshi->Draw("AP");
  c1->cd(4);
  grrmslo->Draw("AP");
  c1->cd(5);
  grrmshfhi->Draw("AP");
  lhigain->Draw();
  c1->cd(6);
  grrmshflo->Draw("AP");
  llogain->Draw();

  cout<<"Average high gain high-frequency RMS is "<<avghi<<endl;
  cout<<"Average low gain high-frequency RMS is "<<avglo<<endl;
  cout<<"Number of bad channels is "<<bad.size()<<endl;
  cout.precision(3);
  if(bad.size() > 0)
    cout<<"Channel"<<'\t'<<"Low gain"<<'\t'<<"High gain"<<endl;
  for(k=0;k<bad.size();k++){
    Int_t ch = bad.at(k);
    cout<<ch<<"\t" <<rmshflo[ch]<<"\t\t"<<rmshfhi[ch]<<endl;
  }

  TString filenameps = "r"+runno+"_"+module+"_Ped.ps";
  TString filenamepng = "r"+runno+"_"+module+"_Ped.png";
  c1->Print(filenameps);
  c1->Print(filenamepng);

}
