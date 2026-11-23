// C++ includes
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>

using namespace std;

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TBranch.h"
#include "TSystem.h"

struct Para
{
  char field[20];
  double l;
  double h;
  double* field_value;
};

struct Sel_type
{
  const char* sel_name;
  const char* eff_name;
  bool calced=false;
  double* sel_value;
  double* eff_value;
  Sel_type() {}
  Sel_type(const char* sn, const char* en, bool c, double* sv, double* ev)
  :sel_name(sn),eff_name(en),calced(c),sel_value(sv),eff_value(ev) {}
};

double muon_cont, muon_tra, muon_sel, hadr, comb;
double muon_cont_eff, muon_tra_eff, muon_sel_eff, hadr_eff, comb_eff;
double x_pos, y_pos, z_pos, XLepMom, YLepMom, ZLepMom, Ehad_veto;
double TotalMom, cos_angle, LongitudinalMom;
double E_vis_true, ev, hadW;
const char* list_of_directories[40]={"0mgsimple","0m","1.75m","2m","4m","5.75m","8m","9.75m","12m","13.75m","16m","17.75m","20m","21.75m","24m","25.75m","26.75m","28m",
"28.25m","28.5m","0mgsimpleRHC","0mRHC","1.75mRHC","2mRHC","4mRHC","5.75mRHC","8mRHC","9.75mRHC","12mRHC","13.75mRHC","16mRHC","17.75mRHC","20mRHC","21.75mRHC","24mRHC",
"25.75mRHC","26.75mRHC","28mRHC","28.25mRHC","28.5mRHC"};
const int NUM_FIELDS=4;

Para pr[]= //position is in units of cm, momentum is in units of GeV/c, angle is in units of rad, and energy is in  units of GeV
{
  {"LepMomTot", 0., 10.,&TotalMom},
  {"cos_LepNuAngle", 0., 1.,&cos_angle},
  {"Ev", 0., 10., &ev},
  {"E_vis_true", 0., 10., &E_vis_true}
  // {"W", 0., 10., &hadW}
};

vector<Sel_type> br=
{
  Sel_type("muon_contained", "muon_contained_eff", false, &muon_cont, &muon_cont_eff),
  Sel_type("muon_tracker", "muon_tracker_eff", false, &muon_tra, &muon_tra_eff),
  Sel_type("muon_selected", "muon_sel_eff", true, &muon_sel, &muon_sel_eff),
  Sel_type("hadron_selected", "hadron_selected_eff", false, &hadr, &hadr_eff ),
  Sel_type("combined", "combined_eff", false, &comb, &comb_eff)
};

