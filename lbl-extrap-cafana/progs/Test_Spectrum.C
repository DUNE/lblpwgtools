
#include <iostream>
#include <string>

#include "argparse/argparse.hpp"

#include "TH1D.h"
#include "TCanvas.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "lbl2025/Axes.h"
#include "lbl2025/Cuts.h"

void Test_Spectrum(const std::string&);

int main(int argc, char **argv)
{
	argparse::ArgumentParser program("Test_Spectrum");

	program.add_argument("outdir")
	       .help("Directory where output should be written");

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

	Test_Spectrum(program.get<std::string>("outdir"));
	return 0;
}


void Test_Spectrum(const std::string & outDir)
{
	const std::string fname ="root://fndca1.fnal.gov/pnfs/fnal.gov/usr/dune/persistent/staging/fardet-vd/6d/35/fardet-vd_mc_list-merge_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000060_final_20241024T024241.root";

	ana::SpectrumLoader loader(fname);

	ana::Spectrum spec_muScores(loader.Interactions(ana::RecoType::kPandora)[ana::kIsNumu].RecoParticles(ana::RecoType::kPandora)[lbl2025::kPartIsMuon], lbl2025::ax_PartScore);

	loader.Go();

	std::unique_ptr<TH1D> h_muScores(spec_muScores.ToTH1(spec_muScores.POT()));

	TCanvas c;
	h_muScores->DrawCopy("hist");
	c.SaveAs( (outDir + "/" + "muscore.png").c_str() );
}

