// Script for creating flux uncertainty inputs for PRISM
// Maybe I could split these classes out into their own files,
// but that seems excessive for just making one .root file.

#include "CAFAna/Core/Utilities.h"

#include "TH2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TROOT.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

//----------------------

class OffAxisUncertaintyHandler {
public:
  OffAxisUncertaintyHandler(TH2D *h_nom_input, TH2D *h_shift_input) {

    h_nom = std::unique_ptr<TH2D>(dynamic_cast<TH2D*>(h_nom_input));
    h_shift = std::unique_ptr<TH2D>(dynamic_cast<TH2D*>(h_shift_input));

    lowY = std::ceil(h_nom->GetYaxis()->GetXmin());
    highY = std::floor(h_nom->GetYaxis()->GetXmax());
    //NYbins = (highY - lowY) / 0.5; // 50cm bin width
    NYbins = (highY - lowY) / 1.0; // 1m bin width
    OffAxisPos = std::make_unique<TAxis>(NYbins, lowY, highY);
    std::cout << "Low OA = " << lowY << ", High OA = " << highY << ", N OA bins = " << NYbins << std::endl;
    std::vector<double> bin_edges;
    for (int binx = 1; binx <= h_nom->GetXaxis()->GetNbins(); binx++) {
      bin_edges.emplace_back(h_nom->GetXaxis()->GetBinLowEdge(binx));
    }
    bin_edges.emplace_back(h_nom->GetXaxis()->GetBinUpEdge(h_nom->GetXaxis()->GetNbins()));

    for (int i = 0; i < NYbins; i++) {
      NDnoms.emplace_back(std::make_unique<TH1D>(("hnom" + std::to_string(i)).c_str(),
                            ("hnom"+ std::to_string(i)).c_str(), h_nom->GetXaxis()->GetNbins(),
                            &bin_edges[0])
                         );
      NDshifts.emplace_back(std::make_unique<TH1D>(("hshift"+ std::to_string(i)).c_str(),
                              ("hshift"+ std::to_string(i)).c_str(), h_shift->GetXaxis()->GetNbins(),
                              &bin_edges[0])
                           );
      NDuncerts.emplace_back(nullptr);
      // Uniform energy binning 
      NDnomsUniformBins.emplace_back(std::make_unique<TH1D>(("hnomUniformBins"+ std::to_string(i)).c_str(),
                             ("hnomUniformBins"+ std::to_string(i)).c_str(), h_shift->GetXaxis()->GetNbins(),
                             &bin_edges[0])
                          );

      NDshiftsUniformBins.emplace_back(std::make_unique<TH1D>(("hshiftUniformBins"+ std::to_string(i)).c_str(),
                             ("hshiftUniformBins"+ std::to_string(i)).c_str(), h_shift->GetXaxis()->GetNbins(),
                             &bin_edges[0])
                          );
    }
  }

  ~OffAxisUncertaintyHandler() {}

  void SortIntoTH1Ds() {
    for (int binx = 1; binx <= h_nom->GetXaxis()->GetNbins(); binx++) {
      for (int biny = 1; biny <= h_nom->GetYaxis()->GetNbins(); biny++) {
        double bin_center_y = h_nom->GetYaxis()->GetBinCenter(biny);
        int newbiny = OffAxisPos->FindFixBin(bin_center_y);
        if (newbiny > 0 && newbiny < NYbins + 1) {
          double nom_content = h_nom->GetBinContent(binx, biny);
          double shift_content = h_shift->GetBinContent(binx, biny);
          NDnoms.at(newbiny - 1)->SetBinContent(binx, nom_content);
          NDshifts.at(newbiny - 1)->SetBinContent(binx, shift_content);
        } else { continue; }
      }
    }
  }

