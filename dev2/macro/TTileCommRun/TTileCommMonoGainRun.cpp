#define TTileCommMonoGainRun_cxx

#include "TTileCommMonoGainRun.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLeafI.h>
#include <TLine.h>

#ifdef TTileCommMonoGainRun_cxx

TTileCommMonoGainRun::TTileCommMonoGainRun(const char *runNumber, const char *runType, const char *treeName, const char *filename)
	: TTileCommRun(runNumber, runType, treeName, filename)
{
	if (strcmp(runType, "MonoCis") != 0 && strcmp(runType, "Led") != 0 && strcmp(runType, "Phys") != 0)
	{
		cout << "Class constructor error: Runtype should be mono gain: MonoCis, Led, or Phys" << endl;
		exit(0);
	}
	
	// Set branch addresses and get branch pointers
	TString branchName;
	for(Int_t iModule = 0; iModule < Nmodules; iModule++)
	{
		if (moduleInNtuple[iModule])
		{
			branchName = "Efit" + moduleName[iModule];
			fChain->SetBranchAddress(branchName, Efit[iModule]);
			b_Efit[iModule] = fChain->GetBranch(branchName.Data());

			branchName = "Sample" + moduleName[iModule];
			fChain->SetBranchAddress(branchName, Sample[iModule]);
			b_Sample[iModule] = fChain->GetBranch(branchName.Data());
		}
	}

	for(Int_t iPMT = 0; iPMT < NPMTs; iPMT++)
	{

		vector<Float_t> auxPMT;
		for(Int_t iModule = 0; iModule < Nmodules; iModule++)
			auxPMT.push_back(0.0);

		PMTEnergies.push_back(auxPMT);
	}

}


void TTileCommMonoGainRun::DeadChannels()
{
	Int_t iEvt, iModule, iPMT;
	Float_t warningLevel = 0.8;
	
	gStyle->SetOptStat(0);


	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
				
			for (iPMT = 0; iPMT < NPMTs; iPMT++)
			{
				if (iPMT == 31 || iPMT == 32 || iPMT == 43) // 3 empty PMT/channels: 32, 33 and 44
					continue;
				//cout << Efit[iModule][iPMT]/float(numEvents) << " ";
				//hchannel.Fill(iPMT + 1, Efit[iModule][iPMT]/float(numEvents));
				PMTEnergies.at(iPMT).at(iModule) += Efit[iModule][iPMT]/float(numEvents);
			}
		}

	}

	TCanvas c1("c1", "", 600, 600);

	cout << "<table><tr><td valign=\"top\">" << endl;
	cout << "<ul>" << endl;
	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		if (!moduleInNtuple[iModule])
			continue;
			
		TString title = "Energy Module " + moduleName[iModule] + " " + fRunType + " Run " + fRunNumber;
		TH1F hchannel("hchannel", title.Data(), NPMTs, 1, NPMTs + 1);
		hchannel.SetXTitle("PMT number");
		hchannel.SetYTitle("Average energy per event (pC)");
		hchannel.SetFillColor(kBlue);
		hchannel.SetMarkerStyle(21);    

		Int_t totalWarnings = 0;
		
		for (iPMT = 0; iPMT < NPMTs; iPMT++)
		{
			if (iPMT == 31 || iPMT == 32 || iPMT == 43) // 3 empty PMT/channels: 32, 33 and 44
				continue;
			//cout << Efit[iModule][iPMT]/float(numEvents) << " ";
			hchannel.Fill(iPMT + 1, PMTEnergies.at(iPMT).at(iModule));
			if (PMTEnergies.at(iPMT).at(iModule) < warningLevel)
				totalWarnings++;

		}
		
		Char_t text[50];
		hchannel.Draw("P");
		TLine warningLine(0, warningLevel, float(NPMTs), warningLevel);
		warningLine.SetLineColor(kRed);
		warningLine.SetLineWidth(3);
		warningLine.Draw();
		
		/*  TPaveText paveup(0.6,0.7,0.90,0.98,"NDC");
		paveup.SetFillColor(0);
		sprintf(text,"");
		paveup.AddText(text);
		sprintf(text,"Mean       %2.2f",meanup);
		paveup.AddText(text);
		sprintf(text,"RMS        %2.2f",rmsup);
		paveup.AddText(text);
		sprintf(text,"Warn Level %2.2f",warningLevelup);
		paveup.AddText(text);
		paveup.Draw();
		*/
		
		TString outFilePS  = outputDir + "LED/" + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.ps";
		TString outFilePNG = outputDir + "LED/" + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.png";

		c1.SaveAs(outFilePS.Data());
		c1.SaveAs(outFilePNG.Data());
