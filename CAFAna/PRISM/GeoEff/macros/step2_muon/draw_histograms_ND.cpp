#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <filesystem>
using namespace std;
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"

struct Para
{
  //static constexpr const char *const S;
  //constexpr const *char , VTX_X="vtx_x", *VTX_Y="vtx_y", *VTX_Z="vtx_z";
  //const char *LMX="LepMomX", *LMY="LepMomY", *LMZ="LepMomZ";
  char field[20];
  const char* units;
  bool iscaf;
  double l;
  double h;
  double* field_value;
};

struct Sel_type
{
  const char* sel_name;
  const char* eff_name;
  bool calced=false;
  int* sel_value;
  double* eff_value;
  Sel_type() {}
  Sel_type(const char* sn, const char* en, bool c, int* sv, double* ev)
  :sel_name(sn),eff_name(en),calced(c),sel_value(sv),eff_value(ev) {}
};

int muon_cont, muon_tra, muon_sel, hadr, comb;
double muon_cont_eff, muon_tra_eff, muon_sel_eff, hadr_eff, comb_eff;
double x_pos, y_pos, z_pos, XLepMom, YLepMom, ZLepMom;
double TotalMom, cos_angle, LongitudinalMom;
double E_vis_true, ev;
const char* list_of_directories[40]={"0mgsimple","0m","1.75m","2m","4m","5.75m","8m","9.75m","12m","13.75m","16m","17.75m","20m","21.75m","24m","25.75m","26.75m","28m",
"28.25m","28.5m","0mgsimpleRHC","0mRHC","1.75mRHC","2mRHC","4mRHC","5.75mRHC","8mRHC","9.75mRHC","12mRHC","13.75mRHC","16mRHC","17.75mRHC","20mRHC","21.75mRHC","24mRHC",
"25.75mRHC","26.75mRHC","28mRHC","28.25mRHC","28.5mRHC"};

Para pr[]= //position is in units of cm, momentum is in units of GeV/c, angle is in units of rad, and energy is in  units of GeV
{ //match the x-ranges with the FD histograms' x-ranges
  // {"vtx_x", "cm", true, -300., 300., &x_pos},
  // {"vtx_y", "cm", true, -100., 100., &y_pos},
  // {"vtx_z", "cm", true, 50., 350., &z_pos},
  // {"LepMomX", "GeV", true, -2., 2., &XLepMom},
  // {"LepMomY", "GeV", true, -4.5, 2., &YLepMom},
  // {"LepMomZ", "GeV", true, -0.5, 7., &ZLepMom},
  {"LepMomTot", "GeV", false, 0., 10., &TotalMom},
  {"cos_LepNuAngle", "", false, 0., 1., &cos_angle},
  // {"LongMom", "GeV", false, -1., 7., &LongitudinalMom},
  {"Ev", "GeV", true, 0., 10., &ev},
  {"E_vis_true", "GeV", false, 0., 10., &E_vis_true}

};

vector<Sel_type> br=
{
  Sel_type("muon_contained", "muon_contained_eff", false, &muon_cont, &muon_cont_eff),
  Sel_type("muon_tracker", "muon_tracker_eff", false, &muon_tra, &muon_tra_eff),
  Sel_type("muon_selected", "muon_sel_eff", true, &muon_sel, &muon_sel_eff),
  Sel_type("hadron_selected", "hadron_selected_eff", false, &hadr, &hadr_eff ),
  Sel_type("combined", "combined_eff", false, &comb, &comb_eff)
};

