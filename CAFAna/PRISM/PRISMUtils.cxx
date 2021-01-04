#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Cuts/TruthCuts.h"

using namespace PRISM;

namespace ana {

FVMassCorrection::FVMassCorrection() {
  double det_min_cm = -400;
  double det_max_cm = 400;
  double step_cm = 25;
  size_t NDetSteps = (det_max_cm - det_min_cm) / step_cm;

  fvmasscor = new TH1D("fvmasscor", "", NDetSteps, det_min_cm, det_max_cm);
  fvmasscor->SetDirectory(nullptr);
  double avg_step = 1E-4;
  size_t const navg_steps = step_cm / avg_step;

  fOverflow = fvmasscor->GetXaxis()->GetNbins() + 1;
  for (int bi_it = 0; bi_it < fvmasscor->GetXaxis()->GetNbins(); ++bi_it) {
    double bin_low_edge = fvmasscor->GetXaxis()->GetBinLowEdge(bi_it + 1);
    for (size_t avg_step_it = 0; avg_step_it < navg_steps; ++avg_step_it) {
      double pos_cm = bin_low_edge + (double(avg_step_it) * avg_step);
      if (!ana::IsInNDFV(pos_cm, 0, 150)) {
        continue;
      }
      fvmasscor->Fill(pos_cm, 1.0 / double(navg_steps));
    }

    if (fvmasscor->GetBinContent(bi_it + 1) > 0) {
      fvmasscor->SetBinContent(bi_it + 1,
                               1.0 / fvmasscor->GetBinContent(bi_it + 1));
    }
  }
}
double FVMassCorrection::GetWeight(double vtx_x_cm) {
  size_t bi_it = fvmasscor->FindFixBin(vtx_x_cm);
  if ((bi_it == 0) || (bi_it == fOverflow)) {
    return 0;
  }
  return fvmasscor->GetBinContent(bi_it);
}

ana::Var NDSliceCorrection(double reference_width_cm,
                           std::vector<double> const &Edges) {
  std::vector<double> Weights;

  for (size_t e_it = 0; e_it < (Edges.size() - 1); ++e_it) {
    double width = Edges[e_it + 1] - Edges[e_it];
    Weights.push_back(FD_ND_FVRatio(width*1E2) * 
                      ((width*1E2)/reference_width_cm));
  }

  return ana::Var([=](const caf::StandardRecord *sr) -> double {
    double pos_x = (sr->det_x + sr->vtx_x) * 1E-2;

    for (size_t e_it = 0; e_it < (Edges.size() - 1); ++e_it) {
      if ((pos_x > Edges[e_it]) && (pos_x < Edges[e_it + 1])) {
        return Weights[e_it];
      }
    }
    return 1;
  });
}

const ana::Var kMassCorrection([](const caf::StandardRecord *sr) -> double {
  return sr->NDMassCorrWeight;
});

namespace {
template <class T> std::unique_ptr<T> LoadFrom_(TDirectory *dir) {
  if (!dir) {
    std::cout << "[ERROR]: Failed to LoadFrom invalid directory. " << std::endl;
    abort();
  }
  return T::LoadFrom(dir);
}
} // namespace

template <class T>
std::unique_ptr<T> LoadFrom(TFile &f, std::string const &dirname) {
  PRISMOUT("Reading from " << dirname);
  TDirectory *dir = f.GetDirectory(dirname.c_str());
  if (!dir) {
    std::cout << "[ERROR]: Failed to LoadFrom directory: " << dirname
              << std::endl;
    abort();
  }
  return LoadFrom_<T>(dir);
}

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname) {

  TestConfigDefinitions();

  PRISMStateBlob blob;
  blob.Init();

  TDirectory *dir = f.GetDirectory((std::string("PRISM_") + varname).c_str());

  if (!dir) {
    std::cout << "[ERROR]: No such directory: "
              << (std::string("PRISM_") + varname)
              << " to load PRISMPrediction from." << std::endl;
    abort();
  }

  blob.PRISM = LoadFrom_<PredictionPRISM>(dir);

  for (size_t it = 0; it < kNPRISMConfigs; ++it) {
    bool IsNu = IsNuConfig(it);
    bool IsND = IsNDConfig(it);
    bool IsND280kA = IsND280kAConfig(it);

    size_t fd_it = 0;
    size_t IsNue = IsNueConfig(it);
    if (!IsND) {
      fd_it = GetFDConfig(it);
    }

    if (IsND) { // Is ND
      dir = f.GetDirectory((std::string("NDMatchInterp_ETrue") +
                            (IsND280kA ? "_280kA" : "_293kA") +
                            (IsNu ? "_nu" : "_nub"))
                               .c_str());
      if (dir) {
        blob.MatchPredInterps[it] = LoadFrom_<PredictionInterp>(dir);
      }

      dir = f.GetDirectory((std::string("NDSelectedInterp_") +
                            (IsND280kA ? "_280kA" : "_293kA") + varname +
                            (IsNu ? "_nu" : "_nub"))
                               .c_str());
      if (dir) {
        blob.SelPredInterps[it] = LoadFrom_<PredictionInterp>(dir);
      }

      if (!IsND280kA) {
        dir = f.GetDirectory((std::string("NDMatrixInterp_ERecETrue") + 
                              (IsNu ? "_nu" : "_nub"))
                                 .c_str());
        if (dir) {
          blob.NDMatrixPredInterps[it] = LoadFrom_<PredictionInterp>(dir);
        }

        dir = f.GetDirectory((std::string("NDUnSelected_ETrue") + 
                             (IsNu ? "_nu" : "_nub")).c_str());
        if (dir) {
          blob.NDUnselTruePredInterps[it] = LoadFrom_<PredictionInterp>(dir);
        }
       
        dir = f.GetDirectory((std::string("NDSelected_ETrue") +
                             (IsNu ? "_nu" : "_nub")).c_str());
        if (dir) {
          blob.NDSelTruePredInterps[it] = LoadFrom_<PredictionInterp>(dir);
        }
      }
    } else { // Is FD
      if (!IsNue) {
        dir = f.GetDirectory(
            (std::string("FDMatchInterp_ETrue_numu") + (IsNu ? "_nu" : "_nub"))
                .c_str());
        if (dir) {
          blob.MatchPredInterps[it] = LoadFrom_<PredictionInterp>(dir);
        }
      }

      dir =
          f.GetDirectory((std::string("FDInterp_") + varname +
                          (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"))
                             .c_str());
      if (dir) {
        blob.FarDetPredInterps[fd_it] = LoadFrom_<PredictionInterp>(dir);
      }

      dir =
          f.GetDirectory((std::string("FDMatrixInterp_ERecETrue") +
                         (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"))
                             .c_str());
      if (dir) {
        blob.FDMatrixPredInterps[it] = LoadFrom_<PredictionInterp>(dir);
      }

      dir = 
          f.GetDirectory((std::string("FDUnSelected_ETrue") + 
                         (IsNu ? "_nu" : "_nub")).c_str());
      if (dir) {
        blob.FDUnselTruePredInterps[fd_it] = LoadFrom_<PredictionInterp>(dir);
      }

      dir = 
          f.GetDirectory((std::string("FDSelected_ETrue") +
                         (IsNu ? "_nu" : "_nub")).c_str());
      if (dir) {
        blob.FDSelTruePredInterps[fd_it] = LoadFrom_<PredictionInterp>(dir);
      } 

      dir =
          f.GetDirectory((std::string("FDDataNonSwap_") + varname +
                          (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"))
                             .c_str());
      if (dir) {
        blob.FarDetData_nonswap[fd_it] = LoadFrom_<OscillatableSpectrum>(dir);
      }

      dir =
          f.GetDirectory((std::string("FDDataNueSwap_") + varname +
                          (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"))
                             .c_str());
      if (dir) {
        blob.FarDetData_nueswap[fd_it] = LoadFrom_<OscillatableSpectrum>(dir);
      }

      dir =
          f.GetDirectory((std::string("FDSelectedInterp_") + varname +
                          (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"))
                             .c_str());
      if (dir) {
        blob.SelPredInterps[it] = LoadFrom_<PredictionInterp>(dir);
      }
    }
  }

  blob.NDFluxPred_293kA_nu =
      LoadFrom<PredictionInterp>(f.GetDirectory("NDFluxPred_293kA_nu"));
  blob.NDFluxPred_293kA_nub =
      LoadFrom<PredictionInterp>(f.GetDirectory("NDFluxPred_293kA_nub"));
  blob.NDFluxPred_280kA_nu =
      LoadFrom<PredictionInterp>(f.GetDirectory("NDFluxPred_280kA_nu"));
  blob.NDFluxPred_280kA_nub =
      LoadFrom<PredictionInterp>(f.GetDirectory("NDFluxPred_280kA_nub"));
  blob.FDFluxPred_293kA_nu =
      LoadFrom<PredictionInterp>(f.GetDirectory("FDFluxPred_293kA_nu"));
  blob.FDFluxPred_293kA_nub =
      LoadFrom<PredictionInterp>(f.GetDirectory("FDFluxPred_293kA_nub"));

#ifdef PRISMDEBUG
  /*std::cout << "PRISMSTATE: " << std::endl;
  std::cout << "\tMatchPredInterps: {\n";
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.MatchPredInterps[i].get()) << std::endl;
  }
  std::cout << "\n\t}\n\tSelPredInterps: {\n";
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.SelPredInterps[i].get()) << std::endl;
  }
  std::cout << "\n\t}\n\tFarDetPredInterps: {\n";
  for (size_t i = 0; i < kNPRISMFDConfigs; ++i) {
    std::cout << "\t\t" << DescribeFDConfig(i) << ": "
              << bool(blob.FarDetPredInterps[i].get()) << std::endl;
  }
  std::cout << "\n\t}\n\tFarDetData_nonswap: {\n";
  for (size_t i = 0; i < kNPRISMFDConfigs; ++i) {
    std::cout << "\t\t" << DescribeFDConfig(i) << ": "
              << bool(blob.FarDetData_nonswap[i].get()) << std::endl;
  }
  std::cout << "\n\t}\n\tFarDetData_nueswap: {\n";
  for (size_t i = 0; i < kNPRISMFDConfigs; ++i) {
    std::cout << "\t\t" << DescribeFDConfig(i) << ": "
              << bool(blob.FarDetData_nueswap[i].get()) << std::endl;
  }
  std::cout << "\n\t}" << std::endl;*/
#endif

  return blob;
} // namespace ana

osc::IOscCalculatorAdjustable *
ConfigureCalc(fhicl::ParameterSet const &ps,
              osc::IOscCalculatorAdjustable *calc) {
  static std::set<std::string> keys{"th13",   "dmsq32", "ssth23", "deltapi",
                                    "dmsq21", "ssth12", "rho"};

  for (std::string const &k : ps.get_names()) {
    if (!keys.count(k)) {
      std::cout << "[ERROR]: Unexpected key in oscillation configuration: " << k
                << std::endl;
      abort();
    }
  }

  if (!calc) {
    calc = NuFitOscCalc(1);
  }

  if (ps.has_key("rho")) {
    calc->SetRho(ps.get<double>("rho"));
  }
  if (ps.has_key("dmsq21")) {
    calc->SetDmsq21(ps.get<double>("dmsq21"));
  }
  if (ps.has_key("dmsq32")) {
    calc->SetDmsq32(ps.get<double>("dmsq32"));
  }
  if (ps.has_key("th13")) {
    calc->SetTh13(ps.get<double>("th13"));
  }
  if (ps.has_key("ssth12")) {
    calc->SetTh12(asin(sqrt(ps.get<double>("ssth12"))));
  }
  if (ps.has_key("ssth23")) {
    calc->SetTh23(asin(sqrt(ps.get<double>("ssth23"))));
  }
  if (ps.has_key("deltapi")) {
    calc->SetdCP(ps.get<double>("deltapi") * M_PI);
  }

  return calc;
}
double GetCalcValue(osc::IOscCalculatorAdjustable *calc,
                    std::string paramname) {
  if (!calc) {
    return 0;
  }

  if (paramname == "rho") {
    return calc->GetRho();
  }
  if (paramname == "dmsq21") {
    return calc->GetDmsq21();
  }
  if (paramname == "dmsq32") {
    return calc->GetDmsq32();
  }
  if (paramname == "th13") {
    return calc->GetTh13();
  }
  if (paramname == "ssth12") {
    return pow(sin(calc->GetTh12()), 2);
  }
  if (paramname == "ssth23") {
    return pow(sin(calc->GetTh23()), 2);
  }
  if (paramname == "deltapi") {
    return calc->GetdCP() / M_PI;
  }
  std::cout << "[ERROR]: Invalid param name: " << paramname << std::endl;
  abort();
}

std::vector<const IFitVar *>
GetOscVars(std::vector<std::string> const &osc_vars, int hie, int oct) {
  std::vector<const IFitVar *> rtn_vars;

  for (auto &v : osc_vars) {
    if (v == "th13" || v == "alloscvars") {
      rtn_vars.push_back(&kFitTheta13);
    }
    // Deal with bounded dmsq32
    if (v == "dmsq32" || v == "alloscvars") {
      if (hie == -1)
        rtn_vars.push_back(&kFitDmSq32IHScaled);
      else if (hie == 1)
        rtn_vars.push_back(&kFitDmSq32NHScaled);
      else
        rtn_vars.push_back(&kFitDmSq32Scaled);
    }

    // Deal with octant boundaries
    if (v == "ssth23" || v == "alloscvars") {
      if (oct == -1)
        rtn_vars.push_back(&kFitSinSqTheta23LowerOctant);
      else if (oct == 1)
        rtn_vars.push_back(&kFitSinSqTheta23UpperOctant);
      else
        rtn_vars.push_back(&kFitSinSqTheta23);
    }
    if (v == "deltapi" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDeltaInPiUnits);
    }

    // Add back in the 21 parameters
    if (v == "dmsq21" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDmSq21Scaled);
    }
    if (v == "ssth12" || v == "alloscvars") {
      rtn_vars.push_back(&kFitSinSq2Theta12);
    }
    // Rho rho rho your boat...
    if (v == "rho" || v == "alloscvars") {
      rtn_vars.push_back(&kFitRho);
    }
  }
  return rtn_vars;
}

void ScrubOscVars(std::vector<const IFitVar *> &oscvars,
                  std::vector<std::string> const &names_to_scrub) {
  for (auto &v : names_to_scrub) {
    if (v == "th13") {
      oscvars.erase(std::remove(oscvars.begin(), oscvars.end(), &kFitTheta13),
                    oscvars.end());
    }
    // Deal with bounded dmsq32
    if (v == "dmsq32") {
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitDmSq32IHScaled),
          oscvars.end());
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitDmSq32NHScaled),
          oscvars.end());
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitDmSq32Scaled),
          oscvars.end());
    }

    // Deal with octant boundaries
    if (v == "ssth23") {
      oscvars.erase(std::remove(oscvars.begin(), oscvars.end(),
                                &kFitSinSqTheta23LowerOctant),
                    oscvars.end());
      oscvars.erase(std::remove(oscvars.begin(), oscvars.end(),
                                &kFitSinSqTheta23UpperOctant),
                    oscvars.end());
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitSinSqTheta23),
          oscvars.end());
    }
    if (v == "deltapi") {
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitDeltaInPiUnits),
          oscvars.end());
    }

    // Add back in the 21 parameters
    if (v == "dmsq21") {
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitDmSq21Scaled),
          oscvars.end());
    }
    if (v == "ssth12") {
      oscvars.erase(
          std::remove(oscvars.begin(), oscvars.end(), &kFitSinSq2Theta12),
          oscvars.end());
    }
    // Rho rho rho your boat...
    if (v == "rho") {
      oscvars.erase(std::remove(oscvars.begin(), oscvars.end(), &kFitRho),
                    oscvars.end());
    }
  }
}

