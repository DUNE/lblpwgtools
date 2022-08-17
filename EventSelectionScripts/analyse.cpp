//#include "paul_tol_colors.hpp"

TFile *f = NULL;
TTree *caf = NULL;

void analyse(std::string name) {
  f = new TFile(name.c_str());
  caf = (TTree*)f->Get("cafTree");

  // Set up some selection requirements

  // Number of tracks in LAr
  TString LArNTrk = "rec.nd.lar.ntracks == 1";
  // Number of tracks in TMS
  TString TMSNTrk = "rec.nd.tms.ntracks == 1 && rec.nd.tms.tracks[0].qual > 0.9";

  // Start point of TMS track, i.e. front-entering TMS
  TString TMSTrkStart = "rec.nd.tms.tracks[0].start.z < 11600 && ";
  TMSTrkStart += "abs(rec.nd.tms.tracks[0].start.x) < 3000";

  TString TMSTrkEnd = "rec.nd.tms.tracks[0].end.z < 18000 &&";
  TMSTrkEnd += "abs(rec.nd.tms.tracks[0].end.x) < 3000";

  // Start point of LAr track, i.e. vertex in LAr FV
  TString LArTrkStart = "abs(rec.nd.lar.tracks[0].start.x) < 300 && ";
  LArTrkStart += "rec.nd.lar.tracks[0].start.y > -150 && ";
  LArTrkStart += "rec.nd.lar.tracks[0].start.y < 50 && ";
  LArTrkStart += "rec.nd.lar.tracks[0].start.z > 450 && ";
  LArTrkStart += "rec.nd.lar.tracks[0].start.z < 850";

  // End point of LAr track, i.e. exits LAr within FV
  TString LArTrkEnd = "rec.nd.lar.tracks[0].end.z > 900 && ";
  LArTrkEnd += "abs(rec.nd.lar.tracks[0].end.x) < 300 && ";
  LArTrkEnd += "rec.nd.lar.tracks[0].end.y > -150 && ";
  LArTrkEnd += "rec.nd.lar.tracks[0].end.y < 50";

  TString LArTrkCont = "rec.nd.lar.tracks[0].end.z < 900 && ";
  LArTrkCont += "abs(rec.nd.lar.tracks[0].end.x) < 300 && ";
  LArTrkCont += "rec.nd.lar.tracks[0].end.y > -150 && ";
  LArTrkCont += "rec.nd.lar.tracks[0].end.y < 50";

  TString TrkMatch = "rec.nd.ntrkmatch == 1";

  // Drawing variable
  TString DrawVar = "rec.LepE";
  // rec.LepNuAngle
  // rec.LepPDG
  // rec.nuPDG
  // rec.isCC
  // rec.Ev
  TString Binning = "(50, 0, 10)";

  /*
  caf->Draw(DrawVar, LArNTrk + " && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  caf->Draw(DrawVar, LArNTrk + " && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd, "same");
                     */

  caf->Draw(DrawVar+">>h0"+Binning, TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  caf->Draw(DrawVar+">>h1"+Binning, "rec.nd.lar.ntracks == 1 && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  caf->Draw(DrawVar+">>h2"+Binning, "rec.nd.lar.ntracks == 2 && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  caf->Draw(DrawVar+">>h3"+Binning, "rec.nd.lar.ntracks == 3 && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  caf->Draw(DrawVar+">>h4"+Binning, "rec.nd.lar.ntracks == 4 && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  caf->Draw(DrawVar+">>h5"+Binning, "rec.nd.lar.ntracks > 4 && " +
                     TMSNTrk + " && " +
                     TMSTrkStart + " && " +
                     TMSTrkEnd + " && " +
                     LArTrkStart + " && " +
                     LArTrkEnd + " && " +
                     TrkMatch);

  TH1D *h0 = (TH1D*)gDirectory->Get("h0");
  TH1D *h1 = (TH1D*)gDirectory->Get("h1");
  TH1D *h2 = (TH1D*)gDirectory->Get("h2");
  TH1D *h3 = (TH1D*)gDirectory->Get("h3");
  TH1D *h4 = (TH1D*)gDirectory->Get("h4");
  TH1D *h5 = (TH1D*)gDirectory->Get("h5");

  h0->SetTitle("Any number of LAr tracks");
  h1->SetTitle("One LAr track");
  h2->SetTitle("Two LAr tracks");
  h3->SetTitle("Three LAr tracks");
  h4->SetTitle("Four LAr tracks");
  h5->SetTitle("More than four LAr tracks");

  /*
  h0->SetLineColor(kBlack);
  h1->SetLineColor(tolcols::kTBriBlue);
  h2->SetLineColor(tolcols::kTBriGreen);
  h3->SetLineColor(tolcols::kTBriYellow);
  h4->SetLineColor(tolcols::kTBriRed);
  h5->SetLineColor(tolcols::kTBriPurple);
  */

  h0->SetLineWidth(2);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);
  h5->SetLineWidth(2);

  TCanvas *canv = new TCanvas("canv", "canv", 1024, 1024);
  h0->Draw();
  h1->Draw("same");
  h2->Draw("same");
  h3->Draw("same");
  h4->Draw("same");
  h5->Draw("same");

  TLegend *leg = (TLegend*)canv->BuildLegend();
  leg->SetHeader("Single TMS track with LAr track match");
  leg->SetX1(0.38);
  leg->SetX2(0.99);
  leg->SetY1(0.44);
  leg->SetY2(0.87);

  h0->GetXaxis()->SetTitle("True lepton momentum (GeV/c)");

  canv->Print("plot.pdf");
}
