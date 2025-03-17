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
#include "TLine.h"
#include "TMarker.h"

#ifdef LBL2025_USE_DUNESTYLE
#include "DUNEStyle.h"
#endif

#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/Style.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "OscLib/OscCalcPMNSOpt.h"

// -------------------------------------------------------------------
void SaveSurfacePlot(ana::FrequentistSurface * surface, const osc::IOscCalcAdjustable * calc, const std::string & plotdir)
{

#ifdef LBL2025_USE_DUNESTYLE
	dunestyle::SetDuneStyle();

	// std::vector contourColors{static_cast<dunestyle::colors::Color_t>(kBlack), dunestyle::colors::kOkabeItoVermilion, dunestyle::colors::kOkabeItoBlue, dunestyle::colors::kOkabeItoYellow};
	std::vector<dunestyle::colors::Color_t> contourColors{kBlack, kBlack, kBlack};
	dunestyle::colors::Color_t bfColor = dunestyle::colors::kOkabeItoRedPurple;
	dunestyle::colors::Color_t trueColor = dunestyle::colors::kOkabeItoBlueGreen;

	dunestyle::CherryInvertedPalette();

#else
#endif

	// it'd be better if the surface 'knew' which vars were used to make it...
	std::vector<const ana::IFitVar*> fitVars {&ana::kFitSinSq2Theta23, &ana::kFitDmSq32Scaled};

	TCanvas c;
	surface->Draw();

	std::cout << "true value: " << fitVars[0]->GetValue(calc) << ", " << fitVars[1]->GetValue(calc) << "\n";
	TMarker markerTrue(fitVars[0]->GetValue(calc), fitVars[1]->GetValue(calc), kFullCircle);
	markerTrue.SetNDC(false);
	markerTrue.SetMarkerColor(trueColor);
	markerTrue.Draw();

	TMarker markerBF(surface->GetBestFitX(), surface->GetBestFitY(), kFullTriangleUp);
	markerBF.SetNDC(false);
	markerBF.SetMarkerColor(bfColor);
	markerBF.Draw();


	TLegend leg(0.15, 0.6, 0.3, 0.9);
	leg.AddEntry(&markerTrue, "Truth", "p");
	leg.AddEntry(&markerBF, "Best fit", "p");
	// leg.SetFillStyle(0);
	// leg.SetTextColor(kWhite);

	std::vector critsig {ana::Gaussian68Percent2D(*surface), ana::Gaussian2Sigma2D(*surface), ana::Gaussian3Sigma2D(*surface)};
	std::vector<std::string> labels {"1 #sigma", "2 #sigma", "3 #sigma"};
	std::vector<TLine> lines(3);

	std::vector lineStyles {kSolid, kDashed, kDotted};
	for (std::size_t idx = 0; idx < 3; ++idx)
	{
		lines[idx].SetLineWidth(2);
		lines[idx].SetLineColor(contourColors[idx]);
		lines[idx].SetLineStyle(lineStyles[idx]);

		surface->DrawContour(critsig[idx], lines[idx].GetLineStyle(), lines[idx].GetLineColor());
		leg.AddEntry(&lines[idx], labels[idx].c_str(), "l");
	}



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
}

// -------------------------------------------------------------------
void Save1DChi2Scans(const ana::FrequentistSurface * surface, const osc::IOscCalcAdjustable * calc, const std::string & plotdir)
{
	std::unique_ptr<TH2> h(surface->ToTH2());
	const std::vector axes {h->GetXaxis(), h->GetYaxis()};

	for (std::size_t idx = 0; idx < axes.size(); ++idx)
	{
		TAxis * axis = axes[idx];
		TCanvas c;
		TH1D prof(axis->GetTitle(), Form(";%s;#Delta #chi^{2}", axis->GetTitle()),
		          axis->GetNbins(), axis->GetXmin(), axis->GetXmax());
		for (int bin = 1; bin <= axis->GetNbins(); ++bin)
		{
			double min = std::numeric_limits<double>::infinity();
			for (int otherbin = 1; otherbin <= axes[int(idx == 0)]->GetNbins(); ++otherbin)
				min = std::min(min, h->GetBinContent(idx == 0 ? bin : otherbin, idx == 0 ? otherbin : bin));
			prof.SetBinContent(bin, min);
		}
		prof.Draw("hist");
		dunestyle::CenterTitles(&prof);
		c.SaveAs(Form("%s/%s.png", plotdir.c_str(), axis->GetTitle()));
	}

}

