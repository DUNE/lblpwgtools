#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Binning.h"

#include "OscLib/IOscCalc.h"
#include "OscLib/IOscCalcSterile.h"

#include <cassert>
#include <map>

#include <string>
#include <iostream>

#include "TH1.h"
#include "TMath.h"

namespace ana
{

  // //----------------------------------------------------------------------
  // /// Helper for constructors
  // template<class T> Eigen::Array<T, Eigen::Dynamic, 1>
  // ToEigen(osc::_IOscCalc<T>* calc, int from, int to)
  // {
  //   const unsigned int N = kTrueEnergyBinCenters.size();

  //   // Have to allow for underflow and overflow
  //   Eigen::Array<T, Eigen::Dynamic, 1> ret(N+2);
  //   ret[0] = 0; // underflow
  //   ret[N+1] = (from == to || to == 0) ? 1 : 0; // overflow

  //   const Eigen::Array<T, Eigen::Dynamic, 1> Ps = calc->P(from, to, kTrueEnergyBinCenters);

  //   // This is clumsy, but hopefully faster than calculating oscillation probs
  //   // for two dummy values.
  //   for(unsigned int i = 0; i < N; ++i) ret[i+1] = Ps[i];

  //   return ret;
  // }


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

    // If we're working with steriles, average over rapid dmsq41 oscillations (see docDB-15433)
    auto scalc = dynamic_cast<osc::IOscCalcSterile*>(calc);
    if (scalc) {
      const double Dmsq41 = scalc->GetDm(4); // Δm²₄₁
      const double L    = scalc->GetL();
      const double f    = 2*1.27*Dmsq41*L;

      // DEBUG: print first 3 sterile OscCurve calls
      static int debugCount = 0;
      const bool doPrint = (debugCount < 3);
      if (doPrint) {
        std::cout << "DEBUG OscCurve[" << debugCount << "]:"
                  << " from=" << from << " to=" << to
                  << " GetL()=" << L
                  << " Dmsq41=" << Dmsq41
                  << " f=" << f << std::endl;
      }

      for (unsigned int i = 1; i <= N; ++i) {
        double eLo = kTrueEnergyBins.Edges()[i-1];
        if (eLo <= 0) eLo = 1e-5;
        double eHi = kTrueEnergyBins.Edges()[i];
        if (eHi <= 0) eHi = 1e-5;
        const double xLo = 1 / eHi;
        const double xHi = 1 / eLo;
        double LOverE = 0.5 * (xLo + xHi);
        double delta = xHi - xLo;
        double dLoE = f == 0 ? 0 : TMath::ACos(TMath::Sin(0.5*f*delta)/(0.5*f*delta))/f;
        double eplus = 1 / (LOverE + dLoE);
        double eminus = 1 / (LOverE - dLoE);
        ret[i] = 0.5 * (scalc->P(from, to, eminus) + scalc->P(from, to, eplus));

        // print sampling energies for a few representative bins
        if (doPrint && (i == 1 || i == 2 || i == 50)) {
          std::cout << "  bin " << i
                    << " [" << eLo << "," << eHi << "] GeV"
                    << " dLoE=" << dLoE
                    << " eminus=" << eminus
                    << " eplus=" << eplus
                    << " Pminus=" << scalc->P(from, to, eminus)
                    << " Pplus=" << scalc->P(from, to, eplus) << std::endl;
        }
        if (doPrint) ++debugCount;

      } // for i
    } else {
      // This is clumsy, but hopefully faster than calculating oscillation probs
      // for two dummy values.
      const Eigen::Array<T, Eigen::Dynamic, 1> Ps = calc->P(from, to, kTrueEnergyBinCenters);
      for(unsigned int i = 0; i < N; ++i) ret[i+1] = Ps[i];
    }
    
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
