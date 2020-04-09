#include "CAFAna/Prediction/PredictionExtrap.h"

#include "OscLib/func/IOscCalculator.h"

#include "CAFAna/Extrap/IExtrap.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "Utilities/func/Stan.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH1D.h"

namespace ana
{
  //----------------------------------------------------------------------
  PredictionExtrap::PredictionExtrap(IExtrap* extrap)
    : fExtrap(extrap)
  {
      mNueSurv       = fExtrap->     NueSurvComponent().getCoreMatrix();
      mAntiNueSurv   = fExtrap-> AntiNueSurvComponent().getCoreMatrix();
      mNumuApp       = fExtrap->     NumuAppComponent().getCoreMatrix();
      mAntiNumuApp   = fExtrap-> AntiNumuAppComponent().getCoreMatrix();
      mTauFromE      = fExtrap->    TauFromEComponent().getCoreMatrix();
      mAntiTauFromE  = fExtrap->AntiTauFromEComponent().getCoreMatrix();

      mNueApp        = fExtrap->       NueAppComponent().getCoreMatrix();
      mAntiNueApp    = fExtrap->   AntiNueAppComponent().getCoreMatrix();
      mNumuSurv      = fExtrap->     NumuSurvComponent().getCoreMatrix();
      mAntiNumuSurv  = fExtrap-> AntiNumuSurvComponent().getCoreMatrix();
      mTauFromMu     = fExtrap->    TauFromMuComponent().getCoreMatrix();
      mAntiTauFromMu = fExtrap->AntiTauFromMuComponent().getCoreMatrix();

      nbinsx = fExtrap->NCComponent().GetBinnings()[0].NBins();

      vNC     = fExtrap->    NCComponent().ToEigenVectorXd();
      potNueSurv       = fExtrap->     NueSurvComponent().POT();
      potAntiNueSurv   = fExtrap-> AntiNueSurvComponent().POT();
      potNumuApp       = fExtrap->     NumuAppComponent().POT();
      potAntiNumuApp   = fExtrap-> AntiNumuAppComponent().POT();
      potTauFromE      = fExtrap->    TauFromEComponent().POT();
      potAntiTauFromE  = fExtrap->AntiTauFromEComponent().POT();

      potNueApp        = fExtrap->       NueAppComponent().POT();
      potAntiNueApp    = fExtrap->   AntiNueAppComponent().POT();
      potNumuSurv      = fExtrap->     NumuSurvComponent().POT();
      potAntiNumuSurv  = fExtrap-> AntiNumuSurvComponent().POT();
      potTauFromMu     = fExtrap->    TauFromMuComponent().POT();
      potAntiTauFromMu = fExtrap->AntiTauFromMuComponent().POT();
  }

