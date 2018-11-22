#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH1.h"

#include <cassert>

namespace ana
{
  //----------------------------------------------------------------------
  DUNEFluxSyst::~DUNEFluxSyst()
  {
    for(int i: {0, 1})
      for(int j: {0, 1})
        for(int k: {0, 1})
          for(int l: {0, 1})
            delete fScale[i][j][k][l];
  }

  //----------------------------------------------------------------------
  void DUNEFluxSyst::Shift(double sigma,
                           Restorer& restore,
                           caf::StandardRecord* sr,
                           double& weight) const
  {
    if(!fScale[0][0][0][0]){
      TFile f((FindCAFAnaDir()+"/Systs/flux_shifts.root").c_str());
      assert(!f.IsZombie());

      for(int det: {0, 1}){
        const std::string detStr = (det == 0) ? "ND" : "FD";
        for(int pdg: {0, 1}){
          std::string pdgStr = (pdg == 0) ? "nue" : "numu";
          for(bool anti: {false, true}){
            if(anti) pdgStr += "bar";

            for(int hc: {0, 1}){
              const std::string hcStr = (hc == 0) ? "FHC" : "RHC";

              TH1*& h = fScale[det][pdg][anti][hc];

              h = (TH1*)f.Get(TString::Format("syst%d/%s_%s_%s",
                                              fIdx,
                                              detStr.c_str(),
                                              pdgStr.c_str(),
                                              hcStr.c_str()).Data());
              h = (TH1*)h->Clone(UniqueName().c_str());
              h->SetDirectory(0);
            }
          }
        }
      }
    } // end if

    if(abs(sr->dune.nuPDGunosc) == 16) return;

    const int det = sr->dune.isFD ? 0 : 1;
    const int pdg = (abs(sr->dune.nuPDGunosc) == 12) ? 0 : 1;
    const int anti = (sr->dune.nuPDGunosc > 0) ? 0 : 1;
    const int hc = sr->dune.isFHC ? 0 : 1;

    TH1* h = fScale[det][pdg][anti][hc];
    assert(h);

    const int bin = h->FindBin(sr->dune.Ev);
    if(bin == 0 || bin == h->GetNbinsX()+1) return;

    weight *= 1 + h->GetBinContent(bin)*sigma;
  }

  //----------------------------------------------------------------------
  const DUNEFluxSyst* GetDUNEFluxSyst(unsigned int i, bool applyPenalty)
  {
    // Make sure we always give the same one back
    static std::vector<const DUNEFluxSyst*> cache;
    if(i >= cache.size()) cache.resize(i+1);
    if(!cache[i]) cache[i] = new DUNEFluxSyst(i, applyPenalty);
    return cache[i];
  }

  //----------------------------------------------------------------------
  DUNEFluxSystVector GetDUNEFluxSysts(unsigned int N, bool applyPenalty)
  {
    DUNEFluxSystVector ret;
    for(unsigned int i = 0; i < N; ++i) ret.push_back(GetDUNEFluxSyst(i, applyPenalty));
    return ret;
  }
} // namespace
