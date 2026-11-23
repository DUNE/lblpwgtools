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
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"


vector<vector<vector<double>>>* xyz_pos=nullptr;
vector<vector<vector<double>>>* xyz_mom=nullptr;
vector<vector<float>>* vetoEnergyFDatND=nullptr; // Veto E of FD events at ND <ND_LAr_pos < vtx_pos <vetoEnergyFDatND> > >
vector<vector<float>>* outEnergyFDatND=nullptr; // out E of FD events at ND <ND_LAr_pos < vtx_pos <outEnergyFDatND> > >
vector<vector<float>>* totEnergyFDatND=nullptr; // out E of FD events at ND <ND_LAr_pos < vtx_pos <totEnergyFDatND> > >

double lepnuangle=0.;
double numu_e=0;
double e_vis_true=0;
double hadW = 0;
double vetoEnergyFD=0;
float vetoEnergyFDatND_f = 0;
float outEnergyFDatND_f = 0;
float totEnergyFDatND_f = 0;

const int NUM_VTX=72;
const int NUM_LAR_DTR=3; // number of on/off axis position of LAr
// double LAr_position[NUM_LAR_DTR]={-2800.,-2575.,-2400.,-2175.,-2000.,-1775.,-1600.,-1375.,-1200.,-975.,-800.,-575.,-400.,-175.,0.};
double LAr_position[NUM_LAR_DTR]={-2800.,-1400.,0.};
// double vertex_position[NUM_VTX]={-299.,-292.,-285.,-278.,-271.,-264.,-216.,-168.,-120.,-72.,-24.,24.,72.,120.,168.,216.,264.,271.,278.,285.,292.,299.};
double vertex_position[NUM_VTX] = {
        -298.55, -291.55, -284.55, -277.55, -270.55, -263.55,
        -246.95, -239.95, -232.95, -225.95, -218.95, -211.95,
        -196.45, -189.45, -182.45, -175.45, -168.45, -161.45,
        -144.85, -137.85, -130.85, -123.85, -116.85, -109.85,
        -94.35, -87.35, -80.35, -73.35, -66.35, -59.35,
        -42.75, -35.75, -28.75, -21.75, -14.75, -7.75,
        7.75, 14.75, 21.75, 28.75, 35.75, 42.75,
        59.35, 66.35, 73.35, 80.35, 87.35, 94.35,
        109.85, 116.85, 123.85, 130.85, 137.85, 144.85,
        161.45, 168.45, 175.45, 182.45, 189.45, 196.45,
        211.95, 218.95, 225.95, 232.95, 239.95, 246.95,
        263.55, 270.55, 277.55, 284.55, 291.55, 298.55
    };
double total_detected[5][NUM_LAR_DTR][NUM_VTX]={};

struct Para
{
  char field[20];
  const char* units;
  double l;
  double h;
};

Para pr[]= //position is in units of cm, momentum is in units of GeV/c, angle is in units of rad, and energy is in  units of GeV //check if the TTrees have LepNuAngle
{ //match the x-ranges with the PRISM histograms' x-ranges
  {"LepMomTot","GeV", 0., 10.},
  {"cos_LepNuAngle"," ", 0., 1.},
  {"Ev", "GeV",0., 10.},// {"ND_Gen_numu_E", 0., 10.},
  {"E_vis_true", "GeV",0., 10.}// {"ND_E_vis_true", 0., 10.}
  // {"W", "GeV", 0., 10.}
};

struct sel_type
{
  const char* sel_name;
  const char* eff_name;
  vector<vector<double>>* eff_value=nullptr;
  sel_type() {}
  sel_type(const char* sn, const char* en)
  :sel_name(sn),eff_name(en) {}
};

vector<sel_type> br=
{
  sel_type("muon_contained", "muon_contained_eff"),
  sel_type("muon_tracker", "muon_tracker_eff"),
  sel_type("muon_selected", "muon_selected_eff"),
  sel_type("hadron_selected", "hadron_selected_eff"),
  sel_type("combined", "combined_eff")
};

