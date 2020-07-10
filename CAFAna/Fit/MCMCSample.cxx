#include "CAFAna/Fit/MCMCSample.h"

#include "TObjString.h"
#include "TParameter.h"

namespace ana
{
  //----------------------------------------------------------------------
  MCMCSample::MCMCSample(double LL, std::vector<double> diagVals, std::vector<double> entryVals,
                         std::vector <std::string> diagBranchNames,
                         std::vector<const ana::IFitVar *> vars, std::vector<const ana::ISyst *> systs)
      : fDiagBranches(std::move(diagBranchNames)), fVars(std::move(vars)), fSysts(std::move(systs))
  {
    assert(diagVals.size() == fDiagBranches.size() && entryVals.size() == fVars.size() + fSysts.size());
    fVals.resize(1 + diagVals.size() + entryVals.size());
    fVals.push_back(LL);
    fVals.insert(fVals.end(), std::move_iterator(diagVals.begin()), std::move_iterator(diagVals.end()));
    fVals.insert(fVals.end(), std::move_iterator(entryVals.begin()), std::move_iterator(entryVals.end()));
  }

  //----------------------------------------------------------------------
  MCMCSample::MCMCSample(const TMap &map, const std::vector<const ana::IFitVar *> &vars,
                         const std::vector<const ana::ISyst *> &systs)
  {
    fVals.resize(map.GetSize());
    std::size_t diagBranchSize = map.GetSize() - vars.size() - systs.size() - 1;

    for (const auto &mapPair : map)
    {
      auto pair = dynamic_cast<TPair *>(mapPair);
      std::string name(dynamic_cast<TObjString *>(pair->Key())->GetString());
      double value = dynamic_cast<TParameter<double> *>(pair->Value())->GetVal();

      auto itVar = std::find_if(vars.begin(), vars.end(), [&name](const IFitVar* v){ return v->ShortName() == name; });
      auto itSyst = (itVar == vars.end()) ? systs.end() : std::find_if(systs.begin(), systs.end(), [&name](const ISyst* s){ return s->ShortName() == name; });

      // find out whether this is a Var, a Syst, or diagnostic branch
      if (itVar != vars.end())
        fVals[1 + diagBranchSize + std::distance(vars.begin(), itVar)] = value;
      else if (itSyst != systs.end())
        fVals[1 + diagBranchSize + vars.size() + std::distance(systs.begin(), itSyst)] = value;
      else if (name == "logprob")
        fVals[0] = value;
      else
      {
        fVals[1 + fDiagBranches.size()] = value;
        fDiagBranches.push_back(name);
      }
    }
  }

  //----------------------------------------------------------------------
  double MCMCSample::DiagnosticVal(const std::string &brName) const
  {
    auto it = std::find(fDiagBranches.begin(), fDiagBranches.end(), brName);
    assert(it != fDiagBranches.end() && ("This MCMCSample does not contain a value called '" + brName + "'").c_str());

    return fVals[1 + std::distance(fDiagBranches.begin(), it)];
  }

  //----------------------------------------------------------------------
  std::unique_ptr<TMap> MCMCSample::ToTMap() const
  {
    auto ret = std::make_unique<TMap>();
    ret->Add(new TObjString("logprob"), new TParameter<double>("", fVals[0]));

    for (const auto & brName : fDiagBranches)
      ret->Add(new TObjString(brName.c_str()), new TParameter<double>("", DiagnosticVal(brName)));
    for (const auto & v : fVars)
      ret->Add(new TObjString(v->ShortName().c_str()), new TParameter<double>("", Val(v)));
    for (const auto & s : fSysts)
      ret->Add(new TObjString(s->ShortName().c_str()), new TParameter<double>("", Val(s)));

    return std::move(ret);
  }

  //----------------------------------------------------------------------
  double MCMCSample::Val(const ana::IFitVar *var) const
  {
    auto it = std::find(fVars.begin(), fVars.end(), var);
    assert(it != fVars.end() && ("This MCMCSample does not contain Var: '" + var->ShortName() + "'").c_str());

    return fVals[1 + fDiagBranches.size() + std::distance(fVars.begin(), it)];
  }

  //----------------------------------------------------------------------
  double MCMCSample::Val(const ana::ISyst *syst) const
  {
    auto it = std::find(fSysts.begin(), fSysts.end(), syst);
    assert(it != fSysts.end() && ("This MCMCSample does not contain Syst: '" + syst->ShortName() + "'").c_str());

    return fVals[1 + fDiagBranches.size() + fVars.size() + std::distance(fSysts.begin(), it)];
  }

}