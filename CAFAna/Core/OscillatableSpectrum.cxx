#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Utilities.h"

#ifdef CAFANA_USE_STAN
#include "CAFAna/Core/Stan.h"
#endif


#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/SRInteraction.h"
#include "OscLib/IOscCalc.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TMD5.h"
#include "TObjString.h"

#include <cassert>
#include <memory>

namespace ana
{
  namespace{
  // Duplicate here because we can't include Vars.h
  // These should be true informations, but I think we only have recoparticle atm 
  const Var kTrueE([](const caf::SRInteractionProxy* ixn)
                   {
                     //assert(sr->mc.nnu == 1);
                     assert(ixn->truth.size()>0);
                     /// uuumm what do we
                     return ixn->Enu.calo;
                   });
     const Cut kHasNu([](const caf::SRInteractionProxy* ixn)
                     {
                       //return sr->truth.index >= 0;
                      // this should return something at the true level, 
                      //in the meantime just check there's interaction or whatever placeholder cut
                        return ixn->truth.size()>0;//dlp.size()>0;
                     });
   }
  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(IInteractionSource& src, const HistAxis& axis)
    : ReweightableSpectrum(src[kHasNu], axis, HistAxis("True Energy (GeV)", kTrueEnergyBins, kTrueE))
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
    
    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum &OscillatableSpectrum::operator=(OscillatableSpectrum &&rhs)
  {
    if (this == &rhs) return *this;

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
#ifdef CAFANA_USE_STAN
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalcStan* calc,
                                            int from, int to) const
  {
    return _Oscillated(calc, from, to);
  }
#endif

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

    const HistAxis recoAxis(labels, bins);

    // ROOT histogram storage is row-major, but Eigen is column-major by
    // default
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen:: Dynamic, Eigen::RowMajor> MatRowMajor;

    auto ret = std::make_unique<OscillatableSpectrum>(Eigen::Map<MatRowMajor>(spect->GetArray(), kTrueEnergyBins.NBins()+2, recoAxis.GetBins1D().NBins()+2),
                                                      recoAxis,
                                                      hPot->Integral(0, -1),
                                                      hLivetime->Integral(0, -1));

    delete spect;

    delete hPot;
    delete hLivetime;

    return ret;
  }
} // namespace ana
