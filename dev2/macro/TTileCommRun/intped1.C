void ntwrite( const char * filename){
  char  outfilename[150];
  char  infilename[150];
  ifstream in; 
  sprintf(outfilename,"data/%s.root", filename);   
  sprintf(infilename,"data/%s", filename);

  in.open(infilename);  
  if (in.is_open() != 1){
    printf("\n Warning: could not open file %s !!", infilename);
    printf("\n Ntuple is not recreated  !!\n");
    return;
  }else{
 
    TFile *f = new TFile(outfilename,"RECREATE");
    TTree int1("int1","");
    Int_t x[50], time,pmtsum;
    Int_t pmt[48];

    printf("\n ...creating ntuple\n"); 
    
    int1.Branch("pmt",&pmt,"pmt[48]/I");   
    int1.Branch("pmtsum",&pmtsum,"pmtsum/I");
    int1.Branch("time",&time,"time/I");

    int kk=0;

    while (1) {
     
      pmtsum = 0.; 
      for (int i=0; i<48; i++){ 
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
  }
  in.close();
}

 
void ntread( const char * filename, Double_t t_low, Double_t t_high, Int_t level, Int_t mincounts,Int_t maxcounts){

  bool is_ok = 1;

  gStyle->SetPalette(1);
  printf("\n ...opening ntuple\n"); 

  char  inputfile[300];
  char  logfile[200];
  sprintf(logfile,"out/logs/%s.log", filename);
  //sprintf(inputfile,"data/%s.root", filename);   
  sprintf(inputfile,"root://tilebeam@pcata007.cern.ch//work/commissioning/team4/integrator/offline/%s.root", filename); 
  printf("\n inputfile: %s ",inputfile);
  ifstream in; 
  FILE *log = fopen(logfile,"w");
  //in.open(inputfile); 
  //if (in.is_open() != 1){
  //printf("\n Warning: could not open file  %s !!", inputfile);
  //printf("\n\n");
  //return;
  //}else{ 
  {
    printf("logfile created: %s \n\n", logfile); 
    //TFile* f = (TFile*) TFile::Open("root://tilebeam@pcata007.cern.ch//work/commissioning/integrator/offline/int_peds_LBC14_oct19.out.root");  
    TFile* f = (TFile*) TFile::Open(inputfile);
 
  printf("\n ...processing  ntuple\n\n");
 
    //TFile *f = new TFile(inputfile); 
    TTree *int1 = (TTree*)f->Get("int1");
    
    Int_t pmtsum, time;
    Double_t partsum[48], partrms[48], partrmser[48], partrms2[48],partrms2er[48] ;
    Double_t pmtrms[48], pmtrmser[48], pmtmean[48];
    Double_t totvar,sumvar,totrms,totrmser,rmscohpc,totcoh, cohfrac;
    Int_t pmt[48];
    
    Int_t nentries = (Int_t)int1->GetEntries();

    Int_t tHigh = nentries;
    Int_t tLow = 0;
    
    if (t_high != -1){
      tHigh = t_high;
      tLow  = t_low;
    }
    
    Int_t pmtHigh = 1000;
    Int_t pmtLow = 0;
    
    if (maxcounts != -1){
      pmtHigh = maxcounts;
    } 
    if (mincounts != -1){
      pmtLow = mincounts;
    } 
    
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


    TH1D *h_pmt0 = new TH1D("h_pmt0","pmt 0 signal",Nstep,pmtLow, pmtHigh);   
    TH1D *h_pmt1 = new TH1D("h_pmt1","pmt 1 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt2 = new TH1D("h_pmt2","pmt 2 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt3 = new TH1D("h_pmt3","pmt 3 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt4 = new TH1D("h_pmt4","pmt 4 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt5 = new TH1D("h_pmt5","pmt 5 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt6 = new TH1D("h_pmt6","pmt 6 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt7 = new TH1D("h_pmt7","pmt 7 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt8 = new TH1D("h_pmt8","pmt 8 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt9 = new TH1D("h_pmt9","pmt 9 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt10 = new TH1D("h_pmt10","pmt 10 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt11 = new TH1D("h_pmt11","pmt 11 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt12 = new TH1D("h_pmt12","pmt 12 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt13 = new TH1D("h_pmt13","pmt 13 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt14 = new TH1D("h_pmt14","pmt 14 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt15 = new TH1D("h_pmt15","pmt 15 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt16 = new TH1D("h_pmt16","pmt 16 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt17 = new TH1D("h_pmt17","pmt 17 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt18 = new TH1D("h_pmt18","pmt 18 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt19 = new TH1D("h_pmt19","pmt 19 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt20 = new TH1D("h_pmt20","pmt 20 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt21 = new TH1D("h_pmt21","pmt 21 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt22 = new TH1D("h_pmt22","pmt 22 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt23 = new TH1D("h_pmt23","pmt 23 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt24 = new TH1D("h_pmt24","pmt 24 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt25 = new TH1D("h_pmt25","pmt 25 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt26 = new TH1D("h_pmt26","pmt 26 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt27 = new TH1D("h_pmt27","pmt 27 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt28 = new TH1D("h_pmt28","pmt 28 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt29 = new TH1D("h_pmt29","pmt 29 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt30 = new TH1D("h_pmt30","pmt 30 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt31 = new TH1D("h_pmt31","pmt 31 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt32 = new TH1D("h_pmt32","pmt 32 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt33 = new TH1D("h_pmt33","pmt 33 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt34 = new TH1D("h_pmt34","pmt 34 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt35 = new TH1D("h_pmt35","pmt 35 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt36 = new TH1D("h_pmt36","pmt 36 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt37 = new TH1D("h_pmt37","pmt 37 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt38 = new TH1D("h_pmt38","pmt 38 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt39 = new TH1D("h_pmt39","pmt 39 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt40 = new TH1D("h_pmt40","pmt 40 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt41 = new TH1D("h_pmt41","pmt 41 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt42 = new TH1D("h_pmt42","pmt 42 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt43 = new TH1D("h_pmt43","pmt 43 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt44 = new TH1D("h_pmt44","pmt 44 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt45 = new TH1D("h_pmt45","pmt 45 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt46 = new TH1D("h_pmt46","pmt 46 signal",Nstep,pmtLow, pmtHigh);
    TH1D *h_pmt47 = new TH1D("h_pmt47","pmt 47 signal",Nstep,pmtLow, pmtHigh);
 
    TH1D* h_pmt[48]={h_pmt0,h_pmt1,h_pmt2,h_pmt3,h_pmt4,h_pmt5,h_pmt6,h_pmt7,h_pmt8,h_pmt9,
		     h_pmt10,h_pmt11,h_pmt12,h_pmt13,h_pmt14,h_pmt15,h_pmt16,h_pmt17,h_pmt18,h_pmt19,
		     h_pmt20,h_pmt21,h_pmt22,h_pmt23,h_pmt24,h_pmt25,h_pmt26,h_pmt27,h_pmt28,h_pmt29,
		     h_pmt30,h_pmt31,h_pmt32,h_pmt33,h_pmt34,h_pmt35,h_pmt36,h_pmt37,h_pmt38,h_pmt39,
		     h_pmt40,h_pmt41,h_pmt42,h_pmt43,h_pmt44,h_pmt45,h_pmt46,h_pmt47};
   
    TH2D *h_pmt0_time = new TH2D("h_pmt0_time","pmt 0 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);   
    TH2D *h_pmt1_time = new TH2D("h_pmt1_time","pmt 1 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt2_time = new TH2D("h_pmt2_time","pmt 2 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt3_time = new TH2D("h_pmt3_time","pmt 3 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt4_time = new TH2D("h_pmt4_time","pmt 4 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt5_time = new TH2D("h_pmt5_time","pmt 5 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt6_time = new TH2D("h_pmt6_time","pmt 6 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt7_time = new TH2D("h_pmt7_time","pmt 7 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt8_time = new TH2D("h_pmt8_time","pmt 8 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt9_time = new TH2D("h_pmt9_time","pmt 9 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt10_time = new TH2D("h_pmt10_time","pmt 10: signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt11_time = new TH2D("h_pmt11_time","pmt 11: signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt12_time = new TH2D("h_pmt12_time","pmt 12 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt13_time = new TH2D("h_pmt13_time","pmt 13 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt14_time = new TH2D("h_pmt14_time","pmt 14 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt15_time = new TH2D("h_pmt15_time","pmt 15 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt16_time = new TH2D("h_pmt16_time","pmt 16 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt17_time = new TH2D("h_pmt17_time","pmt 17 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt18_time = new TH2D("h_pmt18_time","pmt 18 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt19_time = new TH2D("h_pmt19_time","pmt 19 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt20_time = new TH2D("h_pmt20_time","pmt 20 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt21_time = new TH2D("h_pmt21_time","pmt 21 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt22_time = new TH2D("h_pmt22_time","pmt 22 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt23_time = new TH2D("h_pmt23_time","pmt 23 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt24_time = new TH2D("h_pmt24_time","pmt 24 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt25_time = new TH2D("h_pmt25_time","pmt 25 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt26_time = new TH2D("h_pmt26_time","pmt 26 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt27_time = new TH2D("h_pmt27_time","pmt 27 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt28_time = new TH2D("h_pmt28_time","pmt 28 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt29_time = new TH2D("h_pmt29_time","pmt 29 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt30_time = new TH2D("h_pmt30_time","pmt 30 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt31_time = new TH2D("h_pmt31_time","pmt 31 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt32_time = new TH2D("h_pmt32_time","pmt 32 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt33_time = new TH2D("h_pmt33_time","pmt 33 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt34_time = new TH2D("h_pmt34_time","pmt 34 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt35_time = new TH2D("h_pmt35_time","pmt 35 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt36_time = new TH2D("h_pmt36_time","pmt 36 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt37_time = new TH2D("h_pmt37_time","pmt 37 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt38_time = new TH2D("h_pmt38_time","pmt 38 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt39_time = new TH2D("h_pmt39_time","pmt 39 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt40_time = new TH2D("h_pmt40_time","pmt 40 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt41_time = new TH2D("h_pmt41_time","pmt 41 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt42_time = new TH2D("h_pmt42_time","pmt 42 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt43_time = new TH2D("h_pmt43_time","pmt 43 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt44_time = new TH2D("h_pmt44_time","pmt 44 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt45_time = new TH2D("h_pmt45_time","pmt 45 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt46_time = new TH2D("h_pmt46_time","pmt 46 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);
    TH2D *h_pmt47_time = new TH2D("h_pmt47_time","pmt 47 signal vs time", Ntstep, tLow, tHigh, Nstep, pmtLow, pmtHigh);


    TH1D* h_pmt_time[48]={h_pmt0_time,h_pmt1_time,h_pmt2_time,h_pmt3_time,h_pmt4_time,h_pmt5_time,h_pmt6_time,h_pmt7_time,
			  h_pmt8_time,h_pmt9_time,h_pmt10_time,h_pmt11_time,h_pmt12_time,h_pmt13_time,h_pmt14_time,
			  h_pmt15_time,h_pmt16_time,h_pmt17_time,h_pmt18_time,h_pmt19_time,h_pmt20_time,h_pmt21_time,
			  h_pmt22_time,h_pmt23_time,h_pmt24_time,h_pmt25_time,h_pmt26_time,h_pmt27_time,h_pmt28_time,
			  h_pmt29_time,h_pmt30_time,h_pmt31_time,h_pmt32_time,h_pmt33_time,h_pmt34_time,h_pmt35_time,
			  h_pmt36_time,h_pmt37_time,h_pmt38_time,h_pmt39_time,h_pmt40_time,h_pmt41_time,h_pmt42_time,
			  h_pmt43_time,h_pmt44_time,h_pmt45_time,h_pmt46_time,h_pmt47_time};
    
    
    
    double mean[48][48];
    double cova[48][48];
    double corr[48][48];
    
    int nchan = 0;
    
    for (int i=0;i<48;i++){
      for (int j=0;j<48;j++){
	mean[i][j]=0;
	cova[i][j]=0;
	corr[i][j]=0;
      }      
    }
    

    for (Int_t ne=0;ne<nentries;ne++) {
      int1->GetEntry(ne);
      
      h_sumpmt->Fill(pmtsum);      
      for (int i=0;i<48;i++){
	h_pmt[i]->Fill(pmt[i]);
	if(level >0){
	  for (int j=0;j<48;j++){
	    mean[i][j]+=pmt[i]*pmt[j];
	  }
	}//level >0    
      }
      
    }// event loop

 
    for (int i=0;i<48;i++){
      pmtmean[i]   = h_pmt[i]->GetMean(1);
      pmtrms[i]    = h_pmt[i]->GetRMS(1);
      pmtrmser[i]  = h_pmt[i]->GetRMSError(1);
      for (int j=0;j<48;j++){
	mean[i][j] = mean[i][j]/nentries;
      }

    }; 
   

     totrms     = h_sumpmt->GetRMS(1);
    totrmser   = h_sumpmt->GetRMSError(1);  

 
    for (Int_t ne=0;ne<nentries;ne++) {
      int1->GetEntry(ne);
      
      Double_t  t = ne/90.;
      
      
      if(level > 3){       
	Int_t corr_pmt[10];
	for (int i=0;i<10;i++) {    
	  corr_pmt[i]  = pmt[i]-int(pmtmean[i])+300-20*i;      
	  h_pmt_time[i]->Fill(t,corr_pmt[i]);
	}
      }else{
	for (int i=0;i<10;i++) {    
	  h_pmt_time[i]->Fill(t,pmt[i]);
	}
      }
      
      for (int i=10;i<48;i++) {    
	h_pmt_time[i]->Fill(t,pmt[i]);
      }
      
    }// event loop
    
    

    double av_corr = 0;
    double av_cova = 0;  
    int nme=0;
  
    

    for (int i=0;i<48;i++){
      for (int j=0;j<48;j++){
	if(i==31||i==32||i==43||j==31||j==32||j==43){
	  cova[i][j]=0;
	  corr[i][j]=0;
	}else if(pmtmean[i]==0 || pmtmean[j]==0){
	  cova[i][j]=0;
	  corr[i][j]=0;
	}else{
	  cova[i][j]=mean[i][j]-pmtmean[i]*pmtmean[j];
	  corr[i][j]=cova[i][j]/pmtrms[i]/pmtrms[j];
	}
	h_cova1->Fill(i,j,cova[i][j]);
	h_corr1->Fill(i,j,corr[i][j]);
	h_cova2->Fill(i,j,fabs(cova[i][j]));
	h_corr2->Fill(i,j,fabs(corr[i][j]));
	if (i!=j) {
	  av_corr += fabs(corr[i][j]);
	  av_cova += fabs(cova[i][j]);
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
    gStyle->SetPadColor(10);
    gStyle->SetCanvasColor(10);
    gStyle->SetOptTitle(0);
    
    
    int n0 =0;
    int n500 = 0;
    int n700 = 0;
    int nnoise = 0;


    for (int i=0;i<48;i++){
      if (pmtmean[i] <10) n0++;
      if (pmtmean[i] >500) n500++;
      if (pmtmean[i] >700) n700++;
      if (pmtrms[i] > 2.)nnoise ++;
    }
    
    if (n0>3 || n700 >0 || nnoise >0) is_ok = 0;
  
    Double_t npmt[48],npmter[48];
    Double_t av_rms = 0.;
    Double_t av_counts = 0.;
    sumvar =0;
    
    for (int i=0;i<48;i++) {
      npmt[i] = i;
      npmter[i] = 0;
    }
    
    for (int i=0;i<48;i++){
      fprintf(log,"pmt %i: %5.2f +- %3.2f \n",i,pmtmean[i],pmtrms[i]);   
      av_rms +=  pmtrms[i];
      av_counts +=  pmtmean[i];
      sumvar += (pmtrms[i]*pmtrms[i]);
    }
    fprintf(log,"\n\n");
    
    av_rms = av_rms/pmtmax;
    av_counts = av_counts/pmtmax;
    
    totvar = totrms*totrms;
    totcoh = fabs(totvar - sumvar);
    rmscohpc = sqrt(totcoh/pmtmax/(pmtmax-1));
    cohfrac = rmscohpc/av_rms/av_rms;
    
    char  rmsepsfile[50];
    char  rmspngfile[50];
    char  rmsroofile[50];    
    sprintf(rmsepsfile,"out/rms/%s_rms.eps", filename); 
    sprintf(rmsroofile,"out/rms/%s_rms.root", filename); 
    sprintf(rmspngfile,"out/rms/%s_rms.png", filename); 

    c0->SetGridx(1);
    c0->SetGridy(1);  

    double rmsmax[48];
    double rmsmaxer[48];
    for (int i=0;i<48;i++){
      rmsmax[i]   = 2.;
      rmsmaxer[i] = 0.;
    }
    TGraphErrors*  Gr_1 = new TGraphErrors(48, npmt, pmtrms, npmter, pmtrmser); 
    TGraphErrors*  Gr_11 = new TGraphErrors(48, npmt, rmsmax, npmter, rmsmaxer); 
    TMultiGraph* MG_1 = new TMultiGraph();
    Gr_1->SetMarkerStyle(20);
    Gr_1->SetMarkerColor(38);
    Gr_1->SetMarkerSize(1.2);
    Gr_11->SetLineColor(46);
    Gr_11->SetLineWidth(2.0);
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
    if(level >2){
      c0->Print(rmsroofile,"root");   
    }

    
    
    
    //=======================================================================
    
    
    if (level >0){
      
      

      char  pmt_rofile1[50];
      char  pmt_rofile2[50];
      char  pmt_rofile3[50];
      char  pmt_rofile4[50];
      sprintf(pmt_rofile1,"out/exp/pmt_%s_1.root", filename);
      sprintf(pmt_rofile2,"out/exp/pmt_%s_2.root", filename);
      sprintf(pmt_rofile3,"out/exp/pmt_%s_3.root", filename);
      sprintf(pmt_rofile4,"out/exp/pmt_%s_4.root", filename); 
      
      
      
      char  peds_psfile1[50];
      char  peds_psfile2[50];
      char  peds_psfile3[50];
      sprintf(peds_psfile1,"out/peds/%s_peds.ps[", filename);
      sprintf(peds_psfile2,"out/peds/%s_peds.ps]", filename);
      sprintf(peds_psfile3,"out/peds/%s_peds.ps", filename);
      std::string file_open = peds_psfile1;
      std::string file_close = peds_psfile2;
      std::string file_write = peds_psfile3;
      
      
      
      TCanvas *c1 = new TCanvas("c1", "PMTs 1-12", 1.);
      canvas_style(c1);
      c1->Divide(4,3);
      TCanvas *c2 = new TCanvas("c2", "PMTs 13-24", 1.);
      canvas_style(c2);
      c2->Divide(4,3);
      TCanvas *c3 = new TCanvas("c3", "PMTs 25-36", 1.);
      canvas_style(c3);
      c3->Divide(4,3);
      TCanvas *c4 = new TCanvas("c4", "PMTs 37-48", 1.);
      canvas_style(c4);
      c4->Divide(4,3);
      
      c1->Print(file_open.c_str()); 
      
      for (int i=1;i<13;i++){
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
      
      
      c1->Print(file_write.c_str());
      c2->Print(file_write.c_str());
      c3->Print(file_write.c_str());
      c4->Print(file_write.c_str());
      
      for (int i=1;i<13;i++){
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
      
      
      c1->Print(file_write.c_str());
      c2->Print(file_write.c_str());
      c3->Print(file_write.c_str());
      c4->Print(file_write.c_str());
      c4->Print(file_close.c_str());     
      
      
    }


    //=====================================================================
    
    
    if (level >3){
      
      TCanvas *c5 = new TCanvas("c5", "timelines", 800,500);
      canvas_style(c5);
      c5->SetGridx(1);
      
      printf("\n creating detailed timelines \n\n"); 
      
      char  pmttallepsfile[50];
      char  pmttallroofile[50];
      char  pmttallpngfile[50];
      
      sprintf(pmttallepsfile,"out/exp/exp_pmttalltime_%s.eps", filename);  
      sprintf(pmttallroofile,"out/exp/exp_pmttalltime_%s.root", filename);  
      sprintf(pmttallpngfile,"out/exp/exp_pmttalltime_%s.png", filename);  
      
      
      
      
      h_pmt_time[0]->SetYTitle("ADC counts");
      h_pmt_time[0]->SetTitleSize(0.05,"X");
      h_pmt_time[0]->SetTitleSize(0.05,"Y");
      h_pmt_time[0]->SetLabelSize(0.05,"X");
      h_pmt_time[0]->SetLabelSize(0.05,"Y")
      h_pmt_time[0]->GetYaxis()->SetRangeUser(pmtLow,pmtHigh); 
      h_pmt_time[0]->SetMarkerSize(0.4);
      h_pmt_time[0]->SetMarkerStyle(20);
      h_pmt_time[0]->SetMarkerColor(32);
      h_pmt_time[0]->Draw();
      
      for (int i=1;i<10;i++){
	h_pmt_time[i]->SetMarkerSize(0.4);
	h_pmt_time[i]->SetMarkerStyle(20);
	h_pmt_time[i]->SetMarkerColor(32+2*i);
	h_pmt_time[i]->Draw("same");
      }
      
      c5->Print(pmttallepsfile,"eps");
      c5->Print(pmttallroofile,"root");
      c5->Print(pmttallpngfile,"png");
    }
    
    if (level > 0){
      
      char  correpsfile[50];
      char  corrpngfile[50];
      char  corrroofile[50];    
      char  covaepsfile[50];
      char  covapngfile[50];
      char  covaroofile[50]; 
      sprintf(correpsfile,"out/corr/%s_corr.eps", filename); 
      sprintf(corrroofile,"out/corr/%s_corr.root", filename); 
      sprintf(corrpngfile,"out/corr/%s_corr.png", filename); 
      sprintf(covaepsfile,"out/corr/%s_cova.eps", filename); 
      sprintf(covaroofile,"out/corr/%s_cova.root", filename); 
      sprintf(covapngfile,"out/corr/%s_cova.png", filename);      
      
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
      h_cova1->Draw("colz");
      c6->Print(covaepsfile,"eps");
      c6->Print(covapngfile,"png");
    } 
    
    if (n0>3||n500>0||nnoise >0) {
      printf("\n");
      for (int i=1;i<48;i++){ 
	 printf("      -- pmt %i: %5.2f +- %3.2f \n",i,pmtmean[i],pmtrms[i]); 
       }

      printf("\n\n ============================================================================");
      printf("\n\n Warnings, summary:\n");
      if (n0>3)     printf("\n ** Severe: %i PMTs without contact (should be 3) **\n",n0);
      if (n500>0)  {
	printf("\n ** %i PMTs with pedestal > 500 ADC counts  **",n0);
	printf("\n ** Please report ! If HV in ON: Check for light leaks/ leakage currents ! **\n\n");
      }
      if (n700>0)   {
	printf("\n ** Severe: %i PMTs with pedestal > 700 ADC counts                       **",n0);
	printf("\n ** Please report !                                                      **");
	printf("\n ** If HV in ON: Check for light leaks/ leakage currents !               **");
	printf("\n **              Check if problem persists after HV stabilization time ! **\n\n");
      }
      if (nnoise>0) {
	printf("\n ** Severe: %i PMTs with pedestal rms > 2 ADC counts                     **",nnoise);
	printf("\n ** Please report !                                                      **");
	printf("\n ** If HV in ON: Check for light leaks/ leakage currents !               **");
	printf("\n **              Check if problem persists after HV stabilization time ! **\n\n");
      }
      printf("============================================================================");
    }

    if (n0>3||n500>0||nnoise >0) {

      fprintf(log,"\n\n ============================================================================");
      fprintf(log,"\n\n Warnings, summary:\n");
      if (n0>3)     fprintf(log,"\n ** Severe: %i PMTs without contact (should be 3) **\n",n0);
      if (n500>0)  {
	fprintf(log,"\n ** %i PMTs with pedestal > 500 ADC counts  **",n0);
	fprintf(log,"\n ** Please report ! If HV in ON: Check for light leaks/ leakage currents ! **\n\n");
      }
      if (n700>0)   {
	fprintf(log,"\n ** Severe: %i PMTs with pedestal > 700 ADC counts                       **",n0);
	fprintf(log,"\n ** Please report !                                                      **");
	fprintf(log,"\n ** If HV in ON: Check for light leaks/ leakage currents !               **");
	fprintf(log,"\n **              Check if problem persists after HV stabilization time ! **\n\n");
      }
      if (nnoise>0) {
	fprintf(log,"\n ** Severe: %i PMTs with pedestal rms > 2 ADC counts                     **",nnoise);
	fprintf(log,"\n ** Please report !                                                      **");
	fprintf(log,"\n ** If HV in ON: Check for light leaks/ leakage currents !               **");
	fprintf(log,"\n **              Check if problem persists after HV stabilization time ! **\n\n");
      }
      fprintf(log,"============================================================================");
    }




    printf("\n\n==================================================================\n\n");
    printf("  average ADC counts: %5.2f, average rms: %3.2f\n",av_counts, av_rms); 
    printf("\n  average covariance: %4.3f, average correlation: %4.3f \n", av_cova, av_corr); 
    if(av_corr > 0.10){
      printf("\n  ... Warning: average correlation larger than 10\%\n"); 
      is_ok = 0;
    }
    //else if(av_corr > 0.05){
    //    printf("\n  ... Warning: average correlation larger than 5\%\n"); 
    // }
    printf("\n==================================================================\n\n");
    printf("\n\n");
    
    fprintf(log,"\n\n==================================================================\n\n");
    fprintf(log,"  average ADC counts: %5.2f, average rms: %3.2f\n",av_counts, av_rms); 
    fprintf(log,"\n  average covariance: %4.3f, average correlation: %4.3f \n", av_cova, av_corr); 
    if(av_corr > 0.10){
      fprintf(log,"\n  ... Warning: average correlation larger than 10\%\n"); 
      is_ok = 0;
    }
    //else if(av_corr > 0.05){
    //  fprintf(log,"\n  ... Warning: average correlation larger than 5\%\n"); 
    // }
    fprintf(log,"\n==================================================================\n\n");
    fprintf(log,"\n\n");

    if (is_ok){
      printf("\n============================= OK ==============================\n\n");
      fprintf(log,"\n============================= OK ==============================\n\n");
    }else{
      printf("\n============================= not OK ==============================\n\n");
      fprintf(log,"\n============================= not OK ==============================\n\n");
    }
    
  }// if inputfile        
  in.close();  
}

void canvas_style(TCanvas *c,
                  float left_margin=0.15,
		  float right_margin=0.05,
		  float top_margin=0.05,
		  float bottom_margin=0.15,
		  int canvas_color=0,
                  int frame_color=0) {

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



void intped_exp(const char * filename, Int_t t_low, Int_t t_high, Int_t level, Int_t mincounts,Int_t maxcounts ) {
   ntwrite(filename);
   ntread(filename,t_low,t_high, level,mincounts,maxcounts);
}

void intped(const char * filename) {
  intped_exp(filename,10,20,1,-1,-1);
}

void intana(const char * filename) {
  ntread(filename,5,15,1,-1,-1);
  //ntread(filename,0,5,1,0,400);
}