std::vector<ana::ISyst const *>
GetListOfSysts(std::vector<std::string> const &systnames) {
  std::vector<ISyst const *> los;

  for (auto &s : ::GetListOfSysts()) {
    if (std::find(systnames.begin(), systnames.end(), s->ShortName()) !=
        systnames.end()) {
      los.push_back(s);
    }
  }
  return los;
}

SystShifts GetSystShifts(fhicl::ParameterSet const &ps) {
  std::vector<ISyst const *> los = ::GetListOfSysts();

  SystShifts shift;

  for (std::pair<std::string, double> const &s :
       ps.get<std::vector<std::pair<std::string, double>>>("shifts", {})) {
    std::vector<ISyst const *> reduced_los = los;
    KeepSysts(reduced_los, {
                               s.first,
                           });
    if (!reduced_los.size()) {
      std::cout << "[ERROR]: Unknown systematic " << s.first << std::endl;
      abort();
    }
    shift.SetShift(reduced_los[0], s.second);
  }
  return shift;
}

//-----------------------------------------------------
// Class for ND and FD detector extrapolation matrices:
// ----------------------------------------------------

NDFD_Matrix::NDFD_Matrix(std::unique_ptr<PredictionInterp> ND, 
                         std::unique_ptr<PredictionInterp> FD, 
                         double pot) : fPOT(pot), fPRISMExtrap(nullptr) {
  fMatrixND = std::move(ND);
  fMatrixFD = std::move(FD);
}

