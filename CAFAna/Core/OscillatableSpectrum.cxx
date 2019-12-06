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
  //  const Var kTrueE({"mc.nu", "mc.nnu", "mc.nu.E"},
  //                   [](const caf::StandardRecord* sr)
  //                   {return (sr->mc.nnu == 0) ? 0 : sr->mc.nu[0].E;});

  const Var kTrueE({"dune.Ev"},
                   [](const caf::StandardRecord* sr)
                   {return sr->dune.Ev;});

  //----------------------------------------------------------------------
  OscillatableSpectrum::
  OscillatableSpectrum(const std::string& label, const Binning& bins,
                       SpectrumLoaderBase& loader,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift,
                       const Var& wei)
    : ReweightableSpectrum(label, bins, kTrueE)
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
    : ReweightableSpectrum(axis.GetLabels(), axis.GetBinnings(), kTrueE)
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
  OscillatableSpectrum::OscillatableSpectrum(const std::string& label,
                                             const Binning& bins)
    : ReweightableSpectrum(label, bins, kTrueE)
  {
    fTrueLabel = "True Energy (GeV)";

    DontAddDirectory guard;

    fPOT = 0;
    fLivetime = 0;

    fHist = HistCache::NewTH2D("", bins);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(const std::string& label, double pot, double livetime,
                                             const Binning& bins)
    : ReweightableSpectrum(label, bins, kTrueE)
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
    : ReweightableSpectrum(kTrueE, h, labels, bins, pot, livetime)
  {
    fTrueLabel = "True Energy (GeV)";
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(std::unique_ptr<TH2D> h,
                                             const std::vector<std::string>& labels,
                                             const std::vector<Binning>& bins,
                                             double pot, double livetime)
    : ReweightableSpectrum(kTrueE, std::move(h), labels, bins, pot, livetime)
  {
    fTrueLabel = "True Energy (GeV)";
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::~OscillatableSpectrum()
  {
    // Nulls fHist out, so it's safe that ~ReweightableSpectrum tries too
    HistCache::Delete(fHist, Bins1DX().ID(), kTrueEnergyBins.ID());

    for (SpectrumLoaderBase* loader : fLoaderCount)
    { loader->RemoveReweightableSpectrum(this); }

    delete fCachedHash;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(const OscillatableSpectrum& rhs)
    : ReweightableSpectrum(rhs.fLabels, rhs.fBins, kTrueE)
  {
    DontAddDirectory guard;

    fHist = HistCache::Copy(rhs.fHist, rhs.Bins1DX(), kTrueEnergyBins);

    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;

    if (rhs.OscCache<Spectrum>::hash)
    {
      OscCache<Spectrum>::spect = rhs.OscCache<Spectrum>::spect;
      OscCache<Spectrum>::hash = std::make_unique<TMD5>(*rhs.OscCache<Spectrum>::hash);
    }
    if (rhs.OscCache<SpectrumStan>::hash)
    {
      OscCache<SpectrumStan>::spect = rhs.OscCache<SpectrumStan>::spect;
      OscCache<SpectrumStan>::hash = std::make_unique<TMD5>(*rhs.OscCache<SpectrumStan>::hash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum::OscillatableSpectrum(OscillatableSpectrum&& rhs)
    : ReweightableSpectrum(rhs.fLabels, rhs.fBins, kTrueE)
  {
    DontAddDirectory guard;

    fHist = rhs.fHist;
    rhs.fHist = 0;

    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;

    if (rhs.OscCache<Spectrum>::hash)
    {
      OscCache<Spectrum>::spect = std::move(rhs.OscCache<Spectrum>::spect);
      OscCache<Spectrum>::hash = std::move(rhs.OscCache<Spectrum>::hash);
    }
    if (rhs.OscCache<SpectrumStan>::hash)
    {
      OscCache<SpectrumStan>::spect = std::move(rhs.OscCache<SpectrumStan>::spect);
      OscCache<SpectrumStan>::hash = std::move(rhs.OscCache<SpectrumStan>::hash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(const OscillatableSpectrum& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    if(fHist) HistCache::Delete(fHist, Bins1DX().ID());
    fHist = HistCache::Copy(rhs.fHist, rhs.Bins1DX(), kTrueEnergyBins);
    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;
    fLabels = rhs.fLabels;
    fBins = rhs.fBins;

    if (rhs.OscCache<Spectrum>::hash)
    {
      OscCache<Spectrum>::spect = rhs.OscCache<Spectrum>::spect;
      OscCache<Spectrum>::hash = std::make_unique<TMD5>(*rhs.OscCache<Spectrum>::hash);
    }
    if (rhs.OscCache<SpectrumStan>::hash)
    {
      OscCache<SpectrumStan>::spect = rhs.OscCache<SpectrumStan>::spect;
      OscCache<SpectrumStan>::hash = std::make_unique<TMD5>(*rhs.OscCache<SpectrumStan>::hash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum& OscillatableSpectrum::operator=(OscillatableSpectrum&& rhs)
  {
    if(this == &rhs) return *this;

    DontAddDirectory guard;

    if(fHist) HistCache::Delete(fHist, Bins1DX().ID());
    fHist = rhs.fHist;
    rhs.fHist = 0;
    fPOT = rhs.fPOT;
    fLivetime = rhs.fLivetime;
    fLabels = rhs.fLabels;
    fBins = rhs.fBins;

    if (rhs.OscCache<Spectrum>::hash)
    {
      OscCache<Spectrum>::spect = std::move(rhs.OscCache<Spectrum>::spect);
      OscCache<Spectrum>::hash = std::move(rhs.OscCache<Spectrum>::hash);
    }
    if (rhs.OscCache<SpectrumStan>::hash)
    {
      OscCache<SpectrumStan>::spect = std::move(rhs.OscCache<SpectrumStan>::spect);
      OscCache<SpectrumStan>::hash = std::move(rhs.OscCache<SpectrumStan>::hash);
    }

    assert( rhs.fLoaderCount.empty() ); // Copying with pending loads is unexpected
    assert( fLoaderCount.empty() ); // Copying with pending loads is unexpected

    return *this;
  }

  //----------------------------------------------------------------------
  Spectrum OscillatableSpectrum::Oscillated(osc::IOscCalculator* calc,
                                            int from, int to) const
  {
    TMD5* hash = calc->GetParamsHash();
    if(hash && OscCache<Spectrum>::hash && *hash == *OscCache<Spectrum>::hash){
      delete hash;
      return OscCache<Spectrum>::spect;
    }

    const OscCurve curve(calc, from, to);
    TH1D* Ps = curve.ToTH1();

    const Spectrum ret = WeightedBy(Ps);
    if (hash)
    {
      OscCache<Spectrum>::spect = ret;
      OscCache<Spectrum>::hash.reset(hash);
    }
    HistCache::Delete(Ps, kTrueEnergyBins.ID());
    return ret;
  }

  //----------------------------------------------------------------------
  SpectrumStan OscillatableSpectrum::Oscillated(osc::_IOscCalculator<stan::math::var>* calc,
                                                int from, int to) const
  {
    TMD5* hash = calc->GetParamsHash();
    if(hash && OscCache<SpectrumStan>::hash && *hash == *OscCache<SpectrumStan>::hash){
      delete hash;
      return OscCache<SpectrumStan>::spect;
    }

    DontAddDirectory guard;

    TH1 * h;
    if (fHistD)
      h = fHistD;
    else
      h = fHistF;
    const int X = h->GetNbinsX();
    const int Y = h->GetNbinsY();

    std::vector<stan::math::var> binC;
    binC.reserve(X+2);  // count under- and over-flow
    for (int x = 0; x < X+2; x++)
      binC.emplace_back(0.0);

    std::vector<double> binCtr;
    for (int i = 0; i < Y+2; i++)
      binCtr.push_back(h->GetYaxis()->GetBinCenter(i));   // todo: what _should_ happen for under- and overflow?...

    int bin = 0;
    for(int y = 0; y < Y+2; ++y){
      auto w = calc->P(from, to, binCtr[y]);
      for(int x = 0; x < X+2; ++x){
        binC[x] += h->GetBinContent(bin)*w;
        ++bin;
      }
    }

    SpectrumStan ret(std::move(binC), fLabels, fBins, fPOT, fLivetime);

    if (hash)
    {
      OscCache<SpectrumStan>::spect = ret;
      OscCache<SpectrumStan>::hash.reset(hash);
    }
    return std::move(ret);
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

    // invalidate
    OscCache<Spectrum>::hash.reset(nullptr);
    OscCache<SpectrumStan>::hash.reset(nullptr);

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

    // invalidate
    OscCache<Spectrum>::hash.reset(nullptr);
    OscCache<SpectrumStan>::hash.reset(nullptr);

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
      TDirectory* subdir = dir->GetDirectory(TString::Format("bins%d", i));
      if(!subdir) break;
      bins.push_back(*Binning::LoadFrom(subdir));
      TObjString* label = (TObjString*)dir->Get(TString::Format("label%d", i));
      labels.push_back(label ? label->GetString().Data() : "");
      delete subdir;
      delete label;
    }

    if(bins.empty() && labels.empty()){
      // Must be an old file. Make an attempt at backwards compatibility.
      bins.push_back(Binning::FromTAxis(spect->GetXaxis()));
      labels.push_back(spect->GetXaxis()->GetTitle());
    }

    auto ret = std::make_unique<OscillatableSpectrum>(std::unique_ptr<TH2D>(spect),
                                                      labels, bins,
                                                      hPot->GetBinContent(1),
                                                      hLivetime->GetBinContent(1));

    delete hPot;
    delete hLivetime;
    return ret;
  }
}
