// Make a simple ND-GAr plot
// cafe gartkiNp0pi.C

// CAFAna Core includes
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

// CAFAna GAr includes
#include "CAFAna/GAr/GArParams.h"
#include "CAFAna/GAr/GArUtils.h"
#include "CAFAna/GAr/GArTruthCuts.h"

// CAFAna GArTKI includes
#include "CAFAna/GArTKI/GArTKINp0pi.h"

// Standard Library includes
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <memory>

// ROOT includes
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

using namespace ana;

void gartkiNp0pi()
{

    // Path to input CAF file(s)
    const std::string fname = "/exp/dune/data/users/fmlopez/GArProd/ndgar_mini_prod_v1/ndgar_prod_caf_hadd_*.root";

    // Path to output ROOT file
    TFile *fout = new TFile("gartkiNp0pi_nofsi.root","RECREATE");

    // Protons on Target (for ND-GAr 1M set to 5.0e19 POT)
    double pot = 5.0e19;

    // Value of FV cuts
    double delta_l = kHPgTPCFidCutX;
    double delta_r = kHPgTPCFidCutR;

    // Value of signal selection cuts
    const double muon_cut_low = default_muon_cut_low;
    const double muon_cut_hgh = default_muon_cut_hgh;
    const double prtn_cut_low = default_prtn_cut_low;
    const double prtn_cut_hgh = default_prtn_cut_hgh;
    const double pion_cut_hgh = default_pion_cut_hgh;

    // Using FSI?
    const bool fsi = false;

    // Add charged pion mass?
    const bool add_pion_mass = false;

    // Create selection object
    Np0piTKISelection tki_selection(muon_cut_low,
                                    muon_cut_hgh,
                                    prtn_cut_low,
                                    prtn_cut_hgh,
                                    pion_cut_hgh,
                                    fsi,
                                    add_pion_mass);

    // Declare variables
    const SRVar    kNuEnergy        = tki_selection.kEnu();
    const SRVar    kDeltapT         = tki_selection.kDeltapT();
    const SRVar    kDeltaAlphaT     = tki_selection.kDeltaAlphaT();
    const SRVar    kVisibleEnergy   = tki_selection.kVisibleE();
    const SRVar    kAvailableEnergy = tki_selection.kAvailableE();
    const SRVar    kNeutronMomentum = tki_selection.kNeutronMomentum();
    const SRVar    kProtonMult      = tki_selection.kProtonMultiplicity();

    // Declare binnings
    const Binning binsNuEnergy        = Binning::Simple(40, 0.0, 8.0);
    const Binning binsDeltapT         = Binning::Simple(40, 0.0, 1.0);
    const Binning binsDeltaAlphaT     = Binning::Simple(40, 0.0, 180.0);
    const Binning binsVisibleEnergy   = Binning::Simple(40, 0.0, 8.0);
    const Binning binsAvailableEnergy = Binning::Simple(40, 0.0, 1.0);
    const Binning binsNeutronMomentum = Binning::Simple(40, 0.0, 1.0);
    const Binning binsProtonMult      = Binning::Simple(10, 0.5, 10.5);

    // Declare 1D axes
    const SRHistAxis    axisNuEnergy(       "E_{\\nu} [\\mathrm{GeV}]",            binsNuEnergy,        kNuEnergy);
    const SRHistAxis    axisDeltapT(        "\\delta p_{T} [\\mathrm{GeV}/c]",     binsDeltapT,         kDeltapT);
    const SRHistAxis    axisDeltaAlphaT(    "\\delta \\alpha_{T} [\\mathrm{deg}]", binsDeltaAlphaT,     kDeltaAlphaT);
    const SRHistAxis    axisVisibleEnergy(  "E_{vis} [\\mathrm{GeV}]",             binsVisibleEnergy,   kVisibleEnergy);
    const SRHistAxis    axisAvailableEnergy("E_{avail} [\\mathrm{GeV}]",           binsAvailableEnergy, kAvailableEnergy);
    const SRHistAxis    axisNeutronMomentum("p_{n} [\\mathrm{GeV}/c]",             binsNeutronMomentum, kNeutronMomentum);
    const SRHistAxis    axisProtonMult(     "Proton multiplicity",                 binsProtonMult,      kProtonMult);

    // Declare 2D axes 
    const SRHistAxis    axisDeltapTvsAvailE(    "E_{avail} [\\mathrm{GeV}]",       binsAvailableEnergy, kAvailableEnergy,
                                                "\\delta p_{T} [\\mathrm{GeV}/c]", binsDeltapT,         kDeltapT);
    const SRHistAxis    axisDeltapTvsProtonMult("Proton multiplicity",             binsProtonMult,      kProtonMult,
                                                "\\delta p_{T} [\\mathrm{GeV}/c]", binsDeltapT,         kDeltapT);

    // Declare names for histograms (for aesthetic purposes only)
    TString nameNuEnergy    = TString::Format("Enu");
    TString nameDeltapT     = TString::Format("DeltapT");
    TString nameDeltaAlphaT = TString::Format("DeltaAlphaT");
    TString nameVisibleE    = TString::Format("Evis");
    TString nameAvailableE  = TString::Format("Eavail");
    TString nameNeutronP    = TString::Format("pNeutron");
    TString nameProtonMult  = TString::Format("ProtonMult");

    TString nameDeltapTvsAvailE     = TString::Format("DeltapTvsEavail");
    TString nameDeltapTvsProtonMult = TString::Format("DeltapTvsProtonMult");

    // Declare names for cuts (for aesthetic purposes only)
    TString nameNoCut    = TString::Format("");
    TString nameQECut    = TString::Format("QE");
    TString nameMECCut   = TString::Format("MEC");
    TString nameRESCut   = TString::Format("RES");
    TString nameDISCut   = TString::Format("DIS");
    TString nameOtherCut = TString::Format("Other");

    // Define selection cut
    const SRCut kSignalDefinitionCut = kIsTrueNuMuCC && kIsTrueContained(delta_l, delta_r) && tki_selection.kNp0piTKICut();

    // Create map with all axes to include in 1D histograms
    std::map<TString, SRHistAxis>
    axes_1d_map = {
                    {nameNuEnergy,    axisNuEnergy},
                    {nameDeltapT,     axisDeltapT},
                    {nameDeltaAlphaT, axisDeltaAlphaT},
                    {nameVisibleE,    axisVisibleEnergy},
                    {nameAvailableE,  axisAvailableEnergy},
                    {nameNeutronP,    axisNeutronMomentum},
                    {nameProtonMult,  axisProtonMult}
                   };

    // Create map with all axes to include in 2D histograms
    std::map<TString, SRHistAxis>
    axes_2d_map = {
                    {nameDeltapTvsAvailE,     axisDeltapTvsAvailE},
                    {nameDeltapTvsProtonMult, axisDeltapTvsProtonMult}
                   };

    // Create map with the different cut combinations
    std::map<TString, SRCut>
    cuts_map = {
                 {nameNoCut,    kIsNoCut},
                 {nameQECut,    kIsTrueQE},
                 {nameMECCut,   kIsTrueMEC},
                 {nameRESCut,   kIsTrueRES},
                 {nameDISCut,   kIsTrueDIS},
                 {nameOtherCut, kIsTrueOther}
               };

    // Create vector of pointers for 1D spectra
    std::vector<std::unique_ptr<Spectrum>> spectra_1d_vector;

    // Create vector of pointers for 2D spectra
    std::vector<std::unique_ptr<Spectrum>> spectra_2d_vector;

    // Source of events
    SpectrumLoader loader(fname);

    // Register the 1D spectra
    for (const auto& [axis_name, axis] : axes_1d_map) {
        for (const auto& [cut_name, cut] : cuts_map) {
            // Create unique pointer and add to vector
            std::unique_ptr<Spectrum> spec = std::make_unique<Spectrum>(loader[kSignalDefinitionCut && cut], axis);
            spectra_1d_vector.push_back(std::move(spec));
        } // end loop over cuts
    } // end loop over axes

    // Register the 2D spectra
    for (const auto& [axis_name, axis] : axes_2d_map) {
        for (const auto& [cut_name, cut] : cuts_map) {
            // Create unique pointer and add to vector
            std::unique_ptr<Spectrum> spec = std::make_unique<Spectrum>(loader[kSignalDefinitionCut && cut], axis);
            spectra_2d_vector.push_back(std::move(spec));
        } // end loop over cuts
    } // end loop over axes

    // Fill spectra (go, go, go)
    loader.Go();

    // Write histograms
    fout->cd();

    // Save the 1D spectra
    size_t iSpectrum1D = 0;
    for (const auto& [axis_name, axis] : axes_1d_map) {
        // Create directory with var name
        fout->mkdir(axis_name);
        fout->cd(axis_name);
        for (const auto& [cut_name, cut] : cuts_map) {
            TH1D* histogram(spectra_1d_vector[iSpectrum1D]->ToTH1(pot));
            histogram->SetName(axis_name+cut_name);
            histogram->Write(axis_name+cut_name, TObject::kWriteDelete);
            iSpectrum1D++;
        } // end loop over cuts
    } // end loop over axes

    // Save the 2D spectra
    size_t iSpectrum2D = 0;
    for (const auto& [axis_name, axis] : axes_2d_map) {
        // Create directory with var name
        fout->mkdir(axis_name);
        fout->cd(axis_name);
        for (const auto& [cut_name, cut] : cuts_map) {
            TH2* histogram(spectra_2d_vector[iSpectrum2D]->ToTH2(pot));
            histogram->SetName(axis_name+cut_name);
            histogram->Write(axis_name+cut_name, TObject::kWriteDelete);
            iSpectrum2D++;
        } // end loop over cuts
    } // end loop over axes

    fout->Close();

}
