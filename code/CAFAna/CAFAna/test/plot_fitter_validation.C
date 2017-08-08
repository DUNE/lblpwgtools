// This is a straight ROOT macro (ie no need to run under cafe)

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TPad.h"

#include <string>

void plot_fitter_validation()
{
  TFile* fin = new TFile("fitter_validation_cafana.root");

  const int dcpCols[] = {kBlack, kRed, kGreen+2, kBlue};
  const std::string dcpStrs[] = {"0", "#pi/2", "#pi", "3#pi/2"};

  for(std::string hcStr: {"fhc", "rhc"}){
    const std::string HCStr = (hcStr == "fhc") ? "FHC" : "RHC";

    for(std::string chanStr: {"numu", "nue"}){
      const std::string CHANStr = (chanStr == "numu") ? "#nu_{#mu}" : "#nu_{e}";

      for(std::string hieStr: {"nh", "ih"}){
        const std::string HIEStr = (hieStr == "nh") ? "NH" : "IH";

        new TCanvas;

        // 3pi/2 is the tallest histogram, draw it first
        for(int deltaIdx = 3; deltaIdx >= 0; --deltaIdx){
          const std::string dcpStr = TString::Format("%gpi", deltaIdx/2.).Data();

          TH1* h = (TH1*)fin->Get((chanStr+"_"+hcStr+"_"+hieStr+"_"+dcpStr).c_str());
          h->SetLineColor(dcpCols[deltaIdx]);
          h->Draw("same");

          h->SetTitle(("10 yrs "+HCStr+" "+CHANStr+" "+HIEStr).c_str());
        } // end for deltaIdx

        TLegend* leg = new TLegend(.6, .6, .85, .85);
        leg->SetFillStyle(0);
        for(int deltaIdx = 0; deltaIdx < 4; ++deltaIdx){
          TH1* dummy = new TH1F("", "", 1, 0, 1);
          dummy->SetLineColor(dcpCols[deltaIdx]);
          leg->AddEntry(dummy, ("#delta_{CP}="+dcpStrs[deltaIdx]).c_str(), "l");
        }
        leg->Draw("same");

        gPad->Print((hcStr+"_"+chanStr+"_"+hieStr+".pdf").c_str());
      } // end for hieStr
    } // end for chanStr
  } // end for hcStr
}
