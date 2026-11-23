#include <iostream>
#include <stdio.h>
#include <cstring>
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
#include "TBranch.h"
#include "TSystem.h"

struct Para
{
  //static constexpr const char *const S;
  //constexpr const *char , VTX_X="vtx_x", *VTX_Y="vtx_y", *VTX_Z="vtx_z";
  //const char *LMX="LepMomX", *LMY="LepMomY", *LMZ="LepMomZ";
  char field[20];
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
const int NUM_FIELDS=4;

Para pr[]= //position is in units of cm, momentum is in units of GeV/c, angle is in units of rad, and energy is in  units of GeV
{
  // {"vtx_x", true, -300., 300. , &x_pos},
  // {"vtx_y", true, -100., 100., &y_pos},
  // {"vtx_z", true, 50., 350., &z_pos},
  // {"LepMomX", true, -3., 3., &XLepMom},
  // {"LepMomY", true, -4.5, 2., &YLepMom},
  // {"LepMomZ", true, -1., 14, &ZLepMom},
  {"LepMomTot", false, 0., 10.,&TotalMom},
  {"cos_LepNuAngle", false, 0., 1.,&cos_angle},
  // {"LongMom", false, -1., 16.,&LongitudinalMom},
  {"Ev", true, 0., 10., &ev},
  {"E_vis_true", false, 0., 10., &E_vis_true}
};

vector<Sel_type> br=
{
  Sel_type("muon_contained", "muon_contained_eff", false, &muon_cont, &muon_cont_eff),
  Sel_type("muon_tracker", "muon_tracker_eff", false, &muon_tra, &muon_tra_eff),
  Sel_type("muon_selected", "muon_sel_eff", true, &muon_sel, &muon_sel_eff),
  Sel_type("hadron_selected", "hadron_selected_eff", false, &hadr, &hadr_eff ),
  Sel_type("combined", "combined_eff", false, &comb, &comb_eff)
};

void histogram_files_ND()
{
  vector<int> file = {104, 105, 106, 107, 108, 109, 110, 111, 117, 118, 119, 120, 121, 122, 123, 124, 125};
  string baseDir = "/home/fyguo/testbaroncode/0mgsimple/";

  vector<double> geoeff_cut_threshold = {0.0, 0.005, 0.01, 0.1};

  for (int filenum: file)
  {
    for (double geoeff_cut:geoeff_cut_threshold)
    {

      gROOT->SetBatch(kTRUE);
      char eff[999];
      char caf[999];
      vector<TH1D*> histograms1, histograms2, histograms3;
      histograms1.clear();
      histograms2.clear();
      histograms3.clear();

      int first_pass=0;
      for(auto sel:br)
      {
        const char* dt=sel.sel_name;
        for(auto item:pr)
        {
          char *fd=item.field;
          double l=item.l;
          double h=item.h; //insert 11 check
          if (first_pass<NUM_FIELDS) histograms1.push_back(new TH1D(Form("raw_%s", fd), Form("raw %s", fd), 200, l, h)); //remove dt from name
          histograms2.push_back(new TH1D(Form("selection-cut_%s_%s", dt, fd), Form("selected %s %s", dt, fd), 200, l, h));
          histograms3.push_back(new TH1D(Form("geo-corrected_%s_%s", dt, fd), Form("geo corrected %s %s", dt, fd), 200, l, h));
        }
        first_pass=1;
      }

      // Find the directory of CAF file
      std::string dir_str = baseDir + std::to_string(filenum);
      const char* inDir = dir_str.c_str();
      // const char* inDir = "/home/fyguo/testbaroncode/0mgsimple/104";

      char* dir = gSystem->ExpandPathName(inDir);
      void* dirp = gSystem->OpenDirectory(dir);

      const char* entry;
      const char* filename_eff[10000];
      const char* filename_caf[10000];
      Int_t n = 0;
      TString str;
      const char* ext = ".CAF_Eff.root";

      while((entry = (char*)gSystem->GetDirEntry(dirp))) {
        str = entry;
        if(str.EndsWith(ext))
        {
            // Read string to filename_eff
            filename_eff[n++] = gSystem->ConcatFileName(dir, entry);
        }// end if str.EndsWith(ext)
      }


      gSystem->FreeDirectory(dirp);

      // Remove some characters
      const char* prefixToRemove = "/home/fyguo/testbaroncode/";
      const char* suffixToRemove = "_Eff.root";

      for (Int_t i = 0; i < n; i++){
        Printf("file -> %s", filename_eff[i]);
        // Delete the "_Eff" and "/home/fyguo/testbaroncode/" in the eff filename then pass it to filename_caf
        if (filename_eff[i]) { // check if it's not NULL
                const char* startPointer = strstr(filename_eff[i], prefixToRemove);
                if (startPointer) {
                    startPointer += strlen(prefixToRemove);
                    char* newStr = new char[strlen(filename_eff[i]) + 1]; // Allocate memory for new string
                    strcpy(newStr, startPointer); // Copy the part after prefix

                    char* endPointer = strstr(newStr, suffixToRemove);
                    if (endPointer) {
                        *endPointer = '\0'; // Set the end of the new string before the suffix
                    }
                    filename_caf[i] = newStr;
                } else {
                    cout << "geoeff_cut: " << geoeff_cut << ", i: " << i << ", .CAF not found"; // if no prefix is found, just copy the entire string
                }
            }
        // Printf("file -> %s", filename_caf[i]);
        // std::cout << "i: " << i << endl;
      }



      // Generate the required root files
      for (Int_t i = 0; i < n; i++)
      {
        memset(eff, 0, 999); // clear array each time
        memset(caf, 0, 999);
        // sprintf(eff,"/storage/shared/barwu/10thTry/combined1/0m/%02d/FHC.10%05d.CAF_Eff.root",j/1000,j);
        // sprintf(caf,"/storage/shared/wshi/CAFs/NDFHC_PRISM/%02d/FHC.10%05d.CAF.root",j/1000,j);
        sprintf(eff,"%s",filename_eff[i]);
        sprintf(caf,"/storage/shared/barwu/10thTry/NDCAF/%s.root",filename_caf[i]);

        std::cout <<  "geoeff_cut: " << geoeff_cut << ", i: " << i << ", eff: " << eff << ", caf:" << caf<< std::endl;

        if(access(eff, 0)!=0) {continue;}
        TFile eff_file(eff);
        TFile caf_file(caf);
        TTree *event_data=(TTree*)eff_file.Get("event_data");
        TTree *caf_tree=(TTree*)caf_file.Get("caf");

        int isCC=0, inFV=0;
        for(auto sel:br)
        {
          if(sel.calced) continue;
          event_data->SetBranchAddress(sel.sel_name, sel.sel_value);
          event_data->SetBranchAddress(sel.eff_name, sel.eff_value);
        }

        Double_t LepE = 0., eP = 0., ePip = 0., ePim = 0., ePi0 = 0., eOther = 0.;
        int nipi0 = 0;


        caf_tree->SetBranchAddress("isCC",&isCC);
        event_data->SetBranchAddress("inFV",&inFV);
        caf_tree->SetBranchAddress("LepE", &LepE);
        caf_tree->SetBranchAddress("eP", &eP);
        caf_tree->SetBranchAddress("ePip", &ePip);
        caf_tree->SetBranchAddress("ePim", &ePim);
        caf_tree->SetBranchAddress("ePi0", &ePi0);
        caf_tree->SetBranchAddress("eOther", &eOther);
        caf_tree->SetBranchAddress("nipi0", &nipi0);
        caf_tree->SetBranchAddress("vtx_x", &x_pos);

        double pi0_mass = 0.134977; // GeV


        int i_pr=0;
        for (auto item:pr)
        {
          if (i_pr==3)//  E_vis_true
          {
            *item.field_value=E_vis_true;
          }
          else if(i_pr==0)//  TotMom
          {
            TTree *tree=item.iscaf?caf_tree:event_data;
            tree->SetBranchAddress("TotMom", item.field_value);
          }
          else
          {
            TTree *tree=item.iscaf?caf_tree:event_data;
            tree->SetBranchAddress(item.field, item.field_value);
          }
          i_pr++;
        }

        //there are some non-CC events
        Long64_t nentries1=caf_tree->GetEntries();
        Long64_t nentries2=event_data->GetEntries();
        if (nentries1!=nentries2) {cout<<"The efficiency file "<<eff<<" has"<<nentries2<<" events, and the CAF file "<<caf<<" has"<<nentries1<<" events."<<endl;}
        for (int i=0;i<nentries2;i++) {
          caf_tree->GetEntry(i);
          event_data->GetEntry(i);

          // only pick center region
          // if (x_pos <= -50 || x_pos >= 50) continue;  // Skip values outside the (-50, 50) range
          // cout << "x_pos: " << x_pos << endl;
          //
          E_vis_true = LepE + eP + ePip + ePim + ePi0 + eOther + nipi0 * pi0_mass;

          // cout << "i_entry: " << i << endl;
          // cout << "LepE: " << LepE << ", eP: " << eP << ", ePip: " << ePip << ", ePim: " << ePim << ", ePi0: " << ePi0 << ", eOther: " << eOther << ", nipi0: " << nipi0 << endl;
          // cout << "E_vis_true_int: " << E_vis_true << endl;

          // int sel_cut=isCC*inFV;
          // if (sel_cut==0) {continue;}
          // if (sel_cut!=1) {
          //   cout<<"sel_cut="<<sel_cut<<endl;
          //   continue;
          // }
          //calculation for the muon-selected cut
          muon_sel=muon_cont+muon_tra;
          muon_sel_eff=muon_cont_eff+muon_tra_eff;
          if (muon_sel!=0&&muon_sel!=1) {
            cout<<"bad val for muon-selected check! "<<muon_sel<<endl<<"Event #"<<i<<" in file "<<eff<<endl<<"contained-"<<muon_cont<<", tracker-matched-"<<muon_tra<<endl;
            continue;
          }


          int n=0;
          for (auto sel:br) {
            for (auto item:pr) {
              const char *fd=item.field;
              double geo_eff=*sel.eff_value;
              // if (geo_eff<0) continue;
              TH1D* hist1;
              if (n<NUM_FIELDS) hist1=histograms1.at(n);
              TH1D* hist2=histograms2.at(n);
              TH1D* hist3=histograms3.at(n);
              if (n<NUM_FIELDS) hist1->Fill(*item.field_value); // Raw
              n++;


              if (geo_eff<=geoeff_cut) {
                continue;
              } else {
                hist2->Fill(*item.field_value,*sel.sel_value); // Sel
                hist3->Fill(*item.field_value,*sel.sel_value/geo_eff); // Geo-corrected
              }
            }
          }
        }
        eff_file.Close();
        caf_file.Close();
      }//end read caf and eff files

      TFile *raw_files[NUM_FIELDS];
      TFile *sel_files[int(NUM_FIELDS*5)];
      TFile *geo_files[int(NUM_FIELDS*5)];
      int index=0;
      for (auto sel:br) {
        const char *dt=sel.sel_name;
        for (Para item:pr) {
          const char *fd=item.field;

          // Create a folder before writting root file
          // gSystem->mkdir(TString::Format("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug_center/%s", geoeff_cut,fd), kTRUE); // _debug means only choose events w/ geoeff >=0
          gSystem->mkdir(TString::Format("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug/%s", geoeff_cut,fd), kTRUE); // _debug means only choose events w/ geoeff >=0

          if (index<NUM_FIELDS) {
            // raw_files[index]=new TFile(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug_center/%s/raw_%s.root",geoeff_cut, fd,fd),"update");
            raw_files[index]=new TFile(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug/%s/raw_%s.root",geoeff_cut, fd,fd),"update");
            TH1D* raw_hist=histograms1.at(index);
            raw_hist->Write();
            raw_files[index]->Close();
          }

          // sel_files[index]=new TFile(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug_center/%s/selection-cut_%s_%s.root",geoeff_cut,fd,dt,fd),"update");
          sel_files[index]=new TFile(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug/%s/selection-cut_%s_%s.root",geoeff_cut,fd,dt,fd),"update");
          TH1D* sel_hist=histograms2.at(index);
          sel_hist->Write();
          sel_files[index]->Close();

          // geo_files[index]=new TFile(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug_center/%s/geo-corrected_%s_%s.root",geoeff_cut,fd,dt,fd),"update");
          geo_files[index]=new TFile(Form("/home/fyguo/testbaroncode/hist_file/0mgsimple_histograms/%.3f_eff_veto_cut_ND_debug/%s/geo-corrected_%s_%s.root",geoeff_cut,fd,dt,fd),"update");
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
  }// end file loop
}
