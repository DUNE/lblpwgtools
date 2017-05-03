#pragma once

#include "CAFAna/Core/Var.h"

class TH1;
class TH2;

namespace ana
{
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
      : fFname(fname), fGen(gen), fVars(vars), fHist(0), fHist2D(0)
    {
    }

    ~DUNENeutNuWROReweight();

    double operator()(const caf::StandardRecord* sr);

  protected:
    void LoadHists();

    std::string fFname;
    EGenerator fGen;
    ERWVars fVars;
    mutable TH1* fHist;
    mutable TH2* fHist2D;
  };

  const std::string kNeutNuWROReweightFname;

  const Var kReweightEnuToNeut({}, DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNeut, DUNENeutNuWROReweight::kEnu));
  const Var kReweightEnuQ2ToNeut({}, DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNeut, DUNENeutNuWROReweight::kEnuQ2));
  const Var kReweightEnuWToNeut({}, DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNeut, DUNENeutNuWROReweight::kEnuW));

  const Var kReweightEnuToNuWRO({}, DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNuWRO, DUNENeutNuWROReweight::kEnu));
  const Var kReweightEnuQ2ToNuWRO({}, DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNuWRO, DUNENeutNuWROReweight::kEnuQ2));
  const Var kReweightEnuWToNuWRO({}, DUNENeutNuWROReweight(kNeutNuWROReweightFname, DUNENeutNuWROReweight::kNuWRO, DUNENeutNuWROReweight::kEnuW));
}
