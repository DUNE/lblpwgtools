/*
 *  FitFakeData.C:
 *
 *    Demonstration fitting using fake data generated from FD VD 2024 files.
 *
 *   Original author:  J. Wolcott <jwolcott@fnal.gov>
 *                     March 2025
 *
 */

#include <CAFAna/Vars/FitVars.h>

#include "argparse/argparse.hpp"

#include "TCanvas.h"
#include "TLegend.h"
#include "TMarker.h"

#ifdef LBL2025_USE_DUNESTYLE
#include "DUNEStyle.h"
#endif

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Fit/FrequentistSurface.h"

#include "OscLib/OscCalcPMNSOpt.h"

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("FitFakeData");

	program.add_argument("--fitfile")
	       .required()
	       .help("Path to file with fit results to use as input");

	program.add_argument("--fdfile")
	   .required()
	   .help("Path to fake data file to use as input (for truth info)");

	program.add_argument("--calcname")
	   .default_value("calc")
	   .help("Name of calculator object within fake data file");

	program.add_argument("--plotdir")
	       .required()
	       .help("Path to file where output plots should be stored");

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

	std::unique_ptr<ana::FrequentistSurface> surface = ana::LoadFromFile<ana::FrequentistSurface>(program.get<std::string>("fitfile"), "ss2th23-dm32");
	std::unique_ptr<osc::IOscCalc> _c = ana::LoadFromFile<osc::IOscCalc>(program.get<std::string>("fdfile"), program.get<std::string>("calcname"));
	auto calc = dynamic_cast<osc::IOscCalcAdjustable*>(_c.get());

	auto plotdir = program.get<std::string>("plotdir");

#ifdef LBL2025_USE_DUNESTYLE
	dunestyle::SetDuneStyle();

	std::vector contourColors{dunestyle::colors::kOkabeItoVermilion, dunestyle::colors::kOkabeItoBlue, dunestyle::colors::kOkabeItoYellow};
	dunestyle::colors::Color_t bfColor = dunestyle::colors::kOkabeItoRedPurple;
	dunestyle::colors::Color_t trueColor = dunestyle::colors::kOkabeItoBlueGreen;
#else
#endif

	// it'd be better if the surface 'knew' which vars were used to make it...
	std::vector<const ana::IFitVar*> fitVars {&ana::kFitSinSq2Theta23, &ana::kFitDmSq32Scaled};

	TCanvas c;
	surface->Draw();

	TH2* crit1sig = ana::Gaussian68Percent2D(*surface);
	TH2* crit2sig = ana::Gaussian2Sigma2D(*surface);
	TH2* crit3sig = ana::Gaussian3Sigma2D(*surface);

	surface->DrawContour(crit1sig, kSolid, contourColors[0]);
	surface->DrawContour(crit2sig, kDashed, contourColors[1]);
	surface->DrawContour(crit3sig, kDotted, contourColors[2]);

	std::cout << "true value: " << fitVars[0]->GetValue(calc) << ", " << fitVars[1]->GetValue(calc) << "\n";
	TMarker markerTrue(fitVars[0]->GetValue(calc), fitVars[1]->GetValue(calc), kFullCircle);
	markerTrue.SetNDC(false);
	markerTrue.SetMarkerColor(trueColor);
	markerTrue.Draw();

	TMarker markerBF(surface->GetBestFitX(), surface->GetBestFitY(), kFullTriangleUp);
	markerBF.SetNDC(false);
	markerBF.SetMarkerColor(bfColor);
	markerBF.Draw();

	TLegend leg(0.2, 0.7, 0.4, 0.9);
	leg.AddEntry(&markerTrue, "Truth", "p");
	leg.AddEntry(&markerBF, "Best fit", "p");
	leg.SetFillStyle(0);
	leg.Draw();

	c.SetLogz();
	// avert thine eyes
	for (TObject * o : *c.GetListOfPrimitives())
	{
		if (auto h = dynamic_cast<TH2F*>(o))
			h->GetZaxis()->SetTitle("#Delta #chi^{2}");  // avert thine eyes
	}
	c.SaveAs( Form("%s/surface.png", plotdir.c_str()));

	std::vector<TH2*> hists = surface->GetProfiledHists();
	for (unsigned int i = 0; i < hists.size(); ++i)
	{
		c.Clear();
		hists[i]->Draw();
		c.SaveAs(Form("%s/%s.png", plotdir.c_str(), fitVars[i]->ShortName().c_str()));
	}

	return 0;
}

