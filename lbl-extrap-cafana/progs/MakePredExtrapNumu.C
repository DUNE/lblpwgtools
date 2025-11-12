/*
 *  MakePredExtrapNumu.C:
 *
 *  Demonstration prediction generation using NumuDecomp and NumuExtrap for a proof-of-principle LBL analysis w/ FD VD 2024 files.
 *
 *  Original authors: A. Booth <abooth@fnal.gov> & J. Wolcott <jwolcott@fnal.gov>
 *  October 2025
 *
 */

#include "argparse/argparse.hpp"

#include "TFile.h"

#include "lbl2025/Axes.h"
#include "lbl2025/Cuts.h"
#include "lbl2025/Loaders.h"
#include "lbl2025/PredictionGenerator3Flavor.h"


void MakePredExtrapNumu(TFile & outfile);

int main(int argc, char **argv)
{
	argparse::ArgumentParser program("MakePredExtrapNumu");

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

	MakePredExtrapNumu(outfile);
	return 0;
}


void MakePredExtrapNumu(TFile & outfile)
{
  lbl2025::Loaders loaders(ana::RecoType::kPandora);

  //lbl2025::NumuExtrapGenerator predGen(lbl2025::ax_RecoEnu, lbl2025::kRecoVtxInFid && lbl2025::kRecoNumuFD, lbl2025::kRecoVtxInFidND && lbl2025::kRecoNumuND);
  //lbl2025::NumuExtrapGenerator predGen(lbl2025::ax_RecoEnu, lbl2025::kRecoVtxInFid && lbl2025::kRecoNumuFD, lbl2025::kRecoVtxInFidND);
  //lbl2025::NumuExtrapGenerator predGen(lbl2025::ax_RecoEnu, lbl2025::kRecoVtxInFid, lbl2025::kRecoVtxInFidND);
  lbl2025::NumuExtrapGenerator predGen(lbl2025::ax_RecoEnu, lbl2025::kRecoVtxInFid, ana::kNoCut);
  std::unique_ptr<ana::IPrediction> pred = predGen.Generate(*loaders.GetLoaders(ana::FluxType::kFHC), ana::RecoType::kPandora);

  loaders.Go();

  pred->SaveTo(&outfile, "pred");
}