  //first rebin the original flux files in equal energy bins of 0.25 bin width
  void RebinNDTH1DsUniformBinning(){
    for(int i=0; i<NbinsEqual; i++){
     EbinsEqualWidth[i] = 0 + 0.25*i;
    }
    EbinsEqualWidth[NbinsEqual-1] = 119.5; //last Ebin

    for (int i = 0; i < NYbins; i++) {
      NDnomsUniformBins.at(i) = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(NDnoms.at(i)->Rebin(NbinsEqual-1, ("NominalFluxUniformBins_" + std::to_string(i) ).c_str(), EbinsEqualWidth)));
      NDshiftsUniformBins.at(i) = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(NDshifts.at(i)->Rebin(NbinsEqual-1, ("ShiftFluxUniformBins_" + std::to_string(i) ).c_str(), EbinsEqualWidth)));
    }
  }

  //use the uniform E bins histos to calculate the desired binning
  void CalculateUncertaintyBinning(std::string flavor) {
    double TotalErrorEvents[NYbins];
    for (int i = 0; i < NYbins; i++) {
      std::vector<double> bin_edges;
      bin_edges.emplace_back(0);
      double bin_content(0);

      for (int binx = 1; binx <= NDnomsUniformBins.at(i)->GetXaxis()->GetNbins(); binx++) {
        bin_content += NDnomsUniformBins.at(i)->GetBinContent(binx);

        TotalErrorEvents[i] = 1 / sqrt(NDnomsUniformBins[i]->Integral());

        if ((sqrt(bin_content) / bin_content) < PoissonErrorLimit * TotalErrorEvents[i]) { // min stats in each bin
          bin_edges.emplace_back(NDnomsUniformBins.at(i)->GetXaxis()->GetBinLowEdge(binx));
          bin_content = 0;
          continue;
        }
      }

      if (bin_edges.size() <= 0) {
        bin_edges.emplace_back(NDnomsUniformBins.at(i)->GetXaxis()->GetBinLowEdge(1));
      }
      bin_edges.emplace_back(NDnomsUniformBins.at(i)->GetXaxis()
                             ->GetBinUpEdge(NDnomsUniformBins.at(i)->GetXaxis()->GetNbins()));
      NDuncerts.at(i) = std::move(std::make_unique<TH1D>(("huncert"+ std::to_string(i)).c_str(),
                                    ("huncert"+ std::to_string(i)).c_str(),
                                    bin_edges.size() - 1, &bin_edges[0]));

    }
  }

  void CalculateUncertainty(std::string flavor) {
    double TotalErrorEvents[NYbins];
    for (int i = 0; i < NYbins; i++) {
      double bin_content(0);
      double nomsumx(0), shiftsumx(0);
      int sumbin(0);
      for (int binx = 1; binx <= NDnomsUniformBins.at(i)->GetXaxis()->GetNbins(); binx++) {
        nomsumx += NDnomsUniformBins.at(i)->GetBinContent(binx);
        shiftsumx += NDshiftsUniformBins.at(i)->GetBinContent(binx);

        bin_content += NDnomsUniformBins[i]->GetBinContent(binx);

	TotalErrorEvents[i] = 1 / sqrt(NDnomsUniformBins[i]->Integral());

        if ((sqrt(bin_content) / bin_content) < PoissonErrorLimit * TotalErrorEvents[i]) { // min stats in each bin
          sumbin++;
          NDuncerts.at(i)->SetBinContent(sumbin, (shiftsumx - nomsumx) / nomsumx);
          nomsumx = 0;
          shiftsumx = 0;
          bin_content = 0;
          continue;
        }
      }
    }
  }

  int GetYNbins() const { return NYbins; }

  TAxis *GetOAAxis() const { return OffAxisPos.get(); }

  bool IsNue(std::string flavor) {
    if (flavor == "numu" || flavor == "numubar") return false;
    else { return true; }
  }

  TH1D *GetNDUncerts(int pos) const {
    return NDuncerts.at(pos).get();
  }
  TH1D* GetNDNominals(int pos) const {
    return NDnoms.at(pos).get();
  }
  TH1D* GetNDShifted(int pos) const {
    return NDshifts.at(pos).get();
  }
  TH1D* GetNDNominalsUniformBinning(int pos) const {
    return NDnomsUniformBins.at(pos).get();
  }


