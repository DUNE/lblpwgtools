#include "CAFAna/Atmos/AtmosOscillogram.h"

#include "CAFAna/Core/MathUtil.h"

#include "OscLib/EarthModel.h"

#include <iostream>

namespace ana
{
  const Binning kTrueEBins = Binning::Simple(400, 0.1, 100);
  // TODO it's not really clear that 50 cos_zenith bins are enough. Or,
  // shouldn't we bin evenly around the perimeter of the earth instead?
  const Binning kTrueCosZenithBins = Binning::Simple(50, -1, +1);

  const HistAxis kTrueEAxis("True E (GeV)", kTrueEBins);
  const HistAxis kTrueCosZenithAxis("True cos(#theta_{zenith})", kTrueCosZenithBins);

  const HistAxis k2DTrueAxis(kTrueEAxis, kTrueCosZenithAxis);
  const Var2DMapper trueMapper(kTrueEBins, kTrueCosZenithBins);

  osc::EarthModel earth("PREM", .1);

  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  AtmosToEigen(osc::_IOscCalc<T>* calc, int from, int to)
  {
    Eigen::Array<T, Eigen::Dynamic, 1> ret(kTrueEBins.NBins()*kTrueCosZenithBins.NBins()+2);
    ret[0] = ret[ret.size()-1] = 0;

    for(int j = 0; j < kTrueCosZenithBins.NBins(); ++j){
      const double cosQ = (kTrueCosZenithBins.Edges()[j]+kTrueCosZenithBins.Edges()[j+1])/2;

      //      const double R = 6371; // km
      const double dR = 15; // production height (guess)
      //      const double L  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      std::list<double> Ls, Ns;
      earth.LineProfile(dR, cosQ, 0, Ls, Ns);

      double Ltot = 0;
      double Navg = 0;
      for(auto itL = Ls.begin(), itN = Ns.begin(); itL != Ls.end(); ++itL, ++itN){
        Ltot += *itL;
        Navg += *itN * *itL;
      }
      Navg /= Ltot;

      ((osc::IOscCalcAdjustable*)calc)->SetL(Ltot);
      ((osc::IOscCalcAdjustable*)calc)->SetRho(Navg*2);

      for(int i = 0; i < kTrueEBins.NBins(); ++i){
        const double trueE = (kTrueEBins.Edges()[i]+kTrueEBins.Edges()[i+1])/2;

        const T P = calc->P(from, to, trueE); // TODO batch?

        ret[trueMapper.Map(trueE, cosQ)] = P;
      }
    }

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
