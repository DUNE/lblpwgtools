inline double FD_ND_FVRatio(double x_slice_cm) {
  double NDSliceFV = x_slice_cm * 200 * (350 - 50);
  double FDFV = 620 * 1100 * (1244 - 50);

  return FDFV / NDSliceFV;
}

void dumpTalkHists(){
  TFile *f = new TFile("CAF_FHC_PRISM_PROD4-10.root");
  TTree *t = (TTree *)(f->Get("FileSummaryTree"));

  TH1D *stop_POT = new TH1D("stop_POT","", 9, -2, 34);
  t->Draw("det_x >> stop_POT","POT","GOFF");
  stop_POT->SetDirectory(nullptr);

  TFile fout("TalkPlots.root","RECREATE");
  stop_POT->Write();
  fout.Write();

  std::cout << 1.0/FD_ND_FVRatio(50) << std::endl;
}
