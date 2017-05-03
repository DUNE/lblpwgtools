#include "CAFAna/Vars/DUNENeutNuWROReweight.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH2.h"

#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  DUNENeutNuWROReweight::~DUNENeutNuWROReweight()
  {
    delete fHist;
    delete fHist2D;
  }

  //----------------------------------------------------------------------
  double DUNENeutNuWROReweight::operator()(const caf::StandardRecord* sr)
  {
    if(!fHist && !fHist2D) LoadHists();

    const double x = sr->dune.Ev;
    if(x < 0) return 1; // How?

    if(fVars == kEnu){
      if(x > fHist->GetXaxis()->GetXmax()) return 1; // overflow bin
      const double w = fHist->GetBinContent(fHist->FindBin(x));
      if(w == 0) return 1; // probably a low-stats bin
      return w;
    }
    else{
      const double y = (fVars == kEnuQ2) ? sr->dune.Q2 : sr->dune.W;
      if(x > fHist2D->GetXaxis()->GetXmax()) return 1; // overflow bin
      if(y < 0) return 1; // underflow bin
      if(y > fHist2D->GetYaxis()->GetXmax()) return 1; // overflow bin
      const double w = fHist2D->GetBinContent(fHist2D->FindBin(x, y));
      if(w == 0) return 1; // probably a low-stats bin
      return w;
    }
  }

  //----------------------------------------------------------------------
  void DUNENeutNuWROReweight::LoadHists()
  {
    TFile f(fFname.c_str());
    if(f.IsZombie()){
      std::cout << "DUNENeutNuWROReweight: couldn't find " << fFname << std::endl;
      abort();
    }

    std::string histName;
    if(fVars == kEnu) histName = "Ev";
    if(fVars == kEnuQ2) histName = "Ev_Q2";
    if(fVars == kEnuW) histName = "Ev_W";
    histName += "_";
    if(fGen == kNeut) histName += "neut";
    if(fGen == kNuWRO) histName += "nuwro";
    histName += "ratio";

    TObject* h = f.Get(histName.c_str());

    if(!h){
      std::cout << "DUNENeutNuWroReweight: couldn't find " << histName << " in " << fFname << std::endl;
      abort();
    }

    if(fVars == kEnu){
      fHist = (TH1*)h;
      fHist->SetDirectory(0);
    }
    else{
      fHist2D = (TH2*)h;
      fHist2D->SetDirectory(0);
    }
  }
}
