#include "Calcs.h"
#include "OscLib/OscCalcPMNSOpt.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TVectorT.h"
#include "TF1.h"

#include <iostream>
#include <set>
#include <sstream>

// C++ includes

#include <iomanip>
using namespace std;
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <vector> // Need this for generate dictionary for nested vectors
#include "step3_helpers.h"


// Pick six evenly spaced points in each non-dead region
vector<double> generatePoints(double start)
{
  vector<double> points;
  double step = 7.0; // Interval between points
  double current;

  if (start == -300) {
      current = start + 1.45; // Adjusted start for the edge case
  } else if (start == 256.55) {
      current = start + 7.0; // Start for the symmetric edge case
  } else {
      current = start + step; // Regular start
  }

  for (int i = 0; i < 6; ++i) {
      points.push_back(current + i * step);
  }
  return points;
}

//define function type, i.e want LepHad or Etrue FDEventRateAtND probability
enum class FunctionType {
    LepHad,
    ETrue
};


class FunctionCache {
public:
     // (lep, had) → TF1
    std::map<std::pair<int, int>, TF1*> functionCacheLepHad;
    // etrue → TF1
    std::map<int, TF1*> functionCacheETrue;

    TVectorT<double>* kHadBinEdges = nullptr;
    TVectorT<double>* kLepBinEdges = nullptr;
    TVectorT<double>* kTrueBinEdges = nullptr;

    FunctionType funcType;

    void loadFunctions(const std::string& filename, FunctionType type) {
        funcType = type;

        TFile* file = new TFile(filename.c_str(), "READ");
        if (!file->IsOpen()) {
            std::cerr << "Error opening ROOT file!" << std::endl;
            return;
        }

        if (funcType == FunctionType::LepHad) {
          // Example: Load kHadBinEdges and kLepBinEdges from the file (if they exist)
          kHadBinEdges = (TVectorT<double>*)file->Get("kHadBinEdges");
          kLepBinEdges = (TVectorT<double>*)file->Get("kLepBinEdges");

          cout<<" kLepBinEdge size "<<kLepBinEdges->GetNrows()<<endl;

          // Loop over the possible lep and had bins, assuming you know the ranges
          for (int lep = 1; lep <= 13; ++lep) {
            for (int had = 1; had <= 8; ++had) {
              std::string functionName = Form("f_norm_lep%d_had%d", lep, had);
              TF1* f_norm = (TF1*)file->Get(functionName.c_str());
              if (f_norm) {
                  functionCacheLepHad[{lep, had}] = f_norm;
              } else {
                  std::cerr << "Function " << functionName << " not found in file!" << std::endl;
              }
            }
          }
        }
        else if (funcType == FunctionType::ETrue) {
          kTrueBinEdges = (TVectorT<double>*)file->Get("kTrueEBinEdges");

          for (int etrue = 1; etrue <= kTrueBinEdges->GetNrows(); ++etrue) {
            std::string functionName = Form("f_norm_etrue%d", etrue);
            TF1* f_norm = (TF1*)file->Get(functionName.c_str());
            if (f_norm) {
                functionCacheETrue[etrue] = f_norm;
            } else {
                std::cerr << "Function " << functionName << " not found in file!" << std::endl;
            }
          }
        }

        file->Close();
    }

    TF1* getFunction(int lep, int had) {
      auto it = functionCacheLepHad.find({lep, had});
      if (it != functionCacheLepHad.end()) {
          return it->second;
      } else {
          std::cerr << "Function for lep=" << lep << ", had=" << had << " not found in cache!" << std::endl;
          return nullptr;
      }
    }

    TF1* getFunctionETrue(int etrue) {
      auto it = functionCacheETrue.find(etrue);
      if (it != functionCacheETrue.end())
          return it->second;

      std::cerr << "No function for etrue=" << etrue << std::endl;
      return nullptr;
    }

    // Helper functions to determine bin based on Ehad and Emu
    int getLepBin(double Emu) {
        for (int i = 0; i < kLepBinEdges->GetNrows() - 1; ++i) {
          //cout<<" lep edges: "<<(*kLepBinEdges)[i]<<endl;
            if (Emu >= (*kLepBinEdges)[i] && Emu < (*kLepBinEdges)[i + 1]) {
                return i + 1;  // Bin numbers are 1-based
            }
        }
        std::cerr << "Emu out of range!" << std::endl;
        return -1;  // Invalid bin
    }

    int getHadBin(double Ehad) {
        for (int i = 0; i < kHadBinEdges->GetNrows() - 1; ++i) {
            if (Ehad >= (*kHadBinEdges)[i] && Ehad < (*kHadBinEdges)[i + 1]) {
                return i + 1;  // Bin numbers are 1-based
            }
        }
        std::cerr << "Ehad out of range!" << std::endl;
        return -1;  // Invalid bin
    }

    int getTrueBin(double Etrue) {
      for (int i = 0; i < kTrueBinEdges->GetNrows() - 1; ++i) {
          if (Etrue >= (*kTrueBinEdges)[i] && Etrue <  (*kTrueBinEdges)[i + 1]) {
              return i + 1;
          }
      }
      std::cerr << "ETrue out of range!" << std::endl;
      return -1;
    }
};

double FDEventRateAtND(FunctionCache& cache, double Ehad, double Emu, double OAPos) {
    if (cache.funcType != FunctionType::LepHad) {
        std::cerr << "ERROR: FDEventRateAtND called but cache is not LepHad!" << std::endl;
        return 0.0;
    }
    // Dynamically determine lep and had bins based on Ehad and Emu
    int lepBin = cache.getLepBin(Emu);
    int hadBin = cache.getHadBin(Ehad);

    if (lepBin == -1 || hadBin == -1) {
        std::cerr << "Invalid bins for Ehad=" << Ehad << ", Emu=" << Emu << std::endl;
        return 0;
    }

    // Retrieve the corresponding function from the cache
    TF1* f_norm = cache.getFunction(lepBin, hadBin);
    if (f_norm) {
        return f_norm->Eval(OAPos);
    } else {
        std::cerr << "Unable to find the corresponding function in cache." << std::endl;
        return 0;
    }
}

double FDEventRateAtND_ETrue(FunctionCache& cache, double Etrue, double OAPos) {
    if (cache.funcType != FunctionType::ETrue) {
        std::cerr << "ERROR: FDEventRateAtND_ETrue called but cache is not ETrue!" << std::endl;
        return 0.0;
    }
    int etrueBin = cache.getTrueBin(Etrue);
    if (etrueBin == -1) {
        std::cerr << "Invalid Etrue bin: Etrue=" << Etrue << std::endl;
        return 0.0;
    }

    TF1* f_norm = cache.getFunctionETrue(etrueBin);
    if (!f_norm){
      std::cerr << "Unable to find the corresponding function in cache." << std::endl;
      return 0;
    }

    return f_norm->Eval(OAPos);
}


