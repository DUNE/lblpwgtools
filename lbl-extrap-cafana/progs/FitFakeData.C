/*
 *  FitFakeData.C:
 *
 *    Demonstration fitting using fake data generated from FD VD 2024 files.
 *
 *   Original author:  J. Wolcott <jwolcott@fnal.gov>
 *                     March 2025
 *
 */

#include "argparse/argparse.hpp"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Vars/FitVars.h"

#include "OscLib/OscCalcPMNSOpt.h"

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("FitFakeData");

	program.add_argument("--predfile")
	       .required()
	       .help("Path to prediction file to use as input");

	program.add_argument("--predname")
	   .default_value("pred")
	   .help("Name of prediction object within prediction files");

	program.add_argument("--fdfile")
	   .required()
	   .help("Path to fake data file to use as input");

	program.add_argument("--fdname")
	   .default_value("fake-data")
	   .help("Name of Spectrum object within fake data file");

	program.add_argument("--calcname")
		   .default_value("calc")
		   .help("Name of calculator object within fake data file");

	program.add_argument("--outfile")
	       .required()
	       .help("Path to file where output surfaces should be stored");

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

	std::unique_ptr<TFile> outFile(TFile::Open(program.get<std::string>("outfile").c_str(), "recreate"));
	if (!outFile)
	{
		std::cerr << "Error opening output file: '" << program.get<std::string>("outfile") << "'.  Abort\n";
		exit(1);
	}

	std::unique_ptr<TFile> fdFile(TFile::Open(program.get<std::string>("fdfile").c_str()));
	if (!fdFile)
	{
		std::cerr << "Could not open fake data file: '" << fdFile->GetName() << "'.  Abort\n";
		exit(1);
	}

	std::unique_ptr<ana::IPrediction> pred = ana::LoadFromFile<ana::PredictionNoExtrap>(program.get<std::string>("predfile"),
		                                                                                program.get<std::string>("predname"));

	std::unique_ptr<osc::IOscCalc> c = ana::LoadFrom<osc::IOscCalc>(fdFile.get(), program.get<std::string>("calcname"));
	auto calc = dynamic_cast<osc::IOscCalcAdjustable*>(c.get());

	std::unique_ptr<ana::Spectrum> fakedata = ana::LoadFrom<ana::Spectrum>(fdFile.get(),
		                                                                       program.get<std::string>("fdname"));

	ana::SingleSampleExperiment expt(pred.get(), *fakedata);
	ana::FrequentistSurface surface(&expt, calc,
		                            &ana::kFitSinSq2Theta23, 50, 0.5, 1,
		                            &ana::kFitDmSq32Scaled, 100, 2.2, 2.65);

	surface.SaveTo(outFile.get(), "ss2th23-dm32");

	std::cout << "Best fit point: (" << surface.GetBestFitX() << ", " << surface.GetBestFitY() << ")\n";
	std::cout << "Truth point: (" << ana::kFitSinSq2Theta23.GetValue(calc) << ", " << ana::kFitDmSq32Scaled.GetValue(calc) << ")\n";

	return 0;
}