void populate_histograms_FD(char* eff,char* caf,vector<vector<TH1D*>>& hists1,vector<vector<TH1D*>>& hists2, vector<vector<TH1D*>>& hists3, double geoeff_cut)
{
  TFile eff_file(eff);
  TFile caf_file(caf);
  TTree *event_data=(TTree*)eff_file.Get("event_data");
  TTree *thing=(TTree*)caf_file.Get("effTreeND");

  for(auto& sel:br) //efficiencies are in 3d array, but energy is in 1d array
  {
    event_data->SetBranchAddress(sel.eff_name, &(sel.eff_value));
  }
  thing->SetBranchAddress("ND_OffAxis_Sim_mu_start_v_xyz_LAr", &xyz_pos); //why is there no TBranch for angle in the new FD CAF files?
  thing->SetBranchAddress("ND_OffAxis_Sim_mu_start_p_xyz_LAr", &xyz_mom);
  thing->SetBranchAddress("ND_LepNuAngle", &lepnuangle);
  thing->SetBranchAddress("ND_Gen_numu_E", &numu_e);
  thing->SetBranchAddress("ND_E_vis_true", &e_vis_true);
  // thing->SetBranchAddress("ND_W", &hadW);
  thing->SetBranchAddress("vetoEnergyFD", &vetoEnergyFD);
  thing->SetBranchAddress("vetoEnergyFDatND", &vetoEnergyFDatND);
  thing->SetBranchAddress("outEnergyFDatND", &outEnergyFDatND);
  thing->SetBranchAddress("totEnergyFDatND", &totEnergyFDatND);

  Long64_t nentries1=event_data->GetEntries();
  Long64_t nentries2=thing->GetEntries();
  if (nentries1!=nentries2) {cout<<"The efficiency file "<<eff<<" has "<<nentries2<<" events, and the CAF file "<<caf<<" has "<<nentries1<<" events."<<endl;}
  for (int i=0;i<nentries2;i++) {

    event_data->GetEntry(i);
    thing->GetEntry(i);

    // unsigned long lar_pos=14; // on axis pos
    unsigned long lar_pos=2; // on axis pos
    e_vis_true += 0.10566; // 0.10566 is the mass of lepton. since we use only kinetic energy of lepton when reconstructing the e_vis_true

    int k=0;
    for (Para item:pr) {
      double var_type=0.0;

      for (unsigned long vtx_pos=0;vtx_pos<NUM_VTX;vtx_pos++) {
	      int n=0;
        int br_n = 0;
        for (auto& sel:br) {

          const char* dt=sel.sel_name;
          const char* fd=item.field;
          double lowerbound=item.l;
          double upperbound=item.h;

          TH1D* hist1=hists1[n][k];
          TH1D* hist2=hists2[n][k];
          TH1D* hist3=hists3[n][k];
          // cout << "k: " << k << ", n: " << n << ", vtx_pos: " << vtx_pos << ", pr name: " << fd << ", br name: " << dt << endl;

          n++;
          // if (vtx_pos==0||vtx_pos==1||vtx_pos==2||vtx_pos==4||vtx_pos==5||vtx_pos==16||vtx_pos==17||vtx_pos==19||vtx_pos==20||vtx_pos==21) continue;
          //
          // // only pick center region: vtx_pos at -24cm and 24cm
          // if (vtx_pos != 10 && vtx_pos != 11) continue;
          // if (abs(vertex_position[vtx_pos])>50) continue;
          // cout << "vtx_pos: " << vertex_position[vtx_pos] << endl;

          if (k==0) var_type=sqrt(pow((*xyz_mom)[lar_pos][vtx_pos][0],2)+pow((*xyz_mom)[lar_pos][vtx_pos][1],2)+pow((*xyz_mom)[lar_pos][vtx_pos][2],2));
          else if (k==1) var_type=cos(lepnuangle);
          else if (k==2) var_type=numu_e;
          else if (k==3) var_type=e_vis_true;
          // else if (k==3) var_type=hadW;

          br_n++;

          vector<vector<double>>* eff_value=sel.eff_value;
          vector<vector<double>>& eff_value2=*eff_value;

          vetoEnergyFDatND_f = (*vetoEnergyFDatND)[lar_pos][vtx_pos];
          outEnergyFDatND_f = (*outEnergyFDatND)[lar_pos][vtx_pos];
          totEnergyFDatND_f = (*totEnergyFDatND)[lar_pos][vtx_pos];

          double geo_eff=eff_value2[lar_pos][vtx_pos];
          if (geo_eff>1.) {cout<<"eff>1 !!! efficiency of event "<<i<<" at position "<<LAr_position[lar_pos]<<", "<<vertex_position[vtx_pos]<<" is "<<geo_eff<<endl;}
          // else {cout<<"efficiency of event "<<i<<" at LAr position: "<<LAr_position[lar_pos]<<", vtx: "<<vertex_position[vtx_pos]<<" is "<<geo_eff<<endl;}
          // cout << "br_n: "<< br_n <<", br name: " << dt << ",vetoEnergyFD: " << vetoEnergyFD << endl;

          // Get rid of the overflow
          if (var_type > upperbound) continue;
          // else{cout << "br name: " << dt << ",vetoEnergyFD: " << vetoEnergyFD << endl;}


          // if (abs(vertex_position[vtx_pos])<250)
          // {
          //   hist1->Fill(var_type); // red: raw
          // }else
          // {
          //   hist1->Fill(var_type, (1./6)); // red: raw
          // }

          // hist1->Fill(var_type); // red: raw

          // Add FDveto cut for selected hadrons
          // if (br_n==4 && vetoEnergyFD >30){
          //   // cout << "br name: " << dt << ",vetoEnergyFD: " << vetoEnergyFD << endl;
          //   continue;
          // }

          // if(outEnergyFDatND_f!=0) continue;
          // Fill after FD veto cut


          hist1->Fill(var_type); // red: raw


          if( geo_eff >= geoeff_cut)
          {
            // if(br_n==4)
            // {
            //   // if(outEnergyFDatND_f!=0) continue;
            //   // cout << "br name: " << dt << ",vetoEnergyFDatND: " << vetoEnergyFDatND_f << endl;
            //   if (vetoEnergyFDatND_f < 30) hist2->Fill(var_type,geo_eff); // green: mockND
            //   // if(outEnergyFDatND_f/totEnergyFDatND_f>0.05) continue;
            //   if (vetoEnergyFDatND_f < 30) hist3->Fill(var_type,1./geo_eff); // blue: target
            // }else if(br_n!=4)
            // {
            //   hist2->Fill(var_type, geo_eff); // green: mockND
            //   hist3->Fill(var_type); // blue: target
            // }
            hist2->Fill(var_type, geo_eff); // green: mockND
            hist3->Fill(var_type); // blue: target
          }

        }// end br loop
      } // end vtx_pos loop
      k++;
    }// end para item:pr loop
  }
  eff_file.Close();
  caf_file.Close();
}

