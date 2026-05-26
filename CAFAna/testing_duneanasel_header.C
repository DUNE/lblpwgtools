#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/TruthMatching.h"

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "duneanasel/nd/ndlar/Selections.h"

using namespace ana;

const Cut kInFV([](const caf::SRInteractionProxy * sr){
	return sel::beam::ndlar::InFV(*sr);
});
const Cut kNuMuCCLikeContained([](const caf::SRInteractionProxy* sr){
	return sel::beam::ndlar::numode::NuMuCCLikeContained(*sr);
});
const Cut kSelNuMuCC([](const caf::SRInteractionProxy* sr) {
	return sel::beam::ndlar::numode::ApplySelectionV0p1(*sr) == sel::beam::ndlar::kNuMuCCLikeContained;
});
const Cut kNuMuSelected = kInFV && kNuMuCCLikeContained;

const Var kVtxX([](const caf::SRInteractionProxy* sr) -> double {
	return sr->vtx.x;
});
const Var kVtxY([](const caf::SRInteractionProxy* sr) -> double {
	return sr->vtx.y;
});
const Var kVtxZ([](const caf::SRInteractionProxy* sr) -> double {
	return sr->vtx.z;
});

void testing_duneanasel_header(){

	SpectrumLoader loader("/pnfs/dune/persistent/physicsgroups/dunendsim/abooth/nd-production/MicroProdN4p1/run-cafmaker/MicroProdN4p1_NDComplex_FHC.caf.full.spineonly/CAF.flat/0002000/*/*.root");

	Binning binsX = Binning::Simple(50, -350, 350);
  	Binning binsY = Binning::Simple(50, -220,  85);
  	Binning binsZ = Binning::Simple(50,  425, 925);

	HistAxis axVtxX("Vertex X [cm]", binsX, kVtxX);
	HistAxis axVtxY("Vertex Y [cm]", binsY, kVtxY);
	HistAxis axVtxZ("Vertex Z [cm]", binsZ, kVtxZ);

	Spectrum sVtxX(loader.Interactions(RecoType::kDLP)[kInFV], axVtxX);
	Spectrum sVtxY(loader.Interactions(RecoType::kDLP)[kInFV], axVtxY);
	Spectrum sVtxZ(loader.Interactions(RecoType::kDLP)[kInFV], axVtxZ);

	loader.Go();

	TCanvas* c = new TCanvas("c", "Vertex Position", 1200, 400);
	c->Divide(3, 1);
	c->cd(1);
	sVtxX.ToTH1(sVtxX.POT())->Draw("hist");
	c->cd(2);
	sVtxY.ToTH1(sVtxY.POT())->Draw("hist");
	c->cd(3);
	sVtxZ.ToTH1(sVtxZ.POT())->Draw("hist");
	c->SaveAs("vertex_position.png");

} 
