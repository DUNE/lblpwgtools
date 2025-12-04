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
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Analysis/SpectrumComponents.h"

#include "DUNEStyle.h"

#include "lbl2025/Cuts.h"
#include "lbl2025/Vars.h"
#include "lbl2025/Loaders.h"
#include "lbl2025/Util.h"


namespace lbl2025
{
	const std::map<std::string, ana::SpectrumComponents::Component> kOscComponents
	{
		{"1_numuCC", {lbl2025::kTrueVtxInFid && ana::kIsNumuCC, "True #nu_{#mu} CC", dunestyle::colors::kOkabeItoSkyBlue}},
		{"2_NC", {lbl2025::kTrueVtxInFid && ana::kIsNC, "True NC", dunestyle::colors::kOkabeItoRedPurple}},
		{"3_OOFV", {!lbl2025::kTrueVtxInFid, "True non-fiducial", kGray}},
		{"4_Other", {lbl2025::kTrueVtxInFid && !(ana::kIsNC || ana::kIsNumuCC), "Other", kBlack}},
	};

	const std::map<std::string, ana::HistAxis> kRecoVtxAxes
	{
			{"X", {"Reco. vertex x (cm)", 100, -400, 400, lbl2025::kRecoVtxX}},
			{"Y", {"Reco. vertex y (cm)", 100, -750, 750, lbl2025::kRecoVtxY}},
			{"Z", {"Reco. vertex z (cm)", 100, -50, 1050, lbl2025::kRecoVtxZ}},
	};

	const std::map<std::string, ana::HistAxis> kTrueVtxAxes
	{
		{"X", {"True vertex x (cm)", 100, -400, 400, ana::kTrueVtxX}},
		{"Y", {"True vertex y (cm)", 100, -750, 750, ana::kTrueVtxY}},
		{"Z", {"True vertex z (cm)", 100, -50, 1050, ana::kTrueVtxZ}},
	};

	const ana::HistAxis kCVNAxis {"#nu_{#mu} CVN score", 100, 0, 1, lbl2025::kNumuCVNScore};

	const ana::HistAxis kXYVtxAxis
	{
		{"Reco vertex X (cm)", "Reco vertex Y (cm)"},
		{ana::Binning::Simple(100, -400, 400), ana::Binning::Simple(100, -750, 750)},
		{lbl2025::kRecoVtxX, lbl2025::kRecoVtxY}
	};
}


void MakePlots(const std::string & plotdir)
{
	lbl2025::Loaders loaders(ana::RecoType::kPandora);
	ana::IInteractionSource & fhc_nd_nonswap_pandoraIxns = loaders.GetLoaders(ana::FluxType::kFHC)->GetSource(ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNonSwap).Interactions(ana::RecoType::kPandora);

	std::map<std::string, ana::SpectrumComponents> specs_recoVtx;
	for (const auto & vtxVarPair : lbl2025::kRecoVtxAxes)
	{
		specs_recoVtx.emplace(std::piecewise_construct,
		                  std::forward_as_tuple(vtxVarPair.first),
		                  std::forward_as_tuple(ana::kNoCut, vtxVarPair.second, fhc_nd_nonswap_pandoraIxns, lbl2025::kOscComponents));
	}
	std::map<std::string, ana::SpectrumComponents> specs_trueVtx;
	for (const auto & vtxVarPair : lbl2025::kTrueVtxAxes)
	{
		specs_trueVtx.emplace(std::piecewise_construct,
						  std::forward_as_tuple(vtxVarPair.first),
						  std::forward_as_tuple(ana::kNoCut, vtxVarPair.second, fhc_nd_nonswap_pandoraIxns, lbl2025::kOscComponents));
	}

	ana::SpectrumComponents spec_numuCVN(lbl2025::kRecoVtxInFid, lbl2025::kCVNAxis, fhc_nd_nonswap_pandoraIxns, lbl2025::kOscComponents);

	ana::Spectrum spec_nonFid(fhc_nd_nonswap_pandoraIxns[lbl2025::kRecoVtxInFid && !lbl2025::kTrueVtxInFid], lbl2025::kXYVtxAxis);

	loaders.Go();

	dunestyle::SetDuneStyle();

	for (const auto & vtxSpecPair : specs_recoVtx)
	{
		TCanvas c;
		vtxSpecPair.second.DrawComponents(true, ana::kBinContent, 0, false);
		vtxSpecPair.second.DrawLegend();
		TLatex l(0.6, 0.8, "All events");
		l.SetNDC(true);
		l.Draw();
		lbl2025::SaveCanvas(c,  Form("%s/RecoVtx%s", plotdir.c_str(), vtxSpecPair.first.c_str()) );
	}
	for (const auto & vtxSpecPair : specs_trueVtx)
	{
		TCanvas c;
		vtxSpecPair.second.DrawComponents(true, ana::kBinContent, 0, false);
		vtxSpecPair.second.DrawLegend();
		TLatex l(0.6, 0.8, "All events");
		l.SetNDC(true);
		l.Draw();
		lbl2025::SaveCanvas(c,  Form("%s/TrueVtx%s", plotdir.c_str(), vtxSpecPair.first.c_str()) );
	}

	{
		TCanvas c;
		spec_numuCVN.DrawComponents(true, ana::kBinContent, 0, false);
		spec_numuCVN.DrawLegend();
		TLatex l(0.6, 0.8, "Fid. vtx. events");
		l.SetNDC(true);
		l.Draw();
		lbl2025::SaveCanvas(c,  Form("%s/NumuCVN_FidVtxEvts", plotdir.c_str()) );
	}

	{
		TCanvas c;
		std::unique_ptr<TH2> h(spec_nonFid.ToTH2(spec_nonFid.POT()));
		h->Draw("colz");
		lbl2025::SaveCanvas(c, Form("%s/RecoFid_TrueNonFid", plotdir.c_str()) );
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
