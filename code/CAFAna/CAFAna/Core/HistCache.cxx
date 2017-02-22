#include "CAFAna/Core/HistCache.h"

#include "CAFAna/Core/Utilities.h"

#include "TH2.h"

#include <iostream>

namespace ana
{
  std::multimap<HistCache::Key_t, std::unique_ptr<TH1D>> HistCache::fgMap;
  std::multimap<HistCache::Key_t, std::unique_ptr<TH2D>> HistCache::fgMap2D;
  int HistCache::fgOut = 0;
  int HistCache::fgIn = 0;

  long HistCache::fgEstMemUsage = 0;

  //---------------------------------------------------------------------
  TH1D* HistCache::New(const std::string& title, const Binning& bins)
  {
    ++fgOut;

    if(!bins.IsSimple())
      return new TH1D(UniqueName().c_str(), title.c_str(),
                      bins.NBins(), &bins.Edges()[0]);

    return New(title, Key(bins));
  }

  //---------------------------------------------------------------------
  TH1D* HistCache::New(const std::string& title, const TAxis* bins)
  {
    ++fgOut;

    if(bins->GetXbins()->GetArray()) // ie not simple
      return new TH1D(UniqueName().c_str(), title.c_str(),
                      bins->GetNbins(), bins->GetXbins()->GetArray());

    return New(title, Key(bins));
  }

  //---------------------------------------------------------------------
  TH2D* HistCache::NewTH2D(const std::string& title, const Binning& bins)
  {
    ++fgOut;

    if(!bins.IsSimple())
      return new TH2D(UniqueName().c_str(), title.c_str(),
                      bins.NBins(), &bins.Edges()[0],
                      kTrueEnergyBins.NBins(), &kTrueEnergyBins.Edges()[0]);

    return NewTH2D(title, Key(bins));
  }

  //---------------------------------------------------------------------
  TH2D* HistCache::NewTH2D(const std::string& title, const TAxis* bins)
  {
    ++fgOut;

    if(bins->GetXbins()->GetArray()) // ie not simple
      return new TH2D(UniqueName().c_str(), title.c_str(),
                      bins->GetNbins(), bins->GetXbins()->GetArray(),
                      kTrueEnergyBins.NBins(), &kTrueEnergyBins.Edges()[0]);

    return NewTH2D(title, Key(bins));
  }

  //---------------------------------------------------------------------
  TH1D* HistCache::New(const std::string& title, Key_t key)
  {
    auto it = fgMap.find(key);
    if(it != fgMap.end()){
      TH1D* ret = it->second.release();
      fgMap.erase(it);
      ret->Reset();
      ret->SetTitle(title.c_str());

      fgEstMemUsage -= 16*ret->GetNbinsX();

      return ret;
    }
    else{
      return new TH1D(UniqueName().c_str(), title.c_str(),
                      std::get<0>(key), std::get<1>(key), std::get<2>(key));
    }
  }

  //---------------------------------------------------------------------
  TH2D* HistCache::NewTH2D(const std::string& title, Key_t key)
  {
    auto it = fgMap2D.find(key);
    if(it != fgMap2D.end()){
      TH2D* ret = it->second.release();
      fgMap2D.erase(it);
      ret->Reset();
      ret->SetTitle(title.c_str());

      fgEstMemUsage -= 16*ret->GetNbinsX()*100;

      return ret;
    }
    else{
      return new TH2D(UniqueName().c_str(), title.c_str(),
                      std::get<0>(key), std::get<1>(key), std::get<2>(key),
                      kTrueEnergyBins.NBins(), &kTrueEnergyBins.Edges()[0]);
    }
  }

  //---------------------------------------------------------------------
  TH1D* HistCache::Copy(TH1D* h)
  {
    // It has custom binning, we don't want to deal with that, so just copy it
    // dumbly
    if(h->GetXaxis()->GetXbins()->GetArray()){
      ++fgOut;
      return new TH1D(*h);
    }

    TH1D* ret = New(h->GetTitle(), h->GetXaxis());
    *ret = *h;
    return ret;
  }

  //---------------------------------------------------------------------
  TH2D* HistCache::Copy(TH2D* h)
  {
    // It has custom binning, we don't want to deal with that, so just copy it
    // dumbly
    if(h->GetXaxis()->GetXbins()->GetArray()){
      ++fgOut;
      return new TH2D(*h);
    }

    TH2D* ret = NewTH2D(h->GetTitle(), h->GetXaxis());
    *ret = *h;
    return ret;
  }

  //---------------------------------------------------------------------
  void HistCache::Delete(TH1D*& h)
  {
    if(!h) return;

    ++fgIn;

    // Custom binning, don't poison our cache with it
    if(h->GetXaxis()->GetXbins()->GetArray()){
      delete h;
      h = 0;
      return;
    }

    fgMap.emplace(Key(h->GetXaxis()), std::unique_ptr<TH1D>(h));

    fgEstMemUsage += 16*h->GetNbinsX();
    CheckMemoryUse();

    h = 0;
  }

  //---------------------------------------------------------------------
  void HistCache::Delete(TH2D*& h)
  {
    if(!h) return;

    // Sanity check that this is a true energy axis
    assert(h->GetNbinsY() == 100);

    ++fgIn;

    // Custom binning, don't poison our cache with it
    if(h->GetXaxis()->GetXbins()->GetArray()){
      delete h;
      h = 0;
      return;
    }

    fgMap2D.emplace(Key(h->GetXaxis()), std::unique_ptr<TH2D>(h));

    fgEstMemUsage += 16*h->GetNbinsX()*100;
    CheckMemoryUse();

    h = 0;
  }

  //---------------------------------------------------------------------
  HistCache::Key_t HistCache::Key(const TAxis* ax)
  {
    assert(!ax->GetXbins()->GetArray());
    return Key_t(ax->GetNbins(), ax->GetXmin(), ax->GetXmax());
  }

  //---------------------------------------------------------------------
  HistCache::Key_t HistCache::Key(const Binning& bins)
  {
    assert(bins.IsSimple());
    return Key_t(bins.NBins(), bins.Min(), bins.Max());
  }

  //---------------------------------------------------------------------
  void HistCache::CheckMemoryUse()
  {
    if(fgEstMemUsage > 500l*1024*1024){
      std::cerr << "Warning! HistCache memory usage exceeds 500MB. "
                << "That probably means histograms are being returned "
                << "to the cache that weren't originally handed out by it. "
                << "Now clearing cache. This could take a long time..."
                << std::endl;
      ClearCache();
      std::cerr << "Done clearing cache" << std::endl;
    }
  }

  //---------------------------------------------------------------------
  void HistCache::ClearCache()
  {
    fgMap.clear();
    fgMap2D.clear();
    fgEstMemUsage = 0;
  }

  //---------------------------------------------------------------------
  void HistCache::PrintStats()
  {
    // Count number of unique keys
    std::set<Key_t> keys;
    for(auto& it: fgMap) keys.insert(it.first);

    std::cout << "Gave out " << fgOut << " histograms, got back "
              << fgIn << " of them (" << fgOut-fgIn << " lost), in "
              << keys.size() << " different shapes." << std::endl
              << "Holding " << fgMap.size()+fgMap2D.size()
              << " histograms for an estimated memory usage of "
              << fgEstMemUsage << " bytes." << std::endl;
  }
}