// -------------------------------------------------------------------
void SaveSpectrumComparisons(const ana::Spectrum * fakedata, const ana::IPrediction * pred, osc::IOscCalcAdjustable * bfCalc, const std::string & plotdir)
{
	osc::NoOscillations noOscCalc;

	ana::Spectrum bfPred = pred->Predict(bfCalc);
	ana::Spectrum noOscPred = pred->Predict(&noOscCalc);

	TCanvas c;
	std::vector<TPad> ps(2);
	TPad * p1 = &ps[0];
	TPad * p2 = &ps[1];
	dunestyle::SplitCanvas(&c, 0.4, p1, p2);
	p1->cd();
	std::unique_ptr<TH1> h_noOscPred(noOscPred.ToTH1(fakedata->POT(), kBlack, kDashed));
	TH1 * h = ana::DataMCComparisonComponents(*fakedata, pred, bfCalc);
	h->GetXaxis()->SetTitle(""); // this is the upper panel, don't need an x-axis label
	h->GetXaxis()->SetLabelSize(0);
	dunestyle::CenterTitles(h);
	h_noOscPred->Draw("hist same");
	h->SetMaximum(h_noOscPred->GetMaximum() * 1.2);

	// have to recreate the lines by hand, since they're not given back to me
	TLegend leg(0.55, 0.65, 0.85, 0.9);
	leg.SetFillStyle(0);
	TMarker m;
	m.SetMarkerStyle(kFullCircle);
	leg.AddEntry(&m, "Fake data", "pe");
	leg.AddEntry(h_noOscPred.get(), "Total no osc.", "l");
	std::vector<TLine> lines(4);

	// this ordering is from CAFAna/Analysis/Plots.cxx in the DataMCComparisonComponents() method.
	// obviously we'd want to do this differently in a more extensive plotting suite,
	// but this is just an example
	std::vector lineColors {ana::kTotalMCColor, ana::kNumuBackgroundColor, ana::kNCBackgroundColor, ana::kBeamNueBackgroundColor};
	std::vector<std::string> labels {"Total best fit", "   #nu_{#mu} sig", "   NC bkgd", "   Beam #nu_{e} bkgd"};
	for (std::size_t idx = 0; idx < lines.size(); idx++)
	{
		lines[idx].SetLineWidth(2);
		lines[idx].SetLineColor(lineColors[idx]);
		leg.AddEntry(&lines[idx], labels[idx].c_str(), "l");
	}
	leg.Draw();

	p2->cd();
	ana::DataMCRatio(*fakedata, noOscPred, 0, 1);
	// I hate you ROOT
	for (const auto o : *p2->GetListOfPrimitives())
	{
		if ( (h = dynamic_cast<TH1*>(o)) )
		{
			h->GetYaxis()->SetTitle("#splitline{Ratio to}{no osc.}");
			dunestyle::CenterTitles(h);
			break;
		}
	}

	ana::OscCurve oscCurve(bfCalc, 14, 14);
	std::unique_ptr<TH1> h_osc(oscCurve.ToTH1());
	h_osc->SetLineColor(kRed);
	h_osc->SetLineStyle(kDashed);
	h_osc->Draw("hist same");

	p2->cd();
	TLegend legLower(0.43, 0.33, 0.84, 0.38);
	legLower.SetFillStyle(0);
	legLower.SetNColumns(2);
	legLower.AddEntry(h, "Total reco.", "l");
	legLower.AddEntry(h_osc.get(), "True #nu_{#mu} (in true E_{#nu})", "l");
	legLower.Draw();

	c.SaveAs(Form("%s/postfit_spectrum.svg", plotdir.c_str()));

}

// -------------------------------------------------------------------
// -------------------------------------------------------------------

int main(int argc, char** argv)
{
	argparse::ArgumentParser program("FitFakeData");

	program.add_argument("--fitfile")
	       .required()
	       .help("Path to file with fit results to use as input");


	program.add_argument("--predfile")
		   .required()
		   .help("Path to prediction file to use as input");

	program.add_argument("--predname")
	   .default_value("pred")
	   .help("Name of prediction object within prediction files");

	program.add_argument("--fdfile")
	   .required()
	   .help("Path to fake data file to use as input (for truth info)");

	program.add_argument("--fdname")
	   .default_value("fake-data")
	   .help("Name of Spectrum object within fake data file");

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
	std::unique_ptr<TFile> fdFile(TFile::Open(program.get<std::string>("fdfile").c_str()));
	if (!fdFile)
	{
		std::cerr << "Could not open fake data file: '" << fdFile->GetName() << "'.  Abort\n";
		exit(1);
	}

	auto plotdir = program.get<std::string>("plotdir");
	if (! std::filesystem::is_directory(plotdir))
	{
		std::cerr << "Output plot directory '" << plotdir << "' is not a directory\n";
		exit(1);
	}


	std::unique_ptr<ana::IPrediction> pred = ana::LoadFromFile<ana::PredictionNoExtrap>(program.get<std::string>("predfile"),
																						program.get<std::string>("predname"));

	std::unique_ptr<osc::IOscCalc> c = ana::LoadFrom<osc::IOscCalc>(fdFile.get(), program.get<std::string>("calcname"));
	auto calc = dynamic_cast<osc::IOscCalcAdjustable*>(c.get());

	std::unique_ptr<ana::Spectrum> fakedata = ana::LoadFrom<ana::Spectrum>(fdFile.get(),
																			   program.get<std::string>("fdname"));



	SaveSurfacePlot(surface.get(), calc, plotdir);
	Save1DChi2Scans(surface.get(), calc, plotdir);
	SaveSpectrumComparisons(fakedata.get(), pred.get(), calc, plotdir);

	return 0;
}