//-----------------------------------------------------

TH2 * NDFD_Matrix::GetNDMatrix() const {
  return hMatrixND.get();
}

//-----------------------------------------------------

TH2 * NDFD_Matrix::GetFDMatrix() const {
  return hMatrixFD.get();
}

//-----------------------------------------------------

TH1 * NDFD_Matrix::GetPRISMExtrap() const {
  return fPRISMExtrap.get();
}

//-----------------------------------------------------

void NDFD_Matrix::NormaliseETrue(osc::IOscCalculator *calc,
                                 ana::SystShifts shift,
                                 Flavors::Flavors_t NDflav,
                                 Flavors::Flavors_t FDflav,
                                 Current::Current_t curr,
                                 Sign::Sign_t NDsign,
                                 Sign::Sign_t FDsign,
                                 std::vector<double> NDefficiency, 
                                 std::vector<double> FDefficiency) const {
 
  auto sMatrixND = fMatrixND->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  hMatrixND = std::unique_ptr<TH2>(static_cast<TH2*>(sMatrixND.ToTH2(1)));
  
  auto sMatrixFD = fMatrixFD->PredictComponentSyst(calc, shift, FDflav, curr, FDsign);
  hMatrixFD = std::unique_ptr<TH2>(static_cast<TH2*>(sMatrixFD.ToTH2(1)));

  if (!hMatrixND) std::cout << "fMatrixND ERROR" << std::endl;
  //std::unique_ptr<TH2>* NDhist = &hMatrixND;
  //gFile->WriteTObject(NDhist->get(), "NDmatrix_noNorm"); 
 
  std::pair<std::unique_ptr<TH2>*, std::vector<double>> NDpair (&hMatrixND, NDefficiency);
  std::pair<std::unique_ptr<TH2>*, std::vector<double>> FDpair (&hMatrixFD, FDefficiency);
  
  std::vector<std::pair<std::unique_ptr<TH2>*, std::vector<double>>> matrix_pair = 
      {NDpair, FDpair};   

  for (auto &mat : matrix_pair) {
    for (int col_it = 1; col_it <= mat.first->get()->GetXaxis()->GetNbins(); col_it++) {

      std::unique_ptr<TH1D> projY = std::unique_ptr<TH1D>(
                                     mat.first->get()->ProjectionY("_projY", col_it, col_it, "e"));
      // Normalise integral of true bin to 1
      projY->Scale(1 / projY->Integral()); // * mat.second.at(i - 1) 
      projY->Scale(mat.second.at(col_it - 1));
      //std::cout << "True bin integral with eff = " << projY->Integral() << std::endl;
      for (int row_it = 1; row_it <= mat.first->get()->GetYaxis()->GetNbins(); row_it++) {
        mat.first->get()->SetBinContent(col_it, row_it, projY->GetBinContent(row_it));
        mat.first->get()->SetBinError(col_it, row_it, projY->GetBinError(row_it));
      }
    }
  }
}

