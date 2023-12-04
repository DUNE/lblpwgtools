#include "TFile.h"
#include "TH1D.h"

#include <iomanip>
#include <iostream>

void MakeTable() {
  TFile fout("OAPSpec.root", "READ");
  TH1D *oap_numu = nullptr;
  fout.GetObject<TH1D>("oap_numu", oap_numu);
  TH1D *oap_numu_sel = nullptr;
  fout.GetObject<TH1D>("oap_numu_sel", oap_numu_sel);
  TH1D *oap_numu_sel_LAr_cont = nullptr;
  fout.GetObject<TH1D>("oap_numu_sel_LAr_cont", oap_numu_sel_LAr_cont);
  TH1D *oap_numu_sel_tracker = nullptr;
  fout.GetObject<TH1D>("oap_numu_sel_tracker", oap_numu_sel_tracker);

  oap_numu->RebinX(2);
  oap_numu_sel->RebinX(2);
  oap_numu_sel_LAr_cont->RebinX(2);
  oap_numu_sel_tracker->RebinX(2);

  oap_numu->Scale((1.1e21 / 12.0) * (1.4 / (1 * 2 * 3)));
  oap_numu_sel->Scale((1.1e21 / 12.0) * (1.4 / (1 * 2 * 3)));
  oap_numu_sel_LAr_cont->Scale((1.1e21 / 12.0) * (1.4 / (1 * 2 * 3)));
  oap_numu_sel_tracker->Scale((1.1e21 / 12.0) * (1.4 / (1 * 2 * 3)));

  std::cout << "Position /m & $N\\nu_{\\mu}$CC / 1000 kg month & "
               "$\\sqrt(N\\nu_{\\mu})/N\\nu_{\\mu}$ & Selected & Selected (LAr "
               "Cont.) & Selected (GAr) \\\\ "
            << std::endl;
  for (Int_t i = 0; i < oap_numu->GetXaxis()->GetNbins(); i += 4) {
    std::cout << std::setprecision(3)
              << oap_numu->GetXaxis()->GetBinCenter(i + 1) - 0.25 << " & "
              << oap_numu->GetBinContent(i + 1) << " & " << std::setprecision(2)
              << (oap_numu_sel->GetBinContent(i + 1) /
                  oap_numu->GetBinContent(i + 1)) *
                     100.0
              << "\\% & " << std::setprecision(1)
              << (sqrt(oap_numu_sel->GetBinContent(i + 1)) /
                  oap_numu_sel->GetBinContent(i + 1)) *
                     100.0
              << "\\% & " << std::setprecision(2)
              << (oap_numu_sel_LAr_cont->GetBinContent(i + 1) /
                  oap_numu->GetBinContent(i + 1)) *
                     100.0
              << "\\% & "
              << (oap_numu_sel_tracker->GetBinContent(i + 1) /
                  oap_numu->GetBinContent(i + 1)) *
                     100.0
              << "\\% \\\\" << std::endl;
  }
}
