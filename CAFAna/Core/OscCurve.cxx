#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Binning.h"

#include "OscLib/IOscCalc.h"

#include <cassert>
#include <map>

#include "TH1.h"

namespace ana
{
  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  ToEigen(osc::_IOscCalc<T>* calc, int from, int to)
  {
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
  OscCurve::OscCurve(osc::IOscCalc* calc, int from, int to)
    : Ratio(Hist::Adopt(ToEigen(calc, from, to)),
            std::vector<Binning>(1, kTrueEnergyBins),
            std::vector<std::string>(1, "True Energy (GeV)")),
      fFrom(from), fTo(to)
  {
  }

  //----------------------------------------------------------------------
#ifdef CAFANA_USE_STAN
  OscCurve::OscCurve(osc::IOscCalcStan* calc, int from, int to)
    : Ratio(Hist::AdoptStan(ToEigen(calc, from, to)),
            std::vector<Binning>(1, kTrueEnergyBins),
            std::vector<std::string>(1, "True Energy (GeV)")),
      fFrom(from), fTo(to)
  {
  }
#endif

  //----------------------------------------------------------------------
  OscCurve::~OscCurve()
  {
  }

  //----------------------------------------------------------------------
  TH1D* OscCurve::ToTH1(bool title) const
  {
    // Could have a file temporarily open
    DontAddDirectory guard;

    TH1D* ret = Ratio::ToTH1();
    ret->GetYaxis()->SetTitle("Probability");

    if(title){
      // Don't do this work unless it's explicitly requested
      std::map<int, std::string> nus;
      nus[12] = nus[-12] = "e";
      nus[14] = nus[-14] = "#mu";
      nus[16] = nus[-16] = "#tau";
      nus[0] = "active";
      const std::string nu = (fFrom > 0) ? "#nu" : "#bar#nu";

      ret->SetTitle((nu+"_{"+nus[fFrom]+"}#rightarrow"+nu+"_{"+nus[fTo]+"}").c_str());
    }

    return ret;
  }
}
