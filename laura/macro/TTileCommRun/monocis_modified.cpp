////////////Main Code - You must add more lines if you want to run in WIS
///Modified RMS and Mean Code
////
////by Alexis Kalogeropoulos
void monplots(TString runno, TString filename, TString module)
{

	Double_t maxenerms = 1.5;
	Double_t maxpedrms = 0.8;

	TString defdir = "rfio:/castor/cern.ch/user/t/tilebeam/commissioning/";
	TString deffile = "tiletb_"+TString(runno)+TString("_MonoCis.0.root");

	if(filename == ""){
		filename = defdir+deffile;
	}

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
	Int_t i, j, k;

	t->GetEntry(i);
	Double_t charge = 2*4.096*cispar[6]*cispar[7]/1023;
	Double_t uphist = charge +20.0;
	Double_t minmean = charge - 10.0;

	TH1F *hene[48], *hped[48];
	TString nene, tene, nped, tped;

	for(j=0;j<48;j++){
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
	}


	Double_t sumene[48]={0}, sqsumene[48]={0}, sumped[48]={0}, sqsumped[48]={0};
	Double_t chan[48]={0}, meanene[48]={0}, rmsene[48]={0}, meanped[48]={0}, rmsped[48]={0};
	vector<Int_t> bad;
	for(Int_t i=0;i<nevt;i++){
		t->GetEntry(i);

		for(j=0;j<48;j++){

			sumene[j] += efit[j];
			sqsumene[j] += efit[j]**2;
			sumped[j] += pedfit[j];
			sqsumped[j] += pedfit[j]**2;

		}
	}

	cout<<minmean<<"\t\t"<<maxenerms<<"\t\t"<<maxpedrms<<endl;

	for(j=0;j<48;j++){
		chan[j]=j;
		if(j !=31 && j!=32 && j!=43){   


			meanene[j]=(Double_t) sumene[j]/nevt; 
			rmsene[j]= sqrt((Double_t) sqsumene[j]/nevt-meanene[j]**2);

			meanped[j]=(Double_t) sumped[j]/nevt;
			rmsped[j]= sqrt((Double_t) sqsumped[j]/nevt-meanped[j]**2);
			cout<<j<<"\t\t"<<meanene[j]<<"\t\t"<<rmsene[j]<<"\t\t"<<rmsped[j]<<"\t\t"<<meanped[j]<<"  "<<endl;

			///Please, do not modify the next 2 lines you ll get crazy plots!!!
			////////////////////////////////////////////////////////////////
			if ((minmean > meanene[j]) || ( rmsene[j] > maxenerms) || ( rmsped[j] > maxpedrms))
				bad.push_back(j);


			/////////Lines below must change in order this code to work in the WIS interface
			////////Refer to monocis.cpp for more info/////////////
			/////// End of modifications///////////////////////////

			TLine *lmean = new TLine(0, minmean, 50, minmean);
			lmean->SetLineColor(2);
			TLine *lenerms = new TLine(0, maxenerms, 50, maxenerms);
			lenerms->SetLineColor(2);
			TLine *lpedrms = new TLine(0, maxpedrms, 50, maxpedrms);
			lpedrms->SetLineColor(2);

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
		}
	}	
	TCanvas *c1 = new TCanvas("c1", "MonoCis", 700, 500);
	c1->Divide(2,2);
	c1->cd(1);
	genemean->Draw("AP");
	lmean->Draw();
	c1->cd(2);
	generms->Draw("AP");
	lenerms->Draw();
	c1->cd(3);
	gpedmean->Draw("AP");
	c1->cd(4);
	gpedrms->Draw("AP");
	lpedrms->Draw();

	cout.precision(3);
	cout<<"Number of bad channels is "<<bad.size()<<endl;

	if(bad.size() > 0)
		cout<<"Channel"<<'\t'<<"Mean energy"<<'\t'<<"RMS energy"<<'\t'<<"RMS pedestal"<<'\t'<<"Mean pedestal "<<endl;

	for(i=0;i<bad.size();i++){
		Int_t ch = bad.at(i);
		cout<<ch<<"\t"<<meanene[i]<<"\t\t"<<rmsene[i]<<"\t\t"<<rmsped[i]<<"\t\t"<<meanped[i]<<endl;
	}

	TString filenameps = "r"+runno+"_"+module+"_MonoCis.ps";
	TString filenamepng = "r"+runno+"_"+module+"_MonoCis.png";
	c1->Print(filenameps);
	c1->Print(filenamepng);

}
