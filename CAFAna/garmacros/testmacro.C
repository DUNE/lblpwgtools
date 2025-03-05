// Make a simple ND-GAr plot
// cafe testmacro.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "CAFAna/GAr/GArTruthCuts.h"

#include <TFile.h>
#include "TH1.h"

using namespace ana;

void testmacro()
{

    // Path to CAF file(s)
    //const std::string fname = "/exp/dune/data/users/fmlopez/GArProd/ndgar_mini_prod_v1/ndgar_prod_caf_hadd_000.root";
    const std::string fname = "/exp/dune/data/users/fmlopez/GArProd/ndgar_mini_prod_v1/ndgar_prod_caf_hadd_*.root";

    double pot = 5.0e19;

    // Path to output ROOT file
    TFile *fout = new TFile("test.root","RECREATE");

    // Source of events
    SpectrumLoader loader(fname);

    // Declare simple variable, binning and axis
    const TruthVar kTrueE = SIMPLETRUTHVAR(E);
    const Binning binsTrueE = Binning::Simple(40, 0, 8);
    const TruthHistAxis axisTrueE("True neutrino energy [GeV]", binsTrueE, kTrueE);

    // Input to FV cut
    double delta_l = 30.0;
    double delta_r = 50.0;

    // Create Truth cut
    const SRCut kTruthCut = kIsTrueNuMuCC && kIsTrueContained(delta_l, delta_r);

    Spectrum sTrueE(loader[kTruthCut].NuTruths(), axisTrueE);

    loader.Go();

    fout->cd();

    TH1D* hTrueE(sTrueE.ToTH1(pot));
    hTrueE->SetName("TrueE");
    hTrueE->Write("TrueE", TObject::kWriteDelete);

    fout->Close();

}