//-----------------------------------------------------

void NDFD_Matrix::ExtrapolateNDtoFD(Spectrum NDPRISMLCComp) const {
  
  std::unique_ptr<TH2>* NDhist = &hMatrixND;
  std::unique_ptr<TH2>* FDhist = &hMatrixFD;

  //gFile->WriteTObject(NDhist->get(), "NDMatrix");
  //gFile->WriteTObject(FDhist->get(), "FDMatrix");

  TH1D *PRISMND = NDPRISMLCComp.ToTH1(1);

  Eigen::MatrixXd NDmat = GetEigenMatrix(NDhist->get(), 
                                         NDhist->get()->GetYaxis()->GetNbins(),
                                         NDhist->get()->GetXaxis()->GetNbins());
  Eigen::MatrixXd FDmat = GetEigenMatrix(FDhist->get(),
                                         FDhist->get()->GetYaxis()->GetNbins(),
                                         FDhist->get()->GetXaxis()->GetNbins());

  const int NTrueBins = NDmat.cols();
  const int NRecoBins = NDmat.rows();
  
  Eigen::VectorXd NDERec = GetEigenFlatVector(PRISMND);
  
  // Use T-reg to calculate ETrue(ND)
  const double reg = 0.15;
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NTrueBins, NTrueBins);
  for (int row_it = 0; row_it < NTrueBins - 1; row_it++) {
    RegMatrix(row_it, row_it) = reg;
    RegMatrix(row_it, row_it + 1) = -reg;
  }
  RegMatrix(NTrueBins - 1, NTrueBins - 1) = reg;
  
  Eigen::VectorXd NDETrue = ((NDmat.transpose() * NDmat +
                             RegMatrix.transpose() * RegMatrix).inverse() *
                             NDmat.transpose() * NDERec);

  // Output true hist
  std::unique_ptr<TH1> NDETrue_h = std::unique_ptr<TH1>(static_cast<TH1*>(PRISMND->Clone()));
  FillHistFromEigenVector(NDETrue_h.get(), NDETrue);
  NDETrue_h.get()->Scale(1, "width");
  gFile->WriteTObject(NDETrue_h.get(), "NDETrueTik_h");

  Eigen::VectorXd FDERec = FDmat * NDETrue;
  // Keep same binning for extrapolated prediction
  fPRISMExtrap = std::unique_ptr<TH1>(static_cast<TH1*>(PRISMND->Clone()));
  fPRISMExtrap = std::unique_ptr<TH1>(static_cast<TH1*>(HistCache::Copy(PRISMND))); 
 
  FillHistFromEigenVector(fPRISMExtrap.get(), FDERec);
  HistCache::Delete(PRISMND);
} 