void NDaFD_RatioPlots_FNAL(double geoeff_cut)
{
  // vector<double> geoeff_cut_threshold = {0.08};
  // vector<double> geoeff_cut_threshold = {0.1, 0.08, 0.06, 0.04, 0.02, 0.01};

  // for (double geoeff_cut:geoeff_cut_threshold)
  // {

    br[0].sel_name = "muon_contained"; // Have to initialize the br first!!! Not sure the reason, but keep it!!!
    TH1::AddDirectory(kFALSE); // Avoid Using Global Directory and potential memory leak
    // FD files
    char eff[9999];
    char caf[9999];
    memset(eff, 0, 9999); // clear array each time
    memset(caf, 0, 9999);
    gSystem->Exec("rm -f AutoDict*vector*vector*vector*double*"); // Remove old dictionary if exists
    gInterpreter->GenerateDictionary("vector<vector<vector<double>>>", "vector");

    vector<vector<TH1D*>> histograms1;
    vector<vector<TH1D*>> histograms2;
    vector<vector<TH1D*>> histograms3;
    histograms1.clear();
    histograms2.clear();
    histograms3.clear();

    for(auto& sel:br)
    {
      const char* dt=sel.sel_name;
      vector<TH1D*> histset1, histset2,histset3;
      histset1.clear();
      histset2.clear();
      histset3.clear();
      histograms1.push_back(histset1);
      histograms2.push_back(histset2);
      histograms3.push_back(histset3);
      int i=0;
      for (Para item:pr)
      {
        double lowerbound=item.l;
        double upperbound=item.h;
        histograms1.back().push_back(new TH1D(Form("%s_hist_%d",dt,i), Form("raw %s  %d", dt, i), 200, lowerbound, upperbound));
        histograms2.back().push_back(new TH1D(Form("%s_hist_%d",dt,i), Form("mockND %s %d", dt, i), 200, lowerbound, upperbound));
        histograms3.back().push_back(new TH1D(Form("%s_hist_%d",dt,i), Form("target %s %d", dt, i), 200, lowerbound, upperbound));
      i++;
      }
    }



    // for (int j=0; j<100; j++)
    for (int j=0; j<8000; j++)
    {
      memset(eff, 0, 9999); // clear array each time
      memset(caf, 0, 9999);
      sprintf(eff, "/pnfs/dune/persistent/users/flynnguo/NDeff_FDhadron/FDGeoEff_12749829/FDGeoEff_12749829_%d_Eff.root", j);
      sprintf(caf, "/pnfs/dune/persistent/users/flynnguo/FDGeoEffinND/FDGeoEff_12749829/FDGeoEff_12749829_%d.root", j);

      std::cout << "geoeff_cut: " << geoeff_cut << ", j: " << j << ", eff: " << eff << ", caf:" << caf<< std::endl;

      if(access(eff, 0)==0)
      {
        cout<<"Process file" << endl;
        populate_histograms_FD(eff,caf,histograms1,histograms2,histograms3,geoeff_cut);
      } else {
        cout<<"Warning: missing file:"<<eff<<endl;
        continue;
      }
      cout<< "break"<< endl;
    }

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // Read ND files:
    char raw_path[999];
    char sel_path[999];
    char geo_path[999];
    memset(raw_path, 0, 999); //clear array each time
    memset(sel_path, 0, 999);
    memset(geo_path, 0, 999);
    TFile* raw_files[9];
    TFile* sel_files[45];
    TFile* geo_files[45];
    TH1D* raw_histograms[9];
    TH1D* sel_histograms[45];
    TH1D* geo_histograms[45];
    int index=0;

    // br[0].sel_name = "muon_contained"; // Have to initialize the br first!!! Not sure the reason, but keep it!!!

    for(auto sel:br)
    {
      const char* dt=sel.sel_name;
      for (Para item:pr)
      {
        const char *fd=item.field;

        sprintf(sel_path,"/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s/selection-cut_%s_%s.root",geoeff_cut,fd,dt,fd);
        sel_files[index]=new TFile(sel_path, "read");
        sel_histograms[index]=(TH1D*)sel_files[index]->Get(Form("selection-cut_%s_%s",dt,fd));

        sprintf(geo_path,"/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s/geo-corrected_%s_%s.root",geoeff_cut,fd,dt,fd);
        geo_files[index]=new TFile(geo_path, "read");
        geo_histograms[index]=(TH1D*)geo_files[index]->Get(Form("geo-corrected_%s_%s",dt,fd));
        index++;
      }
    }

    int index_raw = 0;
    for (Para item:pr)
    {
      const char *fd=item.field;
      sprintf(raw_path,"/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/%.3f_eff_veto_cut_ND/%s/raw_%s.root",geoeff_cut,fd,fd);
      raw_files[index_raw]=new TFile(raw_path, "read");
      raw_histograms[index_raw]=(TH1D*)raw_files[index_raw]->Get(Form("raw_%s",fd));
      index_raw++;
    }

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    gStyle->SetOptStat(000000000);


    // Create a folder before drawing plots
    gSystem->mkdir(TString::Format("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/ND_noTRIMplots_ICHP/%.3f_eff_veto_cut", geoeff_cut), kTRUE);

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // Create hist arrays
    // Create Canvas
    TCanvas *c_ratio[5];

    TPad** uppad = new TPad*[5];
    TLegend** uppad_L = new TLegend*[5];
    TLegend** dnpad_L = new TLegend*[5];
    TPad** dnpad = new TPad*[5];
    TH1D** ratio_ND_GvR = new TH1D*[25];
    TH1D** ratio_ND_SvR = new TH1D*[25];
    TH1D** ratio_ND_SvG = new TH1D*[25];
    TH1D** ratio_FD_TvR = new TH1D*[25];
    TH1D** ratio_FD_SvR = new TH1D*[25];
    TH1D** ratio_FD_SvT = new TH1D*[25];
    TH1D** ratioNDvFD_1 = new TH1D*[25]; // ND: GvR vs FD: TvR
    TH1D** ratioNDvFD_2 = new TH1D*[25]; // ND: SvR vs FD: SvR
    TH1D** ratioNDvFD_3 = new TH1D*[25]; // ND: SvG vs FD: SvT
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // Draw ND plots
    TCanvas *cs_ND[5];
    TCanvas *rs_ND[5];
    index=0;
    int i_ND=0;

    for(auto sel:br)
    {
      const char *dt=sel.sel_name;
      cs_ND[i_ND]=new TCanvas(Form("c%01d",i_ND+1),dt,3600, 2000);
      cs_ND[i_ND]->Divide(2,2);
      rs_ND[i_ND]=new TCanvas(Form("r%01d",i_ND+1),Form("%s ratios",dt),3600, 2000);
      rs_ND[i_ND]->Divide(2,2);

      for(int k=0;k<4;k++)
      {
        Para item=pr[k];
        double lowerbound=item.l;
        double upperbound=item.h;
        const char *fd=item.field;
        const char *var_unit=item.units;
        cs_ND[i_ND]->cd(k+1);
        TH1D *hist3=geo_histograms[index];
        hist3->SetLineColor(kBlue);
        hist3->Draw("histS");
        TH1D *hist2=sel_histograms[index];
        //hist2->SetLineColor(kGreen);
        hist2->SetLineColor(kTeal+10);
        hist2->Draw("samehistS");
        TH1D *hist1=raw_histograms[int(index%4)];
        hist1->SetLineColor(kPink);
        hist1->Draw("samehistS");

        float max1=hist1->GetMaximum();
        float max2=hist2->GetMaximum();
        float max3=hist3->GetMaximum();
        // float upper_y_bound=max(max(max2,max3), max1)*1.26;
        float upper_y_bound=max1*1.26;
        hist3->SetAxisRange(lowerbound,upperbound,"X");
        hist3->SetAxisRange(0.,upper_y_bound,"Y");
        hist3->SetTitle(Form("%s: %s",fd,dt));
        hist3->GetXaxis()->SetTitle(Form("%s [%s]",fd,var_unit));
        hist3->GetYaxis()->SetTitle("# of events");
        TLegend *leg=new TLegend(0.1,0.75,0.33,0.9);
        // leg->SetHeader("comparison");
        leg->AddEntry(hist1, "raw distribution");
        leg->AddEntry(hist2, "selection-cut distribution");
        leg->AddEntry(hist3, "geo corrected distribution");
        leg->Draw();

        if (k==1) {hist3->SetAxisRange(0.01,upper_y_bound,"Y"); gPad->SetLogy(1);} // set cos_LepNuAngle to be log
        gPad->Update();


        rs_ND[i_ND]->cd(k+1);
        TH1D *rplot1=(TH1D*)hist3->Clone();
        rplot1->Divide(hist1);
        rplot1->GetYaxis()->SetTitle("ratio");
        rplot1->SetAxisRange(lowerbound,upperbound,"X");
        rplot1->SetAxisRange(0.,1.3,"Y");
        rplot1->SetLineColor(kViolet-3);
        rplot1->Draw("hist");
        TH1D *rplot2=(TH1D*)hist2->Clone();
        rplot2->Divide(hist1);
        rplot2->SetLineColor(kOrange+7);
        rplot2->Draw("samehist");
        TH1D *rplot3=(TH1D*)hist2->Clone();
        rplot3->Divide(hist3);
        rplot3->SetLineColor(kCyan);
        rplot3->Draw("samehist");
        TLegend *rleg=new TLegend(0.1,0.77,0.4,0.9);
        // rleg->SetHeader("comparison");
        rleg->AddEntry(rplot1, "geo vs raw");
        rleg->AddEntry(rplot2, "sel vs raw");
        rleg->AddEntry(rplot3, "sel vs geo");
        rleg->Draw();

        int ratio_ND_GvR_index=i_ND*5+k;
        ratio_ND_GvR[ratio_ND_GvR_index]=(TH1D*)hist3->Clone();
        ratio_ND_GvR[ratio_ND_GvR_index]->Divide(hist1);
        ratio_ND_SvR[ratio_ND_GvR_index]=(TH1D*)hist2->Clone();
        ratio_ND_SvR[ratio_ND_GvR_index]->Divide(hist1);
        ratio_ND_SvG[ratio_ND_GvR_index]=(TH1D*)hist2->Clone();
        ratio_ND_SvG[ratio_ND_GvR_index]->Divide(hist3);


        index++;

      }
      cs_ND[i_ND]->Update();
      cs_ND[i_ND]->SaveAs(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/ND_noTRIMplots_ICHP/%.3f_eff_veto_cut/ND_0mgsimple_%s_PRISM_%.3f_eff_veto_cut_ND_hists_200_bins.png",geoeff_cut,dt,geoeff_cut));
      rs_ND[i_ND]->Update();
      rs_ND[i_ND]->SaveAs(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/ND_noTRIMplots_ICHP/%.3f_eff_veto_cut/ND_0mgsimple_%s_PRISM_%.3f_eff_veto_cut_ND_hists_200_bins_ratios.png",geoeff_cut,dt,geoeff_cut));;

      i_ND++;
    } // end ND plots;
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // Draw FD
    TCanvas *cs[5];
    TCanvas *rs[5];
    int i=1;
    for (auto& sel:br)
    {
      const char *dt=sel.sel_name;
      cs[i-1]=new TCanvas(Form("c%01d",i),dt,3600, 2000);
      cs[i-1]->Divide(2,2);
      rs[i-1]=new TCanvas(Form("r%01d",i),dt,3600, 2000);
      rs[i-1]->Divide(2,2);

      int n=0;
      for(Para& item:pr)
      {
        const char *fd=item.field;
        double lowerbound=item.l;
        double upperbound=item.h;
        const char *var_unit=item.units;
        // TVirtualPad *pads=cs[i-1]->cd(n+1);
        cs[i-1]->cd(n+1);
        TH1D *hist3=histograms3[i-1][n];
        hist3->SetLineColor(kBlue);
        hist3->Draw("hist");
        TH1D *hist2=histograms2[i-1][n];
        hist2->SetLineColor(kGreen);
        hist2->Draw("samehist");
        TH1D *hist1=histograms1[i-1][n];
        hist1->SetLineColor(kRed);
        hist1->Draw("samehist");

        float max1=hist1->GetMaximum();
        float max2=hist2->GetMaximum();
        float max3=hist3->GetMaximum();
        // float upper_y_bound=max(max(max2,max3), max1)*1.26;
        float upper_y_bound=max1*1.26;
        hist3->SetAxisRange(lowerbound,upperbound,"X");
        hist3->SetAxisRange(0.,upper_y_bound,"Y");
        hist3->SetTitle(Form("%s: %s",fd,dt));
        hist3->GetXaxis()->SetTitle(fd);
        hist3->GetYaxis()->SetTitle("# of events");
        TLegend *leg=new TLegend(0.1,0.75,0.33,0.9);
        // leg->SetHeader("comparison");
        leg->AddEntry(hist1, "raw distribution");
        leg->AddEntry(hist2, "mockND distribution");
        leg->AddEntry(hist3, "target distribution");
        leg->Draw();

        if (n==1) {hist3->SetAxisRange(0.01,upper_y_bound,"Y"); gPad->SetLogy(1);} // set cos_LepNuAngle to be log
        gPad->Update();

        rs[i-1]->cd(n+1);
        TH1D *rplot1=(TH1D*)hist3->Clone();
        rplot1->Divide(hist1);
        rplot1->SetAxisRange(lowerbound,upperbound,"X");
        rplot1->SetAxisRange(0.,1.3,"Y");
        rplot1->SetLineColor(kViolet-3);
        rplot1->GetXaxis()->SetTitle(fd);
        rplot1->GetYaxis()->SetTitle("ratio");
        rplot1->Draw("hist");
        TH1D *rplot2=(TH1D*)hist2->Clone();
        rplot2->Divide(hist1);
        rplot2->SetLineColor(kOrange+7);
        rplot2->Draw("samehist");
        TH1D *rplot3=(TH1D*)hist2->Clone();
        rplot3->Divide(hist3);
        rplot3->SetLineColor(kCyan);
        rplot3->Draw("samehist");
        TLegend *rleg=new TLegend(0.1,0.77,0.4,0.9);
        // rleg->SetHeader("comparison");
        rleg->AddEntry(rplot1, "targeted vs raw");
        rleg->AddEntry(rplot2, "mockND vs raw");
        rleg->AddEntry(rplot3, "mockND vs targeted");
        rleg->Draw();


        int ratio_FD_TvR_index=(i-1)*5+n;
        ratio_FD_TvR[ratio_FD_TvR_index]=(TH1D*)hist3->Clone();
        ratio_FD_TvR[ratio_FD_TvR_index]->Divide(hist1);
        ratio_FD_SvR[ratio_FD_TvR_index]=(TH1D*)hist2->Clone();
        ratio_FD_SvR[ratio_FD_TvR_index]->Divide(hist1);
        ratio_FD_SvT[ratio_FD_TvR_index]=(TH1D*)hist2->Clone();
        ratio_FD_SvT[ratio_FD_TvR_index]->Divide(hist3);

        n++;
      }
      cs[i-1]->Update();
      rs[i-1]->Update();
      cs[i-1]->SaveAs(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/ND_noTRIMplots_ICHP/%.3f_eff_veto_cut/FD_1760931_%s_less_edge_pos_%.3f_eff_cut_hists.png", geoeff_cut,dt,geoeff_cut));
      rs[i-1]->SaveAs(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/ND_noTRIMplots_ICHP/%.3f_eff_veto_cut/FD_1760931_%s_less_edge_pos_%.3f_eff_cut_hists_ratios.png", geoeff_cut,dt,geoeff_cut));

      i++;
    }// end br loop
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // Draw NDvFD ratio plots
    int i_NDvFD=0;
    for (auto& sel:br)
    {

      const char *dt=sel.sel_name;
      c_ratio[i_NDvFD]=new TCanvas(Form("NDvFDr%01d",i_NDvFD+1),Form("%s NDvFD_ratio",dt),2400,2000);
      c_ratio[i_NDvFD]->Divide(2,2);

      for(int k=0;k<4;k++)
      {
        Para item=pr[k];
        double lowerbound=item.l;
        double upperbound=item.h;
        // if (k==0) upperbound = 10.; // set the range of LepMomTot to be 0 to 10
        const char *fd=item.field;
        const char *var_unit=item.units;

        c_ratio[i_NDvFD]->cd(k+1);
        uppad[i_NDvFD] = new TPad("uppad", "", 0, 0.4, 1, 1.0); // xlow, ylow, xup, yup
        uppad[i_NDvFD]->SetBottomMargin(0);
        uppad[i_NDvFD]->Draw();
        uppad[i_NDvFD]->cd();

        int NDvFD_index=i_NDvFD*5+k;
        cout << "NDvFD_index: " << NDvFD_index << endl;

        // draw ND ratio: geo vs raw
        ratio_ND_GvR[NDvFD_index]->SetLineColor(kBlue);
        ratio_ND_GvR[NDvFD_index]->GetYaxis()->SetTitle("Ratio");
        ratio_ND_GvR[NDvFD_index]->SetAxisRange(0.,1.4,"Y");
        ratio_ND_GvR[NDvFD_index]->Draw("hist");
        // draw ND ratio: sel vs raw
        // ratio_ND_SvR[NDvFD_index]->SetLineColor(kGreen+4);
        // ratio_ND_SvR[NDvFD_index]->Draw("samehist");
        // draw ND ratio: sel vs geo
        // ratio_ND_SvG[NDvFD_index]->SetLineColor(kBlue);
        // ratio_ND_SvG[NDvFD_index]->Draw("samehist");
        // draw FD ratio: targeted vs raw
        ratio_FD_TvR[NDvFD_index]->SetLineColor(kCyan+1);
        ratio_FD_TvR[NDvFD_index]->Draw("samehist");
        // draw FD ratio: selected vs raw
        // ratio_FD_SvR[NDvFD_index]->SetLineColor(kGreen);
        // ratio_FD_SvR[NDvFD_index]->Draw("samehist");
        // draw FD ratio: selected vs targeted
        // ratio_FD_SvT[NDvFD_index]->SetLineColor(kCyan+1);
        // ratio_FD_SvT[NDvFD_index]->Draw("samehist");


        // Draw legend
        uppad_L[i_NDvFD] = new TLegend(0.6,0.7,0.9,0.9);
        uppad_L[i_NDvFD]->SetTextSize(0.035);
        uppad_L[i_NDvFD]->AddEntry(ratio_ND_GvR[NDvFD_index],TString::Format("ND: geo-corrected vs raw"),"l");
        // uppad_L[i_NDvFD]->AddEntry(ratio_ND_SvR[NDvFD_index],TString::Format("ND: selected vs raw"),"l");
        // uppad_L[i_NDvFD]->AddEntry(ratio_ND_SvG[NDvFD_index],TString::Format("ND: selected vs geo-corrected"),"l");
        uppad_L[i_NDvFD]->AddEntry(ratio_FD_TvR[NDvFD_index],TString::Format("FD: targeted vs raw"),"l");
        // uppad_L[i_NDvFD]->AddEntry(ratio_FD_SvR[NDvFD_index],TString::Format("FD: mockND vs raw"),"l");
        // uppad_L[i_NDvFD]->AddEntry(ratio_FD_SvT[NDvFD_index],TString::Format("FD: selected vs target"),"l");
        uppad_L[i_NDvFD]->Draw();

        c_ratio[i_NDvFD]->cd(k+1);
        dnpad[i_NDvFD] = new TPad("dnpad", "", 0, 0.05, 1, 0.4);
        dnpad[i_NDvFD]->SetTopMargin(0);
        dnpad[i_NDvFD]->Draw();
        dnpad[i_NDvFD]->cd();

        // ND: GvR vs FD: TvR
        ratioNDvFD_1[NDvFD_index] = (TH1D*)ratio_ND_GvR[NDvFD_index]->Clone();
        ratioNDvFD_1[NDvFD_index]->Divide(ratio_FD_TvR[NDvFD_index]);
        ratioNDvFD_1[NDvFD_index]->SetLineColor(kBlue);
        ratioNDvFD_1[NDvFD_index]->SetLineStyle(1);
        ratioNDvFD_1[NDvFD_index]->SetMinimum(0.7);
        ratioNDvFD_1[NDvFD_index]->SetMaximum(1.3);
        // ratioNDvFD_1[NDvFD_index]->Sumw2(); //Create structure to store sum of squares of weights.
        ratioNDvFD_1[NDvFD_index]->SetStats(0);
        // ratioNDvFD_1[NDvFD_index]->SetLineWidth(0); // 0: No error bars; 1: error bars
        ratioNDvFD_1[NDvFD_index]->SetTitle("");
        ratioNDvFD_1[NDvFD_index]->SetAxisRange(lowerbound,upperbound,"X");
        ratioNDvFD_1[NDvFD_index]->GetXaxis()->SetTitle(Form("%s [%s]",fd,var_unit));
        // ratioNDvFD_1[NDvFD_index]->GetXaxis()->SetTitleSize(10);
        // ratioNDvFD_1[NDvFD_index]->GetXaxis()->SetTitleFont(43);
        // ratioNDvFD_1[NDvFD_index]->GetXaxis()->SetTitleOffset(10);
        // ratioNDvFD_1[NDvFD_index]->GetXaxis()->SetLabelSize(10); // Labels will be 15 pixels
        // ratioNDvFD_1[NDvFD_index]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        // ratioNDvFD_1[NDvFD_index]->SetAxisRange(0.,2.,"Y");
        ratioNDvFD_1[NDvFD_index]->GetYaxis()->SetTitle("ND vs FD");
        // ratioNDvFD_1[NDvFD_index]->GetYaxis()->SetTitleSize(10);
        // ratioNDvFD_1[NDvFD_index]->GetYaxis()->SetTitleFont(43);
        // ratioNDvFD_1[NDvFD_index]->GetYaxis()->SetTitleOffset(6);
        // ratioNDvFD_1[NDvFD_index]->GetYaxis()->SetLabelSize(10);
        // ratioNDvFD_1[NDvFD_index]->GetYaxis()->SetLabelFont(43);
        ratioNDvFD_1[NDvFD_index]->SetMarkerStyle(21);
        ratioNDvFD_1[NDvFD_index]->SetMarkerColor(kBlue); // 1: black; 3: green; 6: pink
        ratioNDvFD_1[NDvFD_index]->SetMarkerSize(0.5);// 0.4: w/ error bars; 0.5: w/o error bars
        ratioNDvFD_1[NDvFD_index]->Draw("HIST L P");
        //------------------------------------------------------------------------
        // ND: SvR vs FD: SvR
        // ratioNDvFD_2[NDvFD_index] = (TH1D*)ratio_ND_SvR[NDvFD_index]->Clone();
        // ratioNDvFD_2[NDvFD_index]->Divide(ratio_FD_SvR[NDvFD_index]);
        // ratioNDvFD_2[NDvFD_index]->SetLineColor(kGreen);
        // ratioNDvFD_2[NDvFD_index]->SetLineStyle(1);
        // ratioNDvFD_2[NDvFD_index]->SetMinimum(0.7);
        // ratioNDvFD_2[NDvFD_index]->SetMaximum(1.3);
        // // ratioNDvFD_2[NDvFD_index]->Sumw2(); //Create structure to store sum of squares of weights.
        // ratioNDvFD_2[NDvFD_index]->SetStats(0);
        // // ratioNDvFD_2[NDvFD_index]->SetLineWidth(0); // 0: No error bars; 1: error bars
        // ratioNDvFD_2[NDvFD_index]->SetMarkerStyle(21);
        // ratioNDvFD_2[NDvFD_index]->SetMarkerColor(kGreen); // 1: black; 3: green; 6: pink
        // ratioNDvFD_2[NDvFD_index]->SetMarkerSize(0.5);// 0.4: w/ error bars; 0.5: w/o error bars
        // ratioNDvFD_2[NDvFD_index]->Draw("HIST L P SAME");
        //------------------------------------------------------------------------
        // ND: SvG vs FD: SvT
        // ratioNDvFD_3[NDvFD_index] = (TH1D*)ratio_ND_SvG[NDvFD_index]->Clone();
        // ratioNDvFD_3[NDvFD_index]->Divide(ratio_FD_SvT[NDvFD_index]);
        // ratioNDvFD_3[NDvFD_index]->SetLineColor(kBlue);
        // ratioNDvFD_3[NDvFD_index]->SetLineStyle(1);
        // ratioNDvFD_3[NDvFD_index]->SetMinimum(0.);
        // ratioNDvFD_3[NDvFD_index]->SetMaximum(2.);
        // // ratioNDvFD_3[NDvFD_index]->Sumw2(); //Create structure to store sum of squares of weights.
        // ratioNDvFD_3[NDvFD_index]->SetStats(0);
        // // ratioNDvFD_3[NDvFD_index]->SetLineWidth(0); // 0: No error bars; 1: error bars
        // ratioNDvFD_3[NDvFD_index]->SetMarkerStyle(21);
        // ratioNDvFD_3[NDvFD_index]->SetMarkerColor(kBlue); // 1: black; 3: green; 6: pink
        // ratioNDvFD_3[NDvFD_index]->SetMarkerSize(0.5);// 0.4: w/ error bars; 0.5: w/o error bars
        // ratioNDvFD_3[NDvFD_index]->Draw("HIST L P SAME");

        dnpad_L[i_NDvFD] = new TLegend(0.75,0.1,0.9,0.25);
        dnpad_L[i_NDvFD]->SetTextSize(0.04);
        dnpad_L[i_NDvFD]->AddEntry(ratioNDvFD_1[NDvFD_index],TString::Format("ND_GvR vs FD_TvR"),"l");
        // dnpad_L[i_NDvFD]->AddEntry(ratioNDvFD_2[NDvFD_index],TString::Format("ND_SvR vs FD_MvR"),"l");
        // dnpad_L[i_NDvFD]->AddEntry(ratioNDvFD_3[NDvFD_index],TString::Format("ND_SvG vs FD_SvT"),"l");
        dnpad_L[i_NDvFD]->Draw();

        TLine *line = new TLine(lowerbound, 1, upperbound, 1);
        line->SetLineColor(kBlack);  // Set line color to red (or any color of your choice)
        line->SetLineWidth(1);     // Set line width
        line->Draw();


        gPad->Update();
        gPad->Modified();
        gSystem->ProcessEvents();

      }// end para loop

      c_ratio[i_NDvFD]->Update();
      c_ratio[i_NDvFD]->SaveAs(Form("/exp/dune/app/users/flynnguo/NDeff_muon_Plots/0mgsimple_hadronselect_all_0mgsimple_woEhadcut/ND_noTRIMplots_ICHP/%.3f_eff_veto_cut/NDvFD_0mgsimple_%s_PRISM_%.3f_eff_veto_cut_ND_hists_200_bins_ratios.png",geoeff_cut,dt,geoeff_cut));


      i_NDvFD++;
    }// end br loop

    // delete hist var
    histograms1.clear();
    histograms2.clear();
    histograms3.clear();

    delete[] uppad;
    delete[] dnpad;
    delete[] uppad_L;
    delete[] dnpad_L;
    delete[] ratio_ND_GvR;
    delete[] ratio_ND_SvR;
    delete[] ratio_ND_SvG;
    delete[] ratio_FD_TvR;
    delete[] ratio_FD_SvR;
    delete[] ratio_FD_SvT;
    delete[] ratioNDvFD_1;
    delete[] ratioNDvFD_2;
    delete[] ratioNDvFD_3;
  // } //end geoeff_cut_threshold

} // end NDaFD_RatioPlots
