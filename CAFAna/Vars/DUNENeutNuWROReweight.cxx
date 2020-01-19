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
    delete fHistNu; delete fHistAnu;
    delete fHist2DNu; delete fHist2DAnu;
  }

  //----------------------------------------------------------------------
  double DUNENeutNuWROReweight::operator()(const caf::StandardRecord* sr)
  {
    if(!fHistNu && !fHist2DNu) LoadHists();

    const double x = sr->dune.Ev;
    if(x < 0) return 1; // How?

    const bool anti = (sr->dune.nuPDG < 0);

    if(fVars == kEnu){
      TH1* h = (anti ? fHistAnu : fHistNu);
      if(x > h->GetXaxis()->GetXmax()) return 1; // overflow bin
      const double w = h->GetBinContent(h->FindBin(x));
      if(w == 0) return 1; // probably a low-stats bin
      return w;
    }
    else{
      TH2* h = (anti ? fHist2DAnu : fHist2DNu);
      const double y = (fVars == kEnuQ2) ? sr->dune.Q2 : sr->dune.W;
      if(x > h->GetXaxis()->GetXmax()) return 1; // overflow bin
      if(y < 0) return 1; // underflow bin
      if(y > h->GetYaxis()->GetXmax()) return 1; // overflow bin
      const double w = h->GetBinContent(h->FindBin(x, y));
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

    for(bool anti : {false, true}){
      std::string histName;
      if(fVars == kEnu) histName = "Ev";
      if(fVars == kEnuQ2) histName = "Ev_Q2";
      if(fVars == kEnuW) histName = "Ev_W";
      histName += "_";
      if(fGen == kNeut){
        if(anti) histName += "anuneut"; else histName += "nuneut";
      }
      if(fGen == kNuWRO){
        if(anti) histName += "anuwro"; else histName += "nunuwro";
      }
      histName += "ratio";

      TObject* h = f.Get(histName.c_str());

      if(!h){
        std::cout << "DUNENeutNuWroReweight: couldn't find " << histName << " in " << fFname << std::endl;
        abort();
      }

      if(fVars == kEnu){
        if(anti){
          fHistAnu = (TH1*)h;
          fHistAnu->SetDirectory(0);
        }
        else {
          fHistNu = (TH1*)h;
          fHistNu->SetDirectory(0);
        }
      }
      else{
        if(anti){
          fHist2DAnu = (TH2*)h;
          fHist2DAnu->SetDirectory(0);
        }
        else{
          fHist2DNu = (TH2*)h;
          fHist2DNu->SetDirectory(0);
        }
      }
    } // end for anti
  }
}
