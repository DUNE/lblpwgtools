#include "CAFAna/Prediction/PredictionXSecDiag.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Systs/DUNEXSecSysts.h"

#include "TH2.h"
#include "TObjString.h"
#include "TVectorD.h"

namespace ana
{
  std::vector<std::vector<double>> PredictionXSecDiag::fgCoeffs;

  //----------------------------------------------------------------------
  DUNEXSecDiagSyst::DUNEXSecDiagSyst(int i)
    : ISyst(TString::Format("xsec_diag%d", fIdx).Data(),
            TString::Format("Diagonalized xsec #%d", fIdx).Data()),
    fIdx(i)
  {
  }

  //----------------------------------------------------------------------
  const DUNEXSecDiagSyst* GetDUNEXSecDiagSyst(int i)
  {
    assert(i < 32);
    // Make sure we always give the same one back
    static std::vector<const DUNEXSecDiagSyst*> scs(32);
    if(!scs[i]) scs[i] = new DUNEXSecDiagSyst(i);
    return scs[i];
  }

  //----------------------------------------------------------------------
  SystVector<DUNEXSecDiagSyst> GetDUNEXSecDiagSysts()
  {
    SystVector<DUNEXSecDiagSyst> ret;
    for(int i = 0; i < 32; ++i) ret.push_back(GetDUNEXSecDiagSyst(i));
    return ret;
  }

  //----------------------------------------------------------------------
  PredictionXSecDiag::PredictionXSecDiag(IPrediction* p)
    : fPred(p),
      fSysts(GetDUNEXSecSysts()),
      fDiagSysts(GetDUNEXSecDiagSysts())
  {
    InitCoeffs();
  }

  //----------------------------------------------------------------------
  PredictionXSecDiag::PredictionXSecDiag(SpectrumLoaderBase& loader,
                                         const HistAxis&     axis,
                                         Cut                 cut,
                                         const SystShifts&   shift,
                                         const Var&          wei)
    : PredictionXSecDiag(new PredictionScaleComp(loader, axis, cut, GetDUNEXSecSysts(), shift, wei))
  {
  }

  //----------------------------------------------------------------------
  PredictionXSecDiag::
  PredictionXSecDiag(SpectrumLoaderBase& loaderNonswap,
                     SpectrumLoaderBase& loaderNue,
                     SpectrumLoaderBase& loaderNuTau,
                     const HistAxis&     axis,
                     Cut                 cut,
                     const SystShifts&   shift,
                     const Var&          wei)
    : PredictionXSecDiag(new PredictionScaleComp(loaderNonswap, loaderNue, loaderNuTau, axis, cut, GetDUNEXSecSysts(), shift, wei))
  {
  }

  //----------------------------------------------------------------------
  Spectrum PredictionXSecDiag::PredictComponentSyst(osc::IOscCalculator* calc,
                                                    const SystShifts& shift,
                                                    Flavors::Flavors_t flav,
                                                    Current::Current_t curr,
                                                    Sign::Sign_t sign) const
  {
    bool anySyst = false;
    for(const ISyst* s: fSysts){
      if(shift.GetShift(s) != 0) anySyst = true;
    }
    bool anyDiagSyst = false;
    for(const ISyst* s: fDiagSysts){
      if(shift.GetShift(s) != 0) anyDiagSyst = true;
    }

    if(anySyst && anyDiagSyst){
      std::cout << "Can't mix diagonalized and un-diagonalized systs "
                << "in call to PredictionScaleCompXSecDiag::PredictSyst(). "
                << shift.ShortName() << std::endl;
      abort();
    }

    // Nothing needs to be translated, just pass through
    if(!anyDiagSyst) return fPred->PredictComponentSyst(calc, shift,
                                                        flav, curr, sign);

    // We'll translate from our diagonalized shifts to the underlying "physics"
    // shifts and then pass along to our parent class.
    return fPred->PredictComponentSyst(calc, Undiagonalize(shift),
                                       flav, curr, sign);
  }

  //----------------------------------------------------------------------
  SystShifts PredictionXSecDiag::Undiagonalize(const SystShifts& s) const
  {
    double translated[32] = {0,};

    for(int i = 0; i < 32; ++i){
      const double x = s.GetShift(fDiagSysts[i]);
      if(x == 0) continue;

      for(int j = 0; j < 32; ++j){
        translated[j] += x*fgCoeffs[i][j];
      }
    }

    SystShifts ret;
    for(unsigned int i = 0; i < 32; ++i){
      ret.SetShift(fDiagSysts[i], 0);
      ret.SetShift(fSysts[i], translated[i]);
    }
    return ret;
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionXSecDiag> PredictionXSecDiag::
  LoadFrom(TDirectory* dir)
  {
    return std::make_unique<PredictionXSecDiag>(ana::LoadFrom<IPrediction>(dir).release());
  }

  //----------------------------------------------------------------------
  void PredictionXSecDiag::InitCoeffs()
  {
    if(!fgCoeffs.empty()) return;

    // TODO proper path
    TFile f("/dune/data/users/marshalc/total_covariance_XS.root");
    TH2* hcov_big = (TH2*)f.Get("xs_covariance");

    const int N = 32; // We only use systematics up to this point

    TH2F hcov("", "", N, 0, N, N, 0, N);
    for(int i = 1; i <= N; ++i)
      for(int j = 1; j <= N; ++j)
        hcov.SetBinContent(i, j, hcov_big->GetBinContent(i, j));

    TMatrixD mcov(N, N);
    for(int i = 1; i <= N; ++i){
      for(int j = 1; j <= N; ++j){
        mcov(i-1, j-1) = hcov.GetBinContent(i, j);
      }
    }

    // Trial diagonalization to fix eigenvalues
    TVectorD evals;
    TMatrixD evecs = mcov.EigenVectors(evals);
    TMatrixD evalmat(N, N);
    for(int i = 0; i < N; ++i){
      evalmat(i, i) = std::max(1e-14, evals[i]);
    }

    // Cov was real-symmetric
    TMatrixD evecs_inv(TMatrixD::kTransposed, evecs);
    mcov = evecs*evalmat*evecs_inv;

    evecs = mcov.EigenVectors(evals);

    for(int i = 0; i < N; ++i){
      fgCoeffs.push_back(std::vector<double>(N, 0));
      for(int j = 0; j < N; ++j){
        fgCoeffs.back()[j] = sqrt(evals[i])*evecs(j, i);
      }
    }
  }
}