void histogram_files_ND_FNAL()
{
  // vector<double> geoeff_cut_threshold = {0.04,0.1,0.2,0.3};
  vector<double> geoeff_cut_threshold = {0.04, 0.1, 0.2};
  // vector<double> geoeff_cut_threshold = {0.3, 0.25, 0.2, 0.15, 0.1, 0.08, 0.04, 0.01, 0.0};

  for (double geoeff_cut:geoeff_cut_threshold)
  {

    cout << "geoeff_cut: " << geoeff_cut << endl;

    // setup plots
    vector<TH1D*> histograms1, histograms2, histograms3;
    histograms1.clear();
    histograms2.clear();
    histograms3.clear();
    TH1::AddDirectory(false);

    br[0].sel_name = "muon_contained"; // Have to initialize the br first!!! Not sure the reason, but keep it!!!

    int first_pass=0;
    for(auto sel:br)
    {
      const char* dt=sel.sel_name;
      // cout << "sel_name: " << dt << ", sel_val: " << sel.sel_value << ", eff_name: " << sel.eff_name << ", eff_val: " << sel.eff_value << endl;
      for(auto item:pr)
      {
        char *fd=item.field;
        double l=item.l;
        double h=item.h; //insert 11 check
        if (first_pass<NUM_FIELDS) histograms1.push_back(new TH1D(Form("raw_%s", fd), Form("raw %s", fd), 200, l, h)); //remove dt from name
        histograms2.push_back(new TH1D(Form("selection-cut_%s_%s", dt, fd), Form("selected %s %s", dt, fd), 200, l, h));
        histograms3.push_back(new TH1D(Form("geo-corrected_%s_%s", dt, fd), Form("geo corrected %s %s", dt, fd), 200, l, h));
      }
      first_pass+=1;
    }

    // Generate the required root files
    // Input FDroot file
    TString FileIn = "/pnfs/dune/persistent/users/flynnguo/NDeff_muon/0mgsimple/NDGeoEff_0mgsimple.root";
    // Read
    TChain *event_data = new TChain("event_data");
    event_data->Add(FileIn.Data());
    Long64_t nentries=event_data->GetEntries();
    cout << "nentries: " << nentries << endl;

    // Read four invariant variables
    for (auto item:pr)
    {
      event_data->SetBranchAddress(item.field, item.field_value);
    }

    for(auto sel:br)
    {

      if(sel.calced) continue;

      event_data->SetBranchAddress(sel.sel_name, sel.sel_value);
      // cout << "sel.sel_name: " << sel.sel_name << ", value: " << *(sel.sel_value) << endl;
      event_data->SetBranchAddress(sel.eff_name, sel.eff_value);
    }

    event_data->SetBranchAddress("muon_selected",&muon_sel);
    event_data->SetBranchAddress("vtx_x", &x_pos);
    event_data->SetBranchAddress("Ehad_veto", &Ehad_veto);

    // Fill plots

      int veto_number = 0;
    // for (int i=0;i<6482016;i++)
    for (int i=0;i<nentries;i++)
    {

      event_data->GetEntry(i);

      // only pick center region
      // if ((abs(x_pos) > 26 || abs(x_pos) < 22) && (abs(x_pos) < 70 || abs(x_pos) >74)) continue;  // Skip values outside the (-50, 50) range
      // if (abs(x_pos) > 50) continue;  // Skip values outside the (-50, 50) range
      cout <<  "i_entry: " << i  << ", x_pos: " << x_pos << endl;
      //
      // cout << "i_entry: " << i << endl;
      // cout << "LepE: " << LepE << ", eP: " << eP << ", ePip: " << ePip << ", ePim: " << ePim << ", ePi0: " << ePi0 << ", eOther: " << eOther << ", nipi0: " << nipi0 << endl;
      // cout << "E_vis_true_int: " << E_vis_true << endl;

      //calculation for the muon-selected cut
      // const double epsilon = 1e-8; // Threshold
      //
      // // Apply the threshold check to muon_cont and muon_tra
      // double adjusted_muon_cont = std::abs(muon_cont) < epsilon ? 0.0 : muon_cont;
      // double adjusted_muon_tra = std::abs(muon_tra) < epsilon ? 0.0 : muon_tra;
      //
      // // Now calculate muon_sel with adjusted values
      // muon_sel = adjusted_muon_cont + adjusted_muon_tra;

      // Check for bad value
      // if (muon_sel != 0 && muon_sel != 1) {
      //     cout << "bad val for muon-selected check! " << muon_sel << endl;
      //     // additional handling...
      // }

      // muon_sel=muon_cont+muon_tra;
      // cout<<"muon_sel: " << muon_cont + muon_tra << endl;
      muon_sel_eff=muon_cont_eff+muon_tra_eff;
      // Chek if muon_sel is neither 0 nor 1.
      // if (muon_sel!=0&&muon_sel!=1)
      // // if (muon_sel > 1 || muon_sel < 0)
      // {
      //   cout<<"bad val for muon-selected check! "<<muon_sel<<endl<<". Event # "<<i<<", contained: "<<muon_cont<<", tracker-matched: "<<muon_tra<<endl;
      //   continue;
      // }
      //
      // cout << "geoeff_cut: " << geoeff_cut << ", ientry: " << i << ", muon_sel: " << muon_sel << ", combined: " << comb << endl;
      int n=0;
      int br_n = 0;
      for (auto sel:br)
      {
        const char* dt=sel.sel_name;
        // cout << "br n: " << br_n << ",name: " << dt << endl;
        // if (Ehad_veto > 30) cout << "i: " << i << ", Ehad_veto > 30" << endl;
        int pr_n =0;


        for (auto item:pr)
        {
          const char *fd=item.field;
          double geo_eff=*sel.eff_value;
          double l=item.l;
          double h=item.h;

          pr_n++;


          TH1D* hist1;
          if (n<NUM_FIELDS) hist1=histograms1.at(n);
          TH1D* hist2=histograms2.at(n);
          TH1D* hist3=histograms3.at(n);


          if (n<NUM_FIELDS && *item.field_value<h) hist1->Fill(*item.field_value, 1); // Raw
          n++;

          // if (br_n==3 && Ehad_veto > 30 ) {
          //   // if (pr_n ==3 && *item.field_value<1)
          //   // {
          //     // cout << "i: " << i << ", geoeff_cut: " << geoeff_cut << ", geo_eff: " << geo_eff << ",name: " << dt << ", pr_name: " << fd << ", value: " << *item.field_value << ", Ehad_veto: " << Ehad_veto << ", vtx_x: " << x_pos<< endl;
          //   // }
          //   cout << "i: " << i << ", geoeff_cut: " << geoeff_cut << ", geo_eff: " << geo_eff << ",name: " << dt << ", Ehad_veto: " << Ehad_veto << ", vtx_x: " << x_pos<< endl;
          //
          //   veto_number ++;
          //   continue;
          // }

          if( geo_eff >= geoeff_cut && *item.field_value < h)
          {
            if(geoeff_cut != 0)
            {
              hist2->Fill(*item.field_value, *sel.sel_value); // Sel
              hist3->Fill(*item.field_value, *sel.sel_value/geo_eff); // Geo-corrected
            }
            else
            {
              hist2->Fill(*item.field_value, *sel.sel_value); // Sel
              hist3->Fill(*item.field_value, *sel.sel_value); // Geo-corrected
            }
          }

        }// end pr loop
        br_n++;
      }// end br loop


    }
      cout << " veto_number: " << veto_number << ", i: " << nentries << ", ratio: " << veto_number*1.0/nentries << endl;


    TFile *raw_files[NUM_FIELDS];
    TFile *sel_files[int(NUM_FIELDS*5)];
    TFile *geo_files[int(NUM_FIELDS*5)];
    int index=0;
    for (auto sel:br) {
      const char *dt=sel.sel_name;
      for (Para item:pr) {
        const char *fd=item.field;

        // Create a folder before writting root file
        gSystem->mkdir(TString::Format("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s", geoeff_cut,fd), kTRUE); //  means only choose events w/ geoeff >=0

        if (index<NUM_FIELDS) {
          raw_files[index]=new TFile(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s/raw_%s.root",geoeff_cut, fd,fd),"recreate");
          TH1D* raw_hist=histograms1.at(index);
          raw_hist->Write();
          raw_files[index]->Close();
        }

        sel_files[index]=new TFile(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s/selection-cut_%s_%s.root",geoeff_cut,fd,dt,fd),"recreate");
        TH1D* sel_hist=histograms2.at(index);
        sel_hist->Write();
        sel_files[index]->Close();

        geo_files[index]=new TFile(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s/geo-corrected_%s_%s.root",geoeff_cut,fd,dt,fd),"recreate");
        TH1D* geo_hist=histograms3.at(index);
        geo_hist->Write();
        geo_files[index]->Close();
        index++;
      }
    }// end for (auto sel:br)

    histograms1.clear();
    histograms2.clear();
    histograms3.clear();

  }// end geoeff_cut loop

}
