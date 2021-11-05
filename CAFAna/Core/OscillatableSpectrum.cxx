#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Stan.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/SRProxy.h"

#include "OscLib/IOscCalc.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TMD5.h"
#include "TObjString.h"

#include <cassert>
#include <memory>

/*<<<<<<< HEAD
namespace ana {
// Duplicate here because we can't include Vars.h
const Var kTrueE([](const caf::StandardRecord *sr) { return sr->Ev; });

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(
    const std::string &label, const Binning &bins, SpectrumLoaderBase &loader,
    const Var &var, const Cut &cut, const SystShifts &shift, const Var &wei)
    : ReweightableSpectrum(label, bins, kTrueE), fCachedOsc(0, {}, {}, 0, 0),
      fCachedHash(0) {
  fTrueLabel = "True Energy (GeV)";

  DontAddDirectory guard;

  fHist = HistCache::NewTH2D("", bins);

  loader.AddReweightableSpectrum(*this, var, cut, shift, wei);
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(SpectrumLoaderBase &loader,
                                           const HistAxis &axis, const Cut &cut,
                                           const SystShifts &shift,
                                           const Var &wei)
    : ReweightableSpectrum(axis.GetLabels(), axis.GetBinnings(), kTrueE),
      fCachedOsc(0, {}, {}, 0, 0), fCachedHash(0) {
  fTrueLabel = "True Energy (GeV)";

  Binning bins1D = fBins[0];
  if (fBins.size() > 1) {
    int n = 1;
    for (const Binning &b : fBins)
      n *= b.NBins();
    bins1D = Binning::Simple(n, 0, n);
  }

  std::string label;
  for (const std::string &l : fLabels)
    label += l + " and ";
  label.resize(label.size() - 5); // drop the last "and"

  DontAddDirectory guard;

  fHist = HistCache::NewTH2D("", bins1D);

  Var multiDVar = axis.GetVars()[0];
  if (axis.NDimensions() == 2)
    multiDVar = Var2D(axis.GetVars()[0], axis.GetBinnings()[0],
                      axis.GetVars()[1], axis.GetBinnings()[1]);
  if (axis.NDimensions() == 3)
    multiDVar =
        Var3D(axis.GetVars()[0], axis.GetBinnings()[0], axis.GetVars()[1],
              axis.GetBinnings()[1], axis.GetVars()[2], axis.GetBinnings()[2]);

  loader.AddReweightableSpectrum(*this, multiDVar, cut, shift, wei);
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(const std::string &label,
                                           const Binning &bins)
    : ReweightableSpectrum(label, bins, kTrueE), fCachedOsc(0, {}, {}, 0, 0),
      fCachedHash(0) {
  fTrueLabel = "True Energy (GeV)";

  DontAddDirectory guard;

  fPOT = 0;
  fLivetime = 0;

  fHist = HistCache::NewTH2D("", bins);
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(const std::string &label, double pot,
                                           double livetime, const Binning &bins)
    : ReweightableSpectrum(label, bins, kTrueE), fCachedOsc(0, {}, {}, 0, 0),
      fCachedHash(0) {
  fTrueLabel = "True Energy (GeV)";

  DontAddDirectory guard;

  fPOT = pot;
  fLivetime = livetime;

  fHist = HistCache::NewTH2D("", bins);
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(
    TH2 *h, const std::vector<std::string> &labels,
    const std::vector<Binning> &bins, double pot, double livetime)
    : ReweightableSpectrum(kTrueE, h, labels, bins, pot, livetime),
      fCachedOsc(0, {}, {}, 0, 0), fCachedHash(0) {
  fTrueLabel = "True Energy (GeV)";
  if (fHist) {
    HistCache::Adopt(fHist);
  }
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(
    std::unique_ptr<TH2D> h, const std::vector<std::string> &labels,
    const std::vector<Binning> &bins, double pot, double livetime)
    : ReweightableSpectrum(kTrueE, std::move(h), labels, bins, pot, livetime),
      fCachedOsc(0, {}, {}, 0, 0), fCachedHash(0) {
  fTrueLabel = "True Energy (GeV)";
  if (fHist) {
    HistCache::Adopt(fHist);
======= */
namespace ana
{
  // Duplicate here because we can't include Vars.h
  const Var kTrueE([](const caf::SRProxy* sr)
                   {return sr->Ev;});

  //----------------------------------------------------------------------
  OscillatableSpectrum::
  OscillatableSpectrum(const std::string& label, const Binning& bins,
                       SpectrumLoaderBase& loader,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift,
                       const Weight& wei)
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
                                             const Weight& wei)
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
//}