/*
		outFilePS  = standOutputDir + "LED/" + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.ps";
		outFilePNG = standOutputDir + "LED/" + "r" + fRunNumber + "_" + moduleName[iModule] + "_DeadChannels.png";

		c1.SaveAs(outFilePS.Data());
		c1.SaveAs(outFilePNG.Data());
*/
		cout << "<li>" << totalWarnings << " PMTs with low intensity in module " << moduleName[iModule] << "<br />" << endl;
		cout << "<img src=\"" << outputWebDir << "LED/" << basename(outFilePNG.Data()) << "\" />" << "</li>" << "<br />" << endl;
		//printf("\tNumber of channels with low intensity %d\n", totalWarnings);
		
	}
	cout << "</ul>" << endl;
	cout << "</td><td valign=\"top\">" << endl;

	TString rowsTitle = "Module";
	TString colsTitle = "PMT";
	TString table;
	cout << "<b>Average energy per event:</b><br />" << endl;
	buildHTMLTable(table, PMTEnergies, (const Int_t)NPMTs, (const Int_t)Nmodules, (const TString)colsTitle, (const TString)rowsTitle, (TString*)PMTName, (TString*)moduleName, NULL, moduleInNtuple, warningLevel);
	cout << table << endl;

	cout << "</td></tr></table><br />" << endl;

}


void TTileCommMonoGainRun::buildHTMLTable(TString &table, const vector< vector<Float_t> > &matrix, const Int_t &nrows, const Int_t &ncols, const TString &rowsTitle, const TString &colsTitle, TString *rowNames, TString *colNames, Bool_t *selectedRows, Bool_t *selectedCols, Float_t warningLevel)
{
	Int_t i, j;
	
	table  = "<table class=\"main\">\n<tr><th></th>";
	table += "<th colspan=\"";
	table += ncols;
	table += "\">";
	table += colsTitle;
	table += "</th></tr>\n";
	table += "<tr><th>";
	table += rowsTitle;
	table += "</th>";
/*
	cout << (matrix + (48*ncols) + 0) << endl;
	cout << &NSlinkErrors[48][0] << endl;
*/
	for (j = 0; j < ncols; j++)
	{
		//if (j == 31 || j == 32 || j == 43) // 3 empty PMT/channels: 32, 33 and 44
		//	continue;

		if (selectedCols && !selectedCols[j])
			continue;

		table += "<th>";
		table += colNames[j];
		table += "</th>";
	}
	table += "</tr>\n";

	for (i = 0; i < nrows; i++)
	{
		if (selectedRows && !selectedRows[i])
			continue;

		if (i == 31 || i == 32 || i == 43) // 3 empty PMT/channels: 32, 33 and 44
			continue;
		
		table += "<tr><th>";
		table += rowNames[i];
		table += "</th>";
		for (j = 0; j < ncols; j++)
		{
			if (selectedCols && !selectedCols[j])
				continue;

			//if (j == 31 || j == 32 || j == 43) // 3 empty PMT/channels: 32, 33 and 44
			//	continue;

			table += "<td align=\"right\">";
			if (matrix.at(i).at(j) < warningLevel)
			{
				table += "<font color=\"red\"><b>";
				table += matrix.at(i).at(j);
				table += "</b></font>";
			}
			else
				table += matrix.at(i).at(j);
			table += "</td>";
		}
		table += "</tr>\n";

	}
	table += "</table>\n";
}