void draw_histograms_ND(double geoeff_cut)
{
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
  for(auto sel:br)
  {
    const char* dt=sel.sel_name;
    for (Para item:pr)
    {
      const char *fd=item.field;

      sprintf(sel_path,"/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%0.3f_eff_veto_cut_ND/%s/selection-cut_%s_%s.root",geoeff_cut,fd,dt,fd);
      sel_files[index]=new TFile(sel_path, "read");
      sel_histograms[index]=(TH1D*)sel_files[index]->Get(Form("selection-cut_%s_%s",dt,fd));
      sprintf(geo_path,"/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%0.3f_eff_veto_cut_ND/%s/geo-corrected_%s_%s.root",geoeff_cut,fd,dt,fd);
      geo_files[index]=new TFile(geo_path, "read");
      geo_histograms[index]=(TH1D*)geo_files[index]->Get(Form("geo-corrected_%s_%s",dt,fd));
      index++;
    }
  }
  int index_raw = 0;
  for (Para item:pr)
  {
    const char *fd=item.field;
    sprintf(raw_path,"/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%0.3f_eff_veto_cut_ND/%s/raw_%s.root",geoeff_cut,fd,fd);
    raw_files[index_raw]=new TFile(raw_path, "read");
    raw_histograms[index_raw]=(TH1D*)raw_files[index_raw]->Get(Form("raw_%s",fd));
    index_raw++;
  }

  gStyle->SetOptStat(000000000);

  // Create a folder before drawing plots
  gSystem->mkdir(TString::Format("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/ratio_test/%.3f_eff_veto_cut_ND", geoeff_cut), kTRUE);

  TCanvas *cs[5];
  TCanvas *rs[5];
  index=0;
  int i=0;

  for(auto sel:br)
  {
    const char *dt=sel.sel_name;
    cs[i]=new TCanvas(Form("c%01d",i+1),dt,2700, 1500);
    cs[i]->Divide(2,2);
    rs[i]=new TCanvas(Form("r%01d",i+1),Form("%s ratios",dt),2700, 1500);
    rs[i]->Divide(2,2);
    for(int k=0;k<4;k++)
    {
      Para item=pr[k];
      double lowerbound=item.l;
      double upperbound=item.h;
      const char *fd=item.field;
      const char *var_unit=item.units;
      TVirtualPad *p=cs[i]->cd(k+1);
      // if (k==1) {p->SetLogy();} //pad needs to be made logarithmic, not canvas
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
      float upper_y_bound=max(max(max2,max3), max1)*1.1;
      hist3->SetAxisRange(lowerbound,upperbound,"X");
      hist3->SetTitle(Form("%s: %s",fd,dt));
      hist3->GetXaxis()->SetTitle(Form("%s (%s)",fd,var_unit));
      hist3->GetYaxis()->SetTitle("# of events");
      TLegend *leg=new TLegend(0.1,0.75,0.33,0.9);
      leg->SetHeader("comparison");
      leg->AddEntry(hist1, "raw distribution");
      leg->AddEntry(hist2, "selection-cut distribution");
      leg->AddEntry(hist3, "geo corrected distribution");
      leg->Draw();

      if (k==1) {hist3->SetAxisRange(0.01,upper_y_bound,"Y"); gPad->SetLogy(1);} // set cos_LepNuAngle to be log
      gPad->Update();

      rs[i]->cd(k+1);
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
      rleg->SetHeader("comparison");
      rleg->AddEntry(rplot1, "geo vs raw");
      rleg->AddEntry(rplot2, "sel vs raw");
      rleg->AddEntry(rplot3, "sel vs geo");
      rleg->Draw();
      // TPaveStats *prs;
      // prs=(TPaveStats*)rplot->GetListOfFunctions()->FindObject("stats");
      // prs->SetFillStyle(0);
      index++;
    }
    cs[i]->Update();
    cs[i]->SaveAs(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/ratio_test/%0.3f_eff_veto_cut_ND/0mgsimple_%s_PRISM_%0.3f_eff_veto_cut_ND_hists_200_bins.png",geoeff_cut,dt,geoeff_cut));
    rs[i]->Update();
    rs[i]->SaveAs(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/ratio_test/%0.3f_eff_veto_cut_ND/0mgsimple_%s_PRISM_%0.3f_eff_veto_cut_ND_hists_200_bins_ratios.png",geoeff_cut,dt,geoeff_cut));;
    i++;
  }
}
