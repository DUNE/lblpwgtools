#include "StandardRecord/SRProxy.h"

#include "CAFAna/Core/ModeConversionUtilities.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/XSecSystList.h"

#include "CAFAna/PRISM/Cuts.h"
#include "CAFAna/PRISM/Weights.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <set>
#include <sstream>

using namespace PRISM;
using namespace ana;

bool multi_file = false;

template <class T>
bool SetBranchChecked(TTree *tr, const std::string &bname, T *dest) {
  static std::set<std::string> alreadyWarned;

  if (tr->FindBranch(bname.c_str())) {
    tr->SetBranchAddress(bname.c_str(), dest);
    return true;
  } else {
    if(!alreadyWarned.count(bname)){
      alreadyWarned.insert(bname);
      std::cout << "Warning: Branch '" << bname
                << "' not found, field will not be filled" << std::endl;
    }
  }
  return false;
}

//Essentially a copy of SpectrumLoader::HandleFile
void ProcessFile(TFile &f, std::function<void(const caf::SRProxy& sr)> ev_processor){

  assert(!f.IsZombie());
  TTree* tr = 0;
  // In files with both "caf" and "cafTree", "cafTree" is the correct
  // version. "caf" is ROOT's temporary save while the file is being produced
  // and may be incomplete.
  tr = (TTree*)f.Get("cafTree");
  if (!tr){
    // Old (MCC10 era) files only have "caf"
    tr = (TTree*)f.Get("caf");
    if(tr) std::cout << "Warning, didn't find 'cafTree' in " << f.GetName() << " but did find 'caf' - using that" << std::endl;
  }
  assert(tr);

  caf::StandardRecord sr;

  SetBranchChecked(tr, "Ev_reco", &sr.Ev_reco);
  SetBranchChecked(tr, "Ev_reco_nue", &sr.Ev_reco_nue);
  SetBranchChecked(tr, "Ev_reco_numu", &sr.Ev_reco_numu);
  SetBranchChecked(tr, "Elep_reco", &sr.Elep_reco);
  SetBranchChecked(tr, "theta_reco", &sr.theta_reco);
  SetBranchChecked(tr, "mvaresult", &sr.mvaresult);
  SetBranchChecked(tr, "mvanue", &sr.mvanue);
  SetBranchChecked(tr, "mvanumu", &sr.mvanumu);
  SetBranchChecked(tr, "cvnnue", &sr.cvnnue);
  SetBranchChecked(tr, "cvnnumu", &sr.cvnnumu);
  SetBranchChecked(tr, "numu_pid", &sr.numu_pid);
  SetBranchChecked(tr, "nue_pid", &sr.nue_pid);
  SetBranchChecked(tr, "reco_q", &sr.reco_q);
  SetBranchChecked(tr, "RecoLepEnNue", &sr.RecoLepEnNue);
  SetBranchChecked(tr, "RecoHadEnNue", &sr.RecoHadEnNue);
  SetBranchChecked(tr, "RecoLepEnNumu", &sr.RecoLepEnNumu);
  SetBranchChecked(tr, "RecoHadEnNumu", &sr.RecoHadEnNumu);
  // ND pseudo-reconstruction flags
  SetBranchChecked(tr, "reco_numu", &sr.reco_numu);
  SetBranchChecked(tr, "reco_nue", &sr.reco_nue);
  SetBranchChecked(tr, "reco_nc", &sr.reco_nc);
  // CW: add variables that Chris (M) wants for ND selections
  SetBranchChecked(tr, "muon_exit", &sr.muon_exit);
  SetBranchChecked(tr, "muon_contained", &sr.muon_contained);
  SetBranchChecked(tr, "muon_ecal", &sr.muon_ecal);
  SetBranchChecked(tr, "muon_tracker", &sr.muon_tracker);
  SetBranchChecked(tr, "Ehad_veto", &sr.Ehad_veto);
  SetBranchChecked(tr, "Ev", &sr.Ev);
  SetBranchChecked(tr, "Elep", &sr.Elep);
  //    SetBranchChecked(tr, "ccnc", &sr.ccnc);
  SetBranchChecked(tr, "isCC", &sr.isCC);
  //    SetBranchChecked(tr, "beamPdg", &sr.beamPdg);
  //    SetBranchChecked(tr, "neu", &sr.neu);
  SetBranchChecked(tr, "nuPDG", &sr.nuPDG);
  SetBranchChecked(tr, "nuPDGunosc", &sr.nuPDGunosc);
  SetBranchChecked(tr, "LepPDG", &sr.LepPDG);
  SetBranchChecked(tr, "mode", &sr.mode);
  SetBranchChecked(tr, "nP", &sr.nP);
  SetBranchChecked(tr, "nN", &sr.nN);
  SetBranchChecked(tr, "nipi0", &sr.nipi0);
  SetBranchChecked(tr, "nipip", &sr.nipip);
  SetBranchChecked(tr, "nipim", &sr.nipim);
  SetBranchChecked(tr, "nik0", &sr.nik0);
  SetBranchChecked(tr, "nikp", &sr.nikp);
  SetBranchChecked(tr, "nikm", &sr.nikm);
  SetBranchChecked(tr, "niem", &sr.niem);
  SetBranchChecked(tr, "nNucleus", &sr.nNucleus);
  SetBranchChecked(tr, "Q2", &sr.Q2);
  SetBranchChecked(tr, "W", &sr.W);
  SetBranchChecked(tr, "Y", &sr.Y);
  SetBranchChecked(tr, "X", &sr.X);
  //    SetBranchChecked(tr, "cc", &sr.cc);
  SetBranchChecked(tr, "NuMomX", &sr.NuMomX);
  SetBranchChecked(tr, "NuMomY", &sr.NuMomY);
  SetBranchChecked(tr, "NuMomZ", &sr.NuMomZ);
  SetBranchChecked(tr, "LepMomX", &sr.LepMomX);
  SetBranchChecked(tr, "LepMomY", &sr.LepMomY);
  SetBranchChecked(tr, "LepMomZ", &sr.LepMomZ);
  SetBranchChecked(tr, "LepE", &sr.LepE);
  SetBranchChecked(tr, "LepNuAngle", &sr.LepNuAngle);

  // Numu track containment flag
  SetBranchChecked(tr, "LongestTrackContNumu", &sr.LongestTrackContNumu);

  SetBranchChecked(tr, "vtx_x", &sr.vtx_x);
  SetBranchChecked(tr, "vtx_y", &sr.vtx_y);
  SetBranchChecked(tr, "vtx_z", &sr.vtx_z);
  SetBranchChecked(tr, "det_x", &sr.det_x);

  SetBranchChecked(tr, "eP", &sr.eP);
  SetBranchChecked(tr, "eN", &sr.eN);
  SetBranchChecked(tr, "ePip", &sr.ePip);
  SetBranchChecked(tr, "ePim", &sr.ePim);
  SetBranchChecked(tr, "ePi0", &sr.ePi0);
  SetBranchChecked(tr, "eOther", &sr.eOther);
  SetBranchChecked(tr, "eRecoP", &sr.eRecoP);
  SetBranchChecked(tr, "eRecoN", &sr.eRecoN);
  SetBranchChecked(tr, "eRecoPip", &sr.eRecoPip);
  SetBranchChecked(tr, "eRecoPim", &sr.eRecoPim);
  SetBranchChecked(tr, "eRecoPi0", &sr.eRecoPi0);
  SetBranchChecked(tr, "eRecoOther", &sr.eRecoOther);

  SetBranchChecked(tr, "eDepP", &sr.eDepP);
  SetBranchChecked(tr, "eDepN", &sr.eDepN);
  SetBranchChecked(tr, "eDepPip", &sr.eDepPip);
  SetBranchChecked(tr, "eDepPim", &sr.eDepPim);
  SetBranchChecked(tr, "eDepPi0", &sr.eDepPi0);
  SetBranchChecked(tr, "eDepOther", &sr.eDepOther);

  SetBranchChecked(tr, "run", &sr.run);
  SetBranchChecked(tr, "isFD", &sr.isFD);
  SetBranchChecked(tr, "isFHC", &sr.isFHC);

  SetBranchChecked(tr, "sigma_Ev_reco", &sr.sigma_Ev_reco);
  SetBranchChecked(tr, "sigma_Elep_reco", &sr.sigma_Elep_reco);
  SetBranchChecked(tr, "sigma_numu_pid", &sr.sigma_numu_pid);
  SetBranchChecked(tr, "sigma_nue_pid", &sr.sigma_nue_pid);

  // XSec uncertainties and CVs
  std::vector<std::array<double, caf::kMaxSystUniverses>> XSSyst_tmp;
  std::vector<double> XSSyst_cv_tmp;
  std::vector<int> XSSyst_size_tmp;

  std::vector<std::string> const XSSyst_names = GetAllXSecSystNames();
  XSSyst_tmp.resize(XSSyst_names.size());
  XSSyst_cv_tmp.resize(XSSyst_names.size());
  XSSyst_size_tmp.resize(XSSyst_names.size());

  sr.xsSyst_wgt.resize(XSSyst_names.size());

  for (unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {

    sr.xsSyst_wgt[syst_it].resize(caf::kMaxSystUniverses);

    if (!SetBranchChecked(tr, "wgt_" + XSSyst_names[syst_it],
                          &XSSyst_tmp[syst_it])) {
      std::fill_n(XSSyst_tmp[syst_it].begin(), caf::kMaxSystUniverses, 1);
      XSSyst_cv_tmp[syst_it] = 1;
      XSSyst_size_tmp[syst_it] = 1;
      continue;
    }

    SetBranchChecked(tr, XSSyst_names[syst_it] + "_nshifts",
                     &XSSyst_size_tmp[syst_it]);
    SetBranchChecked(tr, XSSyst_names[syst_it] + "_cvwgt",
                     &XSSyst_cv_tmp[syst_it]);
  }

  TTree *potFriend = 0;
  f.GetObject("OffAxisWeightFriend", potFriend);
  if (potFriend) {
    tr->AddFriend(potFriend);
    SetBranchChecked(potFriend, "perPOT", &sr.perPOTWeight);
    SetBranchChecked(potFriend, "perFile", &sr.perFileWeight);
    SetBranchChecked(potFriend, "massCorr", &sr.NDMassCorrWeight);
    SetBranchChecked(potFriend, "specRunWght", &sr.SpecialRunWeight);
    SetBranchChecked(potFriend, "specRunId", &sr.SpecialHCRunId);
 
    std::cout << "[INFO]: Found Off axis weight friend tree "
                 "in input file, hooking up!"
              << std::endl;
  } else {
    std::cout << "[WARNING]: Off axis weightings NOT being set." << std::endl;
    sr.perPOTWeight = 1;
    sr.perFileWeight = 1;
    sr.NDMassCorrWeight = 1;
    sr.SpecialRunWeight = 1;
    sr.SpecialHCRunId = 293;
  }

  TFile fin(std::getenv("PRISM_TOTAL_OFFAXIS_EXPOSURE_INPUT"));
  assert(fin.IsOpen());

  std::map<int, TH1D *> FileExposures;
  
  for (int SpecRunID_local : {-293, -280, 280, 293}) {
    std::stringstream ss("");
    ss << ((SpecRunID_local < 0) ? "m" : "") << SpecRunID_local;
    fin.GetObject(("FileExposure_" + ss.str()).c_str(), FileExposures[SpecRunID_local]);
    if (!FileExposures[SpecRunID_local]) abort(); 
  } 

  Long64_t Nentries = tr->GetEntries();

  Long64_t every = Nentries/100;

  for (Long64_t n = 0; n < Nentries; ++n) {
    tr->GetEntry(n);

    if(n && !(n % every)){
      std::cout << "\rProcessed: " << (n*100/Nentries) << " %" << std::flush;
    }

    if (!sr.isFD) sr.abspos_x = -std::abs(sr.det_x + sr.vtx_x);

    // Change the perFile weight if we have multiple ND files
    if (!sr.isFD && multi_file) { 
      double nfiles = FileExposures[sr.SpecialHCRunId]->GetBinContent(
          FileExposures[sr.SpecialHCRunId]->FindFixBin(sr.abspos_x));
      sr.perFileWeight = 1 / nfiles;
    }

    // Set GENIE_ScatteringMode and eRec_FromDep
    if (sr.isFD) {
      sr.eRec_FromDep = sr.eDepP + sr.eDepN + sr.eDepPip + sr.eDepPim +
                        sr.eDepPi0 + sr.eDepOther + sr.LepE;

      sr.GENIE_ScatteringMode = ana::GetGENIEModeFromSimbMode(sr.mode);
    } else {
      sr.eRec_FromDep = sr.eRecoP + sr.eRecoN + sr.eRecoPip + sr.eRecoPim +
                        sr.eRecoPi0 + sr.eRecoOther + sr.LepE;

      sr.GENIE_ScatteringMode = sr.mode;
    }

    // Common EvisReco variable for ND and FD
    if (sr.isFD) {
      if (sr.nuPDG == 14 || sr.nuPDG == -14) { // FD numu event 
        sr.RecoLepEnNue = sr.RecoLepEnNumu;
      } else if (sr.nuPDG == 12 || sr.nuPDG == -12) {
        sr.RecoLepEnNue = sr.RecoLepEnNue;
      }
      //sr.HadEVisReco_ND = sr.eRecoP + sr.eRecoPip + sr.eRecoPim + sr.eRecoPi0;
      sr.HadEVisReco_ND = sr.eDepP + sr.eDepPip + sr.eDepPim +
                         sr.eDepPi0 + sr.eDepOther; 
    } else {
      sr.RecoLepEnNue = sr.Elep_reco;
      sr.HadEVisReco_ND = sr.eRecoP + sr.eRecoPip + sr.eRecoPim + // no neutron
                         sr.eRecoPi0 + sr.eRecoOther;
    }
    sr.EVisReco_ND = sr.HadEVisReco_ND + sr.RecoLepEnNue;

    double eother = 0;
    if (std::isnormal(sr.eOther)) {
      eother = sr.eOther;
    }
    sr.eRecProxy = sr.LepE + sr.eP + sr.ePip + sr.ePim + sr.ePi0 +
                   0.135 * sr.nipi0 + eother;
    // If it is a NC event remove LepE, this is the neutrino evergy and is 
    // not detected
    if (!sr.isCC) {
      sr.eRecProxy = sr.eRecProxy - sr.LepE;
    }

    // Variable for true hadronic energy
    sr.HadE = sr.eP + sr.ePip + sr.ePim + sr.ePi0 + (0.135 * sr.nipi0) + eother;

    // Sum of the true charged pion KE
    sr.ePipm = sr.ePip + sr.ePim;
    // True total energy of pi0 (KE + mass)
    sr.eTotalPi0 = sr.ePi0 + (0.135 * sr.nipi0);

    sr.OffAxisFluxConfig = -1;
    sr.OffAxisFluxBin = -1;
    sr.total_xsSyst_cv_wgt = 1;

    for (size_t syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {
      const size_t Nuniv = XSSyst_size_tmp[syst_it];
      if (!Nuniv) {
        continue;
      }

      if (caf::kMaxSystUniverses < Nuniv) {
        std::cout << "[ERROR]: Syst weight array in standard record "
                     "(kMaxSystUniverses = "
                  << caf::kMaxSystUniverses
                  << ") is too small to hold this syst: "
                  << XSSyst_names[syst_it] << " which requires " << Nuniv
                  << " universes." << std::endl;
        abort();
      }

      assert(Nuniv <= XSSyst_tmp[syst_it].size());

      static std::vector<bool> alreadyWarned(XSSyst_names.size(), false);

      if (IsDoNotIncludeSyst(syst_it)) { // Multiply CV weight back into
                                         // response splines.
        if (std::isnan(XSSyst_cv_tmp[syst_it]) ||
            std::isinf(XSSyst_cv_tmp[syst_it]) ||
            XSSyst_cv_tmp[syst_it] == 0) {
          if(!alreadyWarned[syst_it]){
            alreadyWarned[syst_it] = true;
            std::cout << "Warning: " << XSSyst_names[syst_it]
                      << " has a bad CV of " << XSSyst_cv_tmp[syst_it]
                      << " - will only warn once"
                      << std::endl;
          }
        } else {
          for (size_t univ_it = 0; univ_it < Nuniv; ++univ_it) {
            XSSyst_tmp[syst_it][univ_it] *= XSSyst_cv_tmp[syst_it];
          }
        }
      } else { // Include CV weight in the total
        // Do some error checking here
        if (std::isnan(XSSyst_cv_tmp[syst_it]) ||
            std::isinf(XSSyst_cv_tmp[syst_it]) ||
            XSSyst_cv_tmp[syst_it] == 0) {
          if(!alreadyWarned[syst_it]){
            alreadyWarned[syst_it] = true;
            std::cout << "Warning: " << XSSyst_names[syst_it]
                      << " has a bad CV of " << XSSyst_cv_tmp[syst_it]
                      << " - will only warn once"
                      << std::endl;
          }
        } else {
          sr.total_xsSyst_cv_wgt *= XSSyst_cv_tmp[syst_it];
        }
      }

      // Copy the spline in
      std::copy_n(XSSyst_tmp[syst_it].begin(), caf::kMaxSystUniverses,
                  sr.xsSyst_wgt[syst_it].begin());
    }

    ev_processor(sr);
  }
  std::cout << "\rProcessing finished." << std::endl;

}

int main(int argc, char const *argv[]){

  TFile out("Evloopout.root","RECREATE");
  TH2D *Sel_ERec_off_axis = new TH2D("Sel_ERec_off_axis",
    ";EVisReco;OffAxis;Count/10^{21} POT",
    100,0,10, 60,-30, 0);

  TH2D *Sel_ERec_off_axis_no_reconumu = new TH2D("Sel_ERec_off_axis_no_reconumu",
    ";EVisReco;OffAxis;Count/10^{21} POT",
    100,0,10, 60,-30, 0);

  TH2D *Sel_ERec_off_axis_no_contained = new TH2D("Sel_ERec_off_axis_no_contained",
    ";EVisReco;OffAxis;Count/10^{21} POT",
    100,0,10, 60,-30, 0);

  TH2D *Sel_ERec_off_axis_no_tracker = new TH2D("Sel_ERec_off_axis_no_tracker",
    ";EVisReco;OffAxis;Count/10^{21} POT",
    100,0,10, 60,-30, 0);

  TH2D *Sel_ERec_off_axis_no_ehad_veto = new TH2D("Sel_ERec_off_axis_no_ehad_veto",
    ";EVisReco;OffAxis;Count/10^{21} POT",
    100,0,10, 60,-30, 0);

  TH2D *True_ERec_off_axis = new TH2D("True_ERec_off_axis",
    ";EVisReco;OffAxis;Count/10^{21} POT",
    100,0,10, 60,-30, 0);

  TH2D *Sel_EHadELep_EReco1GeV = new TH2D("Sel_EHadELep_EReco1GeV",
    ";ELep;EHad;Count/10^{21} POT",
    33,0.1,1, 33,0.1,2);

  TH2D *True_EHadELep_EReco1GeV = new TH2D("True_EHadELep_EReco1GeV",
    ";ELep;EHad;Count/10^{21} POT",
    33,0.1,1, 33,0.1,2);

  TH2D *Sel_EHadELep_EReco2GeV = new TH2D("Sel_EHadELep_EReco2GeV",
    ";ELep;EHad;Count/10^{21} POT",
    33,0.1,2, 33,0.1,2);

  TH2D *True_EHadELep_EReco2GeV = new TH2D("True_EHadELep_EReco2GeV",
    ";ELep;EHad;Count/10^{21} POT",
    33,0.1,2, 33,0.1,2);

  multi_file = argc > 2;

  for(int i = 1; i < argc; ++i){
    TFile f(argv[i]);
    std::cout << "Processing File " << i << "/" << (argc-1) << std::endl;

    ana::Cut SelCut = kPRISMNDSignal_Selected_numu && kCut280kARun;

    ana::Cut TrueCut = kIsNumuCC && 
                       !kIsAntiNu && 
                       kIsTrueFV &&
                       kIsOutOfTheDesert && 
                       kCut280kARun;
    ana::Weight kNDCVWeight = GetNDCVWeight();

    ProcessFile(f,[&](const caf::SRProxy& sr){ 

        if(!sr.isCC || (sr.LepPDG != 13)){
          return;
        }
  
        double CVWeight = 1.0E21*kNDCVWeight(&sr);
        if(TrueCut(&sr)){
          if(SelCut(&sr)){
            Sel_ERec_off_axis->Fill(sr.EVisReco_ND, 
              sr.abspos_x/100.0,
              CVWeight);

            if((sr.EVisReco_ND > 0.95) && (sr.EVisReco_ND < 1.05)){
              Sel_EHadELep_EReco1GeV->Fill(sr.RecoLepEnNue, 
                sr.HadEVisReco_ND,
                CVWeight);
            }
            if((sr.EVisReco_ND > 1.95) && (sr.EVisReco_ND < 2.05)){
              Sel_EHadELep_EReco2GeV->Fill(sr.RecoLepEnNue, 
                sr.HadEVisReco_ND,
                CVWeight);
            }
          }

          if(((sr.muon_contained || sr.muon_tracker) && (sr.Ehad_veto<30))){
            Sel_ERec_off_axis_no_reconumu->Fill(sr.EVisReco_ND, 
              sr.abspos_x/100.0,
              CVWeight);
          }
          if((sr.reco_numu && (sr.muon_tracker) && (sr.Ehad_veto<30))){
            Sel_ERec_off_axis_no_contained->Fill(sr.EVisReco_ND, 
              sr.abspos_x/100.0,
              CVWeight);
          }
          if((sr.reco_numu && (sr.muon_contained) && (sr.Ehad_veto<30))){
            Sel_ERec_off_axis_no_tracker->Fill(sr.EVisReco_ND, 
              sr.abspos_x/100.0,
              CVWeight);
          }
          if((sr.reco_numu && (sr.muon_contained || sr.muon_tracker))){
            Sel_ERec_off_axis_no_ehad_veto->Fill(sr.EVisReco_ND, 
              sr.abspos_x/100.0,
              CVWeight);
          }

          True_ERec_off_axis->Fill(sr.EVisReco_ND, 
            sr.abspos_x/100.0,
            CVWeight);

          if((sr.EVisReco_ND > 0.95) && (sr.EVisReco_ND < 1.05)){
            True_EHadELep_EReco1GeV->Fill(sr.RecoLepEnNue, 
                sr.HadEVisReco_ND,
                CVWeight);
          }
          if((sr.EVisReco_ND > 1.95) && (sr.EVisReco_ND < 2.05)){
            True_EHadELep_EReco2GeV->Fill(sr.RecoLepEnNue, 
                sr.HadEVisReco_ND,
                CVWeight);
          }
        }
      });
  }

  TH2D * Eff_ERec_off_axis = (TH2D*)Sel_ERec_off_axis->Clone("Eff_ERec_off_axis");
  TH2D * Eff_EHadELep_EReco1GeV = (TH2D*)Sel_EHadELep_EReco1GeV->Clone("Eff_EHadELep_EReco1GeV");
  TH2D * Eff_EHadELep_EReco2GeV = (TH2D*)Sel_EHadELep_EReco2GeV->Clone("Eff_EHadELep_EReco2GeV");

  Eff_ERec_off_axis->Divide(True_ERec_off_axis);
  Eff_EHadELep_EReco1GeV->Divide(True_EHadELep_EReco1GeV);
  Eff_EHadELep_EReco2GeV->Divide(True_EHadELep_EReco2GeV);

  Eff_ERec_off_axis->SetDirectory(&out);
  Eff_EHadELep_EReco1GeV->SetDirectory(&out);
  Eff_EHadELep_EReco2GeV->SetDirectory(&out);


  TH2D * Eff_ERec_off_axis_no_reconumu = (TH2D*)Sel_ERec_off_axis_no_reconumu->Clone("Eff_ERec_off_axis_no_reconumu");
  TH2D * Eff_ERec_off_axis_no_contained = (TH2D*)Sel_ERec_off_axis_no_contained->Clone("Eff_ERec_off_axis_no_contained");
  TH2D * Eff_ERec_off_axis_no_tracker = (TH2D*)Sel_ERec_off_axis_no_tracker->Clone("Eff_ERec_off_axis_no_tracker");
  TH2D * Eff_ERec_off_axis_no_ehad_veto = (TH2D*)Sel_ERec_off_axis_no_ehad_veto->Clone("Eff_ERec_off_axis_no_ehad_veto");

  Eff_ERec_off_axis_no_reconumu->Divide(True_ERec_off_axis);
  Eff_ERec_off_axis_no_contained->Divide(True_ERec_off_axis);
  Eff_ERec_off_axis_no_tracker->Divide(True_ERec_off_axis);
  Eff_ERec_off_axis_no_ehad_veto->Divide(True_ERec_off_axis);

  Eff_ERec_off_axis_no_reconumu->SetDirectory(&out);
  Eff_ERec_off_axis_no_contained->SetDirectory(&out);
  Eff_ERec_off_axis_no_tracker->SetDirectory(&out);
  Eff_ERec_off_axis_no_ehad_veto->SetDirectory(&out);

  out.Write();
  out.Close();

}