//----------------------------------------------------
// Class for ND and FD MC based efficiency correction:
//----------------------------------------------------
MCEffCorrection::MCEffCorrection(std::unique_ptr<PredictionInterp> NDunsel,
                                 std::unique_ptr<PredictionInterp> NDsel,
                                 std::unique_ptr<PredictionInterp> FDunsel,
                                 std::unique_ptr<PredictionInterp> FDsel) {
  fNDunselected = std::move(NDunsel);
  fNDselected = std::move(NDsel);
  fFDunselected = std::move(FDunsel);
  fFDselected = std::move(FDsel);
}

//----------------------------------------------------

void MCEffCorrection::CalcEfficiency(osc::IOscCalculator *calc,
                                     ana::SystShifts shift,
                                     Flavors::Flavors_t NDflav,
                                     Flavors::Flavors_t FDflav,
                                     Current::Current_t curr,
                                     Sign::Sign_t NDsign, 
                                     Sign::Sign_t FDsign) const {

  shift = kNoShift;

  auto sNDunselected = fNDunselected->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  TH2D *hNDunselected = static_cast<TH2D*>(sNDunselected.ToTH2(1));
  
  auto sNDselected = fNDselected->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  TH2D *hNDselected = static_cast<TH2D*>(sNDselected.ToTH2(1));
 
  auto sFDunselected = fFDunselected->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  TH1D *hFDunselected = sFDunselected.ToTH1(1);

  auto sFDselected = fFDselected->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  TH1D *hFDselected = sFDselected.ToTH1(1);

  //gFile->WriteTObject(hNDselected.get(), "NDSelected");
  //gFile->WriteTObject(hNDunselected.get(), "NDUnselected");
  //gFile->WriteTObject(hFDselected.get(), "FDSelected");
  //gFile->WriteTObject(hFDunselected.get(), "FDUnselected");

  // Calculate ND efficiency
  // efficiency fluctuates slightly with OA position
  // take average over for 20 OA positions
  for (int i = 0; i < hNDselected->GetXaxis()->GetNbins(); i++) { // energy axis
    double NDbin_eff;
    double sum(0);
    int OAbinstart = 30; // Lots of statistical noise further OA
    for (int j = OAbinstart; j < hNDselected->GetYaxis()->GetNbins(); j++) { // OA pos axis 
      NDbin_eff = hNDselected->GetBinContent(i + 1, j + 1) / // changed from NDselected
                  hNDunselected->GetBinContent(i + 1, j + 1);
      sum += NDbin_eff;
    }
    NDefficiency.push_back(sum / 
                           (hNDselected->GetYaxis()->GetNbins() - OAbinstart));
  }

  // Calculate FD efficiency
  for (int i = 0; i < hFDselected->GetXaxis()->GetNbins(); i++) {
    double FDbin_eff = hFDselected->GetBinContent(i + 1) / // changed from FDselected
                       hFDunselected->GetBinContent(i + 1);
    FDefficiency.push_back(FDbin_eff);
  }

  HistCache::Delete(hNDunselected);
  HistCache::Delete(hNDselected);
  HistCache::Delete(hFDunselected);
  HistCache::Delete(hFDselected);

}


} // namespace ana
