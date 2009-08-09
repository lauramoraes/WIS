#define TTileCommCISRun_cxx

#include "TTileCommCISRun.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TLeafI.h>
#include <TLine.h>

#ifdef TTileCommCISRun_cxx

TTileCommCISRun::TTileCommCISRun(const char *runNumber, const char *runType, const char *treeName, const char *filename)
	: TTileCommRun(runNumber, "CIS", treeName, filename)
{ }


void TTileCommCISRun::PulseHeight()
{
	Int_t iModule, i;

	TCanvas c1("c1", "High Gain", 900, 900);
	c1.Divide(6,8);
	
	TCanvas c2("c2", "Low Gain", 900, 900);
	c2.Divide(6,8);
	
	TString varhi, varlo;
	
	TH1F *h1[48];
	TH1F *h2[48];
	
	TString hnamehi, htitlehi, hnamelo, htitlelo;

	cout << "<ul>" << endl;
	for (iModule = 0; iModule < Nmodules; iModule++)
	{
		if (!moduleInNtuple[iModule])
			continue;
	
		for (i = 0; i < 48; i++)
		{
			c1.cd(i+1);
			gPad->SetLogy();
			
			hnamehi = "ChannelHigh";
			hnamehi += i;
			htitlehi = "Channel ";
			htitlehi += i;
			htitlehi += " High";
			
			h1[i] = new TH1F(hnamehi, htitlehi, 1024, 0, 1023);
			
			varhi = "Sample" + moduleName[iModule] + "hi[";
			varhi += i;
			varhi += "]>>";
			varhi += hnamehi;
			
			fChain->Draw(varhi, "", "", 16000, 16000);
			
			c2.cd(i+1);
			gPad->SetLogy();
			
			hnamelo = "ChannelLow";
			hnamelo += i;
			htitlelo = "Channel ";
			htitlelo += i;
			htitlelo += " Low";
			
			h2[i] = new TH1F(hnamelo, htitlelo, 1024, 0, 1023);
			
			varlo = "Sample" + moduleName[iModule] + "lo[";
			varlo += i;
			varlo += "]>>";
			varlo += hnamelo;
			
			fChain->Draw(varlo, "", "", 16000, 16000);
		}
	
		TString outFilePShi = outputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_hi_PulseHeight.ps";
		TString outFilePSlo = outputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_lo_PulseHeight.ps";
		TString outFilePNGhi= outputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_hi_PulseHeight.png";
		TString outFilePNGlo= outputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_lo_PulseHeight.png";
		
		c1.Print(outFilePShi);
		c2.Print(outFilePSlo);
		
		c1.Print(outFilePNGhi);
		c2.Print(outFilePNGlo);
/*
		outFilePShi = standOutputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_hi_PulseHeight.ps";
		outFilePSlo = standOutputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_lo_PulseHeight.ps";
		outFilePNGhi= standOutputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_hi_PulseHeight.png";
		outFilePNGlo= standOutputDir + "CIS/r" + fRunNumber + "_" + "LB" + moduleName[iModule] + "_lo_PulseHeight.png";
		
		c1.Print(outFilePShi);
		c2.Print(outFilePSlo);
		
		c1.Print(outFilePNGhi);
		c2.Print(outFilePNGlo);
*/
		cout << "<li><b>Module LB" << moduleName[iModule] << "</b>" << " High Gain:<br />" << endl;
		cout << "<img src=\"" << outputWebDir << "CIS/" << basename(outFilePNGhi.Data()) << "\" /><br /><br />" << endl;
		cout << "</li>" << endl;

		cout << "<li><b>Module LB" << moduleName[iModule] << "</b>" << " Low Gain:<br />" << endl;
		cout << "<img src=\"" << outputWebDir << "CIS/" << basename(outFilePNGlo.Data()) << "\" />" << endl;
		cout << "</li>" << endl;
		
		for (i = 0; i < 48; i++)
		{
			delete h1[i];
			delete h2[i];
		}
	}
	cout << "</ul>" << endl;

}

#endif // #ifdef TTileCommCISRun_cxx
