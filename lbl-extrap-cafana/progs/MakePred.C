#include "argparse/argparse.hpp"

#include "TFile.h"

#include "CAFAna/Extrap/TrivialExtrap.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "lbl2025/Axes.h"
#include "lbl2025/Cuts.h"
#include "lbl2025/Loaders.h"

void MakePred(TFile & outfile);

int main(int argc, char **argv)
{
	argparse::ArgumentParser program("MakePred");

	program.add_argument("outfile")
		   .help("Path to file where prediction objects should be stored");

	try
	{
		program.parse_args(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		std::cerr << program << "\n";
		return 1;
	}

	TFile outfile(program.get<std::string>("outfile").c_str(), "recreate");
    if (outfile.IsZombie())
    {
      std::cerr << "Designated output file '" << outfile.GetName() << "' cannot be opened.  Abort.\n";
    	return 1;
    }

	MakePred(outfile);
	return 0;
}


void MakePred(TFile & outfile)
{
  lbl2025::Loaders loaders(ana::RecoType::kPandora);

  ana::NoExtrapGenerator predGen(lbl2025::ax_RecoEnu, lbl2025::kRecoNumuFD);
  std::unique_ptr<ana::IPrediction> pred = predGen.Generate(*loaders.GetLoaders(ana::FluxType::kFHC), ana::RecoType::kPandora);

  loaders.Go();


  pred->SaveTo(&outfile, "pred");
}