//Essentially a copy of SpectrumLoader::HandleFile
void ProcessFile(TFile *fHad, TFile *fMu){

  assert(!fHad->IsZombie() && !fMu->IsZombie());
  TTree* t_effTree = 0;
  t_effTree = (TTree*)fHad->Get("effTreeND");
  if (!t_effTree){
    std::cerr << "Error: effTreeND not found in file " << fHad->GetName() << std::endl;
    return;
  }
  assert(t_effTree);

  TTree* t_effValues = 0;
  t_effValues = (TTree*)fHad->Get("effValues");
    if (!t_effValues) {
        std::cerr << "Error: effValues not found in file " << fHad->GetName() << std::endl;
        return;
    }

  TTree *t_PosVec = 0;
  t_PosVec= (TTree*)fHad->Get("effPosND");
  if (!t_PosVec) {
      std::cerr << "Error: effPosND not found in file " << fHad->GetName() << std::endl;
      return;
  }

  // Process second file (muonTree)
  TTree* t_effMu = (TTree*)fMu->Get("event_data");
  if (!t_effMu) {
      std::cerr << "Error: event_data not found in file " << fMu->GetName() << std::endl;
      return;
  }

  gSystem->Exec("rm -f AutoDict*vector*vector*vector*double*"); // Remove old dictionary if exists
  gInterpreter->GenerateDictionary("vector<vector<vector<double> > >", "vector");

  bool useCombinedEfficiency = true; //set to false if only hadron eff desired
  bool scaleToCombinedEfficiency = false; //set to false if hadron eff scaling is desired
  //File with coefficients histogram
  TFile* FileWithCoeffs = new TFile("data/step3/FileWithCoeffsNuMu.root", "READ");
  FileWithCoeffs->cd();
  TH1D* CoefficientsHist = (TH1D*) FileWithCoeffs->Get("CoeffPRISMUpTo3mOA");
  CoefficientsHist->SetDirectory(0);
  FileWithCoeffs->Close();

  // //file storing the spline functions for the FDEventRateAtND
  // Create an instance of the cache
  FunctionCache cacheLepHad;
  FunctionCache cacheEtrue;

  // Load functions from the ROOT file (only do this once)
  cacheLepHad.loadFunctions("data/step3/Splines_FDEventRateAtND.root", FunctionType::LepHad);
  cacheEtrue.loadFunctions("data/step3/Splines_FDEventRateAtND_ETrue.root", FunctionType::ETrue);

  // LoadFDEventRateData("Splines_FDEventRateAtND.root");
  cout<<" called fdevrate fct"<<endl;

  // 0. FD: read event from FD MC ntuple: before earth curvature rotation
  vector<Double_t> a_ND_off_axis_pos_vec = {0, -1.75, -2, -4, -5.75, -8, -9.75, -12, -13.75, -16, -17.75, -20, -21.75, -24, -25.75, -26.25, -28, -28.25, -28.5}; //{0};
  vector<Double_t> a_ND_vtx_vx_vec;
  vector<Double_t> OAPosition;
  // Generate six evenly spaced point in each non-dead region (NDFV between -200cm to 200cm as in ND CAFs)
  vector<pair<double, double>> non_dead_regions = {
      {-203.45, -154.45}, {-151.85, -102.85},
      {-101.35, -52.35}, {-49.75, -0.75}, {0.75, 49.75}, {52.35, 101.35},
      {102.85, 151.85}, {154.45, 203.45}
  };


  // Iterate over each non-dead region and generate points
  for (const auto& region : non_dead_regions) {
      auto points = generatePoints(region.first);
      // std::cout << "Points between " << region.first << " and " << region.second << ": ";
      for (auto point : points) {
          std::cout << point << " ";
          a_ND_vtx_vx_vec.emplace_back(point);
      }
      std::cout << std::endl;
  }
  // Define variables for FD event

  int ND_Sim_n_hadronic_Edep_b; // #nr of hadronic energy deposits
  //newly added
  Int_t iwritten; //event counter
  Double_t ND_LAr_dtctr_pos; // unit: cm, ND LAr detector off-axis choices for each FD evt
  Double_t ND_LAr_vtx_pos; // unit: cm, vtx x choices for each FD evt in ND_Lar volume
  Double_t ND_GeoEff; //geom efficiency of FD Event at ND (when hadron energy outside ND is trimmed)
  int validThrows;  // count no. of throws that meet ND FV cut for this evt
  int NPassedThrows; //nr of total passed throws (hadron pass) for each event

  vector<uint64_t> *ThrowResults = nullptr; //all throw results
  vector<float> *VetoEnergyEventsPass = nullptr; // veto energy of events that pass the throw
  vector<float> *TrimEnergyEventsPass = nullptr; //trimmed energy of events that pass the throw
  vector<float> *TotalEnergyEventsPass = nullptr; //total energy of current throw (energy deposits sum -- hadron energy )
  vector<float> *VtxYEventsPass = nullptr; // y vtx position of events passing the hadron cut
  vector<float> *VtxZEventsPass = nullptr; // z vtx position of events passing the hadron cut

  t_effValues->SetBranchAddress("iwritten",         &iwritten);
  t_effValues->SetBranchAddress("ND_LAr_dtctr_pos",   &ND_LAr_dtctr_pos);
  t_effValues->SetBranchAddress("ND_LAr_vtx_pos",       &ND_LAr_vtx_pos);
  t_effValues->SetBranchAddress("ND_GeoEff",        &ND_GeoEff);
  t_effValues->SetBranchAddress("ThrowResults",      &ThrowResults);
  t_effValues->SetBranchAddress("validThrows",  &validThrows);
  t_effValues->SetBranchAddress("NPassedThrows", &NPassedThrows);
  t_effValues->SetBranchAddress("VetoEnergyEventsPass", &VetoEnergyEventsPass);
  t_effValues->SetBranchAddress("TrimEnergyEventsPass",  &TrimEnergyEventsPass);
  t_effValues->SetBranchAddress("TotalEnergyEventsPass",  &TotalEnergyEventsPass);
  t_effValues->SetBranchAddress("VtxYEventsPass",  &VtxYEventsPass);
  t_effValues->SetBranchAddress("VtxZEventsPass",  &VtxZEventsPass);
//newlyadded
  Float_t totEnergyFDatND_f; //total hadronic energy of the event in the FD
  double muonEdep_f; //Deposited Energy of muon in the FD [MeV]
  double muonTrackLength_f; //muon track length in cm


  vector<float> *HadronHitEdeps =0; // Hadron hit segment energy deposits [MeV]
  vector<float> *CurrentThrowTotE = 0;
  vector<vector<float>> *ND_OffAxis_Sim_hadronic_hit_xyz=0; // coordinates of hadron hits before random throws

  double ND_Gen_numu_E; // Energy of generator level neutrino [GeV]
  double ND_E_vis_true; // True visible energy of neutrino [GeV]
  std::vector<std::vector<std::vector<double>>>* xyz_mom=0; // muon momentum vector for a given off-axis location
  int TotalSimNeutralPions; //Number of neutral pions in the event. Used to calculate VisEtrue

  // Extract event info from ntuple
  t_effTree->SetBranchAddress("ND_Sim_n_hadronic_Edep_b",         &ND_Sim_n_hadronic_Edep_b);
  t_effTree->SetBranchAddress("CurrentThrowTotE",          &CurrentThrowTotE);
  t_effTree->SetBranchAddress("HadronHitEdeps",            &HadronHitEdeps);
  t_effTree->SetBranchAddress("ND_E_vis_true",             &ND_E_vis_true);
  t_effTree->SetBranchAddress("ND_Gen_numu_E",             &ND_Gen_numu_E);
  t_effTree->SetBranchAddress("ND_OffAxis_Sim_mu_start_p_xyz_LAr", &xyz_mom);
  t_effValues->SetBranchAddress("totEnergyFDatND_f",   &totEnergyFDatND_f);
  t_effValues->SetBranchAddress("muonEdep_f",   &muonEdep_f);
  t_effValues->SetBranchAddress("muonTrackLength_f",   &muonTrackLength_f);
  t_effTree->SetBranchAddress("TotalSimNeutralPions", &TotalSimNeutralPions);

  double LepMomTot;
  vector<Double_t> *ND_LAr_dtctr_pos_vec = 0; // unit: cm, ND off-axis choices for each FD evt
  vector<Double_t> *ND_vtx_vx_vec = 0;       // unit: cm, vtx x choices for each FD evt in ND volume
  vector<Int_t> *iwritten_vec = 0;
  TBranch *b_ND_LAr_dtctr_pos_vec = 0; // unit: cm, ND LAr detector off-axis choices for each FD evt
  TBranch *b_ND_vtx_vx_vec = 0;
  TBranch *b_iwritten_vec = 0;
  t_PosVec->SetBranchAddress("ND_LAr_dtctr_pos_vec",      &ND_LAr_dtctr_pos_vec , &b_ND_LAr_dtctr_pos_vec);
  t_PosVec->SetBranchAddress("ND_vtx_vx_vec",             &ND_vtx_vx_vec,         &b_ND_vtx_vx_vec);
  t_PosVec->SetBranchAddress("iwritten_vec",              &iwritten_vec,          &b_iwritten_vec);

  Long64_t tentry = t_PosVec->LoadTree(0);
  b_ND_LAr_dtctr_pos_vec->GetEntry(tentry);
  b_ND_vtx_vx_vec->GetEntry(tentry);
  b_iwritten_vec->GetEntry(tentry);

  Int_t ND_LAr_dtctr_pos_vec_size = ND_LAr_dtctr_pos_vec->size();
  Int_t ND_vtx_vx_vec_size = ND_vtx_vx_vec->size(); //TEMPORARY comments, too big vtxX size (wouldn't happen once i Rerun with NDFV (-200, 200 cm))

  Int_t iwritten_vec_size = iwritten_vec->size();
  Int_t tot_size = ND_LAr_dtctr_pos_vec_size*ND_vtx_vx_vec_size;
  Int_t hadronhit_n_plots = tot_size*N_throws;
  Int_t nentries = t_effValues->GetEntries();

  vector<double> *vtxX=0;
  vector<double> *combined_eff=0;
  vector<double> *hadron_selected_eff=0;
  vector<double> *muon_selected_eff=0;
  vector<double> *muon_tracker_eff=0;
  vector<vector<double>> *muContained=0;
  vector<vector<double>> *weightPmuon=0;

  t_effMu->SetBranchAddress("vtxX", &vtxX);
  t_effMu->SetBranchAddress("combined_eff", &combined_eff);
  t_effMu->SetBranchAddress("hadron_selected_eff", &hadron_selected_eff);
  t_effMu->SetBranchAddress("muon_selected_eff", &muon_selected_eff);
  t_effMu->SetBranchAddress("muon_tracker_eff", &muon_tracker_eff);
  t_effMu->SetBranchAddress("muContained", &muContained);
  t_effMu->SetBranchAddress("weightPmuon", &weightPmuon);

  //save nr of throws per each vtx and / event
  int nPassThrowsPerVtx[ND_vtx_vx_vec_size+1];
  int nPassThrowsPerEvent;
  vector<double> vectND_Gen_numu_E;
  //make sure same nr of events (should be really same events)
  if (t_effMu->GetEntries() != t_effTree->GetEntries()) {
    cout<<"The hadron efficiency file  has "<<t_effTree->GetEntries()<<" events, and the muon eff file  "<<t_effMu->GetEntries()<<" events."<<endl;
    return;
  }

  vector<Double_t> Coefficients;

  int nFDEvents = iwritten_vec->size();
  int nvtxXpositions = a_ND_vtx_vx_vec.size();
  int nDetPos = a_ND_off_axis_pos_vec.size();

  double OAPos;
  double OAPos2;
  double CoefficientsAtOAPos;
  double WeightEventsAtOaPos;
  double EnuTrue[nFDEvents];
  double TotalLeptonMom[nFDEvents];
  cout<<" nFDEvents = "<<nFDEvents<<" nXpos = "<<nvtxXpositions<<endl;

  TH1D* HistVetoE[nFDEvents][nvtxXpositions];
  TH1D* HistEtrim[nFDEvents][nvtxXpositions];
  TH1D* HistEtrimPmuWeighted[nFDEvents][nvtxXpositions];
  TH1D* HistEtrimPmuWeightedCrossCheck[nFDEvents][nvtxXpositions];

  TH1D* HistEtrimDetPos[nFDEvents][nvtxXpositions][nDetPos];
  TH1D* HistEtrimDetPosNoFDEventRate[nFDEvents][nvtxXpositions][nDetPos];
  TH1D* HistEtrimDetPosWithFDEventRate[nFDEvents][nvtxXpositions][nDetPos];
  TH1D* HistEtrimDetPosCoeff1[nFDEvents][nvtxXpositions][nDetPos];

  TH1D* HistEtrimAllVtxX[nFDEvents];
  TH1D* HistEtrimPmuWeightedAllVtxX[nFDEvents];

  TH1D* HistEtrimAllVtxXTimesCoeff[nFDEvents];
  TH1D* HistEtrimAllVtxXTimesCoeffOscillated[nFDEvents];

  TH1D* HistEtrimAllVtxXTimesCoeffWithFDEvRate[nFDEvents];
  TH1D* HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[nFDEvents];

  TH1D* HistOAPosVtxX[nFDEvents][nvtxXpositions];


  // save histo with total hadronic energy at FD for all FD events
  TH1D* hist_FDTotEnergy = new TH1D("hist_FDTotEnergy", "hist_FDTotEnergy", 25000, 0, 25000);
  //hist with muon energy deposit
  TH1D* hist_muEdep = new TH1D("hist_muEdep", "hist_muEdep", 25000, 0, 25000);
  hist_muEdep->GetXaxis()->SetTitle("Muon Deposited Energy (GeV)");
  //hist with muon track length -
  TH1D* hist_muTrackLength = new TH1D("hist_muTrackLength", "hist_muTrackLength", 2500, 0, 50000);
  //save histo with muon energy at FD for all FD events
  TH1D* hist_TotalMuEnergy = new TH1D("hist_TotalMuEnergy", "hist_TotalMuEnergy", 25000, 0, 25);
  hist_TotalMuEnergy->GetXaxis()->SetTitle("LepMomTot (GeV)");
  //save histo with total neutrino energy at FD for all FD events
  TH1D* hist_EnuFDEnergy = new TH1D("hist_EnuFDEnergy", "hist_EnuFDEnergy", 25000, 0, 25);
  //save histo with oscillated neutrino energy spectrum at FD for all FD events
  TH1D* hist_EnuFDEnergy_Osc = new TH1D("Oschist_EnuFDEnergy", "Oschist_EnuFDEnergy", 25000, 0, 25);
  // save histo with visible neutrino energy at FD for all FD events
  TH1D* hist_visEnuFDEnergy = new TH1D("hist_visEnuFDEnergy", "hist_visEnuFDEnergy", 25000, 0, 25);


  TGraph* PlotEfficiencyVsVtxX[nFDEvents];
  TGraph* PlotMuonEfficiencyVsVtxX[nFDEvents];
  TGraph* PlotMuTrackerEfficiencyVsVtxX[nFDEvents];
  TGraph* PlotCombinedEfficiencyVsVtxX[nFDEvents];
  TGraph* EfficiencyVsOAPos[nFDEvents];
  // TH1D* HistOAPos = new TH1D("HistOAPos", "HistOAPos", 67, -30.5, 3);
  TH1D* HistOAPos[nFDEvents];
  TH2D* CoefficientsAtOAPosHist = new TH2D("CoefficientsAtOAPosHist", "CoefficientsAtOAPosHist", 67, -30.5, 3, 60, -0.3, 0.3);


  TFile* FileWithHistoInfo = new TFile("FileWithHistEtrim_MuAndHaddEff_VisEtrim_WithOscProb_FDEvRateAtND_NDFV4m_2DHistosWithSelectedAndThrownEvents_WithCAFLikeMuCut.root", "RECREATE");
  FileWithHistoInfo->cd();

  // Or oscillated, in this case using reasonable parameters from
  // Analysis/Calcs.h
  osc::IOscCalc* calc = ana::DefaultOscCalc();

  // structure to save Throw information
  struct ThrowInfo {
    float Etrim;
    double Emu;
    double weightPmuon;
  };

  std::vector<std::vector<std::vector<ThrowInfo>>> AllThrowInfo;

  double weightCAFLike[nFDEvents]; // this is going to be a weight similar to CAFs: if Edep/tracklength > 3 MeV / cm || trackLength <100 cmm then the muon is not reco -> Selected Mu =0 -> mu eff = 0;
  double VisEtrue[nFDEvents];
  
  for (int i_iwritten = 0; i_iwritten<nFDEvents; i_iwritten++) //looping over events
  { HistOAPos[i_iwritten] = new TH1D(Form("HistOAPos_FDEvt_%d", i_iwritten), Form("HistOAPos_FDEvt_%d", i_iwritten), 67, -30.5, 3);

    t_effTree->GetEntry(i_iwritten);

    hist_EnuFDEnergy->Fill(ND_Gen_numu_E);
    EnuTrue[i_iwritten] = ND_Gen_numu_E; 
    LepMomTot =sqrt(pow((*xyz_mom)[0][0][0],2)+pow((*xyz_mom)[0][0][1],2)+pow((*xyz_mom)[0][0][2],2) );
    TotalLeptonMom[i_iwritten] = LepMomTot;
    hist_TotalMuEnergy->Fill(LepMomTot);
    cout<<" Enu = "<<ND_Gen_numu_E<<" Evis = "<<ND_E_vis_true<<" Lep mom " <<LepMomTot<<" mu energy: "<<muonEdep_f<< endl;
    hist_visEnuFDEnergy->Fill(ND_E_vis_true);
		//fill osc histos
		hist_EnuFDEnergy_Osc->Fill(ND_Gen_numu_E, calc->P(14,14,ND_Gen_numu_E));
    //hist_TotalMuEnergy_Osc->Fill(LepMomTot, calc->P(14,14,ND_Gen_numu_E));
    //hist_visEnuFDEnergy_Osc->Fill(ND_E_vis_true, calc->P(14,14,ND_Gen_numu_E));
    VisEtrue[i_iwritten] = ND_E_vis_true - TotalSimNeutralPions * 0.134977 + 0.10566; //Add back in the muon mass to get the total lepton energy, and subtract pi0 mass to get only the KE
    cout << "            ND_E_vis_true = " << ND_E_vis_true << endl;
    cout << "            TotalSimNeutralPions = " << TotalSimNeutralPions << endl;
    cout << "            VisEtrue = " << VisEtrue[i_iwritten] << endl;

    for (int i_entry=tot_size*i_iwritten; i_entry < tot_size * (i_iwritten+1); i_entry++ ) //i_entry goes from evNr*nr of vtxXpositions to nrOf vtxX positon * (evNr+1) effectively a loop over nrvtxX positions for diff events
    { 
      /*We are looping through the entries in a "flattened" Ttree: t_effValues has a total of #events*#vtxXpositions*#dtctrPos. 
              The first tot_size = #vtxXpositions*#dtctrPos belong to event 0, the next #vtxXpositions*#dtctrPos belog to event 1, etc. */
      t_effValues->GetEntry(i_entry);

      //cout<<" iev = "<< i_iwritten<<"  ND_LAr_vtx_pos " << ND_LAr_vtx_pos<<" i_entry: "<<i_entry<<endl;

      if(ND_LAr_vtx_pos == -196.45) {//only want to fill the histos for 1 vtxX

        hist_FDTotEnergy->Fill(totEnergyFDatND_f);
        hist_muEdep->Fill(muonEdep_f);
        hist_muTrackLength->Fill(muonTrackLength_f);
        cout<<" i_iwritten "<<i_iwritten<<" totEnergyFDatND_f "<<totEnergyFDatND_f<<" mu energy: "<<muonEdep_f<<" mu Track Length: "<<muonTrackLength_f<< " Enu = "<<ND_Gen_numu_E<<endl;
        weightCAFLike[i_iwritten] = 1.;
        if(muonTrackLength_f < 100. || muonEdep_f/muonTrackLength_f > 3.){
          cout<<"** this event will not be selected!! "<<" ev nr "<<i_iwritten<<" muon e dep: "<<muonEdep_f<<" muon track length: "<<muonTrackLength_f<<endl;
          //cout<< "Commented out weightCAFLike for muon track length temporarily. If you see this, you need to remove the comment" << endl;
          weightCAFLike[i_iwritten] = 0.; // TEMPORARILY COMMENTED OUT!! MAKE SURE TO REMOVE COMMENT
        }


      }

      //this is not needed anymore: i.e every events will be put in the same OA positions -> don't need this histo / event
      Int_t i_detposInCoefRange = 0;
      for (Double_t i_ND_LAr_dtctr_pos: a_ND_off_axis_pos_vec)
      {
        i_detposInCoefRange +=1;
        // //calculate OAPos=vtx_x+det_pos
        OAPos2 = ND_LAr_vtx_pos/100.0 + a_ND_off_axis_pos_vec[i_detposInCoefRange-1];
        HistOAPos[i_iwritten]->Fill(OAPos2);
      }
    }
    HistOAPos[i_iwritten]->Write(Form("HistOAPos_FDEvt_%d", i_iwritten));
  }




    //std::vector<std::vector<std::vector<std::pair<float, float>>>> EtrimEmuValues; //store Etrim and Emu values for each event at each vtxX position
    AllThrowInfo.resize(nFDEvents);
    // start the loop with efficiencies etc

    /*This is the loop where for each event and each vtxX, trimmed hadron energy (Etrim), the muon energy, and probability that the muon is contained or tracker matched of the throws 
        that passed the hadronic contaiment cut are put into a structure (info) to allow them to be accessed to fill histograms later.  */
      //loop over all of the FD events:
    for (Int_t i_iwritten = 0; i_iwritten<nFDEvents; i_iwritten++) 
    {
      cout<<" i_iwritten: "<<i_iwritten<<" weight CAF like: "<<weightCAFLike[i_iwritten]<<endl;

      //Why skip events with high muon energy? Are these muon events fully contained in FD? Would they ever pass the muon selection in the ND?
      if(TotalLeptonMom[i_iwritten] > 20) {
         cout<<" Emu > 20 GeV, Emu = "<<TotalLeptonMom[i_iwritten]<<" not interested in so high energies, skip event " <<endl;
         continue;
      }

      t_effMu->GetEntry(i_iwritten);
      t_effTree->GetEntry(i_iwritten);

      int numOApos = vtxX->size();

      AllThrowInfo[i_iwritten].resize(numOApos);
      cout<< "      There are " << numOApos << " off-axis positions" << endl;

      int NumThrowsCounter = 0;

        for (int i_ND_LAr_vtx_pos = 0; i_ND_LAr_vtx_pos < numOApos; i_ND_LAr_vtx_pos++ )
        {
          /*because of how t_effValues is structured, we need to skip the previous events' entries in t_effValues 
              (there are numOApos*i_iwritten previous entries to skip), so we use hadEntry to do so. */
          Int_t hadEntry = numOApos*i_iwritten+i_ND_LAr_vtx_pos; 
          t_effValues->GetEntry(hadEntry);

          //cout<<" i_iwritten "<<i_iwritten<<" totEnergyFDatND_f " <<totEnergyFDatND_f<<endl;

          int nthrowsToLoop = NPassedThrows; //NPassedThrows is the number of throws that passed the hadron containment cut at this vertex
          /* Use these lines to check looping if needed. 
          cout << "   NumThrowsCounter = " << NumThrowsCounter << endl;
          cout << "   nthrowsToLoop = " << nthrowsToLoop << endl; */
          for (Int_t ithrow = NumThrowsCounter; ithrow < nthrowsToLoop+NumThrowsCounter; ithrow++ ){ 
	          ThrowInfo info;

            if(TrimEnergyEventsPass->at(ithrow-NumThrowsCounter)*1E-3 > 20){
                  cout<<" skipping this throw, Ehad = "<<TrimEnergyEventsPass->at(ithrow-NumThrowsCounter)*1E-3<<" GeV, > 20 GeV"<<endl;
                  continue;
                }
            
            /* //Checks to ensure the correct values of Etrim and weightPmuon are used by checking indexes used. 
            if(ithrow == NumThrowsCounter){
              cout << "     first loop: " << endl;
              cout << "         ithrow+1 = " << ithrow +1 << endl;
              cout << "         ithrow-NumThrowsCounter = " << ithrow-NumThrowsCounter << endl;
            }
            if(ithrow == nthrowsToLoop+NumThrowsCounter-1){
              cout << "     last loop: " << endl;
              cout << "         ithrow+1 = " << ithrow +1 << endl;
              cout << "         ithrow-NumThrowsCounter = " << ithrow-NumThrowsCounter << endl;
            }
            */

            info.Etrim = TrimEnergyEventsPass->at(ithrow-NumThrowsCounter);  //save trimmed hadron energy per throw (subtracting off NumThrowsCounter because we don't need to skip the previous events throws for TrimEnergyEventsPass (this comes from t_effValues))
            info.Emu   = TotalLeptonMom[i_iwritten]*1E3;                     //Currently, we are using the true muon energy, so this is the same for every throw. Written here per throw for future iterations of code where reconstruction is available. 
            info.weightPmuon = (*weightPmuon)[ithrow+1][0];                  //The first entry in weightPmuon is empty, so we have to skip 1 entry to get to the correct one, hence the ithrow+1
          
            AllThrowInfo[i_iwritten][i_ND_LAr_vtx_pos].push_back(info);

          } //end throw

          NumThrowsCounter += nthrowsToLoop;

        }// end vtx loop
    }//end iwritten

    TH2D* AllThrownEventsVsOAPosVsTotalETrim[nFDEvents];
    TH2D* SelectedEventsVsOAPosVsTotalETrim[nFDEvents];
    TH2D* SelectedEventsVsOAPosVsTotalETrimNoEvtRate[nFDEvents];

    //Setup Matrices for VisEtrue to Etrim (used to convert ND energy to FD Etrim Energy)
    TH2D* EtrimVsEvisTrueNoWeights[nFDEvents];
    TH2D* EtrimVsEvisTrueWithPWeightMuon[nFDEvents];
    TH2D* EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon[nFDEvents];
    TH2D* EtrimVsEvisTrueWithNDEventRateWithPWeightMuon[nFDEvents];
    TH2D* EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon[nFDEvents];

    std::cout<<" saving 2D histos with selected and all thrown events vs Etrim+Emu vs OA Pos for same binning as in PRISM "<<std::endl;
    //energy edges same as in CAFAna
    std::vector<double> edges;
    edges.emplace_back(0.);
    edges.emplace_back(0.5);

    while (edges.back() < 2.)
        edges.emplace_back(edges.back() + 0.04);

    while (edges.back() < 3.)
        edges.emplace_back(edges.back() + 0.08);

    while (edges.back() < 4.)
        edges.emplace_back(edges.back() + 0.1);

    edges.emplace_back(4.5);
    edges.emplace_back(5.);
    edges.emplace_back(6.);
    edges.emplace_back(10.);
    edges.emplace_back(120.);

    // Convert for ROOT
    int nBinsEnergy = edges.size() - 1;
    double* EnergyEdges = edges.data();


    for (Int_t i_iwritten = 0; i_iwritten<nFDEvents; i_iwritten++)
    {
       cout<<" i_iwritten: "<<i_iwritten<<endl;
       if(TotalLeptonMom[i_iwritten] > 20) {
          cout<<" Emu > 20 GeV, Emu = "<<TotalLeptonMom[i_iwritten]<<" not interested in so high energies, skip event " <<endl;
          continue;
       }

       Double_t x_ND_LAr_vtx_pos[ND_vtx_vx_vec_size];
       Double_t y_geoeff[ND_vtx_vx_vec_size];
       Double_t y_geoEffOAPos[ND_vtx_vx_vec_size * nDetPos];
       Double_t X_OAPos[ND_vtx_vx_vec_size * nDetPos];

       TString HistEtrimAllVtxX_name = Form("HistEtrimAllVtxX_FDEvt_%d", i_iwritten);
       HistEtrimAllVtxX[i_iwritten] = new TH1D(HistEtrimAllVtxX_name, HistEtrimAllVtxX_name, 25000, 0, 25000);
       TString HistEtrimPmuWeightedAllVtxX_name = Form("HistEtrimPmuWeightedAllVtxX_FDEvt_%d", i_iwritten);
       HistEtrimPmuWeightedAllVtxX[i_iwritten] = new TH1D(HistEtrimPmuWeightedAllVtxX_name, HistEtrimPmuWeightedAllVtxX_name, 25000, 0, 25000);
       //only calculate the linear combination resulting Etrim for either had eff only or combined eff (more memory efficient)
       TString HistEtrimAllVtxXTimesCoeff_name;
       TString HistEtrimAllVtxXTimesCoeff_FDEvRateAtND_name;
       if(!useCombinedEfficiency)
         HistEtrimAllVtxXTimesCoeff_name = Form("HistEtrimAllVtxXTimesCoeff_FDEvt_%d", i_iwritten);
       else{
         HistEtrimAllVtxXTimesCoeff_name = Form("HistEtrimPmuWeightedAllVtxXTimesCoeff_NoFDEvRate_FDEvt_%d", i_iwritten);
         HistEtrimAllVtxXTimesCoeff_FDEvRateAtND_name = Form("HistEtrimPmuWeightedAllVtxXTimesCoeff_FDEvRateAtND_FDEvt_%d", i_iwritten);
       }

       HistEtrimAllVtxXTimesCoeff[i_iwritten] = new TH1D(HistEtrimAllVtxXTimesCoeff_name, HistEtrimAllVtxXTimesCoeff_name, 25000, 0, 25000);
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten] = new TH1D(HistEtrimAllVtxXTimesCoeff_FDEvRateAtND_name, HistEtrimAllVtxXTimesCoeff_FDEvRateAtND_name, 25000, 0, 25000);

       TString NameAllThrownEventsVsOAPosVsTotalETrim = Form("AllThrownEventsVsOAPosVsTotalETrim_FDEvt_%d", i_iwritten);
       AllThrownEventsVsOAPosVsTotalETrim[i_iwritten] = new TH2D(NameAllThrownEventsVsOAPosVsTotalETrim, NameAllThrownEventsVsOAPosVsTotalETrim, nBinsEnergy, EnergyEdges, 65, -30.5, 2 );
       TString NameSelectedEventsAverageEfficiency = Form("SelectedEventsTwoDHisto_FDEvt_%d", i_iwritten);
       SelectedEventsVsOAPosVsTotalETrim[i_iwritten] = new TH2D(NameSelectedEventsAverageEfficiency, NameSelectedEventsAverageEfficiency, nBinsEnergy, EnergyEdges, 65, -30.5, 2 );
       TString NameSelectedEventsNoEvtRate = Form("SelectedEventsTwoDHisto_NoFDEvt_%d", i_iwritten);
       SelectedEventsVsOAPosVsTotalETrimNoEvtRate[i_iwritten] = new TH2D(NameSelectedEventsNoEvtRate, NameSelectedEventsNoEvtRate, nBinsEnergy, EnergyEdges, 65, -30.5, 2 );

       TString NameEtrimVsEvisTrueNoWeights = Form("EtrimVsEvisTrueNoWeights_%d", i_iwritten);
       EtrimVsEvisTrueNoWeights[i_iwritten] = new TH2D(NameEtrimVsEvisTrueNoWeights, NameEtrimVsEvisTrueNoWeights, nBinsEnergy, EnergyEdges, nBinsEnergy, EnergyEdges );
       TString NameEtrimVsEvisTrueWithPWeightMuon = Form("EtrimVsEvisTrueWithPWeightMuon_%d", i_iwritten);
       EtrimVsEvisTrueWithPWeightMuon[i_iwritten] = new TH2D(NameEtrimVsEvisTrueWithPWeightMuon, NameEtrimVsEvisTrueWithPWeightMuon, nBinsEnergy, EnergyEdges, nBinsEnergy, EnergyEdges );
       TString NameEtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon = Form("EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon_%d", i_iwritten);
       EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon[i_iwritten] = new TH2D(NameEtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon, NameEtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon, nBinsEnergy, EnergyEdges, nBinsEnergy, EnergyEdges );
       TString NameEtrimVsEvisTrueWithNDEventRateWithPWeightMuon = Form("EtrimVsEvisTrueWithNDEventRateWithPWeightMuon_%d", i_iwritten);
       EtrimVsEvisTrueWithNDEventRateWithPWeightMuon[i_iwritten] = new TH2D(NameEtrimVsEvisTrueWithNDEventRateWithPWeightMuon, NameEtrimVsEvisTrueWithNDEventRateWithPWeightMuon, nBinsEnergy, EnergyEdges, nBinsEnergy, EnergyEdges );
       TString NameEtrimVsEvisTrueWithLinComCoeffWithPWeightMuon = Form("EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon_%d", i_iwritten);
       EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon[i_iwritten] = new TH2D(NameEtrimVsEvisTrueWithLinComCoeffWithPWeightMuon, NameEtrimVsEvisTrueWithLinComCoeffWithPWeightMuon, nBinsEnergy, EnergyEdges, nBinsEnergy, EnergyEdges );

       Int_t n_plot = 0;
       Int_t i_n_plot = 0;
       Int_t i_vtxX_plot=0;
       Int_t iOAPostAtVtxX = 0;

       t_effMu->GetEntry(i_iwritten);
       t_effTree->GetEntry(i_iwritten);

       nPassThrowsPerEvent = 0;

       AllThrowInfo[i_iwritten].resize(a_ND_vtx_vx_vec.size());
         for (Double_t i_ND_LAr_vtx_pos: a_ND_vtx_vx_vec)
         {

           i_vtxX_plot +=1;


           TString HistVetoE_name = Form("HistVetoE_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
           HistVetoE[i_iwritten][i_vtxX_plot-1] = new TH1D(HistVetoE_name, HistVetoE_name, 60, 0, 60);
           TString HistEtrim_name = Form("HistEtrim_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
           HistEtrim[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEtrim_name, HistEtrim_name, 25000, 0, 25000);
           TString HistEtrimPmuWeighted_name = Form("HistEtrimPmuWeighted_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
           HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEtrimPmuWeighted_name, HistEtrimPmuWeighted_name, 25000, 0, 25000);
           //cross check histo filled fromm structure
           TString HistEtrimPmuWeighted_namecrossCheck = Form("HistEtrimPmuWeightedCrossCheck_FDEvt_%d_vtxXpost_%f", i_iwritten, i_ND_LAr_vtx_pos);
           HistEtrimPmuWeightedCrossCheck[i_iwritten][i_vtxX_plot-1] = new TH1D(HistEtrimPmuWeighted_namecrossCheck, HistEtrimPmuWeighted_namecrossCheck, 25000, 0, 25000);

           Int_t i_entry = tot_size * i_iwritten;

           for (i_entry ; i_entry < tot_size * (i_iwritten+1); i_entry++ )
           {
             t_effValues->GetEntry(i_entry);

             //cout<<" i_iwritten "<<i_iwritten<<" totEnergyFDatND_f " <<totEnergyFDatND_f<<endl;

             if ( ND_LAr_vtx_pos == i_ND_LAr_vtx_pos ){

               x_ND_LAr_vtx_pos[i_vtxX_plot-1] = ND_LAr_vtx_pos/100;
               y_geoeff[i_vtxX_plot-1] = ND_GeoEff;
               PlotEfficiencyVsVtxX[i_iwritten] = new TGraph(a_ND_vtx_vx_vec.size(), x_ND_LAr_vtx_pos, y_geoeff);
               // PlotMuonEfficiencyVsVtxX[i_iwritten] = new TGraph(a_ND_vtx_vx_vec.size(), x_ND_LAr_vtx_pos, muon_selected_eff->data());
               // PlotMuTrackerEfficiencyVsVtxX[i_iwritten] = new TGraph(a_ND_vtx_vx_vec.size(), x_ND_LAr_vtx_pos, muon_tracker_eff->data());
               PlotCombinedEfficiencyVsVtxX[i_iwritten] = new TGraph(a_ND_vtx_vx_vec.size(), x_ND_LAr_vtx_pos, combined_eff->data());

               int nthrowsToLoop = NPassedThrows; //this is going to be the validThrows
               const auto& throwList = AllThrowInfo[i_iwritten][i_vtxX_plot-1];

               PlotEfficiencyVsVtxX[i_iwritten]->SetTitle(Form("Total hadFD E = %.2f MeV, Muon E = %.2f, Enu = %.2f", totEnergyFDatND_f, TotalLeptonMom[i_iwritten], EnuTrue[i_iwritten]));
               //PlotMuonEfficiencyVsVtxX[i_iwritten]->SetTitle(Form("Total hadFD E = %.2f MeV, Muon E = %.2f, Enu = %.2f", totEnergyFDatND_f, TotalLeptonMom[i_iwritten], EnuTrue[i_iwritten]));
               // PlotMuTrackerEfficiencyVsVtxX[i_iwritten]->SetTitle(Form("Total hadFD E = %.2f MeV, Muon E = %.2f, Enu = %.2f", totEnergyFDatND_f, TotalLeptonMom[i_iwritten], EnuTrue[i_iwritten]));
               PlotCombinedEfficiencyVsVtxX[i_iwritten]->SetTitle(Form("Total hadFD E = %.2f MeV, Muon E = %.2f, Enu = %.2f", totEnergyFDatND_f, TotalLeptonMom[i_iwritten], EnuTrue[i_iwritten]));
               //===probably will need to scale to MuEff*ND_GeoEff/nthrowsToLoop
               // HistEtrim[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
               // HistEtrim[i_iwritten][i_vtxX_plot-1]->GetYaxis()->SetTitle("norm");
               // HistEtrim[i_iwritten][i_vtxX_plot-1]->GetXaxis()->SetTitle("had FD E_{trim} (MeV)");
               //
               // //Pmu weighted histEtrim:
               // HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1]->Scale(ND_GeoEff/nthrowsToLoop);
               // HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1]->GetYaxis()->SetTitle("norm");
               // HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1]->GetXaxis()->SetTitle("Vis E_{trim} (MeV)");
               // HistEtrim[i_iwritten][i_vtxX_plot-1]->Draw("hist");
               //here loop over DetPos here -> for now just assume same efficiency at every det position..
               Int_t i_detpos = 0;

               for (Double_t i_ND_LAr_dtctr_pos: a_ND_off_axis_pos_vec)
               {
                   i_detpos+=1;
                   iOAPostAtVtxX += 1;

                   // //calculate OAPos=vtx_x+det_pos
                   OAPos = ND_LAr_vtx_pos/100.0 + a_ND_off_axis_pos_vec[i_detpos-1];

                   y_geoEffOAPos[iOAPostAtVtxX-1] = ND_GeoEff;
                   X_OAPos[iOAPostAtVtxX-1] = OAPos;
                   EfficiencyVsOAPos[i_iwritten] =  new TGraph(ND_vtx_vx_vec_size * nDetPos, X_OAPos, y_geoEffOAPos);



                   TString HistEtrimDetPos_name = Form("HistEtrim_FDEvt_%d_vtxXpost_%f_DetPos_%f", i_iwritten, i_ND_LAr_vtx_pos, a_ND_off_axis_pos_vec[i_detpos-1] );
                   TString HistEtrimDetPosNoFDEventRate_name = Form("HistVisEtrimNoFDEvRate_FDEvt_%d_vtxXpost_%f_DetPos_%f", i_iwritten, i_ND_LAr_vtx_pos, a_ND_off_axis_pos_vec[i_detpos-1] );
                   TString HistEtrimDetPosWithFDEventRate_name = Form("HistEtrimDetPosWithFDEventRate_FDEvt_%d_vtxXpost_%f_DetPos_%f", i_iwritten, i_ND_LAr_vtx_pos, a_ND_off_axis_pos_vec[i_detpos-1] );
                   //same efficiency at all detecto positions means same events passing the cuts so same HisEtrim[i_vtxX_plot-1]

                   //uncomment this line and comment below if interested in lin combination of events with had eff only (no mu eff applied)
                   // if (!useCombinedEfficiency)
                   //   HistEtrimDetPos[i_iwritten][i_vtxX_plot-1][i_detpos-1] = (TH1D*) HistEtrim[i_iwritten][i_vtxX_plot-1]->Clone();
                   // else
                   //   HistEtrimDetPos[i_iwritten][i_vtxX_plot-1][i_detpos-1] = (TH1D*) HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1]->Clone();
                   //
                   // HistEtrimDetPos[i_iwritten][i_vtxX_plot-1][i_detpos-1]->SetName(HistEtrimDetPos_name);

                   HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1] = new TH1D(HistEtrimDetPosNoFDEventRate_name, HistEtrimDetPosNoFDEventRate_name, 25000, 0, 25000);
                   HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->SetName(HistEtrimDetPosNoFDEventRate_name);
                   HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1] = new TH1D(HistEtrimDetPosWithFDEventRate_name, HistEtrimDetPosWithFDEventRate_name, 25000, 0, 25000);
                   HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->SetName(HistEtrimDetPosWithFDEventRate_name);

                   // TString HistEtrimDetPosCoeff1_name = Form("HistEtrimCoeff1_FDEvt_%d_vtxXpost_%f_DetPos_%f", i_iwritten, i_ND_LAr_vtx_pos, a_ND_off_axis_pos_vec[i_detpos-1] );
                   // HistEtrimDetPosCoeff1[i_iwritten][i_vtxX_plot-1][i_detpos-1] = (TH1D*) HistEtrim[i_iwritten][i_vtxX_plot-1]->Clone();
                   // HistEtrimDetPosCoeff1[i_iwritten][i_vtxX_plot-1][i_detpos-1]->SetName(HistEtrimDetPosCoeff1_name);

                   CoefficientsAtOAPos = CoefficientsHist->GetBinContent(CoefficientsHist->FindBin(OAPos));
                   CoefficientsAtOAPosHist->Fill(OAPos, CoefficientsAtOAPos);
                   WeightEventsAtOaPos = HistOAPos[i_iwritten]->GetBinContent(HistOAPos[i_iwritten]->FindBin(OAPos));
                   if (OAPos > 0 && i_iwritten == 1) {
                      cout << "Current Event Number: " << i_iwritten << endl;
                      cout << "Current OAPos: " << OAPos << endl;
                      cout << "WeightEventsAtOaPos: " << WeightEventsAtOaPos << endl;
                    }

                   for (const auto& info : throwList) {

                      HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Fill(info.Etrim + info.Emu , info.weightPmuon * weightCAFLike[i_iwritten] ); //*FDEvatNDRate(info.Etrim, info.Emu, OAPos)
                      HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Fill(info.Etrim + info.Emu , info.weightPmuon * FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos)* weightCAFLike[i_iwritten] );//FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos)* weightCAFLike[i_iwritten] );

                      SelectedEventsVsOAPosVsTotalETrim[i_iwritten]->Fill((info.Etrim + info.Emu)/1000 ,OAPos, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten] *  FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos)); //FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos));
                      AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]->Fill((info.Etrim + info.Emu)/1000 , OAPos, double(validThrows)/throwList.size()* 1.0/WeightEventsAtOaPos * FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos)); //FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos));
                      SelectedEventsVsOAPosVsTotalETrimNoEvtRate[i_iwritten]->Fill((info.Etrim + info.Emu)/1000 ,OAPos, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten]);

                      EtrimVsEvisTrueNoWeights[i_iwritten]->Fill(VisEtrue[i_iwritten], (info.Etrim + info.Emu)/1000, 1.0/WeightEventsAtOaPos * weightCAFLike[i_iwritten]);
                      EtrimVsEvisTrueWithPWeightMuon[i_iwritten]->Fill(VisEtrue[i_iwritten], (info.Etrim + info.Emu)/1000, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten]);
                      EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon[i_iwritten]->Fill(VisEtrue[i_iwritten], (info.Etrim + info.Emu)/1000, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten] * CoefficientsAtOAPos * FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos));
                      EtrimVsEvisTrueWithNDEventRateWithPWeightMuon[i_iwritten]->Fill(VisEtrue[i_iwritten], (info.Etrim + info.Emu)/1000, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten] * FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos));
                      EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon[i_iwritten]->Fill(VisEtrue[i_iwritten], (info.Etrim + info.Emu)/1000, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten] * CoefficientsAtOAPos);

                   //   cout<<" rate "<< " Etrim " <<info.Etrim *1E-3<<" emu "<< info.Emu*1E-3<< "  OApos" <<OAPos<<" vtx x = "<<ND_LAr_vtx_pos/100.0 <<"det pos = "<<a_ND_off_axis_pos_vec[i_detpos-1] <<" rate: "<<FDEventRateAtND(cache, info.Etrim *1E-3 , info.Emu*1E-3, OAPos)<<" nthrowspass: "<< NPassedThrows<< " entries in histo = "<< HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->GetEntries()<<endl;
                   }
                  // cout<<" event nr = "<<i_iwritten<<" Enu = "<<EnuTrue[i_iwritten]<<" FD Event Rate at ND (Etrue) = "<<FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos) <<endl;

                   //====scale events to desired efficiency and apply lin. comb OA coefficients
                   HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Write(HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->GetName());
                   HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Write(HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->GetName());

                }//end LAr pos

                 // HistEtrim[i_iwritten][i_vtxX_plot-1]->Write(HistEtrim[i_iwritten][i_vtxX_plot-1]->GetName());
                 // HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1]->Write(HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1]->GetName());
                 // HistEtrimPmuWeightedCrossCheck[i_iwritten][i_vtxX_plot-1]->Write(HistEtrimPmuWeightedCrossCheck[i_iwritten][i_vtxX_plot-1]->GetName());
                 // delete HistEtrim[i_iwritten][i_vtxX_plot-1];
                 // delete HistEtrimPmuWeighted[i_iwritten][i_vtxX_plot-1];
                 // delete HistEtrimPmuWeightedCrossCheck[i_iwritten][i_vtxX_plot-1];


               //  cout<<" integral: "<<HistEtrim[i_iwritten][i_vtxX_plot-1]->Integral()<< " nd geo: "<<ND_GeoEff;
             }// end vtx selection
           }//end ientry

         }//end vtx pos inside LAr




       PlotEfficiencyVsVtxX[i_iwritten]->SetMarkerStyle(20);
       // PlotEfficiencyVsVtxX[i_iwritten]->Draw("AP");
       PlotEfficiencyVsVtxX[i_iwritten]->Write(Form("GraphEffficiency_FDEventNr_%d",i_iwritten));
       // PlotMuonEfficiencyVsVtxX[i_iwritten]->SetMarkerStyle(20);
       // PlotMuonEfficiencyVsVtxX[i_iwritten]->Write(Form("GraphMuffficiency_FDEventNr_%d",i_iwritten));
       // PlotMuTrackerEfficiencyVsVtxX[i_iwritten]->SetMarkerStyle(20);
       // PlotMuTrackerEfficiencyVsVtxX[i_iwritten]->Write(Form("GraphMuTrackerEffficiency_FDEventNr_%d",i_iwritten));
       PlotCombinedEfficiencyVsVtxX[i_iwritten]->SetMarkerStyle(20);
       PlotCombinedEfficiencyVsVtxX[i_iwritten]->Write(Form("GraphCombinedEffficiency_FDEventNr_%d",i_iwritten));
       EfficiencyVsOAPos[i_iwritten]->Write(Form("GraphEffficiency_AllNDDetPos_FDEventNr_%d",i_iwritten));
       //HistEtrimAllVtxX[i_iwritten]->Write(HistEtrimAllVtxX[i_iwritten]->GetName());
      // HistEtrimPmuWeightedAllVtxX[i_iwritten]->Write(HistEtrimPmuWeightedAllVtxX[i_iwritten]->GetName());

      //----add together all visEtrim (hadron and muon efficiency ) with OA coeffs applied at all vtxX and detPos -> final distribution of the FD event at ND
       HistEtrimAllVtxXTimesCoeff[i_iwritten] = (TH1D*)HistEtrimDetPosNoFDEventRate[i_iwritten][0][0]->Clone();
       HistEtrimAllVtxXTimesCoeff[i_iwritten]->Reset();
       HistEtrimAllVtxXTimesCoeff[i_iwritten]->SetName(HistEtrimAllVtxXTimesCoeff_name);
       HistEtrimAllVtxXTimesCoeff[i_iwritten]->SetTitle(Form("TotalFD Energy = %.2f MeV", totEnergyFDatND_f));

       for(int ivtxX = 0; ivtxX < nvtxXpositions; ivtxX++){
         for(int iDetPos = 0; iDetPos < nDetPos; iDetPos++){
           if(HistEtrimDetPosNoFDEventRate[i_iwritten][ivtxX][iDetPos]->GetEntries() > 0){
             HistEtrimAllVtxXTimesCoeff[i_iwritten]->Add(HistEtrimDetPosNoFDEventRate[i_iwritten][ivtxX][iDetPos]);
           }
               delete HistEtrimDetPosNoFDEventRate[i_iwritten][ivtxX][iDetPos];

         }
       }
       //scale to 1/nVtx and 1/dePos (to get average efficiency after summing over all vtxX and detPos ) - each Etrim hist [vtx][detpos] has integral = combined efficiency (vtxX) before applying OACoeffs
       HistEtrimAllVtxXTimesCoeff[i_iwritten]->Scale(1.0/ND_vtx_vx_vec_size);
       HistEtrimAllVtxXTimesCoeff[i_iwritten]->Scale(1.0/nDetPos);
       HistEtrimAllVtxXTimesCoeff[i_iwritten]->Write(HistEtrimAllVtxXTimesCoeff[i_iwritten]->GetName());

       cout<<" total DetPos: "<<nDetPos<<endl;
       //Get the oscillated spectrum: scale to Posc(Enu)
       HistEtrimAllVtxXTimesCoeffOscillated[i_iwritten] = (TH1D*)HistEtrimAllVtxXTimesCoeff[i_iwritten]->Clone();
       HistEtrimAllVtxXTimesCoeffOscillated[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       HistEtrimAllVtxXTimesCoeffOscillated[i_iwritten]->SetName( Form("NuOscHistEtrimPmuWeightedAllVtxXTimesCoeff_FDEvt_%d", i_iwritten));
       HistEtrimAllVtxXTimesCoeffOscillated[i_iwritten]->Write();

       //---add together all visEtrim (hadron and muon efficiency ) with OA coeffs applied and FD event rate at ND, at all vtxX and detPos -> final distribution of the FD event at ND(account for FD ev rate at ND)
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten] = (TH1D*) HistEtrimDetPosWithFDEventRate[i_iwritten][0][0]->Clone();
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->Reset();
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->SetName(HistEtrimAllVtxXTimesCoeff_FDEvRateAtND_name);
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->SetTitle(Form("Total hadronic FD Energy = %.2f MeV", totEnergyFDatND_f));

       for(int ivtxX = 0; ivtxX < nvtxXpositions; ivtxX++){
         for(int iDetPos = 0; iDetPos < nDetPos; iDetPos++){
           if(HistEtrimDetPosWithFDEventRate[i_iwritten][ivtxX][iDetPos]->GetEntries() > 0){
             HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->Add(HistEtrimDetPosWithFDEventRate[i_iwritten][ivtxX][iDetPos]);
           }
               delete HistEtrimDetPosWithFDEventRate[i_iwritten][ivtxX][iDetPos];

         }
       }

       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->Scale(1.0/ND_vtx_vx_vec_size);
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->Scale(1.0/nDetPos);
       HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->Write(HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->GetName());
       //get the oscillated spectrum: scale to Posc(Enu)
       HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[i_iwritten] = (TH1D*) HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]->Clone();
       HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       cout<< "aaaaaaa enu = "<<EnuTrue[i_iwritten]<<" P = "<<calc->P(14,14,EnuTrue[i_iwritten])<<endl;
       HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[i_iwritten]->SetName( Form("NuOscHistEtrimPmuWeightedAllVtxXTimesCoeffWithFDEvRateAtND_FDEvt_%d", i_iwritten));
       HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[i_iwritten]->Write();

       //get oscillated spectrum
       SelectedEventsVsOAPosVsTotalETrim[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       SelectedEventsVsOAPosVsTotalETrim[i_iwritten]->Write();
       AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]->Write();
       SelectedEventsVsOAPosVsTotalETrimNoEvtRate[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       SelectedEventsVsOAPosVsTotalETrimNoEvtRate[i_iwritten]->Write();

       EtrimVsEvisTrueNoWeights[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       EtrimVsEvisTrueNoWeights[i_iwritten]->Write();
       EtrimVsEvisTrueWithPWeightMuon[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       EtrimVsEvisTrueWithPWeightMuon[i_iwritten]->Write();
       EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon[i_iwritten]->Write();
       EtrimVsEvisTrueWithNDEventRateWithPWeightMuon[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       EtrimVsEvisTrueWithNDEventRateWithPWeightMuon[i_iwritten]->Write();
       EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
       EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon[i_iwritten]->Write();

       // cout<<" ndet pos = "<<nDetPos<<endl;

       // HistOAPos[i_iwritten]->Write(Form("HistOAPos_FDEvt_%d", i_iwritten));

       delete HistEtrimAllVtxXTimesCoeff[i_iwritten];
       delete HistEtrimAllVtxXTimesCoeffOscillated[i_iwritten];
       delete HistOAPos[i_iwritten];
       delete HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten];
       delete HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[i_iwritten];
       delete SelectedEventsVsOAPosVsTotalETrim[i_iwritten];
       delete AllThrownEventsVsOAPosVsTotalETrim[i_iwritten];
       delete SelectedEventsVsOAPosVsTotalETrimNoEvtRate[i_iwritten];

       delete EtrimVsEvisTrueNoWeights[i_iwritten];
       delete EtrimVsEvisTrueWithPWeightMuon[i_iwritten];
       delete EtrimVsEvisTrueWithNDEventRateWithLinComCoeffWithPWeightMuon[i_iwritten];
       delete EtrimVsEvisTrueWithNDEventRateWithPWeightMuon[i_iwritten];
       delete EtrimVsEvisTrueWithLinComCoeffWithPWeightMuon[i_iwritten];


     }//end iwritten

     // CoefficientsAtOAPosHist->Write("CoefficientsAtOAPosHist");
     CoefficientsHist->Write("CoefficientsHist");
   //  hist_FDTotEnergy->Scale(1.0/72); // we have 72 vtxX positions therefore instead of 1 event in the histo we have 72 entries
     hist_FDTotEnergy->Write("hist_FDTotEnergy");
     hist_muEdep->Write("hist_muEdep");
     hist_muTrackLength->Write("hist_muTrackLength");
     hist_TotalMuEnergy->Write("LepMomTot");
     hist_EnuFDEnergy->Write("hist_EnuFDEnergy");
     hist_visEnuFDEnergy->Write("hist_visEnuFDEnergy");
     hist_EnuFDEnergy_Osc->Write("Oschist_EnuFDEnergy");

     // HistOAPos->Draw("hist");

     //FileWithHistoInfo->Write();
     FileWithHistoInfo->Close();

     delete hist_FDTotEnergy;
     delete hist_muEdep;
     delete hist_muTrackLength;
     delete hist_TotalMuEnergy;
     delete hist_EnuFDEnergy;
     delete hist_visEnuFDEnergy;
     delete hist_EnuFDEnergy_Osc;

     cout<<" done, should close now"<<endl;




}

int main(int argc, char const *argv[]){

  if (argc < 3) {
  std::cerr << "Usage: " << argv[0] << " <file1.root> <file2.root>" << std::endl;
  return 1;
  }

  TFile *f1 = TFile::Open(argv[1]);
  TFile *f2 = TFile::Open(argv[2]);

  if (f1->IsZombie() || f2->IsZombie()) {
    std::cerr << "Error: Could not open one of the files!" << std::endl;
    return 1;
  }

  std::cout << "Processing files: " << argv[1] << " and " << argv[2] << std::endl;

  // Call the function for two files
  ProcessFile(f1, f2 );


}
