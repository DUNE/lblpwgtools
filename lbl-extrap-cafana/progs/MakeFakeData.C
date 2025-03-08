#include "argparse/argparse.hpp"

#include "TFile.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "OscLib/IOscCalc.h"

ana::Spectrum MakeFakeData(const ana::IPrediction* pred, osc::IOscCalcAdjustable * calc, int exposureYears, bool poisson)
{
	ana::Spectrum CV = pred->Predict(calc);
	auto pot = ana::kFDPOT.at(exposureYears);
	if (poisson)
		return CV.MockData(pot);
	else
		return CV.AsimovData(pot);
}

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("MakeFakeData");

	program.add_argument("--infile")
	       .required()
	       .help("Path to prediction file to use as input");

	program.add_argument("--pred-folder")
	       .help("Name of folder within file")
	       .default_value("pred");

	program.add_argument("--calc")
	       .scan<'i', unsigned int>()
	       .help("Oscillation calc configuration to use (see CAFAna/Analysis/CalcsNuFit.h)")
	       .default_value(0);

	program.add_argument("--poisson")
	       .implicit_value(true)
	       .default_value(false)
	       .help("add Poisson fluctuations to fake data?");

	// ugh
	std::set<uint> validYears;
	std::transform(ana::kFDPOT.begin(), ana::kFDPOT.end(), std::inserter(validYears, validYears.end()),
	               [](const auto & yearPOTpair) { return yearPOTpair.first; });

	program.add_argument("--exposure-years")
	       .help("Number of years exposure to use (valid options: " +
	       	     std::accumulate(validYears.begin(), validYears.end(), std::string(),
	       	                     [](const std::string & s, int y){return s + ", " + std::to_string(y);}))
	       .default_value(7);

	program.add_argument("--outfile")
	       .required()
	       .help("Path to file where fake data spectrum should be stored");

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

	std::unique_ptr<osc::IOscCalcAdjustable> calc(ana::NuFitOscCalc(1, 1, program.get<int>("calc")));

	std::unique_ptr<ana::IPrediction> pred = ana::LoadFromFile<ana::IPrediction>(program.get<std::string>("infile"),
	                                                                             program.get<std::string>("pred-folder"));

	TFile outfile(program.get<std::string>("outfile").c_str(), "recreate");
	if (outfile.IsZombie())
	{
		std::cerr << "Designated output file '" << outfile.GetName() << "' cannot be opened.  Abort.\n";
		return 1;
	}

	ana::Spectrum fd = MakeFakeData(pred.get(), calc.get(), program.get<int>("exposure-years"), program.get<bool>("poisson"));
	fd.SaveTo(&outfile, "fake-data");

	ana::SaveTo(*calc, &outfile, "calc");

	return 0;
}
