#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Registry.h"

#include "Utilities/func/MathUtil.h"
#include "Utilities/func/StanUtils.h"

#include <cassert>
#include <memory>
#include <iostream>

#include "TDirectory.h"
#include "TH1D.h"
#include "TObjString.h"
#include "TString.h"

namespace ana {
// Reserve 0 for unshifted
int SystShifts::fgNextID = 1;

//----------------------------------------------------------------------
SystShifts::SystShifts() : fID(0) {}

//----------------------------------------------------------------------
SystShifts::SystShifts(const ISyst *syst, double shift) : fID(fgNextID++) {
  if (shift != 0)
    fSystsDbl.emplace(syst, Clamp(shift, syst));
}

//----------------------------------------------------------------------
SystShifts::SystShifts(const ISyst* syst, stan::math::var shift)
    : fID(fgNextID++)
{
  fSystsStan.emplace(syst, Clamp(shift, syst));
  // we're always going to maintain a "double" copy
  // so that when the Stan cache gets invalidated we can still return something usable.
  fSystsDbl.emplace(syst, Clamp(util::GetValAs<double>(shift), syst));
}

//----------------------------------------------------------------------
SystShifts::SystShifts(const std::map<const ISyst *, double> &shifts)
    : fID(fgNextID++) {
  for (auto it : shifts)
    if (it.second != 0)
      fSystsDbl.emplace(it.first, Clamp(it.second, it.first));
}

//----------------------------------------------------------------------
SystShifts::SystShifts(const std::map<const ISyst*, stan::math::var>& shifts)
    : fID(fgNextID++)
{
  for(auto it: shifts)
  {
    fSystsStan.emplace(it.first, it.second);
    fSystsDbl.emplace(it.first, util::GetValAs<double>(it.second));
  }
}

//----------------------------------------------------------------------
std::unique_ptr<SystShifts> SystShifts::Copy() const
{
  return std::make_unique<SystShifts>(*this);
}


//----------------------------------------------------------------------
void SystShifts::SetShift(const ISyst* syst, double shift, bool force)
{
  fID = fgNextID++;

    // if this slot already exists in the Stan systs, and we're not setting the same value,
    // some shenanigans are going on that we need to figure out.  abort.
    auto itStan = fSystsStan.find(syst);
    if(itStan != fSystsStan.end() && itStan->second != shift)
    {
      std::cerr << "Error Syst '" << syst->ShortName() << " already has a Stan pull set (" << itStan->second << ") "
                << "and you're trying to set a different double one (" << shift << ")." << std::endl;
      std::cerr << "You almost certainly didn't mean to do that." << std::endl;
      std::cerr << "Abort." << std::endl;
      abort();
    }

    fSystsDbl.erase(syst);
    if(force || shift != 0.) fSystsDbl.emplace(syst, Clamp(shift, syst));
}

//----------------------------------------------------------------------
void SystShifts::SetShift(const ISyst* syst, stan::math::var shift)
{
  fSystsStan.erase(syst);
  // note: _always_ put the syst in, even if the value is 0.
  // autodiff relies on the calculation happening so as to get the gradient
  fSystsStan.emplace(syst, shift);
  SetShift(syst, util::GetValAs<double>(shift), true);
}

//----------------------------------------------------------------------
template <>
double SystShifts::GetShift(const ISyst* syst) const
{
  assert(syst);

  auto it = fSystsDbl.find(syst);
  return (it == fSystsDbl.end()) ? 0 : it->second;
}

//----------------------------------------------------------------------
template <>
stan::math::var SystShifts::GetShift(const ISyst* syst) const
{
  assert(syst);

  auto it = fSystsStan.find(syst);
  //assert ( (it == fSystsStan.end()) == (fSystsDbl.find(syst) == fSystsDbl.end()) );     // if you're asking for a Stan syst, and it's not there but a double one is, something went wrong
  if (it == fSystsStan.end())
  {
    auto itDbl = fSystsDbl.find(syst);
    if (itDbl != fSystsDbl.end())
    {
      std::cout << "Warning: creating stan::math::var out of double value for syst '" << syst->ShortName() << "'." << std::endl;
      std::cout << "  If you see this repeatedly, it's likely a problem.  (A few times during startup is probably harmless.)" << std::endl;
      fSystsStan[syst] = stan::math::var(itDbl->second);
      it = fSystsStan.find(syst);
    }
  }
  return (it == fSystsStan.end()) ? 0 : it->second;
}

//----------------------------------------------------------------------
void SystShifts::ResetToNominal()
{
  fID = 0;

  fSystsDbl.clear();
  fSystsStan.clear();
}

//----------------------------------------------------------------------
double SystShifts::Penalty() const
{
  double ret = 0;
  // Systematics are all expressed in terms of sigmas
  for(auto it: fSystsDbl) ret += it.second * it.second;
  return ret;
}


//----------------------------------------------------------------------
stan::math::var SystShifts::LogPrior() const { return log(Prior()); }

//----------------------------------------------------------------------
void SystShifts::Shift(Restorer &restore, caf::StandardRecord *sr,
                       double &weight) const {
  // always fSystsDbl here because this is only used in the event loop, not in fitting
  // (so the autodiff'd version is not needed)
  for (auto it : fSystsDbl)
    it.first->Shift(it.second, restore, sr, weight);
}

//----------------------------------------------------------------------
std::string SystShifts::ShortName() const {
  if (IsNominal())
    return "nominal";

  std::string ret;
  for (auto it : fSystsDbl) {
    if (!ret.empty())
      ret += ",";
    ret += it.first->ShortName() + TString::Format("=%+g", it.second).Data();
  }

  return ret;
}

//----------------------------------------------------------------------
std::string SystShifts::LatexName() const {
  if (IsNominal())
    return "Nominal";

  std::string ret;
  for (auto it : fSystsDbl) {
    if (!ret.empty())
      ret += ", ";
    ret += it.first->LatexName() + TString::Format(" = %+g", it.second).Data();
  }

  return ret;
}

//----------------------------------------------------------------------
std::vector<const ISyst *> SystShifts::ActiveSysts() const {
  std::vector<const ISyst *> ret;
  for (auto it : fSystsDbl)
    ret.push_back(it.first);
  return ret;
}

//----------------------------------------------------------------------
template <typename T>
T SystShifts::Clamp(const T& x, const ISyst *s) {
  static bool once = true;
  static bool shouldClamp;
  if (once) {
    once = false;
    char *e = getenv("CAFANA_DONT_CLAMP_SYSTS");
    if (!e) {
      shouldClamp = true;
    } else {
      shouldClamp = (TString(e) == "0");
    }

    if (!shouldClamp) {
      std::cout << "SystShifts.cxx: clamping disabled by environment variable"
                << std::endl;
    }
  }

  if (shouldClamp)
    return std::min(std::max(x, util::GetValAs<T>(s->Min())), util::GetValAs<T>(s->Max()));
  else
    return x;
}

// manually instantiate the template for the correct types
template double SystShifts::Clamp<double>(const double&, const ISyst*);
template stan::math::var SystShifts::Clamp<stan::math::var>(const stan::math::var&, const ISyst*);

//----------------------------------------------------------------------
void SystShifts::SaveTo(TDirectory* dir) const
{
  TDirectory* tmp = gDirectory;

  dir->cd();
  TObjString("SystShifts").Write("type");

  // Don't write any histogram for the nominal case
  if(!fSystsDbl.empty()){
    TH1D h("", "", fSystsDbl.size(), 0, fSystsDbl.size());
    int ibin = 0;
    for(auto it: fSystsDbl){
      ++ibin;
      h.GetXaxis()->SetBinLabel(ibin, it.first->ShortName().c_str());
      h.SetBinContent(ibin, it.second);
    }
    h.Write("vals");
  }

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<SystShifts> SystShifts::LoadFrom(TDirectory* dir)
{
  TObjString* tag = (TObjString*)dir->Get("type");
  assert(tag);
  assert(tag->GetString() == "SystShifts");

  auto ret = std::make_unique<SystShifts>();

  TH1* h = (TH1*)dir->Get("vals");
  if(h){ // no histogram means nominal
    for(int i = 1; i <= h->GetNbinsX(); ++i){
      ret->SetShift(Registry<ISyst>::ShortNameToPtr(h->GetXaxis()->GetBinLabel(i)),
                    h->GetBinContent(i));
    }
  }

  return ret;
}


  //----------------------------------------------------------------------
  std::unique_ptr<SystShifts> GaussianPriorSystShifts::Copy() const
  {
    return std::make_unique<GaussianPriorSystShifts>(*this);
  }

  //----------------------------------------------------------------------
  stan::math::var GaussianPriorSystShifts::LogPrior() const
  {
    stan::math::var ret = 0;
    // Systematics are all expressed in terms of sigmas
    for(auto it: fSystsStan) ret += it.second * it.second;
    return ret;
  }

  //----------------------------------------------------------------------
  stan::math::var GaussianPriorSystShifts::Prior() const
  {
    return exp(LogPrior());
  }
} // namespace ana
