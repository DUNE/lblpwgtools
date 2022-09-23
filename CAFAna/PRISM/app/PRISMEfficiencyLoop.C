#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/Cuts.h"
#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TStyle.h"
#include "TTree.h"

#include <cassert>
#include <iostream>
#include <string>

//----------------------------------------------------------------------
// Helper function that can give us a friendlier error message
template <class T>
bool SetBranchChecked(TTree *tr, const std::string &bname, T *dest) {
  if (tr->FindBranch(bname.c_str())) {
    tr->SetBranchAddress(bname.c_str(), dest);
    return true;
  } else {
    std::cout << "Warning: Branch '" << bname
              << "' not found, field will not be filled" << std::endl;
  }
  return false;
}

int main(int argc, char const *argv[]) {

// old error, from when running over a single ND CAF file rather then a whole bunch of ND CAf files.  Need to input several input CAFFiles. 
//  if (argc != 2) {
//    std::cout << "[ERROR]: Expected to be passed a single input CAFFile"
//              << std::endl;
//  }

// from ciaran
//  std::vector<std::string> infilenames;
//  for (int i = 1; i < argc; i++) {
//    std::string str(argv[i]);
//    infilenames.push_back(str);
//  }

  ana::Cut IsNue = ana::kActuallyIsNueCC;
  ana::Cut IsSignal = ana::kIsTrueFV && PRISM::kIsOutOfTheDesert && IsNue; //all nues, total true nues //like unselected 
  ana::Cut IsSignalSelected = IsSignal && ana::kPassND_FHC_NUE; //selected nues  // compare to selected in state file one
  
  TH2D *sig = new TH2D("sig", ";enu (GeV);offaxis_x (m);count", 30, 0, 10,
                       64, -30, 2);
			//64 * 2, -30, 2);
                       //64 * 2, 2, -30);
  TH2D *sig_sel = new TH2D("sig_sel", ";enu (GeV);offaxis_x (m);count", 30, 0,
                           10, 64, -30, 2);
                           //10, 64 * 2, -30, 2);
                           //10, 64 * 2, 2, -30);

  std::vector<std::string> infilenames;
  for (int i = 1; i < argc; i++) {
    std::string str(argv[i]);
    infilenames.push_back(str);
    TFile *f = TFile::Open(argv[i]);
    
    TTree *tr;
    tr = (TTree *)f->Get("caf");
    if (!tr) {
      tr = (TTree *)f->Get("cafTree");
    }
    assert(tr);

    Long64_t Nent = tr->GetEntries();

    caf::StandardRecord sr;
    std::cout<<"end of caf loop line 65"<<std::endl;

    
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
    SetBranchChecked(tr, "niem", &sr.niem);
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
    std::cout<<"after SetBrachCheckeds"<<std::endl;
    // } // previous end of for loop 
  

  //  ana::Cut IsNue = ana::kActuallyIsNueCC;
  //  ana::Cut IsSignal = ana::kIsTrueFV && PRISM::kIsOutOfTheDesert && IsNue; //
  //  ana::Cut IsSignalSelected = IsSignal && ana::kPassND_FHC_NUE;
  //eran need add here IsNDNue, IsNDNueSignal??  not ana::Cut though???
  
    for (Long64_t ent = 0; ent < Nent; ++ent) {
      tr->GetEntry(ent);
      
      if (!(ent % 1000)) {
	std::cout << "\r[READ] " << ent << "/" << Nent << std::flush;
      }
      
      if (!IsNue((caf::SRProxy *)&sr)) {
	continue;
      }
      
      std::cout << "\rEntry: " << ent << ", nuPDG: " << sr.nuPDG
		<< ", Enu = " << sr.Ev << ", vtx_x: " << sr.vtx_x
		<< ", det_x: " << sr.det_x
		<< "\n\tinFV: " << ana::kIsTrueFV((caf::SRProxy *)&sr)
              << ", outofdesert: "
		<< PRISM::kIsOutOfTheDesert((caf::SRProxy *)&sr)
		<< ", NDNueSig: " << IsSignal((caf::SRProxy *)&sr)
		<< ", NDNueSel: " << IsSignalSelected((caf::SRProxy *)&sr)
		<< std::endl;
      
      //maybe do if IsSignal, then Fill the histogram, only do it once becuse all of this in the for loop for the nbumber of entries.  If passes the cut, then fill in the histogram 
      //last part of Fill is usually weight?
//sr.Ev energy, sr.det_x position of the center OA  vtx_x, is vertex x so together abs. off axis postion,
      sig->Fill(sr.Ev, (sr.det_x + sr.vtx_x) * 1E-2, //det_x and vtx_x both in cm?
		IsSignal((caf::SRProxy *)&sr));
      sig_sel->Fill(sr.Ev, (sr.det_x + sr.vtx_x) * 1E-2,
		    IsSignalSelected((caf::SRProxy *)&sr));
    }
    std::cout << "\r[READ] " << Nent << "/" << Nent << std::endl;
    f->Close();
  }// new end of for loop, ACTUAL end of for loop over filenames, 220914


  gStyle->SetOptStat(false);
  TCanvas *c1 = new TCanvas("c1", "");
  //TCanvas *c1 = new TCanvas("c1", "");
  TCanvas *c2 = new TCanvas ("c2", "");
  TCanvas *c3 = new TCanvas ("c3", "");
  c2->cd();
  sig->Draw("COLZ");
  c2->Print("220921_sig_allCAF_2.pdf");
  c3->cd();
  sig_sel->Draw("COLZ");
  c3->Print("220921_sig_sel_allCAF_2.pdf");
  c1->cd();
  //sig->SetAxisRange(-2., 30., "Y");
  //sig_sel->GetYaxis()->SetRange(-2, 30);
  sig_sel->Divide(sig);
  sig_sel->Draw("COLZ");
  c1->Print("220921_nueeff_allCAF_2.pdf");

  std::cout<<"right before rename plot"<<std::endl;
//  c1->Print("220915_NUEEFF_allCAF.pdf");
//  c2->Print("220915_SIG_allCAF.pdf");
//  c3->Print("220915_SIG_SEL_allCAF.pdf");
  std::cout<<"right after rename plot"<<std::endl;

  //}// don't put end here // new of for loop
}
