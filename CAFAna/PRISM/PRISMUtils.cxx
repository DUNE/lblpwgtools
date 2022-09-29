#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Analysis/AnalysisVars.h"

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

ana::Weight NDSliceCorrection(double reference_width_cm,
                              std::vector<double> const &Edges) {
  std::vector<double> Weights;

  for (size_t e_it = 0; e_it < (Edges.size() - 1); ++e_it) {
    double width = std::fabs(Edges[e_it + 1] - Edges[e_it]);
    Weights.push_back((width*1E2) / reference_width_cm);
  }

  return ana::Weight([=](const caf::StandardRecord *sr) -> double {
    double pos_x = (sr->det_x + sr->vtx_x) * 1E-2;

    for (size_t e_it = 0; e_it < (Edges.size() - 1); ++e_it) {
      if ((pos_x > Edges[e_it]) && (pos_x < Edges[e_it + 1])) {
        return Weights[e_it];
      }
    }
    return 1;
  });
}

namespace {
template <class T> std::unique_ptr<T> LoadFrom_(TDirectory *dir, const std::string &name) {
  if (!dir) {
    std::cout << "[ERROR]: Failed to LoadFrom invalid directory. " << std::endl;
    abort();
  }
  return T::LoadFrom(dir, name);
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

  f.cd();
  TDirectory *dir = gDirectory;

  std::string path = (std::string("PRISM_") + varname);
  if (!f.GetDirectory(path.c_str())) {
    std::cout << "[ERROR]: No such directory: "
              << (std::string("PRISM_") + varname)
              << " to load PRISMPrediction from." << std::endl;
    abort();
  }
  blob.PRISM = LoadFrom_<PredictionPRISM>(dir, path.c_str());


  for (size_t config = 0; config < kNPRISMConfigs; ++config) {
    bool IsNu = IsNuConfig(config);
    bool IsND = IsNDConfig(config);
    bool IsND280kA = IsND280kAConfig(config);

    size_t fd_config = 0;
    size_t IsNue = IsNueConfig(config);
    size_t IsNuTau = IsNutauConfig(config);
    if (!IsND) {
      fd_config = GetFDConfig(config);
    }

    if (IsND) { // Is ND
      path = (std::string("NDMatchInterp_ETrue") +
             (IsND280kA ? "_280kA" : "_293kA") +
             (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) { 
        blob.MatchPredInterps[config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
      }
      if (!IsND280kA) {
        path = (std::string("NDMatrixInterp_ERecETrue") +
               (IsNu ? "_nu" : "_nub"));
        if (f.GetDirectory(path.c_str())) {
          blob.NDMatrixPredInterps[config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
        }
      }
      path = (std::string("NDUnSelected_ETrue") +
             (IsND280kA ? "_280kA" : "_293kA") +
             (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.NDUnselTruePredInterps[config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
      }
      path = (std::string("NDSelected_ETrue") +
             (IsND280kA ? "_280kA" : "_293kA") +
             (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.NDSelTruePredInterps[config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
      }
    } else { // Is FD numu/nue/nutau
      if (!IsNue && !IsNuTau) {
        path = (std::string("FDMatchInterp_ETrue_numu") + (IsNu ? "_nu" : "_nub"));
        if (f.GetDirectory(path.c_str())) {
          blob.MatchPredInterps[config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
        }
      }

      path = (std::string("FDMatrixInterp_ERecETrue") +
             (IsNue ? "_nue" : (IsNuTau ? "_nutau" : "_numu") ) + (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.FDMatrixPredInterps[fd_config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
      }

      path = (std::string("FDUnSelected_ETrue") +
             (IsNue ? "_nue" : (IsNuTau ? "_nutau" : "_numu") ) + (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.FDUnselTruePredInterps[fd_config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
      }

      path = (std::string("FDSelected_ETrue") +
             (IsNue ? "_nue" : (IsNuTau ? "_nutau" : "_numu") ) + (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.FDSelTruePredInterps[fd_config] = LoadFrom_<PredictionInterp>(dir, path.c_str());
      }

      path = (std::string("FDDataPred_") + varname +
             (IsNue ? "_nue" : (IsNuTau ? "_nutau" : "_numu") ) + (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.FarDetDataPreds[fd_config] = LoadFrom_<DataPredictionNoExtrap>(dir, path.c_str());
      }

      path = (std::string("FDFakeDataBiasPred_") + varname +
             (IsNue ? "_nue" : (IsNuTau ? "_nutau" : "_numu") ) + (IsNu ? "_nu" : "_nub"));
      if (f.GetDirectory(path.c_str())) {
        blob.FarDetFakeDataBiasPreds[fd_config] = LoadFrom_<DataPredictionNoExtrap>(dir,
                                                                                    path.c_str());
      }
    } // end else
  }

  return blob;
} // namespace ana

void DumpLoadedSpectra(PRISMStateBlob const &blob){
  std::cout << "PRISMSTATE: " << std::endl;

  std::cout << "MatchPredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.MatchPredInterps[i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "NDMatrixPredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(!IsNDConfig(i)){ continue; }
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.NDMatrixPredInterps[i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "FDMatrixPredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(IsNDConfig(i)){ continue; }
    size_t fd_i = GetFDConfig(i);
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.FDMatrixPredInterps[fd_i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "NDUnselTruePredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(!IsNDConfig(i)){ continue; }
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.NDUnselTruePredInterps[i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "NDSelTruePredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(!IsNDConfig(i)){ continue; }
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.NDSelTruePredInterps[i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "FDUnselTruePredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(IsNDConfig(i)){ continue; }
    size_t fd_i = GetFDConfig(i);
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.FDUnselTruePredInterps[fd_i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "FDSelTruePredInterps: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(IsNDConfig(i)){ continue; }
    size_t fd_i = GetFDConfig(i);
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.FDSelTruePredInterps[fd_i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "FarDetDataPreds: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(IsNDConfig(i)){ continue; }
    size_t fd_i = GetFDConfig(i);
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.FarDetDataPreds[fd_i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
  std::cout << "FarDetFakeDataBiasPreds: {" << std::endl;
  for (size_t i = 0; i < kNPRISMConfigs; ++i) {
    if(IsNDConfig(i)){ continue; }
    size_t fd_i = GetFDConfig(i);
    std::cout << "\t\t" << DescribeConfig(i) << ": "
              << bool(blob.FarDetFakeDataBiasPreds[fd_i].get()) << std::endl;
  }
  std::cout << "}" << std::endl;
}

osc::IOscCalcAdjustable *
ConfigureCalc(fhicl::ParameterSet const &ps,
              osc::IOscCalcAdjustable *calc) {
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
double GetCalcValue(osc::IOscCalcAdjustable *calc,
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
    if (v == "ss2th13" || v == "alloscvars") {
      rtn_vars.push_back(&kFitSinSq2Theta13);
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

  for (auto &s : ana::GetListOfSysts()) {
    if (std::find(systnames.begin(), systnames.end(), s->ShortName()) !=
        systnames.end()) {
      los.push_back(s);
    }
  }
  return los;
}

SystShifts GetSystShifts(fhicl::ParameterSet const &ps) {
  std::vector<ISyst const *> los = ana::GetListOfSysts();

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

// Function to take a HistAxis with potentially multiple binnings
// and convert e.g. 2 binnings into 1 binning. Useful for the smearing matrix
// axes and covariance matrix axes, where each axis could be for multiple variables.
HistAxis GetMatrixAxis(const std::vector<HistAxis> &axisvec) {
  bool twoDaxis(false);
  // Not yet forseeing more than a 2D axis.
  if (axisvec.at(0).GetVars().size() == 2) twoDaxis = true;

  std::vector<std::string> LabelsVec;
  std::vector<Binning> BinsVec;
  std::vector<Var> VarsVec;
  // Loop over input axes.
  for (const auto &axis : axisvec) {
    std::string NewLabel;
    Var NewVar = axis.GetVars().at(0);
    Binning NewBins = axis.GetBinnings().at(0);
    for (const std::string &l : axis.GetLabels()) NewLabel += l + " and ";
    NewLabel.resize(NewLabel.size() -5);

    if (twoDaxis) {
      Binning binsa = axis.GetBinnings().at(0);
      Binning binsb = axis.GetBinnings().at(1);
      int n = binsa.NBins() * binsb.NBins();
      NewBins = Binning::Simple(n, 0, n);
      NewVar = Var(axis.GetVars().at(0), binsa,
                   axis.GetVars().at(1), binsb);
    } else { // 1D axis
      NewVar = axis.GetVars().at(0);
      NewBins = axis.GetBinnings().at(0);
    }

    LabelsVec.push_back(NewLabel);
    BinsVec.push_back(NewBins);
    VarsVec.push_back(NewVar);
  }

  return HistAxis(LabelsVec, BinsVec, VarsVec);
}

HistAxis GetTwoDAxis(const HistAxis &axis1, const HistAxis &axis2) {
  std::vector<std::string> axis_Labels = axis1.GetLabels();
  std::vector<Binning> axis_Bins = axis1.GetBinnings();
  std::vector<Var> axis_Vars = axis1.GetVars();
  axis_Labels.push_back(axis2.GetLabels().front());
  axis_Bins.push_back(axis2.GetBinnings().front());
  axis_Vars.push_back(axis2.GetVars().front());

  return HistAxis(axis_Labels, axis_Bins, axis_Vars);
}


} // namespace ana
