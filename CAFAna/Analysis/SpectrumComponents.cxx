#include "CAFAna/Analysis/SpectrumComponents.h"

#include <map>
#include <string>

#include "TDirectory.h"
#include "TH1F.h"
#include "TKey.h"
#include "TLegend.h"
#include "TObjString.h"
#include "TParameter.h"
#include "TVirtualPad.h"

#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Core/Spectrum.h"

namespace ana
{
  template <typename RecT>
  const _Cut<RecT> _SpectrumComponents<RecT>::kNoCut = _Cut<RecT>(NoCut<RecT>());

  template <typename RecT>
  const _Weight<RecT> _SpectrumComponents<RecT>::kUnweighted = Unweighted<RecT>();

  //-------------------------------------------------------------------------
  template <typename RecT>
  _SpectrumComponents<RecT>::_SpectrumComponents(const ana::_Cut<RecT>& baseCut,
                                               const ana::_HistAxis<_Var<RecT>> &axis,
                                               ana::_IRecordSource<RecT> &source,
                                               const std::map<std::string, Component> &categories,
                                               const _Weight<RecT> &wei)
  :
    fSel(baseCut),
    fTot(source.Weighted(wei)[fSel], axis)
  {
    if (!categories.empty())
    {
      for (const auto & componentPair : categories)
      {
        fComponentDescr.emplace(componentPair.first, componentPair.second);
        fComponentSpecs.emplace(std::piecewise_construct,
                                std::forward_as_tuple(componentPair.first),
                                std::forward_as_tuple(source.Weighted(wei)[fSel && componentPair.second.cut], axis));
      }
    }
  }

  // todo: MultiVarHistAxis is currently disabled in CAFAnaCore
  // //-------------------------------------------------------------------------
  // SpectrumComponents::SpectrumComponents(const ana::Cut& baseCut,
  //                                         const ana::MultiVarHistAxis & axis,
  //                                         ana::SpectrumLoaderBase & loader,
  //                                         const Weight & wei,
  //                                         const std::map<std::string, Component>& categories)
  // :
  //   fSel(baseCut),
  //   fTot(loader, axis, fSel, kNoShift, wei)
  // {
  //   if (!categories.empty())
  //   {
  //     for (const auto & componentPair : categories)
  //     {
  //       fComponentDescr.emplace(componentPair.first, componentPair.second);
  //       fComponentSpecs.emplace(std::piecewise_construct,
  //                               std::forward_as_tuple(componentPair.first),
  //                               std::forward_as_tuple(loader, axis, fSel && componentPair.second.cut, kNoShift, wei));
  //     }
  //   }
  // }

  //-------------------------------------------------------------------------
  template <typename RecT>
  TLegend* _SpectrumComponents<RecT>::DrawLegend(double drawThreshold, TLegend * leg) const
  {
    if (!leg)
      leg = AutoPlaceLegend(0.25, 0.18 + 0.04*fComponentDescr.size());

    double thresh = 0;
    if (drawThreshold >= 0 && drawThreshold <= 1.0)
      thresh = fTot.Integral(fTot.POT()) * drawThreshold;
    else
      // this is supposed to be a fraction
      assert(drawThreshold <= 1 && "SpectrumComponents::DrawLegend(): legend draw thresholds must be between 0.0 and 1.0");
    for (const auto & mcSpecPair : fComponentDescr)
    {
      auto spec = fComponentSpecs.at(mcSpecPair.first);
      if (spec.Integral(spec.POT()) < thresh)
        continue;

      TH1F* comp = new TH1F();
      comp->SetLineColor(kBlack);
      comp->SetFillColor(mcSpecPair.second.color);
      leg->AddEntry(comp, mcSpecPair.second.blurb.c_str(), "f");
    }


    leg->SetFillColor(0);
    leg->SetFillStyle(0);

    for(const auto& obj:*leg->GetListOfPrimitives())
    {
      if(obj->InheritsFrom("TAttFill")){
        ((TAttFill*)obj)->SetFillStyle(0);
      }

    }
    leg->Draw();
    return leg;
  }

