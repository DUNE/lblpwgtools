/////////////////////////////////////////////////////////////////////////////////
// NDOscCurve.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Calculate ND oscillations.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Core/NDOscCurve.h"
#include "CAFAna/Core/Binning.h"
#include "OscLib/OscCalcSterile.h"

// root
#include "TH1.h"
#include "TMath.h"

// stl
#include <cassert>
#include <map>

namespace ana {

  //----------------------------------------------------------------------
  /// Helper for constructors
  Eigen::ArrayXd ToEigenSterile(osc::IOscCalc* calc, int from, int to) {

    osc::OscCalcSterile* sterile = osc::DowncastToSterile(calc);

    double lold = sterile->GetL();
    sterile->SetL(1.0);

    const unsigned int N = kTrueLOverTrueEBins.NBins();

    // Have to allow for underflow and overflow
    Eigen::ArrayXd ret(N+2);
    ret[0] = 0; // underflow
    ret[N+1] = (from == to || to == 0) ? 1 : 0; // overflow

    for (unsigned int i = 1; i <= N; ++i) {
      const double x0 = kTrueLOverTrueEBins.Edges()[i-1];
      const double x1 = kTrueLOverTrueEBins.Edges()[i];

      const double LOverE = (x0+x1)/2;
      if (LOverE > 0) {
	//Adding Oscillation Averaging Function from docDB-15433
	double delta = x1-x0;
	double Dmsq = sterile->GetDm(4);
	double f = 2*1.27*Dmsq;
	double dLoE = 0.;
	if (f != 0) dLoE = TMath::ACos(TMath::Sin(0.5*f*delta)/(0.5*f*delta) )/f;
	double LoE_plus  = LOverE + dLoE;
	double LoE_minus = LOverE - dLoE;
	double AvProb = 0.5*(sterile->P(from,to,1.0/LoE_minus)+sterile->P(from,to,1.0/LoE_plus));
	ret[i] = AvProb;
      }
      else {
	ret[i] = (from == to) ? 1 : 0;
      }
    }

    sterile->SetL(lold);

    return ret;
  }

  //----------------------------------------------------------------------
  NDOscCurve::NDOscCurve(osc::IOscCalc* calc, int from, int to)
    : Ratio(Hist::Adopt(ToEigenSterile(calc, from, to)),
	    std::vector<Binning>(1, kTrueEnergyBins),
	    std::vector<std::string>(1, "True Energy (GeV)")),
      fFrom(from), fTo(to) {
  }

  //----------------------------------------------------------------------
  NDOscCurve::~NDOscCurve() {
  }

  //----------------------------------------------------------------------
  TH1D* NDOscCurve::ToTH1(bool title) const {

    // Could have a file temporarily open
    DontAddDirectory guard;

    TH1D* ret = Ratio::ToTH1();
    ret->GetYaxis()->SetTitle("Probability");

    if (title) {
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
