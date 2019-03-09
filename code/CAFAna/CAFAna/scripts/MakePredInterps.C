#include "common_fit_definitions.C"

inline std::string str_replace(std::string const &inp, std::string const &from,
                               std::string const &to) {
  std::stringstream ss("");

  size_t nextOccurence = 0;
  size_t prevOccurence = 0;
  bool AtEnd = false;
  while (!AtEnd) {
    nextOccurence = inp.find(from, prevOccurence);
    if (nextOccurence == std::string::npos) {
      if (prevOccurence == inp.length()) {
        break;
      }
      AtEnd = true;
    }
    if ((nextOccurence != prevOccurence) || (nextOccurence == 0)) {
      ss << inp.substr(prevOccurence, (nextOccurence - prevOccurence));
      if (!AtEnd) {
        ss << to;
      }
    }
    prevOccurence = nextOccurence + from.size();
  }
  return ss.str();
}

template <typename T> inline T str2T(std::string const &str) {
  std::istringstream stream(str);
  T d;
  stream >> d;

  if (stream.fail()) {
    std::cerr << "[WARN]: Failed to parse string: " << str
              << " as requested type." << std::endl;
    return T();
  }

  return d;
}

int main(int argc, char const *argv[]) {

  if ((argc < 2) || (std::string(argv[1]) == "-?") ||
      (std::string(argv[1]) == "--help") || (argc < 6)) {
    std::cout << "[RUNLIKE]: " << argv[0]
              << " <Sample Type> </path/to/file_%N.root> <NStart> <NLast> "
                 "<OutputFileName> [<AxesBlobName>]"
              << std::endl;
    return 0;
  }

  SampleType stype = GetSampleType(argv[1]);
  std::string file_name_fmt = argv[2];
  size_t NStart = str2T<size_t>(argv[3]);
  size_t NLast = str2T<size_t>(argv[4]);
  std::string output_file_name = argv[5];

  std::string AxisBlobName = "default";
  if (argc == 7) {
    AxisBlobName = argv[6];
    std::cout << "[INFO]: Will build state for " << AxisBlobName
              << " axis definition." << std::endl;
  }

  std::vector<std::string> file_list;

  for (size_t i = NStart; i <= NLast; ++i) {
    file_list.push_back(str_replace(file_name_fmt, "%N", std::to_string(i)));
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

    PredictionInterp predInterpNDNumu(GetListOfSysts(), nullptr, genNDNumu,
                                      these_loaders);
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
