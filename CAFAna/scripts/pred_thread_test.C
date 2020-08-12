#include "CAFAna/Analysis/common_fit_definitions.h"

#include <omp.h>

using namespace ana;

int main(int argc, char const *argv[]) {

  // Make sure the syst registry has been populated with all the systs we could
  // want to use
  (void)GetListOfSysts();

  gROOT->SetBatch(1);
  gROOT->SetMustClean(false);

  TFile *fin = TFile::Open(argv[1], "READ"); // Allows xrootd streaming
  if (!(fin && !fin->IsZombie())) {
    std::cout << "[ERROR]: Failed to read " << argv[1] << std::endl;
    return 1;
  }

  std::cout << "[LOAD]: Retrieving fd_interp_numu_fhc from " << argv[1]
            << std::endl;
  std::unique_ptr<PredictionInterp> FD_FHCNumu =
    LoadFrom<PredictionInterp>(fin, "fd_interp_numu_fhc");
  delete fin;

  std::vector<const ISyst *> systlist = FD_FHCNumu->GetAllSysts();
  osc::IOscCalcAdjustable *osc = NuFitOscCalc(1, 1, 0);

  for (size_t try_it = 0; try_it < 10000; ++try_it) {

    SystShifts fakeThrowSyst;
    for (auto s : systlist) {
      fakeThrowSyst.SetShift(
          s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    }

    omp_set_num_threads(4);
    Spectrum s_4 = FD_FHCNumu->PredictSyst(osc, fakeThrowSyst);
    omp_set_num_threads(1);
    Spectrum s_1 = FD_FHCNumu->PredictSyst(osc, fakeThrowSyst);

    TH1 *h_4 = s_4.ToTH1(1E22);
    TH1 *h_1 = s_1.ToTH1(1E22);

    for (int bi_it = 0; bi_it < h_4->GetXaxis()->GetNbins(); ++bi_it) {
      if (fabs(h_4->GetBinContent(bi_it + 1) - h_1->GetBinContent(bi_it + 1)) >
          1E-10) {
        std::cout << "[ERROR]: Try " << try_it << " hist_bin = " << bi_it
                  << ", found difference between threaded and unthreaded: "
                  << h_4->GetBinContent(bi_it + 1)
                  << " != " << h_1->GetBinContent(bi_it + 1) << std::endl;
      }
    }
  }
}
