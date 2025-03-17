/*
*  SelectionPlots.C:
 *
 *    Plots investigating the performance of the selection.
 *
 *   Original author:  J. Wolcott <jwolcott@fnal.gov>
 *                     March 2025
 *
 */

#include "argparse/argparse.hpp"

#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"

#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Spectrum.h"

#include "DUNEStyle.h"

#include "lbl2025/Cuts.h"
#include "lbl2025/Vars.h"
#include "lbl2025/Loaders.h"


namespace lbl2025
{
	const std::map<std::string, ana::HistAxis> vtxAxes
	{
			{"X", {"Reco. vertex x (cm)", 100, -400, 400, lbl2025::kRecoVtxX}},
			{"Y", {"Reco. vertex y (cm)", 100, -750, 750, lbl2025::kRecoVtxY}},
			{"Z", {"Reco. vertex z (cm)", 100, -50, 1050, lbl2025::kRecoVtxZ}},
	};

	const ana::HistAxis CVNAxis {"#nu_{#mu} CVN score", 100, 0, 1, lbl2025::kNumuCVNScore};
}


void MakePlots(const std::string & plotdir)
{
	lbl2025::Loaders loaders(ana::RecoType::kPandora);
	ana::SpectrumLoader & fhc_nd_nonswap_loader = loaders.GetLoaders(ana::FluxType::kFHC)->GetSource(ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNonSwap);

	std::map<std::string, ana::Spectrum> specs_vtx;
	for (const auto & vtxVarPair : lbl2025::vtxAxes)
	{
		specs_vtx.emplace(std::piecewise_construct,
		                  std::forward_as_tuple(vtxVarPair.first),
		                  std::forward_as_tuple(fhc_nd_nonswap_loader.Interactions(ana::RecoType::kPandora), vtxVarPair.second));
	}
	ana::Spectrum spec_numuCVN(fhc_nd_nonswap_loader.Interactions(ana::RecoType::kPandora)[lbl2025::kRecoVtxInFid], lbl2025::CVNAxis);

	loaders.Go();

	dunestyle::SetDuneStyle();

	for (const auto & vtxSpecPair : specs_vtx)
	{
		TCanvas c;
		std::unique_ptr<TH1> h(vtxSpecPair.second.ToTH1(vtxSpecPair.second.POT()));
		h->Draw("hist");
		dunestyle::CenterTitles(h.get());
		TLatex l(0.6, 0.8, "All events");
		l.SetNDC(true);
		l.Draw();
		c.SaveAs( Form("%s/Vtx%s.png", plotdir.c_str(), vtxSpecPair.first.c_str()) );
	}

	{
		TCanvas c;
		std::unique_ptr<TH1> h(spec_numuCVN.ToTH1(spec_numuCVN.POT()));
		h->Draw("hist");
		dunestyle::CenterTitles(h.get());
		TLatex l(0.6, 0.8, "Fid. vtx. events");
		l.SetNDC(true);
		l.Draw();
		c.SaveAs( Form("%s/NumuCVN_FidVtxEvts.png", plotdir.c_str()) );
	}
}

int main(int argc, char **argv)
{
	argparse::ArgumentParser program("SelectionPlots");

	program.add_argument("--plotdir")
	       .required()
		   .help("Directory where output plots should go");

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

	auto plotdir = program.get<std::string>("plotdir");
	if (! std::filesystem::is_directory(plotdir))
	{
		std::cerr << "Output plot directory '" << plotdir << "' is not a directory\n";
		exit(1);
	}

	MakePlots(plotdir);

	return 0;
}