protected:

  int NYbins;
  double lowY, highY;
  static const int NbinsEqual = 480; //equal number of x-bins with 0.25 width
  double EbinsEqualWidth[NbinsEqual];
  //binning based on percentage of events: i.e if events in an energy bin are less than 1.77% of the total events in the histogram then keep extending the bin
  const double PoissonErrorLimit = 7.5; // this is the Poisson error limit equivalent to 1.77% events  

  std::unique_ptr<TAxis> OffAxisPos;
  std::unique_ptr<TH2D> h_nom;
  std::unique_ptr<TH2D> h_shift;
  std::vector<std::unique_ptr<TH1D>> NDnoms;
  std::vector<std::unique_ptr<TH1D>> NDshifts;
  std::vector<std::unique_ptr<TH1D>> NDuncerts;
  std::vector<std::unique_ptr<TH1D>> NDnomsUniformBins;
  std::vector<std::unique_ptr<TH1D>> NDshiftsUniformBins;

};

//----------------------

class FDUncertaintyHelper {
public:
  FDUncertaintyHelper(TH1D *h_nom_input, TH1D *h_shift_input) {
    FDnom = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(h_nom_input));
    FDshift = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(h_shift_input));
    FDshiftOverFDnom = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(h_shift_input->Clone()));
    FDshiftOverFDnom->Divide(dynamic_cast<TH1D*>(h_nom_input));

    FDuncert = nullptr;
  }

  ~FDUncertaintyHelper() {}

  void RebinFDTH1DsUniformBinning(){
    for(int i=0; i<NbinsEqual; i++){
     EbinsEqualWidth[i] = 0 + 0.25*i;
    }
    EbinsEqualWidth[NbinsEqual-1] = 119.5; //last Ebin

    FDnomUniformBins = std::unique_ptr<TH1D>( dynamic_cast<TH1D*>(FDnom->Rebin(NbinsEqual-1, "FDnomUniformBins", EbinsEqualWidth)) );
    FDshiftUniformBins = std::unique_ptr<TH1D>( dynamic_cast<TH1D*>(FDshift->Rebin(NbinsEqual-1, "FDshiftUniformBins", EbinsEqualWidth)) );
  }

  void CalculateUncertaintyBinning(std::string flavor) {
    std::vector<double> bin_edges;
    bin_edges.emplace_back(0);
    
    double TotalErrorEventsFD;
    double bin_content(0);
    for (int binx = 1; binx <= FDnomUniformBins->GetXaxis()->GetNbins(); binx++) {
      bin_content += FDnomUniformBins->GetBinContent(binx);
      TotalErrorEventsFD = 1 / sqrt(FDnomUniformBins->Integral());
      if ((sqrt(bin_content) / bin_content) < PoissonErrorLimit * TotalErrorEventsFD) { // min stats in each bin
        bin_edges.emplace_back(FDnomUniformBins->GetXaxis()->GetBinLowEdge(binx));
        bin_content = 0;
        continue;
      }
    }
    if (bin_edges.size() <= 0) {
      bin_edges.emplace_back(FDnomUniformBins->GetXaxis()->GetBinLowEdge(1));
    }
    bin_edges.emplace_back(FDnomUniformBins->GetXaxis()
                           ->GetBinUpEdge(FDnomUniformBins->GetXaxis()->GetNbins()));
    FDuncert = std::move(std::make_unique<TH1D>("FDuncert", "FDuncert",
                           bin_edges.size() - 1, &bin_edges[0]));
   
  }

  void CalculateUncertainty(std::string flavor) {
    double nomsumx(0), shiftsumx(0);
    double bin_content(0);
    int sumbin(0);
    double TotalErrorEventsFD;
    for (int binx = 1; binx <= FDnomUniformBins->GetXaxis()->GetNbins(); binx++) {
      bin_content += FDnomUniformBins->GetBinContent(binx);
      TotalErrorEventsFD = 1 / sqrt(FDnomUniformBins->Integral());

      nomsumx += FDnomUniformBins->GetBinContent(binx);
      shiftsumx += FDshiftUniformBins->GetBinContent(binx);
      if ((sqrt(bin_content) / bin_content) < PoissonErrorLimit * TotalErrorEventsFD){ // min stats in each bin
        sumbin++;
        FDuncert->SetBinContent(sumbin, (shiftsumx - nomsumx) / nomsumx);
        bin_content = 0;
        nomsumx = 0;
        shiftsumx = 0;
        continue;
      }
    }
  }

  bool IsNue(std::string flavor) {
    if (flavor == "numu" || flavor == "numubar") return false;
    else { return true; }
  }

  TH1D *GetFDUncert() const { return FDuncert.get(); }
  TH1D *GetFDNominal() const {return FDnom.get(); }
  TH1D *GetFDShift() const {return FDshift.get(); }

