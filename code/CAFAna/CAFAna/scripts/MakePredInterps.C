#include "common_fit_definitions.C"

#include <cstdio>

int main(int argc, char const *argv[]) {

  if ((argc < 2) || (std::string(argv[1]) == "-?") ||
      (std::string(argv[1]) == "--help") || (argc < 6)) {
    std::cout << "[RUNLIKE]: " << argv[0]
              << " <Sample Type> </path/to/file_%d.root> <NStart> <NLast> "
                 "<OutputFileName> [<AxesBlobName>]"
              << std::endl;
    return 0;
  }

  SampleType stype = GetSampleType(argv[1]);
  std::string file_name_fmt = argv[2];
  size_t NStart = std::stol(argv[3]);
  size_t NLast = std::stol(argv[4]);
  std::string output_file_name = argv[5];

  std::string AxisBlobName = "default";
  if (argc == 7) {
    AxisBlobName = argv[6];
    std::cout << "[INFO]: Will build state for " << AxisBlobName
              << " axis definition." << std::endl;
  }

  std::vector<std::string> file_list;

  for (size_t i = NStart; i <= NLast; ++i) {
    char buf[1000];
    snprintf(buf, 1000, file_name_fmt.c_str(), i);
    file_list.push_back(buf);
    std::cout << "[INFO]: Will read " << file_list.back() << std::endl;
  }

  AxisBlob axes = GetAxisBlob(AxisBlobName);

  TFile fout(output_file_name.c_str(), "RECREATE");

  bool isfhc = (stype == kNDFHC);
  if ((stype == kNDFHC) || (stype == kNDRHC)) {
    // Now ND
    Loaders these_loaders;
    SpectrumLoader loaderNumu(file_list);
    these_loaders.AddLoader(&loaderNumu, caf::kNEARDET, Loaders::kMC);

    NoOscPredictionGenerator genNDNumu(
        *axes.NDAx, (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV,
        kGENIEWeights);

    PredictionInterp predInterpNDNumu(GetListOfSysts(), NuFitOscCalc(1),
                                      genNDNumu, these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(stype) << std::endl;
    predInterpNDNumu.SaveTo(fout.mkdir(
        (std::string("nd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));
  } else {
    std::cout << "[ERROR]: Sample type " << argv[1]
              << " is not yet supported by this app." << std::endl;
    return 1;
  }
  fout.Write();
}