//----------------------------------------------------------------------
/*OscillatableSpectrum::~OscillatableSpectrum() {
  // Nulls fHist out, so it's safe that ~ReweightableSpectrum tries too
  HistCache::Delete(fHist, Bins1DX().ID(), kTrueEnergyBins.ID());

  for (SpectrumLoaderBase *loader : fLoaderCount) {
    loader->RemoveReweightableSpectrum(this);
  }

  delete fCachedHash;
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(const OscillatableSpectrum &rhs)
    : ReweightableSpectrum(rhs.fLabels, rhs.fBins, kTrueE),
      fCachedOsc(0, {}, {}, 0, 0), fCachedHash(0) {
  DontAddDirectory guard;

  fHist = HistCache::Copy(rhs.fHist, rhs.Bins1DX(), kTrueEnergyBins);

  fPOT = rhs.fPOT;
  fLivetime = rhs.fLivetime;

  if (rhs.fCachedHash) {
    fCachedOsc = rhs.fCachedOsc;
    fCachedHash = new TMD5(*rhs.fCachedHash);
  }

  assert(rhs.fLoaderCount.empty()); // Copying with pending loads is unexpected
}

//----------------------------------------------------------------------
OscillatableSpectrum::OscillatableSpectrum(OscillatableSpectrum &&rhs)
    : ReweightableSpectrum(rhs.fLabels, rhs.fBins, kTrueE),
      fCachedOsc(0, {}, {}, 0, 0), fCachedHash(0) {
  DontAddDirectory guard;

  fHist = rhs.fHist;
  rhs.fHist = 0;

  fPOT = rhs.fPOT;
  fLivetime = rhs.fLivetime;

  if (rhs.fCachedHash) {
    fCachedOsc = std::move(rhs.fCachedOsc);
    fCachedHash = rhs.fCachedHash;
    rhs.fCachedHash = 0;
  }

  assert(rhs.fLoaderCount.empty()); // Copying with pending loads is unexpected
}

//----------------------------------------------------------------------
OscillatableSpectrum &OscillatableSpectrum::
operator=(const OscillatableSpectrum &rhs) {
  if (this == &rhs)
    return *this;

  DontAddDirectory guard;

  if (fHist)
    HistCache::Delete(fHist, Bins1DX().ID());
  fHist = HistCache::Copy(rhs.fHist, rhs.Bins1DX(), kTrueEnergyBins);
  fPOT = rhs.fPOT;
  fLivetime = rhs.fLivetime;
  fLabels = rhs.fLabels;
  fBins = rhs.fBins;
=======*/
  //----------------------------------------------------------------------
  OscillatableSpectrum::~OscillatableSpectrum()
  {
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(const OscillatableSpectrum& rhs)
    : ReweightableSpectrum(rhs)
  {
    if(rhs.fCache->hash){
      fCache->spect = rhs.fCache->spect;
      fCache->hash = std::make_unique<TMD5>(*rhs.fCache->hash);
    }

    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(OscillatableSpectrum&& rhs)
    : ReweightableSpectrum(rhs)
  {
    if(rhs.fCache->hash){
      fCache->spect = std::move(rhs.fCache->spect);
      fCache->hash = std::move(rhs.fCache->hash);
    }

    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(const OscillatableSpectrum& rhs)
  {
    if(this == &rhs) return *this;

    ReweightableSpectrum::operator=(rhs);

    if(rhs.fCache->hash){
      fCache->spect = rhs.fCache->spect;
      fCache->hash = std::make_unique<TMD5>(*rhs.fCache->hash);
    }
    else{
      fCache->hash.reset();
    }

    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
    assert( fReferences.empty() ); // Copying with pending loads is unexpected
//>>>>>>> origin

/*    if (rhs.fCachedHash) {
      fCachedOsc = rhs.fCachedOsc;
      delete fCachedHash;
      fCachedHash = new TMD5(*rhs.fCachedHash);
    }
  
    assert(rhs.fLoaderCount.empty()); // Copying with pending loads is unexpected
    assert(fLoaderCount.empty());     // Copying with pending loads is unexpected
*/
//<<<<<<< HEAD
    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum &OscillatableSpectrum::operator=(OscillatableSpectrum &&rhs) {
    if (this == &rhs) return *this;

  /*DontAddDirectory guard;

  if (fHist)
    HistCache::Delete(fHist, Bins1DX().ID());
  fHist = rhs.fHist;
  rhs.fHist = 0;
  fPOT = rhs.fPOT;
  fLivetime = rhs.fLivetime;
  fLabels = rhs.fLabels;
  fBins = rhs.fBins;
======= */
    ReweightableSpectrum::operator=(rhs);

    if(rhs.fCache->hash){
      fCache->spect = std::move(rhs.fCache->spect);
      fCache->hash = std::move(rhs.fCache->hash);
    }
    else{
      fCache->hash.reset();
    }

    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
    assert( fReferences.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  template<class T> Spectrum OscillatableSpectrum::
  _Oscillated(osc::_IOscCalc<T>* calc, int from, int to) const
  {
    TMD5* hash = calc->GetParamsHash();
    if(hash && fCache->hash && *hash == *fCache->hash){
      delete hash;
      return fCache->spect;
    }

    const OscCurve curve(calc, from, to);
    const Spectrum ret = WeightedBy(curve);
    if(hash){
      fCache->spect = ret;
      fCache->hash.reset(hash);
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
    fCache->hash.reset(nullptr);

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
    fCache->hash.reset(nullptr);

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
} // namespace ana
