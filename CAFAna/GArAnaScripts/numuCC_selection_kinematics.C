// Make a simple ND-GAr plot
// cafe numuCC_selection_kinematics.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "CAFAna/Core/UtilsExt.h"

#include "GArUtils.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/SRGAr.h"

#include <TFile.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TRatioPlot.h"
#include "TLegend.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TString.h"

#include <string>
#include <vector>
#include <unordered_map>

using namespace ana;

void numuCC_selection_kinematics(float muon_score_cut, float delta_r, bool momentum_method, int sign_selection)
{

  // Environment variables and wildcards work. As do SAM datasets.
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };

  TFile *fout = new TFile("numuCC_selection_kinematics.root","RECREATE");

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;
  
  // Source of events
  SpectrumLoader loader(fname);

  // Define the cut given the input parameters
  const SRCut kTruthCut = kIsTrueNuMuCC && kIsTrueContained(delta_r);
  const SRCut kRecoCut = kIsContainedNuMuCC(muon_score_cut, delta_r, momentum_method, sign_selection);

  const Binning binsMomentum = Binning::Simple(40, 0, 5);

  const Binning binsLongiMomentum = Binning::Simple(40, 0.0, 6.0);
  const Binning binsTransMomentum = Binning::Simple(40, 0.0, 2.5);

  const Binning binsAngle = Binning::Simple(40, -1, 1);

  const Binning binsX = Binning::Simple(41, kHPgTPCX-kHPgTPCLength, kHPgTPCX+kHPgTPCLength);
  const Binning binsY = Binning::Simple(41, kHPgTPCY-kHPgTPCRadius, kHPgTPCY+kHPgTPCRadius);
  const Binning binsZ = Binning::Simple(41, kHPgTPCZ-kHPgTPCRadius, kHPgTPCZ+kHPgTPCRadius);
  
  const Binning binsFractionalResiduals = Binning::Simple(41, -0.35, 0.35);
  const Binning binsFractionalResidualsZ = Binning::Simple(41, -0.15, 0.15);

  // Define the true variables
  const SRVar kTrueMuonMomentum = kTruePrimaryMuonMomentum("total");
  const SRVar kTrueLongiMuonMomentum = kTruePrimaryMuonMomentum("longitudinal");
  const SRVar kTrueTransMuonMomentum = kTruePrimaryMuonMomentum("transverse");

  const SRVar kTrueMuonAngle = kTruePrimaryMuonAngle();

  const SRVar kTrueVertexPositionX = kTrueVertexPosition("X");
  const SRVar kTrueVertexPositionY = kTrueVertexPosition("Y");
  const SRVar kTrueVertexPositionZ = kTrueVertexPosition("Z");
  
  // Define the reco variables
  const SRVar kMuonMomentum = kPrimaryMuonMomentum(muon_score_cut, delta_r, momentum_method, sign_selection, "total");
  const SRVar kLongiMuonMomentum = kPrimaryMuonMomentum(muon_score_cut, delta_r, momentum_method, sign_selection, "longitudinal");
  const SRVar kTransMuonMomentum = kPrimaryMuonMomentum(muon_score_cut, delta_r, momentum_method, sign_selection, "transverse");

  const SRVar kMuonAngle = kPrimaryMuonAngle(muon_score_cut, delta_r, momentum_method, sign_selection);

  const SRVar kVertexPositionX = kVertexPosition(muon_score_cut, delta_r, momentum_method, sign_selection, "X");
  const SRVar kVertexPositionY = kVertexPosition(muon_score_cut, delta_r, momentum_method, sign_selection, "Y");
  const SRVar kVertexPositionZ = kVertexPosition(muon_score_cut, delta_r, momentum_method, sign_selection, "Z");

  // Define other variables
  const SRVar kVertexPositionFractionalResidualX = kVertexPositionFractionalResidual(muon_score_cut, delta_r, momentum_method, sign_selection, "X");
  const SRVar kVertexPositionFractionalResidualY = kVertexPositionFractionalResidual(muon_score_cut, delta_r, momentum_method, sign_selection, "Y");
  const SRVar kVertexPositionFractionalResidualZ = kVertexPositionFractionalResidual(muon_score_cut, delta_r, momentum_method, sign_selection, "Z");

  //
  Spectrum sTrueMuonMomentumTP(loader[kRecoCut && kTruthCut], SRHistAxis("True muon momentum [GeV/c]", binsMomentum, kTrueMuonMomentum));
  Spectrum sTrueMuonMomentumFP(loader[kRecoCut && !kTruthCut], SRHistAxis("True muon momentum [GeV/c]", binsMomentum, kTrueMuonMomentum));
  Spectrum sTrueMuonMomentumFN(loader[!kRecoCut && kTruthCut], SRHistAxis("True muon momentum [GeV/c]", binsMomentum, kTrueMuonMomentum));

  Spectrum sTrueMuonAngleTP(loader[kRecoCut && kTruthCut], SRHistAxis("True muon angle", binsAngle, kTrueMuonAngle));
  Spectrum sTrueMuonAngleFP(loader[kRecoCut && !kTruthCut], SRHistAxis("True muon angle", binsAngle, kTrueMuonAngle));
  Spectrum sTrueMuonAngleFN(loader[!kRecoCut && kTruthCut], SRHistAxis("True muon angle", binsAngle, kTrueMuonAngle));

  Spectrum sRecoMuonMomentumTP(loader[kRecoCut && kTruthCut], SRHistAxis("Reco muon momentum [GeV/c]", binsMomentum, kMuonMomentum));
  Spectrum sRecoMuonMomentumFP(loader[kRecoCut && !kTruthCut], SRHistAxis("Reco muon momentum [GeV/c]", binsMomentum, kMuonMomentum));
  Spectrum sRecoMuonMomentumFN(loader[!kRecoCut && kTruthCut], SRHistAxis("Reco muon momentum [GeV/c]", binsMomentum, kMuonMomentum));

  Spectrum sRecoMuonAngleTP(loader[kRecoCut && kTruthCut], SRHistAxis("Reco muon angle", binsAngle, kMuonAngle));
  Spectrum sRecoMuonAngleFP(loader[kRecoCut && !kTruthCut], SRHistAxis("Reco muon angle", binsAngle, kMuonAngle));
  Spectrum sRecoMuonAngleFN(loader[!kRecoCut && kTruthCut], SRHistAxis("Reco muon angle", binsAngle, kMuonAngle));

  // Create spectra with the true vs reco comparisons
  Spectrum sTrueXRecoMuonMomentum(loader[kRecoCut], SRHistAxis("True muon momentum [GeV/c]", binsMomentum, kTrueMuonMomentum,
                                                               "Reco muon momentum [GeV/c]", binsMomentum, kMuonMomentum));

  Spectrum sTrueXRecoLongiMuonMomentum(loader[kRecoCut], SRHistAxis("True longitudinal muon momentum [GeV/c]", binsLongiMomentum, kTrueLongiMuonMomentum,
                                                                    "Reco longitudinal muon momentum [GeV/c]", binsLongiMomentum, kLongiMuonMomentum));

  Spectrum sTrueXRecoTransMuonMomentum(loader[kRecoCut], SRHistAxis("True transverse muon momentum [GeV/c]", binsTransMomentum, kTrueTransMuonMomentum,
                                                                    "Reco transverse muon momentum [GeV/c]", binsTransMomentum, kTransMuonMomentum));

  Spectrum sTrueXRecoVertexPositionX(loader[kRecoCut], SRHistAxis("True vertex X [cm]", binsX, kTrueVertexPositionX,
                                                                  "Reco vertex X [cm]", binsX, kVertexPositionX));

  Spectrum sTrueXRecoVertexPositionY(loader[kRecoCut], SRHistAxis("True vertex Y [cm]", binsY, kTrueVertexPositionY,
                                                                  "Reco vertex Y [cm]", binsY, kVertexPositionY));

  Spectrum sTrueXRecoVertexPositionZ(loader[kRecoCut], SRHistAxis("True vertex Z [cm]", binsZ, kTrueVertexPositionZ,
                                                                  "Reco vertex Z [cm]", binsZ, kVertexPositionZ));

  // Create other additional spectra
  Spectrum sTrueTransXLongiMuonMomentum(loader[kRecoCut], SRHistAxis("Transverse muon momentum [GeV/c]", binsTransMomentum, kTrueTransMuonMomentum,
                                                                     "Longitudinal muon momentum [GeV/c]", binsLongiMomentum, kTrueLongiMuonMomentum));

  Spectrum sTransXLongiMuonMomentum(loader[kRecoCut], SRHistAxis("Transverse muon momentum [GeV/c]", binsTransMomentum, kTransMuonMomentum,
                                                                 "Longitudinal muon momentum [GeV/c]", binsLongiMomentum, kLongiMuonMomentum));

  Spectrum sVertexPositionFractionalResidualX(loader[kRecoCut], SRHistAxis("(X_{true}-X_{reco})/X_{true}", binsFractionalResiduals, kVertexPositionFractionalResidualX));

  Spectrum sVertexPositionFractionalResidualY(loader[kRecoCut], SRHistAxis("(Y_{true}-Y_{reco})/Y_{true}", binsFractionalResiduals, kVertexPositionFractionalResidualY));

  Spectrum sVertexPositionFractionalResidualZ(loader[kRecoCut], SRHistAxis("(Z_{true}-Z_{reco})/Z_{true}", binsFractionalResidualsZ, kVertexPositionFractionalResidualZ));

  // Go, go, go!
  loader.Go();

  fout->cd();

  //
  TH1D* hTrueMuonMomentumTP(sTrueMuonMomentumTP.ToTH1(pot));
  hTrueMuonMomentumTP->SetName("TrueMuonMomentumTP");
  hTrueMuonMomentumTP->Write("TrueMuonMomentumTP", TObject::kWriteDelete);

  TH1D* hTrueMuonMomentumFP(sTrueMuonMomentumFP.ToTH1(pot));
  hTrueMuonMomentumFP->SetName("TrueMuonMomentumFP");
  hTrueMuonMomentumFP->Write("TrueMuonMomentumFP", TObject::kWriteDelete);

  TH1D* hTrueMuonMomentumFN(sTrueMuonMomentumFN.ToTH1(pot));
  hTrueMuonMomentumFN->SetName("TrueMuonMomentumFN");
  hTrueMuonMomentumFN->Write("TrueMuonMomentumFN", TObject::kWriteDelete);

  TH1D* hTrueMuonAngleTP(sTrueMuonAngleTP.ToTH1(pot));
  hTrueMuonAngleTP->SetName("TrueMuonAngleTP");
  hTrueMuonAngleTP->Write("TrueMuonAngleTP", TObject::kWriteDelete);

  TH1D* hTrueMuonAngleFP(sTrueMuonAngleFP.ToTH1(pot));
  hTrueMuonAngleFP->SetName("TrueMuonAngleFP");
  hTrueMuonAngleFP->Write("TrueMuonAngleFP", TObject::kWriteDelete);

  TH1D* hTrueMuonAngleFN(sTrueMuonAngleFN.ToTH1(pot));
  hTrueMuonAngleFN->SetName("TrueMuonAngleFN");
  hTrueMuonAngleFN->Write("TrueMuonAngleFN", TObject::kWriteDelete);

  //
  TH1D *hTrueMuonAngleEff = new TH1D();
  *hTrueMuonAngleEff = (*hTrueMuonAngleTP) / ((*hTrueMuonAngleTP) + (*hTrueMuonAngleFN));
  hTrueMuonAngleEff->SetName("TrueMuonAngleEff");
  hTrueMuonAngleEff->Write("TrueMuonAngleEff", TObject::kWriteDelete);
  //

  TH1D* hRecoMuonMomentumTP(sRecoMuonMomentumTP.ToTH1(pot));
  hRecoMuonMomentumTP->SetName("RecoMuonMomentumTP");
  hRecoMuonMomentumTP->Write("RecoMuonMomentumTP", TObject::kWriteDelete);

  TH1D* hRecoMuonMomentumFP(sRecoMuonMomentumFP.ToTH1(pot));
  hRecoMuonMomentumFP->SetName("RecoMuonMomentumFP");
  hRecoMuonMomentumFP->Write("RecoMuonMomentumFP", TObject::kWriteDelete);

  TH1D* hRecoMuonMomentumFN(sRecoMuonMomentumFN.ToTH1(pot));
  hRecoMuonMomentumFN->SetName("RecoMuonMomentumFN");
  hRecoMuonMomentumFN->Write("RecoMuonMomentumFN", TObject::kWriteDelete);

  TH1D* hRecoMuonAngleTP(sRecoMuonAngleTP.ToTH1(pot));
  hRecoMuonAngleTP->SetName("RecoMuonAngleTP");
  hRecoMuonAngleTP->Write("RecoMuonAngleTP", TObject::kWriteDelete);

  TH1D* hRecoMuonAngleFP(sRecoMuonAngleFP.ToTH1(pot));
  hRecoMuonAngleFP->SetName("RecoMuonAngleFP");
  hRecoMuonAngleFP->Write("RecoMuonAngleFP", TObject::kWriteDelete);

  TH1D* hRecoMuonAngleFN(sRecoMuonAngleFN.ToTH1(pot));
  hRecoMuonAngleFN->SetName("RecoMuonAngleFN");
  hRecoMuonAngleFN->Write("RecoMuonAngleFN", TObject::kWriteDelete);

  // Get the histograms for the true vs reco filled spectra
  TH2* hTrueXRecoMuonMomentum(sTrueXRecoMuonMomentum.ToTH2(pot));
  hTrueXRecoMuonMomentum->SetName("TrueXRecoMuonMomentum");
  hTrueXRecoMuonMomentum->Write("TrueXRecoMuonMomentum", TObject::kWriteDelete);

  TH2* hTrueXRecoLongiMuonMomentum(sTrueXRecoLongiMuonMomentum.ToTH2(pot));
  hTrueXRecoLongiMuonMomentum->SetName("TrueXRecoLongiMuonMomentum");
  hTrueXRecoLongiMuonMomentum->Write("TrueXRecoLongiMuonMomentum", TObject::kWriteDelete);

  TH2* hTrueXRecoTransMuonMomentum(sTrueXRecoTransMuonMomentum.ToTH2(pot));
  hTrueXRecoTransMuonMomentum->SetName("TrueXRecoTransMuonMomentum");
  hTrueXRecoTransMuonMomentum->Write("TrueXRecoTransMuonMomentum", TObject::kWriteDelete);

  TH2* hTrueXRecoVertexPositionX(sTrueXRecoVertexPositionX.ToTH2(pot));
  hTrueXRecoVertexPositionX->SetName("TrueXRecoVertexPositionX");
  hTrueXRecoVertexPositionX->Write("TrueXRecoVertexPositionX", TObject::kWriteDelete);

  TH2* hTrueXRecoVertexPositionY(sTrueXRecoVertexPositionY.ToTH2(pot));
  hTrueXRecoVertexPositionY->SetName("TrueXRecoVertexPositionY");
  hTrueXRecoVertexPositionY->Write("TrueXRecoVertexPositionY", TObject::kWriteDelete);

  TH2* hTrueXRecoVertexPositionZ(sTrueXRecoVertexPositionZ.ToTH2(pot));
  hTrueXRecoVertexPositionZ->SetName("TrueXRecoVertexPositionZ");
  hTrueXRecoVertexPositionZ->Write("TrueXRecoVertexPositionZ", TObject::kWriteDelete);

  // Get the histograms for the other filled spectra
  TH2* hTrueTransXLongiMuonMomentum(sTrueTransXLongiMuonMomentum.ToTH2(pot));
  hTrueTransXLongiMuonMomentum->SetName("TrueTransXLongiMuonMomentum");
  hTrueTransXLongiMuonMomentum->Write("TrueTransXLongiMuonMomentum", TObject::kWriteDelete);

  TH2* hTransXLongiMuonMomentum(sTransXLongiMuonMomentum.ToTH2(pot));
  hTransXLongiMuonMomentum->SetName("TransXLongiMuonMomentum");
  hTransXLongiMuonMomentum->Write("TransXLongiMuonMomentum", TObject::kWriteDelete);

  TH1D* hVertexPositionFractionalResidualX(sVertexPositionFractionalResidualX.ToTH1(pot));
  hVertexPositionFractionalResidualX->SetName("VertexPositionFractionalResidualX");
  hVertexPositionFractionalResidualX->Write("VertexPositionFractionalResidualX", TObject::kWriteDelete);

  TH1D* hVertexPositionFractionalResidualY(sVertexPositionFractionalResidualY.ToTH1(pot));
  hVertexPositionFractionalResidualY->SetName("VertexPositionFractionalResidualY");
  hVertexPositionFractionalResidualY->Write("VertexPositionFractionalResidualY", TObject::kWriteDelete);

  TH1D* hVertexPositionFractionalResidualZ(sVertexPositionFractionalResidualZ.ToTH1(pot));
  hVertexPositionFractionalResidualZ->SetName("VertexPositionFractionalResidualZ");
  hVertexPositionFractionalResidualZ->Write("VertexPositionFractionalResidualZ", TObject::kWriteDelete);

  fout->Close();

}
