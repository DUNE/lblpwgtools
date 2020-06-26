#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Binning.h"

#include "OscLib/func/IOscCalculator.h"

#include <cassert>
#include <map>

#include "TH1.h"

namespace ana
{
  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  ToEigen(osc::_IOscCalculator<T>* calc, int from, int to)
  {
    return calc->P(from, to, kTrueEnergyBinCenters);
    /*
    const int N = kTrueEnergyBins.NBins();
    Eigen::Array<T, Eigen::Dynamic, 1> hist(N+2);

    hist[0] = 0; // underflow
    for(int i = 1; i <= N; ++i){
      const double E = kTrueEnergyBinCenters[i];
      hist[i] = calc->P(from, to, E);
    }
    hist[N+1] = (from == to || to == 0) ? 1 : 0;

    return hist;
    */
  }

  //----------------------------------------------------------------------
  OscCurve::OscCurve(osc::IOscCalculator* calc, int from, int to)
    : Ratio(Hist::Adopt(ToEigen(calc, from, to)),
            std::vector<Binning>(1, kTrueEnergyBins),
            std::vector<std::string>(1, "True Energy (GeV)")),
      fFrom(from), fTo(to)
  {
  }

  //----------------------------------------------------------------------
  OscCurve::OscCurve(osc::IOscCalculatorStan* calc, int from, int to)
    : Ratio(Hist::Adopt(ToEigen(calc, from, to)),
            std::vector<Binning>(1, kTrueEnergyBins),
            std::vector<std::string>(1, "True Energy (GeV)")),
      fFrom(from), fTo(to)
  {
  }

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
