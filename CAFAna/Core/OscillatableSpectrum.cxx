#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Ratio.h"
#include "Utilities/func/Stan.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/IOscCalc.h"

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
  OscillatableSpectrum::OscillatableSpectrum(const Eigen::MatrixXd&& mat,
                                             const HistAxis& recoAxis,
                                             double pot, double livetime)
    : ReweightableSpectrum(std::move(mat), recoAxis,
                           HistAxis("True Energy (GeV)", kTrueEnergyBins, kTrueE),
                           pot, livetime)
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
  _Oscillated(osc::_IOscCalc<T>* calc, int from, int to) const
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
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalc* calc,
                                            int from, int to) const
  {
    return _Oscillated(calc, from, to);
  }

  //----------------------------------------------------------------------
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalcStan* calc,
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
    _SaveTo(dir, name, "OscillatableSpectrum");
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

    delete dir;

    auto ret = std::make_unique<OscillatableSpectrum>(kNullLoader,
                                                      HistAxis(labels, bins),
                                                      kNoCut);

    // ROOT histogram storage is row-major, but Eigen is column-major by
    // default
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen:: Dynamic, Eigen::RowMajor> MatRowMajor;
    ret->fMat = Eigen::Map<MatRowMajor>(spect->GetArray(),
                                        ret->fMat.rows(),
                                        ret->fMat.cols());

    delete spect;

    ret->fPOT = hPot->Integral(0, -1);
    ret->fLivetime = hLivetime->Integral(0, -1);

    delete hPot;
    delete hLivetime;

    return ret;
  }
}
