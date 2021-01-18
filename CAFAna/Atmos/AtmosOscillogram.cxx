#include "CAFAna/Atmos/AtmosOscillogram.h"

#include "CAFAna/Core/MathUtil.h"

#include "OscLib/EarthModel.h"

#include "OscLib/OscCalcAnalytic.h"

#include <iostream>

namespace ana
{
  const Binning kTrueEBins = Binning::Simple(400, 0.1, 100);
  // TODO it's not really clear that 50 cos_zenith bins are enough. Or,
  // shouldn't we bin evenly around the perimeter of the earth instead?
  const Binning kTrueCosZenithBins = Binning::Simple(50, -1, +1);

  /*const*/ std::vector<double> kTrueECenters = BinCenters(kTrueEBins);

  const Eigen::ArrayXd kTrueECentersEig = Eigen::Map<Eigen::ArrayXd>(kTrueECenters.data(), kTrueECenters.size());

  const HistAxis kTrueEAxis("True E (GeV)", kTrueEBins);
  const HistAxis kTrueCosZenithAxis("True cos(#theta_{zenith})", kTrueCosZenithBins);

  const HistAxis k2DTrueAxis(kTrueEAxis, kTrueCosZenithAxis);
  const Var2DMapper trueMapper(kTrueEBins, kTrueCosZenithBins);

  //----------------------------------------------------------------------
  class LayerCache
  {
  public:
    static LayerCache& Instance()
    {
      static LayerCache lc;
      return lc;
    }

    std::vector<std::vector<osc::Layer>> layers; // indexed by cosQ bin

  protected:
    LayerCache();
  };

  //----------------------------------------------------------------------
  LayerCache::LayerCache()
  {
    osc::EarthModel earth("PREM", .1);

    layers.resize(kTrueCosZenithBins.NBins());

    for(int j = 0; j < kTrueCosZenithBins.NBins(); ++j){
      const double cosQ = (kTrueCosZenithBins.Edges()[j]+kTrueCosZenithBins.Edges()[j+1])/2;

      std::list<double> Ls, Ns;
      const double dR = 15; // production height (guess)
      earth.LineProfile(dR, cosQ, 0, Ls, Ns);

      layers[j].reserve(Ls.size());
      for(auto itL = Ls.begin(), itN = Ns.begin(); itL != Ls.end(); ++itL, ++itN){
        if(*itL > 1e-3) layers[j].emplace_back(*itL, *itN * 2);
      }
    } // end for j
  }

  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  AtmosToEigen(osc::_IOscCalc<T>* calc2, int from, int to)
  {
    osc::OscCalcAnalytic* calc = dynamic_cast<osc::OscCalcAnalytic*>(calc2);
    if(!calc){
      std::cout << "Must use OscCalcAnalytic" << std::endl;
      abort();
    }

    using EigenArrayXT = Eigen::Array<T, Eigen::Dynamic, 1>;
    EigenArrayXT ret(kTrueEBins.NBins()*kTrueCosZenithBins.NBins()+2);
    ret[0] = ret[ret.size()-1] = 0;

    for(int j = 0; j < kTrueCosZenithBins.NBins(); ++j){
      const double cosQ = (kTrueCosZenithBins.Edges()[j]+kTrueCosZenithBins.Edges()[j+1])/2;

      const EigenArrayXT Ps = calc->P(from, to, kTrueECentersEig, LayerCache::Instance().layers[j]);

      for(int i = 0; i < kTrueEBins.NBins(); ++i){
        const double trueE = kTrueECentersEig[i];
        const T P = Ps[i];

        // TODO understand nans/infs
        ret[trueMapper.Map(trueE, cosQ)+1] = (isnan(P) || isinf(P)) ? 0 : P;
      } // end for i (trueE)
    } // end for j (cosQ)

    return ret;
  }

  //----------------------------------------------------------------------
  AtmosOscillogram::AtmosOscillogram(osc::IOscCalc* calc, int from, int to)
    : Ratio(Hist::Adopt(AtmosToEigen(calc, from, to)),
            {kTrueEBins, kTrueCosZenithBins},
            {"True Energy (GeV)", "True cos(#theta_{zenith})"}),
      fFrom(from), fTo(to)
  {
  }
}
