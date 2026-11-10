#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Binning.h"

#include "OscLib/IOscCalc.h"
#include "OscLib/OscCalcSterile.h"

#include <cassert>
#include <map>
#include <iostream>

#include "TH1.h"
#include "TMath.h"

namespace ana
{
  //----------------------------------------------------------------------
  /// Helper for constructors
  template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  ToEigen(osc::_IOscCalc<T>* calc, int from, int to)
  {
    const unsigned int N = TrueEnergyBinCenters().size();

    // Have to allow for underflow and overflow
    Eigen::Array<T, Eigen::Dynamic, 1> ret(N+2);
    ret[0] = 0; // underflow
    ret[N+1] = (from == to || to == 0) ? 1 : 0; // overflow

    // If we're working with steriles, average over rapid dmsq41 oscillations (NOvA docDB-15433)
    // This does not affect standard oscillations fits
    auto scalc = dynamic_cast<osc::IOscCalcSterile*>(calc);
    if (scalc) {
      const std::vector<double> edges = TrueEnergyBins().Edges();
      const double L = scalc->GetL();
      for (unsigned int i = 1; i <= N; ++i) {
        double xLo = L / edges[i];
        double xHi = L / edges[i-1];
        // A bin starting at E=0 extends to infinite L/E, where this average is undefined
        // Give it the L/E width of its neighbour, centred on the bin center
        if (edges[i-1] <= 0) {
          const double xMid = L / (0.5 * (edges[i-1] + edges[i]));
          const double halfWidth = 0.5 * (L / edges[i] - L / edges[i+1]);
          xLo = xMid - halfWidth;
          xHi = xMid + halfWidth;
        }
        double LOverE = 0.5 * (xLo + xHi);
        double f = 2 * 1.27 * scalc->GetDm(4);
        double delta = xHi - xLo;
        double dLoE = f == 0 ? 0 : TMath::ACos(TMath::Sin(0.5*f*delta)/(0.5*f*delta))/f;
        double eplus = L / (LOverE + dLoE);
        double eminus = L / (LOverE - dLoE);
        ret[i] = 0.5 * (scalc->P(from, to, eminus) + scalc->P(from, to, eplus));
      } // for i
    } else {
      // This is clumsy, but hopefully faster than calculating oscillation probs
      // for two dummy values.
      const Eigen::Array<T, Eigen::Dynamic, 1> Ps = calc->P(from, to, TrueEnergyBinCenters());
      for(unsigned int i = 0; i < N; ++i) ret[i+1] = Ps[i];
    }

    return ret;
  }

  //----------------------------------------------------------------------
  OscCurve::OscCurve(osc::IOscCalc* calc, int from, int to)
    : Ratio(Hist::Adopt(ToEigen(calc, from, to)),
            std::vector<Binning>(1, TrueEnergyBins()),
            std::vector<std::string>(1, "True Energy (GeV)")),
      fFrom(from), fTo(to)
  {
  }

  //----------------------------------------------------------------------
#ifdef CAFANA_USE_STAN
  OscCurve::OscCurve(osc::IOscCalcStan* calc, int from, int to)
    : Ratio(Hist::AdoptStan(ToEigen(calc, from, to)),
            std::vector<Binning>(1, TrueEnergyBins()),
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