protected:

  static const int NbinsEqual = 480; //equal number of x-bins with 0.25 width
  double EbinsEqualWidth[NbinsEqual];
  //binning based on percentage of events: i.e if events in an energy bin are less than 1.77% of the total events in the histogram then keep extending the bin
  const double PoissonErrorLimit = 7.5; // this is the Poisson error limit equivalent to 1.77% events
  
  std::unique_ptr<TH1D> FDnom;
  std::unique_ptr<TH1D> FDshift;
  std::unique_ptr<TH1D> FDuncert;
  std::unique_ptr<TH1D> FDshiftOverFDnom;
  std::unique_ptr<TH1D> FDnomUniformBins;
  std::unique_ptr<TH1D> FDshiftUniformBins;

};

//----------------------

int main(int argc, char** argv) {

  std::string fluxdir("/pnfs/dune/persistent/users/pweather/fluxfiles/g4lbne/v3r5p9/QGSP_BERT/");

  std::string nominal_flux_dir("OfficialEngDesignSept2021");

  std::vector<std::string> shift_flux_dirs{"OEDS21_HornADisplaceTransverseX_pos_1_sigma",
                                           "OEDS21_HornBDisplaceTransverseX_pos_1_sigma",
                                           "OEDS21_HornCDisplaceTransverseX_pos_1_sigma",
                                           "OEDS21_HornADisplaceTransverseY_pos_1_sigma",
                                           "OEDS21_HornBDisplaceTransverseY_pos_1_sigma",
                                           "OEDS21_HornCDisplaceTransverseY_pos_1_sigma",
                                           "OEDS21_DecayPipe3SegmentBowingX_pos_1_sigma",
                                           "OEDS21_DecayPipe3SegmentBowingY_pos_1_sigma",
                                           "OEDS21_DecayPipeDisplaceTransverseX_pos_1_sigma",
                                           "OEDS21_DecayPipeDisplaceTransverseY_pos_1_sigma",
                                           "OEDS21_DecayPipeEllipticalCrossSectionXA_pos_1_sigma",
                                           "OEDS21_DecayPipeEllipticalCrossSectionYB_pos_1_sigma",
                                           "OEDS21_DecayPipeGeoBField_pos_1_sigma",
                                           "OEDS21_DecayPipeLength_pos_1_sigma",
                                           "OEDS21_DecayPipeRadius_pos_1_sigma",
                                           "OEDS21_DecayPipeTiltX_DSOA_pos_1_sigma",
                                           "OEDS21_DecayPipeTiltY_DSOA_pos_1_sigma",
                                           "OEDS21_HornADisplaceLongitudinalZ_pos_1_sigma",
                                           "OEDS21_HornAEccentricityXInducedBField_pos_1_sigma",
                                           "OEDS21_HornAEllipticityXInducedBField_pos_1_sigma",
                                           "OEDS21_HornATiltTransverseX_pos_1_sigma",
                                           "OEDS21_HornATiltTransverseY_pos_1_sigma",
                                           "OEDS21_HornBDisplaceLongitudinalZ_pos_1_sigma",
                                           //"OEDS21_HornBEccentricityXInducedBField_pos_1_sigma",
                                           "OEDS21_HornBEllipticityXInducedBField_pos_1_sigma",
                                           "OEDS21_HornBTiltTransverseX_pos_1_sigma",
                                           "OEDS21_HornBTiltTransverseY_pos_1_sigma",
                                           "OEDS21_HornCDisplaceLongitudinalZ_pos_1_sigma",
                                           "OEDS21_HornCEccentricityXInducedBField_pos_1_sigma",
                                           "OEDS21_HornCEllipticityXInducedBField_pos_1_sigma",
                                           "OEDS21_HornCTiltTransverseX_pos_1_sigma",
                                           "OEDS21_HornCTiltTransverseY_pos_1_sigma",
                                           "OEDS21_HornCurrent_pos_1_sigma",
                                           "OEDS21_HornWaterLayerThickness_pos_1_sigma",
                                           "OEDS21_ProtonBeamAngleX_pos_1_sigma",
                                           "OEDS21_ProtonBeamAngleY_pos_1_sigma",
                                           "OEDS21_ProtonBeamRadius_pos_1_sigma",
                                           "OEDS21_ProtonBeamTransverseX_pos_1_sigma",
                                           "OEDS21_ProtonBeamTransverseY_pos_1_sigma",
                                           "OEDS21_TargetDensity_pos_1_sigma",
                                           "OEDS21_TargetDisplaceTransverseX_pos_1_sigma",
                                           "OEDS21_TargetDisplaceTransverseY_pos_1_sigma",
                                           "OEDS21_TargetLength_pos_1_sigma",
                                           "OEDS21_TargetTiltTransverseX_pos_1_sigma",
                                           "OEDS21_TargetTiltTransverseY_pos_1_sigma",
                                           "OEDS21_TargetUpstreamDegredation_pos_1_sigma"};

  std::vector<std::string> vflavors{"numu", "numubar", "nue", "nuebar"};
  std::vector<std::string> vsign{"neutrino", "antineutrino"};
  std::vector<std::string> vspecrun{"_", "_specrun_"};

  std::string outfile = ana::FindCAFAnaDir() + "/../../Systs/flux_shifts_OffAxis2023.root";
  std::cout << "FILE OUTPUT: " << outfile << std::endl;
  TFile *TotalUncert = TFile::Open(outfile.c_str(), "RECREATE");

  TAxis *OATaxis = nullptr;

  for (const std::string& shiftname : shift_flux_dirs) {

    // Some string parsing
    size_t dash1 = shiftname.find("_");
    std::string syst = shiftname.substr(dash1 + 1, shiftname.find("_", dash1 + 1) - (dash1 + 1));
    std::cout << "[LOADING] " << syst << std::endl;

    // Create ND flux uncertainties first
    for (const std::string& sign : vsign) {
      for (const std::string& flavor : vflavors) {
        for (const std::string& specrun : vspecrun) {

        std::cout << flavor << ", " << sign << std::endl;

        std::string write_sign = (sign == "neutrino" ? "nu" : "nubar");
        TotalUncert->mkdir(("FluxParameters/param_" + syst + "/ND_" +
                            write_sign + specrun + flavor).c_str());

        //--------------
        // Load ND fluxes
        std::string nominal_file_ND = fluxdir + nominal_flux_dir + "/" + sign + "/flux/" +
                                   "histos_g4lbne_v3r5p9_QGSP_BERT_" + nominal_flux_dir +
                                   "_" + sign + "_LAr_center.root";

        std::string shift_file_ND = fluxdir + shiftname + "/" + sign + "/flux/" +
                                 "histos_g4lbne_v3r5p9_QGSP_BERT_" + shiftname +
                                 "_" + sign + "_LAr_center.root";

        //std::cout << "Shift file: " << shift_file_ND << std::endl;
        //std::cout << "Shift xrootd file: " << ana::pnfs2xrootd(shift_file_ND) << std::endl;

        TFile *fnom_ND = TFile::Open(ana::pnfs2xrootd(nominal_file_ND).c_str());

        if (!fnom_ND || fnom_ND->IsZombie()) abort();
        TDirectory *current_dir = gDirectory;

        TH2D *h_nom_ND =
          (TH2D*)current_dir->Get(("Unosc_" + flavor + "_flux_DUNEPRISM_LAr_center").c_str());
        h_nom_ND->SetDirectory(nullptr);

        TFile *fshift_ND = TFile::Open(ana::pnfs2xrootd(shift_file_ND).c_str());

        if (!fshift_ND || fshift_ND->IsZombie()) abort();
        current_dir = gDirectory;

        TH2D *h_shift_ND =
          (TH2D*)current_dir->Get(("Unosc_" + flavor + "_flux_DUNEPRISM_LAr_center").c_str());
        h_shift_ND->SetDirectory(nullptr);

        fnom_ND->Close();
        fshift_ND->Close();

        //----------------
        // Build ND flux uncertainties
        TotalUncert->cd(("FluxParameters/param_" + syst + "/ND_" +
                         write_sign + specrun + flavor).c_str());

        OffAxisUncertaintyHandler offaxisuncert =
          OffAxisUncertaintyHandler(h_nom_ND, h_shift_ND);
        offaxisuncert.SortIntoTH1Ds();
        offaxisuncert.RebinNDTH1DsUniformBinning();
        offaxisuncert.CalculateUncertaintyBinning(flavor);
        offaxisuncert.CalculateUncertainty(flavor);

        for (int pos = 0; pos < offaxisuncert.GetYNbins(); pos++) {
          TH1D *NDSliceUncert = offaxisuncert.GetNDUncerts(pos);
          NDSliceUncert->Write(("ND_" + write_sign + specrun + flavor + "_" +
                                std::to_string(pos)).c_str());
        }
        OATaxis = new TAxis(*offaxisuncert.GetOAAxis());
        }
      }
    }
    // Write a TAxis of the off axis position bins for each syst param
    if (OATaxis) {
      TotalUncert->cd(("FluxParameters/param_" + syst).c_str());
      OATaxis->Write("OffAxisTAxis");
    } else {
      std::cout << "[ERROR] Not found a TAxis for position." << std::endl;
      exit(1);
    }

    // Create FD flux uncertainties second
    for (const std::string& sign : vsign) {
      for (const std::string& flavor : vflavors) {

        std::string write_sign = (sign == "neutrino" ? "nu" : "nubar");
        //--------------
        // Load FD fluxes
        std::string nominal_file_FD = fluxdir + nominal_flux_dir + "/" + sign + "/flux/" +
                                   "histos_g4lbne_v3r5p9_QGSP_BERT_" + nominal_flux_dir +
                                   "_" + sign + "_finemc.root";

        std::string shift_file_FD = fluxdir + shiftname + "/" + sign + "/flux/" +
                                 "histos_g4lbne_v3r5p9_QGSP_BERT_" + shiftname +
                                 "_" + sign + "_finemc.root";

        TFile *fnom_FD = TFile::Open(nominal_file_FD.c_str());

        if (!fnom_FD || fnom_FD->IsZombie()) abort();
        TDirectory *current_dir = gDirectory;

        TH1D *h_nom_FD =
          (TH1D*)current_dir->Get(("Unosc_flux_" + flavor + "_finemc_DUNEFD").c_str());
        h_nom_FD->SetDirectory(nullptr);

        TFile *fshift_FD = TFile::Open(shift_file_FD.c_str());

        if (!fshift_FD || fshift_FD->IsZombie()) abort();
        current_dir = gDirectory;

        TH1D *h_shift_FD =
          (TH1D*)current_dir->Get(("Unosc_flux_" + flavor + "_finemc_DUNEFD").c_str());
        h_shift_FD->SetDirectory(nullptr);

        fnom_FD->Close();
        fshift_FD->Close();

        //--------------
        // Build FD flux uncertainties
        TotalUncert->cd(("FluxParameters/param_" + syst).c_str());

        FDUncertaintyHelper fduncert =
          FDUncertaintyHelper(h_nom_FD, h_shift_FD);
        fduncert.RebinFDTH1DsUniformBinning();
        fduncert.CalculateUncertaintyBinning(flavor);
        fduncert.CalculateUncertainty(flavor);

        TH1D *FDUncert_h = fduncert.GetFDUncert();
        FDUncert_h->Write(("FD_" + write_sign + "_" + flavor).c_str());
      }
    }
  }
  TotalUncert->Write();
  TotalUncert->Close();

  return 0;
}
