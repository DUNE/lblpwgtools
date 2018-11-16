#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

#include <cassert>
#include <memory>

namespace ana
{
  // Duplicate here because we can't include Vars.h
  //  const Var kTrueE({"mc.nu", "mc.nnu", "mc.nu.E"},
  //                   [](const caf::StandardRecord* sr)
  //                   {return (sr->mc.nnu == 0) ? 0 : sr->mc.nu[0].E;});

  const Var kTrueE({"dune.Ev"},
                   [](const caf::StandardRecord* sr)
                   {return sr->dune.Ev;});

  //----------------------------------------------------------------------
  OscillatableSpectrum::
  OscillatableSpectrum(std::string label, const Binning& bins,
                       SpectrumLoaderBase& loader,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift,
                       const Var& wei)
    : ReweightableSpectrum(label, bins, kTrueE),
      fOscCache(0, {}, {}, 0, 0), fOscHash(kUninitHash)
  {
    fTrueLabel = "True Energy (GeV)";

    DontAddDirectory guard;

    fHist = HistCache::NewTH2D("", bins);

    loader.AddReweightableSpectrum(*this, var, cut, shift, wei);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(SpectrumLoaderBase& loader,
                                             const HistAxis& axis,
                                             const Cut& cut,
                                             const SystShifts& shift,
                                             const Var& wei)
    : ReweightableSpectrum(axis.GetLabels(), axis.GetBinnings(), kTrueE),
      fOscCache(0, {}, {}, 0, 0), fOscHash(kUninitHash)
  {
    fTrueLabel = "True Energy (GeV)";

    Binning bins1D = fBins[0];
    if(fBins.size() > 1){
      int n = 1;
      for(const Binning& b: fBins) n *= b.NBins();
      bins1D = Binning::Simple(n, 0, n);
    }

    std::string label;
    for(const std::string& l: fLabels) label += l + " and ";
    label.resize(label.size()-5); // drop the last "and"

    DontAddDirectory guard;

    fHist = HistCache::NewTH2D("", bins1D);

    Var multiDVar = axis.GetVars()[0];
    if(axis.NDimensions() == 2)
      multiDVar = Var2D(axis.GetVars()[0], axis.GetBinnings()[0],
                        axis.GetVars()[1], axis.GetBinnings()[1]);
    if(axis.NDimensions() == 3)
      multiDVar = Var3D(axis.GetVars()[0], axis.GetBinnings()[0],
                        axis.GetVars()[1], axis.GetBinnings()[1],
                        axis.GetVars()[2], axis.GetBinnings()[2]);

    loader.AddReweightableSpectrum(*this, multiDVar, cut, shift, wei);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(std::string label,
                                             const Binning& bins)
    : ReweightableSpectrum(label, bins, kTrueE),
      fOscCache(0, {}, {}, 0, 0), fOscHash(kUninitHash)
  {
    fTrueLabel = "True Energy (GeV)";

    DontAddDirectory guard;

    fPOT = 0;
    fLivetime = 0;

    fHist = HistCache::NewTH2D("", bins);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(std::string label, double pot, double livetime,
                                             const Binning& bins)
    : ReweightableSpectrum(label, bins, kTrueE),
      fOscCache(0, {}, {}, 0, 0), fOscHash(kUninitHash)
  {
    fTrueLabel = "True Energy (GeV)";

    DontAddDirectory guard;

    fPOT = pot;
    fLivetime = livetime;

    fHist = HistCache::NewTH2D("", bins);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(TH2* h,
                                             const std::vector<std::string>& labels,
                                             const std::vector<Binning>& bins,
                                             double pot, double livetime)
    : ReweightableSpectrum(kTrueE, h, labels, bins, pot, livetime),
      fOscCache(0, {}, {}, 0, 0), fOscHash(kUninitHash)
  {
    fTrueLabel = "True Energy (GeV)";
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::~OscillatableSpectrum()
  {
    // Nulls fHist out, so it's safe that ~ReweightableSpectrum tries too
    HistCache::Delete(fHist);

    for (SpectrumLoaderBase* loader : fLoaderCount)
    { loader->RemoveReweightableSpectrum(this); }
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(const OscillatableSpectrum& rhs)
    : ReweightableSpectrum(rhs.fLabels, rhs.fBins, kTrueE),
      fOscCache(0, {}, {}, 0, 0), fOscHash(rhs.fOscHash),
      fOscCacheFrom(rhs.fOscCacheFrom), fOscCacheTo(rhs.fOscCacheTo)
  {
    if(rhs.fOscHash != kUninitHash) fOscCache = rhs.fOscCache;

    DontAddDirectory guard;

    fHist = HistCache::Copy(rhs.fHist);

    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(OscillatableSpectrum&& rhs)
    : ReweightableSpectrum(rhs.fLabels, rhs.fBins, kTrueE),
      fOscCache(0, {}, {}, 0, 0), fOscHash(rhs.fOscHash),
      fOscCacheFrom(rhs.fOscCacheFrom), fOscCacheTo(rhs.fOscCacheTo)
  {
    if(rhs.fOscHash != kUninitHash) fOscCache = rhs.fOscCache;

    DontAddDirectory guard;

    fHist = rhs.fHist;
    rhs.fHist = 0;

    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(const OscillatableSpectrum& rhs)
  {
    if(this == &rhs) return *this;

    if(rhs.fOscHash != kUninitHash){
      fOscCache = rhs.fOscCache;
      fOscHash = rhs.fOscHash;
      fOscCacheFrom = rhs.fOscCacheFrom;
      fOscCacheTo = rhs.fOscCacheTo;
    }

    DontAddDirectory guard;

    HistCache::Delete(fHist);
    fHist = HistCache::Copy(rhs.fHist);
    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;
    fLabels = rhs.fLabels;
    fBins = rhs.fBins;

    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(OscillatableSpectrum&& rhs)
  {
    if(this == &rhs) return *this;

    if(rhs.fOscHash != kUninitHash){
      fOscCache = rhs.fOscCache;
      fOscHash = rhs.fOscHash;
      fOscCacheFrom = rhs.fOscCacheFrom;
      fOscCacheTo = rhs.fOscCacheTo;
    }

    DontAddDirectory guard;

    HistCache::Delete(fHist);
    fHist = rhs.fHist;
    rhs.fHist = 0;
    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;
    fLabels = rhs.fLabels;
    fBins = rhs.fBins;

    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalculator* calc,
                                            int from, int to) const
  {
    std::unique_ptr<TMD5> hash(calc->GetParamsHash());

    if(from == fOscCacheFrom && to == fOscCacheTo &&
       hash && *hash == fOscHash){
      return fOscCache;
    }

    const OscCurve curve(calc, from, to);
    std::unique_ptr<TH1> Ps(curve.ToTH1()); // Don't leak the histogram
    Spectrum ret = WeightedBy(Ps.get());
    if(hash){
      fOscHash = *hash;
      fOscCache = ret;
      fOscCacheFrom = from;
      fOscCacheTo = to;
    }
    return ret;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator+=(const OscillatableSpectrum& rhs)
  {
    if(rhs.fPOT){
      fHist->Add(rhs.fHist, fPOT/rhs.fPOT);
    }
    else{
      // How can it have events but no POT?
      assert(rhs.fHist->Integral() == 0);
    }
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
    if(rhs.fPOT){
      fHist->Add(rhs.fHist, -fPOT/rhs.fPOT);
    }
    else{
      // How can it have events but no POT?
      assert(rhs.fHist->Integral() == 0);
    }
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
  void OscillatableSpectrum::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();

    TObjString("OscillatableSpectrum").Write("type");

    fHist->Write("hist");
    TH1D hPot("", "", 1, 0, 1);
    hPot.Fill(.5, fPOT);
    hPot.Write("pot");
    TH1D hLivetime("", "", 1, 0, 1);
    hLivetime.Fill(.5, fLivetime);
    hLivetime.Write("livetime");

    for(unsigned int i = 0; i < fBins.size(); ++i){
      TObjString(fLabels[i].c_str()).Write(TString::Format("label%d", i).Data());
      fBins[i].SaveTo(dir->mkdir(TString::Format("bins%d", i)));
    }

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<OscillatableSpectrum> OscillatableSpectrum::LoadFrom(TDirectory* dir)
  {
    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "OscillatableSpectrum");

    TH2D* spect = (TH2D*)dir->Get("hist");
    assert(spect);
    TH1* hPot = (TH1*)dir->Get("pot");
    assert(hPot);
    TH1* hLivetime = (TH1*)dir->Get("livetime");
    assert(hLivetime);

    std::vector<std::string> labels;
    std::vector<Binning> bins;

    for(int i = 0; ; ++i){
      TDirectory* subdir = dir->GetDirectory(TString::Format("bins%d", i));
      if(!subdir) break;
      bins.push_back(*Binning::LoadFrom(subdir));
      TObjString* label = (TObjString*)dir->Get(TString::Format("label%d", i));
      labels.push_back(label ? label->GetString().Data() : "");
    }

    if(bins.empty() && labels.empty()){
      // Must be an old file. Make an attempt at backwards compatibility.
      bins.push_back(Binning::FromTAxis(spect->GetXaxis()));
      labels.push_back(spect->GetXaxis()->GetTitle());
    }

    return std::make_unique<OscillatableSpectrum>(spect,
                                                  labels, bins,
                                                  hPot->GetBinContent(1),
                                                  hLivetime->GetBinContent(1));
  }
}