  //-------------------------------------------------------------------------
  template <typename RecT>
  void _SpectrumComponents<RecT>::DrawComponents(bool stacked, EBinType bintype, double POT, bool sameAll) const
  {
    if (POT == 0)
      POT = fComponentSpecs.begin()->second.POT();

    TH1 * last = nullptr;
    std::vector<TH1*> hists;

    // iterate backwards so that they're drawn in the specified order from top to bottom
    for (auto it_comp = fComponentSpecs.rbegin(); it_comp != fComponentSpecs.rend(); it_comp++)
    {
      auto h = it_comp->second.ToTH1(POT, kBlack, kSolid, kPOT, bintype);
      if (stacked)
      {
        if (last)
         h->Add(last);
        h->SetFillColor(fComponentDescr.at(it_comp->first).color);
        h->SetLineWidth(1);
        last = h;
      }

      hists.push_back(h);
    }
    bool drawn = false;
    for (auto it_hist = hists.rbegin(); it_hist != hists.rend(); it_hist++)
    {
      std::string opt = "hist";
      if (drawn || sameAll)
        opt += " same";
      (*it_hist)->Draw(opt.c_str());
      if (!drawn)
        drawn = true;
    }

    // if there were fills they will be overlapping the axis ticks
    gPad->RedrawAxis();
  }

  //-------------------------------------------------------------------------
  template <typename RecT>
  float _SpectrumComponents<RecT>::Purity(const std::set<std::string>& signalCatNames) const
  {
    double bkgCounts = 0;
    double POT = fComponentSpecs.begin()->second.POT();
    for (const auto & mcCatPair : fComponentSpecs)
    {
      if (signalCatNames.find(mcCatPair.first) != signalCatNames.end())
        continue;
      bkgCounts += mcCatPair.second.ToTH1(POT)->GetEntries();
    }
    return 1 - bkgCounts / fTot.ToTH1(POT)->GetEntries();
  }

  //-------------------------------------------------------------------------
  template <typename RecT>
  void _SpectrumComponents<RecT>::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    fTot.SaveTo(dir, "Total");

    TDirectory * allCompsDir = dir->mkdir("Components");
    for (const auto & specPair : fComponentSpecs)
    {
      TDirectory* compDir = allCompsDir->mkdir(specPair.first.c_str());
      specPair.second.SaveTo(compDir, "spectrum");

      compDir->cd();
      const auto & comp = fComponentDescr.at(specPair.first);
      TObjString blurb(comp.blurb.c_str());
      blurb.Write("blurb");

      TParameter<int> color;
      color.SetVal(comp.color);
      color.Write("color");
    }

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //-------------------------------------------------------------------------
  template <typename RecT>
  std::unique_ptr<_SpectrumComponents<RecT>> _SpectrumComponents<RecT>::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    Spectrum tot = *Spectrum::LoadFrom(dir, "Total");

    std::map<std::string, Component> compDescrs;
    std::map<std::string, Spectrum> compSpecs;
    TDirectory * compDir = dynamic_cast<TDirectory*>(dir->Get("Components"));
    for ( TObject * obj : *(compDir->GetListOfKeys()) )
    {
      TKey * key = dynamic_cast<TKey*>(obj);
      TDirectory * subdir = dynamic_cast<TDirectory*>(key->ReadObj());
      if (!subdir)
        continue;

      compSpecs.emplace(key->GetName(), *(Spectrum::LoadFrom(subdir, "spectrum").release()));
      compDescrs.emplace(std::piecewise_construct,
                           std::forward_as_tuple(key->GetName()),
                           std::forward_as_tuple(kNoCut,  // Cut object won't be re-used anyhow since Spectra are already made?
                                                 dynamic_cast<TObjString*>(subdir->Get("blurb"))->String().Data(),
                                                 dynamic_cast<TParameter<int>*>(subdir->Get("color"))->GetVal() )
      );
    }

    auto ret = std::make_unique<_SpectrumComponents>(
        std::move(tot),
        std::move(compDescrs),
        std::move(compSpecs)
    );

    delete dir;

    return ret;
  }

  //-------------------------------------------------------------------------
  // we can add more instantiations if we need to
  template class _SpectrumComponents<caf::SRInteractionProxy>;
  template class _SpectrumComponents<caf::SRRecoParticleProxy>;
  template class _SpectrumComponents<caf::SRTrueInteractionProxy>;
  template class _SpectrumComponents<caf::SRTrueParticleProxy>;


}
