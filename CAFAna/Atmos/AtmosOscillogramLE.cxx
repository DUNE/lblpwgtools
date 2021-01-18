// There is too much copy/paste from the regular AtmosOscillogram. If we did
// want to be able to do things both ways we would have to consolidate.

#include "CAFAna/Atmos/AtmosOscillogramLE.h"

#include "CAFAna/Core/MathUtil.h"

#include "OscLib/EarthModel.h"

#include "OscLib/OscCalcAnalytic.h"

#include <iostream>

namespace ana
{
  const Binning kTrueLOverEBins = Binning::Simple(500, 0, 5000);
  //  const Binning kTrueCosZenithBins = Binning::Simple(50, -1, +1);

  const Binning kTrueLBins = Binning::Simple(50, 0, 13000);

  /*const*/ std::vector<double> kTrueLOverECenters = BinCenters(kTrueLOverEBins);

  const Eigen::ArrayXd kTrueLOverECentersEig = Eigen::Map<Eigen::ArrayXd>(kTrueLOverECenters.data(), kTrueLOverECenters.size());

  const HistAxis kTrueLOverEAxis("True L / E (km / GeV)", kTrueLOverEBins);
  //  const HistAxis kTrueCosZenithAxis("True cos(#theta_{zenith})", kTrueCosZenithBins);
  const HistAxis kTrueLAxis("True L (km)", kTrueLBins);

  const HistAxis k2DTrueAxisLE(kTrueLOverEAxis, kTrueLAxis);//CosZenithAxis);
  const Var2DMapper trueMapperLE(kTrueLOverEBins, kTrueLBins);//CosZenithBins);

  osc::EarthModel earthLE("PREM", .1);

  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  AtmosToEigenLE(osc::_IOscCalc<T>* calc2, int from, int to)
  {
    osc::OscCalcAnalytic* calc = dynamic_cast<osc::OscCalcAnalytic*>(calc2);
    if(!calc){
      std::cout << "Must use OscCalcAnalytic" << std::endl;
      abort();
    }

    using EigenArrayXT = Eigen::Array<T, Eigen::Dynamic, 1>;
    EigenArrayXT ret(kTrueLOverEBins.NBins()*kTrueLBins/*CosZenithBins*/.NBins()+2);
    ret[0] = ret[ret.size()-1] = 0;

    //    for(int j = 0; j < kTrueCosZenithBins.NBins(); ++j){
    //      const double cosQ = (kTrueCosZenithBins.Edges()[j]+kTrueCosZenithBins.Edges()[j+1])/2;
    for(int j = 0; j < kTrueLBins.NBins(); ++j){
      // NB this may not be precisely the totL we use below
      const double L = (kTrueLBins.Edges()[j]+kTrueLBins.Edges()[j+1])/2;

      const double R = 6371; // km
      const double dR = 15; // production height (guess)
      //      const double approxL  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      const double cosQ = (-L*L*R*R + dR*dR*dR*dR + 4*dR*dR*dR*R + 5*dR*dR*R*R + 2*dR*R*R*R)/(2*L*R*R*(R+dR));
      if(cosQ < -1 || cosQ > +1){
        for(double LE: kTrueLOverECenters){
          ret[trueMapperLE.Map(LE, L)+1] = 0;
        }
        continue;
      }

      std::list<double> Ls, Ns;
      earthLE.LineProfile(dR, cosQ, 0, Ls, Ns);

      std::vector<osc::Layer> layers;
      layers.reserve(Ls.size());
      double totL = 0;
      for(auto itL = Ls.begin(), itN = Ns.begin(); itL != Ls.end(); ++itL, ++itN){
        if(*itL > 1e-3) layers.emplace_back(*itL, *itN * 2);
        totL += *itL;
      }

      Eigen::ArrayXd trueECenters = kTrueLOverECentersEig;
      for(int i = 0; i < trueECenters.size(); ++i){
        const double LOverE = trueECenters[i];
        trueECenters[i] = totL / LOverE;
      }

      const EigenArrayXT Ps = calc->P(from, to, trueECenters, layers);

      for(int i = 0; i < kTrueLOverEBins.NBins(); ++i){
        const double trueLoverE = kTrueLOverECentersEig[i];
        const T P = Ps[i];

        // TODO understand nans/infs
        ret[trueMapperLE.Map(trueLoverE, L/*cosQ*/)+1] = (isnan(P) || isinf(P)) ? 0 : P;
      } // end for i (trueE)
    } // end for j (cosQ)

    return ret;
  }

  //----------------------------------------------------------------------
  AtmosOscillogramLE::AtmosOscillogramLE(osc::IOscCalc* calc, int from, int to)
    : Ratio(Hist::Adopt(AtmosToEigenLE(calc, from, to)),
            {kTrueLOverEBins, kTrueLBins},//kTrueCosZenithBins},
            {"True L/E (km/GeV)", "True L (km)"}),//cos(#theta_{zenith})"}),
      fFrom(from), fTo(to)
  {
  }
}
