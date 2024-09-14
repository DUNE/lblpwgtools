#pragma once

#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Weight.h"

class TH1;
class TH2;

namespace ana
{
  class SRTrueInteractionProxy;

  class DUNENeutNuWROReweight
  {
  public:
    enum EGenerator{
      kNeut = 0,
      kNuWRO = 1
    };

    enum ERWVars{
      kEnu = 0,
      kEnuQ2 = 1,
      kEnuW = 2
    };

    DUNENeutNuWROReweight(const std::string& fname,
                          EGenerator gen,
                          ERWVars vars)
      : fFname(fname), fGen(gen), fVars(vars),
        fHistNu(0), fHistAnu(0),
        fHist2DNu(0), fHist2DAnu(0)
    {
    }

    ~DUNENeutNuWROReweight();

    double operator()(const caf::SRTrueInteractionProxy* nu);

  protected:
    void LoadHists();

    std::string fFname;
    EGenerator fGen;
    ERWVars fVars;
    mutable TH1* fHistNu;
    mutable TH1* fHistAnu;
    mutable TH2* fHist2DNu;
    mutable TH2* fHist2DAnu;
  };

  const std::string kNeutNuWROReweightFname = "/dune/data/users/marshalc/NEUT_GENIE_ratio.root";

  const NuTruthWeight kReweightEnuToNeut(DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNeut, DUNENeutNuWROReweight::kEnu));
  const NuTruthWeight kReweightEnuQ2ToNeut(DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNeut, DUNENeutNuWROReweight::kEnuQ2));
  const NuTruthWeight kReweightEnuWToNeut(DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNeut, DUNENeutNuWROReweight::kEnuW));

  const NuTruthWeight kReweightEnuToNuWRO(DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNuWRO, DUNENeutNuWROReweight::kEnu));
  const NuTruthWeight kReweightEnuQ2ToNuWRO(DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNuWRO, DUNENeutNuWROReweight::kEnuQ2));
  const NuTruthWeight kReweightEnuWToNuWRO(DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNuWRO, DUNENeutNuWROReweight::kEnuW));
}
