#include "CAFAna/Atmos/AtmosOscillogram.h"

namespace ana
{ 
  const double kRefBaseline = 1000;

  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  ToEigen(osc::_IOscCalc<T>* calc, int from, int to)
  {
    ((osc::IOscCalcAdjustable*)calc)->SetL(kRefBaseline);

    const unsigned int N = kTrueEnergyBinCenters.size();

    // Have to allow for underflow and overflow
    Eigen::Array<T, Eigen::Dynamic, 1> ret(N+2);
    ret[0] = 0; // underflow
    ret[N+1] = (from == to || to == 0) ? 1 : 0; // overflow

    const Eigen::Array<T, Eigen::Dynamic, 1> Ps = calc->P(from, to, kTrueEnergyBinCenters);

    // This is clumsy, but hopefully faster than calculating oscillation probs
    // for two dummy values.
    for(unsigned int i = 0; i < N; ++i) ret[i+1] = Ps[i];

    return ret;
  }

  //----------------------------------------------------------------------
  AtmosOscillogram::AtmosOscillogram(osc::IOscCalc* calc, int from, int to)
    : Ratio(Hist::Adopt(ToEigen(calc, from, to)),
            std::vector<Binning>(1, kTrueEnergyBins),
            std::vector<std::string>(1, "1000 #times True E / L (GeV / km)")),
      fFrom(from), fTo(to)
  {
  }
}