void TTileCommMonoGainRun::Baseline()
{
	Int_t iEvt, iModule, j;
	Float_t warningLevel = 0.8;
	
	Double_t mean[Nmodules][48]={0}, rms[Nmodules][48]={0}, chan[Nmodules][48];

	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
				
			for(j = 0; j < 48; j++)
				mean[iModule][j] += (Double_t)Sample[iModule][j][0]/(Double_t)numEvents;
		}
	}
//	for (j = 0; j < 48; j++)
//		mean[j] = mean[j]/(Double_t)numEvents;

	for (iEvt = 0; iEvt < numEvents; iEvt++)
	{
		fChain->GetEntry(iEvt);
		for (iModule = 0; iModule < Nmodules; iModule++)
		{
			if (!moduleInNtuple[iModule])
				continue;
				
			for (j = 0; j < 48; j++)
				rms[iModule][j] += TMath::Power((Sample[iModule][j][0] - mean[iModule][j]), 2)/numEvents;
		}
	}
//	for (j = 0; j < 48; j++)
//		rms[j] = rms[j]/numEvents;

	TString outdir = outputDir;
	TString typeDir;
	if (fRunType == "Led")
	{
		outdir += "LED/";
		typeDir = "LED/";
	}
	else
	if(fRunType == "MonoCis")
	{
		outdir += "MonoCis/";
		typeDir = "MonoCis/";
	}
	else
	if(fRunType == "Phys")
	{
		outdir += "physics/";
		typeDir = "physics/";
	}

	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		if (!moduleInNtuple[iModule])
			continue;
			
		for (j = 0; j < 48; j++)
		{
			rms[iModule][j] = TMath::Sqrt(rms[iModule][j]);
			chan[iModule][j] = j;
		}

		TString pref = "Run " + fRunNumber + " Mod LB" + moduleName[iModule] + ":";

		TGraph *grm = new TGraph(48, chan[iModule], mean[iModule]);
		grm->SetMarkerStyle(21);
		grm->SetTitle(pref + " Mean First Sample");
		grm->GetXaxis()->SetTitle("Channel number");
		grm->GetYaxis()->SetTitle("ADC counts");

		TGraph *grr = new TGraph(48, chan[iModule], rms[iModule]);
		grr->SetMarkerStyle(21);
		grr->SetTitle(pref + " RMS first Sample");
		grr->GetXaxis()->SetTitle("Channel number");
		grr->GetYaxis()->SetTitle("ADC counts");
		
		TCanvas *c1 = new TCanvas();
		c1->Divide(1, 2);
		c1->cd(1);
		grm->Draw("AP");
		c1->cd(2);
		grr->Draw("AP");
		
		TString outFilePS = outdir + "r" + fRunNumber +"_" + moduleName[iModule] + "_baseline.ps";
		TString outFilePNG = outdir + "r" + fRunNumber + "_" + moduleName[iModule] + "_baseline.png";
		
		c1->Print(outFilePS);
		c1->Print(outFilePNG);
		/*
		outFilePS = standOutputDir + "r" + fRunNumber +"_" + moduleName[iModule] + "_baseline.ps";
		outFilePNG = standOutputDir + "r" + fRunNumber + "_" + moduleName[iModule] + "_baseline.png";
		
		c1->Print(outFilePS);
		c1->Print(outFilePNG);
		*/
		cout << "<ul><b>Module LB" << moduleName[iModule] << "<br />" << endl;
		cout << "<img src=\"" << outputWebDir << typeDir << basename(outFilePNG.Data()) << "\" /><br /><br />" << endl;
		cout << "</ul>" << endl;
		
		delete c1;
		delete grm;
		delete grr;

	}


}


#endif // #ifdef TTileCommMonoGainRun_cxx
