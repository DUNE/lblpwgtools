#pragma once

#include <map>
#include <string>

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Var.h"

class TLegend;

namespace ana
{

  template <typename RecT>
  class _SpectrumComponents
  {
    // shortcuts
    const static _Cut<RecT> kNoCut;          // can't be constexpr because CutBase (an ancestor) has a nontrivial destructor
    const static _Weight<RecT> kUnweighted;  // can't be constexpr because VarBase (an ancestor) has a nontrivial destructor

    public:
      struct Component
      {
        Component(const _Cut<RecT>& cut_, const std::string & blurb_="", int color_=kBlue)
          : cut(cut_), blurb(blurb_), color(color_)
        {}

        _Cut<RecT> cut;
        std::string blurb;
        int color;
      };

      /// \brief \ref SpectrumComponents makes a collection of Spectra differing by a single (often truth) Cut.
      ///
      /// \param baseCut    the selection cut shared between all components
      /// \param axis       \ref HistAxis shared between all components
      /// \param loaderData \ref SpectrumLoader for data
      /// \param loaderMC   \ref SpectrumLoader for MC
      /// \param wei        Weight to apply to events
      /// \param systs      std::vector of ISyst* for drawing plots with syst bands
      /// \param bkg        \ref Cut object to define background
      /// \param mcCategories  the various \ref Cut s defining each category
      _SpectrumComponents(const ana::_Cut<RecT>& baseCut,
                         const ana::_HistAxis<_Var<RecT>> & axis,
                         ana::_IRecordSource<RecT> & source,
                         const std::map<std::string, Component>& categories = {},
                         const _Weight<RecT> & wei = kUnweighted);

      // todo: MultiVarHistAxis is currently disabled in CAFAnaCore
      // SpectrumComponents(const ana::_Cut<RecT>& baseCut,
      //                    const ana::_MultiVarHistAxis<RecT> & axis,
      //                    ana::SpectrumLoaderBase & loader,
      //                    const _Weight<RecT> & wei = kUnweighted,
      //                    const std::map<std::string, Component>& categories = {});

      /// Constructor for use in LoadFrom()
      _SpectrumComponents(Spectrum&& tot,
                         std::map<std::string, Component>&& compDescrs,
                         std::map<std::string, Spectrum>&& compSpecs)
       : fSel(kNoCut),
         fTot(tot),
         fComponentDescr(compDescrs),
         fComponentSpecs(compSpecs)
      {}

      /// Constructor from copies...
      _SpectrumComponents(const Spectrum& tot,
                         const std::map<std::string, Component>& compDescrs,
                         const std::map<std::string, Spectrum>& compSpecs)
       : fSel(kNoCut),
         fTot(tot),
         fComponentDescr(compDescrs),
         fComponentSpecs(compSpecs)
      {}

      /// \brief Draw legend on plots
      /// \param drawThreshold   Only create entries for histograms whose integral is greater than this fraction of the total
       TLegend* DrawLegend(double drawThreshold = 0, TLegend * leg = nullptr) const;

      /// \brief Draw MC components distribution
      ///
      /// \param stack    By default components will be shown stacked.  Pass false to show them unstacked instead.
      /// \param bintype  Raw bin contents or bin-width normalized?
      /// \param POT      Normalize to specific POT?  Otherwise use internal Spectrum POT
      /// \param sameAll  Draw all hists with "same" option?
      void DrawComponents(bool stacked=true, EBinType bintype = kBinContent, double POT=0, bool sameAll=true) const;

      /// \brief Purity of the MC selection based on the MC subcategories.
      ///
      /// \param signalCatNames  If any of your subcategories are "signal", pass them here to avoid counting them as background
      float Purity(const std::set<std::string>& signalCatNames={}) const;

      void SaveTo(TDirectory* dir, const std::string& name) const;
      static std::unique_ptr<_SpectrumComponents> LoadFrom(TDirectory* dir, const std::string& name);

      const ana::_Cut<RecT> & Sel() const { return fSel; };

      Spectrum & Tot() { return fTot; };
      const Spectrum & Tot() const { return fTot; };

      unsigned int             NDimensions() const { return fTot.NDimensions(); }
      std::vector<std::string> GetLabels()   const { return fTot.GetLabels(); }
      std::vector<Binning>     GetBinnings() const { return fTot.GetBinnings(); }

      const std::map<std::string, Component> & ComponentDescrs() const { return fComponentDescr; };
            std::map<std::string, Component> & ComponentDescrs()       { return fComponentDescr; };
      const std::map<std::string, Spectrum> &  ComponentSpecs()  const { return fComponentSpecs; };

    private:
      ana::_Cut<RecT> fSel;
      Spectrum fTot;
      std::map<std::string, Component> fComponentDescr;
      std::map<std::string, Spectrum> fComponentSpecs;

  };

  typedef _SpectrumComponents<caf::SRInteractionProxy> SpectrumComponents;
}