  //----------------------------------------------------------------------
  PredictionExtrap::~PredictionExtrap()
  {
    //    delete fExtrap;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionExtrap::Predict(osc::IOscCalculator* calc) const
  {
    return PredictComponent(calc,
                            Flavors::kAll,
                            Current::kBoth,
                            Sign::kBoth);
  }

  //----------------------------------------------------------------------
  SpectrumStan PredictionExtrap::Predict(osc::IOscCalculatorStan* calc) const
  {
    return PredictComponent(calc,
                            Flavors::kAll,
                            Current::kBoth,
                            Sign::kBoth);
  }

  //----------------------------------------------------------------------
  // the actual implementation
  template <typename U, typename T>
  U PredictionExtrap::_PredictComponent(osc::_IOscCalculator<T>* calc,
                                        Flavors::Flavors_t flav,
                                        Current::Current_t curr,
                                        Sign::Sign_t sign) const
  {
    using VectorXT = Eigen::Matrix<T,Eigen::Dynamic,1>;
    VectorXT ret = VectorXT::Zero(nbinsx+2);
    double POT = fExtrap->NCComponent().POT();

    if(curr & Current::kCC){
      if(flav & Flavors::kNuEToNuE    && sign & Sign::kNu)     ret += OscillatedEigen(calc, mNueSurv,        +12, +12, POT, potNueSurv      );
      if(flav & Flavors::kNuEToNuE    && sign & Sign::kAntiNu) ret += OscillatedEigen(calc, mAntiNueSurv,    -12, -12, POT, potAntiNueSurv  );

      if(flav & Flavors::kNuEToNuMu   && sign & Sign::kNu)     ret += OscillatedEigen(calc, mNumuApp,        +12, +14, POT, potNumuApp      );
      if(flav & Flavors::kNuEToNuMu   && sign & Sign::kAntiNu) ret += OscillatedEigen(calc, mAntiNumuApp,    -12, -14, POT, potAntiNumuApp  );

      if(flav & Flavors::kNuEToNuTau  && sign & Sign::kNu)     ret += OscillatedEigen(calc, mTauFromE,       +12, +16, POT, potTauFromE     );
      if(flav & Flavors::kNuEToNuTau  && sign & Sign::kAntiNu) ret += OscillatedEigen(calc, mAntiTauFromE,   -12, -16, POT, potAntiTauFromE );

      if(flav & Flavors::kNuMuToNuE   && sign & Sign::kNu)     ret += OscillatedEigen(calc, mNueApp,         +14, +12, POT, potNueApp       );
      if(flav & Flavors::kNuMuToNuE   && sign & Sign::kAntiNu) ret += OscillatedEigen(calc, mAntiNueApp,     -14, -12, POT, potAntiNueApp   );

      if(flav & Flavors::kNuMuToNuMu  && sign & Sign::kNu)     ret += OscillatedEigen(calc, mNumuSurv,       +14, +14, POT, potNumuSurv     );
      if(flav & Flavors::kNuMuToNuMu  && sign & Sign::kAntiNu) ret += OscillatedEigen(calc, mAntiNumuSurv,   -14, -14, POT, potAntiNumuSurv );

      if(flav & Flavors::kNuMuToNuTau && sign & Sign::kNu)     ret += OscillatedEigen(calc, mTauFromMu,      +14, +16, POT, potTauFromMu    );
      if(flav & Flavors::kNuMuToNuTau && sign & Sign::kAntiNu) ret += OscillatedEigen(calc, mAntiTauFromMu,  -14, -16, POT, potAntiTauFromMu);
    }
    if(curr & Current::kNC){
      assert(flav == Flavors::kAll); // Don't know how to calculate anything else

      ret += this->vNC;
    }

    return U(ret,
             fExtrap->NCComponent().GetLabels()[0],
             fExtrap->NCComponent().GetBinnings()[0],
             fExtrap->NCComponent().POT(),
             fExtrap->NCComponent().Livetime());
  }

  //----------------------------------------------------------------------
  // just call the templated guy
  Spectrum PredictionExtrap::PredictComponent(osc::IOscCalculator* calc,
                                              Flavors::Flavors_t flav,
                                              Current::Current_t curr,
                                              Sign::Sign_t sign) const
  {
    return _PredictComponent<Spectrum>(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  // just call the templated guy
  SpectrumStan PredictionExtrap::PredictComponent(osc::IOscCalculatorStan* calc,
                                                  Flavors::Flavors_t flav,
                                                  Current::Current_t curr,
                                                  Sign::Sign_t sign) const
  {
    return _PredictComponent<SpectrumStan>(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum PredictionExtrap::ComponentCC(int from, int to) const
  {
    if(from == +12 && to == +12) return fExtrap->NueSurvComponent();
    if(from == -12 && to == -12) return fExtrap->AntiNueSurvComponent();

    if(from == +12 && to == +14) return fExtrap->NumuAppComponent();
    if(from == -12 && to == -14) return fExtrap->AntiNumuAppComponent();

    if(from == +12 && to == +16) return fExtrap->TauFromEComponent();
    if(from == -12 && to == -16) return fExtrap->AntiTauFromEComponent();

    if(from == +14 && to == +12) return fExtrap->NueAppComponent();
    if(from == -14 && to == -12) return fExtrap->AntiNueAppComponent();

    if(from == +14 && to == +14) return fExtrap->NumuSurvComponent();
    if(from == -14 && to == -14) return fExtrap->AntiNumuSurvComponent();

    if(from == +14 && to == +16) return fExtrap->TauFromMuComponent();
    if(from == -14 && to == -16) return fExtrap->AntiTauFromMuComponent();

    assert(0 && "Not reached");
    throw;//shuts up compiler error.
  }

  //----------------------------------------------------------------------
  Spectrum PredictionExtrap::ComponentNC() const
  {
    return fExtrap->NCComponent();
  }

  //----------------------------------------------------------------------
  void PredictionExtrap::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;

    dir->cd();

    TObjString("PredictionExtrap").Write("type");

    fExtrap->SaveTo(dir->mkdir("extrap"));

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionExtrap> PredictionExtrap::LoadFrom(TDirectory* dir)
  {
    assert(dir->GetDirectory("extrap"));
    IExtrap* extrap = ana::LoadFrom<IExtrap>(dir->GetDirectory("extrap")).release();

    return std::unique_ptr<PredictionExtrap>(new PredictionExtrap(extrap));
  }

  //----------------------------------------------------------------------
  template <typename T>
  Eigen::Matrix<T,Eigen::Dynamic,1> PredictionExtrap::OscillatedEigen(osc::_IOscCalculator<T> * calc,
						const Eigen::MatrixXd & m,
						int from, 
						int to, 
						double POT,
						double fPOT) const {
    return m * calc->P(from, to, kTrueEnergyBinCenters)* POT / fPOT;
  }
}
