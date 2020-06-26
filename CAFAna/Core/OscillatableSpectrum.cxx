#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Ratio.h"
#include "Utilities/func/Stan.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TMD5.h"
#include "TObjString.h"

#include <cassert>
#include <memory>

namespace ana
{
  // Duplicate here because we can't include Vars.h
  const Var kTrueE([](const caf::StandardRecord* sr)
                   {return sr->Ev;});

  //----------------------------------------------------------------------
  OscillatableSpectrum::
  OscillatableSpectrum(const std::string& label, const Binning& bins,
                       SpectrumLoaderBase& loader,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift,
                       const Var& wei)
    : ReweightableSpectrum(loader,
                           HistAxis(label, bins, var),
                           HistAxis("True Energy (GeV)", kTrueEnergyBins, kTrueE),
                           cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(SpectrumLoaderBase& loader,
                                             const HistAxis& axis,
                                             const Cut& cut,
                                             const SystShifts& shift,
                                             const Var& wei)
    : ReweightableSpectrum(loader, axis,
                           HistAxis("True Energy (GeV)", kTrueEnergyBins, kTrueE),
                           cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::~OscillatableSpectrum()
  {
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(const OscillatableSpectrum& rhs)
    : ReweightableSpectrum(rhs)
  {
    if(rhs.fOscCacheHash){
      fOscCacheSpect = rhs.fOscCacheSpect;
      fOscCacheHash = std::make_unique<TMD5>(*rhs.fOscCacheHash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(OscillatableSpectrum&& rhs)
    : ReweightableSpectrum(rhs)
  {
    if(rhs.fOscCacheHash){
      fOscCacheSpect = std::move(rhs.fOscCacheSpect);
      fOscCacheHash = std::move(rhs.fOscCacheHash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(const OscillatableSpectrum& rhs)
  {
    if(this == &rhs) return *this;

    ReweightableSpectrum::operator=(rhs);

    if(rhs.fOscCacheHash){
      fOscCacheSpect = rhs.fOscCacheSpect;
      fOscCacheHash = std::make_unique<TMD5>(*rhs.fOscCacheHash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(OscillatableSpectrum&& rhs)
  {
    if(this == &rhs) return *this;

    ReweightableSpectrum::operator=(rhs);

    if(rhs.fOscCacheHash){
      fOscCacheSpect = std::move(rhs.fOscCacheSpect);
      fOscCacheHash = std::move(rhs.fOscCacheHash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  template<class T> Spectrum OscillatableSpectrum::
  _Oscillated(osc::_IOscCalculator<T>* calc, int from, int to) const
  {
    TMD5* hash = calc->GetParamsHash();
    if(hash && fOscCacheHash && *hash == *fOscCacheHash){
      delete hash;
      return fOscCacheSpect;
    }

    const OscCurve curve(calc, from, to);
    const Spectrum ret = WeightedBy(curve);
    if(hash){
      fOscCacheSpect = ret;
      fOscCacheHash.reset(hash);
    }

    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalculator* calc,
                                            int from, int to) const
  {
    return _Oscillated(calc, from, to);
  }

  //----------------------------------------------------------------------
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalculatorStan* calc,
                                            int from, int to) const
  {
    return _Oscillated(calc, from, to);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator+=(const OscillatableSpectrum& rhs)
  {
    ReweightableSpectrum::operator+=(rhs);

    // invalidate
    fOscCacheHash.reset(nullptr);

    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum OscillatableSpectrum::operator+(const OscillatableSpectrum& rhs) const
  {
    OscillatableSpectrum ret = *this;
    ret += rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator-=(const OscillatableSpectrum& rhs)
  {
    ReweightableSpectrum::operator-=(rhs);

    // invalidate
    fOscCacheHash.reset(nullptr);

    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum OscillatableSpectrum::operator-(const OscillatableSpectrum& rhs) const
  {
    OscillatableSpectrum ret = *this;
    ret -= rhs;
    return ret;
  }

  //----------------------------------------------------------------------
  void OscillatableSpectrum::SaveTo(TDirectory* dir, const std::string& name) const
  {
    // TODO should mostly (entirely?) forward to RWSpect

    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("OscillatableSpectrum").Write("type");

    TH2D* h = ToTH2(fPOT);
    h->Write("hist");

    TH1D hPot("", "", 1, 0, 1);
    hPot.Fill(.5, fPOT);
    hPot.Write("pot");
    TH1D hLivetime("", "", 1, 0, 1);
    hLivetime.Fill(.5, fLivetime);
    hLivetime.Write("livetime");

    for(unsigned int i = 0; i < fBins.size(); ++i){
      TObjString(fLabels[i].c_str()).Write(TString::Format("label%d", i).Data());
      fBins[i].SaveTo(dir, TString::Format("bins%d", i).Data());
    }

    dir->Write();
    delete dir;

    HistCache::Delete(h);

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<OscillatableSpectrum> OscillatableSpectrum::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    DontAddDirectory guard;

    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "OscillatableSpectrum");
    delete tag;

    TH2D* spect = (TH2D*)dir->Get("hist");
    assert(spect);
    TH1* hPot = (TH1*)dir->Get("pot");
    assert(hPot);
    TH1* hLivetime = (TH1*)dir->Get("livetime");
    assert(hLivetime);

    std::vector<std::string> labels;
    std::vector<Binning> bins;

    for(int i = 0; ; ++i){
      const std::string subname = TString::Format("bins%d", i).Data();
      TDirectory* subdir = dir->GetDirectory(subname.c_str());
      if(!subdir) break;
      delete subdir;
      bins.push_back(*Binning::LoadFrom(dir, subname));
      TObjString* label = (TObjString*)dir->Get(TString::Format("label%d", i));
      labels.push_back(label ? label->GetString().Data() : "");
      delete label;
    }

    if(bins.empty() && labels.empty()){
      // Must be an old file. Make an attempt at backwards compatibility.
      bins.push_back(Binning::FromTAxis(spect->GetXaxis()));
      labels.push_back(spect->GetXaxis()->GetTitle());
    }

    delete dir;

    // Can't restore the Vars, go with a dummy value
    const std::vector<Var> vars(labels.size(), kUnweighted);
    auto ret = std::make_unique<OscillatableSpectrum>(kNullLoader,
                                                      HistAxis(labels, bins, vars),
                                                      kNoCut);

    *ret->fMat = Eigen::Map<const Eigen::MatrixXd>(spect->GetArray(),
                                                   ret->fMat->rows(),
                                                   ret->fMat->cols());

    return ret;
  }
}
