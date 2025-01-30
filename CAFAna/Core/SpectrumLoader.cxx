#include "CAFAna/Analysis/AnalysisVersion.h"

#include "CAFAna/Core/SpectrumLoader.h"

#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"
#ifndef DONT_USE_SAM
#include "CAFAna/Core/SAMProjectSource.h"
#endif
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Analysis/XSecSystList.h"
#include "CAFAna/Core/FitqunConverter.h"

#include "CAFAna/Core/ModeConversionUtilities.h"

#include "StandardRecord/StandardRecord.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include "TRandom3.h"

namespace ana {
//----------------------------------------------------------------------
SpectrumLoader::SpectrumLoader(const std::string &wildcard, DataSource src,
                               int max)
    : SpectrumLoaderBase(wildcard, src), max_entries(max) {}

//----------------------------------------------------------------------
SpectrumLoader::SpectrumLoader(const std::vector<std::string> &fnames,
                               DataSource src, int max)
    : SpectrumLoaderBase(fnames, src), max_entries(max) {}

//----------------------------------------------------------------------
SpectrumLoader::SpectrumLoader(DataSource src) : SpectrumLoaderBase(src) {}

#ifndef DONT_USE_SAM
//----------------------------------------------------------------------
SpectrumLoader SpectrumLoader::FromSAMProject(const std::string &proj,
                                              DataSource src, int fileLimit) {
  SpectrumLoader ret;
  ret.fSource = src;
  ret.fWildcard = "project " + proj;
  ret.fFileSource =
      std::unique_ptr<IFileSource>(new SAMProjectSource(proj, fileLimit));
  return ret;
}
#endif
//----------------------------------------------------------------------
SpectrumLoader::~SpectrumLoader() {}

struct CompareByID {
  bool operator()(const Cut &a, const Cut &b) { return a.ID() < b.ID(); }
};

bool SpectrumLoader::AddSmear(double &smear_val){
  static bool add_nd_smear = getenv("CAFANA_ND_SMEAR") ? 1 : 0;
  static double nd_smear = getenv("CAFANA_ND_SMEAR") ? atof(getenv("CAFANA_ND_SMEAR")) : 0;
  smear_val = nd_smear;
  return add_nd_smear;
}
/////////////////////////////////////////////////////////////// hacked by Guang Yang

// Function to sample from histogram data based on its bin contents
double SpectrumLoader::sampleFromHistogram(const std::vector<double>& histogram, const std::vector<double>& binEdges, TRandom3* gRandom) {
    // Create CDF from histogram
    std::vector<double> cdf(histogram.size());
    double sum = 0.0;
    for (size_t i = 0; i < histogram.size(); ++i) {
        sum += histogram[i];
        cdf[i] = sum;
    }

    // Normalize CDF to ensure it ranges from 0 to 1
    for (size_t i = 0; i < cdf.size(); ++i) {
        cdf[i] /= sum;
    }

    // Sample from the CDF
    double randomValue = gRandom->Uniform();  // Uniform random number between 0 and 1
    auto it = std::lower_bound(cdf.begin(), cdf.end(), randomValue);
    size_t index = std::distance(cdf.begin(), it);

    // Return the corresponding bin center value
    if (index == 0) {
        return binEdges[0];
    } else if (index >= binEdges.size()) {
        return binEdges.back();
    } else {
        return binEdges[index - 1];
    }
}

// Function to load smearing histograms into vectors and create CDFs
void SpectrumLoader::loadSmearingVectors(std::vector<double>& nu_had_smear_vector, std::vector<double>& antinu_had_smear_vector,
                         std::vector<double>& nu_bin_edges, std::vector<double>& antinu_bin_edges) {
    // Load neutrino smearing histogram
    std::string neutrinoSmearFilePath = "/exp/dune/app/users/gyang/ratpac_analyzer/swap0_neutrino.root";
    std::string antineutrinoSmearFilePath = "/exp/dune/app/users/gyang/ratpac_analyzer/swap0_antineutrino.root";

    TFile f_nu_smear(neutrinoSmearFilePath.c_str());
    if (!f_nu_smear.IsOpen()) {
        std::cerr << "[ERROR] Failed to open " << neutrinoSmearFilePath << std::endl;
        exit(1);
    }
    TH1F* nu_had_smear_hist = (TH1F*)f_nu_smear.Get("h1_mus_projection");
    if (!nu_had_smear_hist) {
        std::cerr << "[ERROR] Failed to load h1_mus_projection from " << neutrinoSmearFilePath << std::endl;
        exit(1);
    }

    // Convert TH1F to vector and then to CDF
    for (int i = 1; i <= nu_had_smear_hist->GetNbinsX(); ++i) {
        nu_had_smear_vector.push_back(nu_had_smear_hist->GetBinContent(i));
        nu_bin_edges.push_back(nu_had_smear_hist->GetBinCenter(i));
    }

    // Load antineutrino smearing histogram
    TFile f_antinu_smear(antineutrinoSmearFilePath.c_str());
    if (!f_antinu_smear.IsOpen()) {
        std::cerr << "[ERROR] Failed to open " << antineutrinoSmearFilePath << std::endl;
        exit(1);
    }
    TH1F* antinu_had_smear_hist = (TH1F*)f_antinu_smear.Get("h1_mus_projection");
    if (!antinu_had_smear_hist) {
        std::cerr << "[ERROR] Failed to load h1_mus_projection from " << antineutrinoSmearFilePath << std::endl;
        exit(1);
    }

    // Convert TH1F to vector and then to CDF
    for (int i = 1; i <= antinu_had_smear_hist->GetNbinsX(); ++i) {
        antinu_had_smear_vector.push_back(antinu_had_smear_hist->GetBinContent(i));
        antinu_bin_edges.push_back(antinu_had_smear_hist->GetBinCenter(i));
    }

    f_nu_smear.Close();
    f_antinu_smear.Close();
}


// Function to load graph errors from external files
void SpectrumLoader::loadGraphErrors(std::vector<std::vector<TGraphErrors*>>& graphError) {
    std::string graphErrorBaseFilePath = "/exp/dune/app/users/gyang/ratpac_analyzer/";
    for (int i = 0; i < 3; i++) {
        graphError.emplace_back(); // Add new vector for each set
        for (int j = 1; j < 11; j++) {
            std::string filePath = graphErrorBaseFilePath + Form("sub_e%d_sample%d.root", j, i);
            TFile ff(filePath.c_str());
            if (!ff.IsOpen()) {
                std::cerr << "[ERROR] Failed to open " << filePath << std::endl;
                exit(1);
            }
            TGraphErrors* gErr = (TGraphErrors*)ff.Get(Form("sb_ratio_e%d_sample%d", j, i));
            if (!gErr) {
                std::cerr << "[ERROR] Failed to load sb_ratio_e" << j << "_sample" << i << std::endl;
                exit(1);
            }
            graphError[i].push_back(gErr);
            ff.Close();
        }
    }
}

// Function to compute reconstructed energy based on lepton and hadron energies
std::pair<double, double> SpectrumLoader::computeRecoE(int particleType, int absPdg, int status, double lepE_tr, double Ev, 
                                       const std::vector<double>& nu_had_smear_vector, const std::vector<double>& antinu_had_smear_vector,
                                       const std::vector<double>& nu_bin_edges, const std::vector<double>& antinu_bin_edges,
                                       TRandom3* gRandom, std::vector<std::vector<TGraphErrors*>>& graphError, 
                                       double res_emu_factor) {
    double lepE = lepE_tr; // Initial lepton energy
    double recoE = 0.0;
    double hadE = 0.0;

    // Smearing logic based on particle type (neutrino/antineutrino)
    if ((absPdg == 13 && status == 1) || (absPdg == 11 && status == 1)) {
        int energyBin = std::min(static_cast<int>((lepE_tr - 0.1057) * 1000. / 200.), 9); // Adjust for muon mass

        // Calculate smearing factors based on the graphs
        double yfluc = graphError[0][energyBin]->GetErrorY(10) / graphError[0][energyBin]->GetY()[10];
        double yfluc2 = graphError[1][energyBin]->GetErrorY(10) / graphError[1][energyBin]->GetY()[10];
        double yfluc3 = graphError[2][energyBin]->GetErrorY(10) / graphError[2][energyBin]->GetY()[10];

        // Smear lepE based on particle type
        if (absPdg == 13) { // Muon neutrino or antineutrino
            lepE = lepE_tr * (1 + gRandom->Gaus(0, yfluc));
        } else if (absPdg == 11) { // Electron neutrino or antineutrino
            lepE = lepE_tr * (1 + gRandom->Gaus(0, yfluc * res_emu_factor));
        }

        // For neutrinos
        if (particleType == 14) {
            hadE = (Ev - lepE) * (1 + sampleFromHistogram(nu_had_smear_vector, nu_bin_edges, gRandom)); // Neutrino smearing
            recoE = hadE + lepE; // Reconstructed energy
        }
        // For antineutrinos
        else if (particleType == -14) {
            hadE = (Ev - lepE) * (1 + sampleFromHistogram(antinu_had_smear_vector, antinu_bin_edges, gRandom)); // Antineutrino smearing
            recoE = hadE + lepE; // Reconstructed energy
        }
        else if (particleType == 12) {
            hadE = (Ev - lepE) * (1 + sampleFromHistogram(nu_had_smear_vector, nu_bin_edges, gRandom)); // Electron neutrino smearing
            recoE = hadE + lepE;
        }
        else if (particleType == -12) {
            hadE = (Ev - lepE) * (1 + sampleFromHistogram(antinu_had_smear_vector, antinu_bin_edges, gRandom)); // Electron antineutrino smearing
            recoE = hadE + lepE;
        }
    }
    else if (absPdg == 16 || absPdg == -16) { // Tau neutrinos or antineutrinos
        recoE = lepE_tr; // No smearing for tau neutrinos
    }

    // Return both lepE and recoE
    return std::make_pair(lepE, recoE);
}

// Function to load data and perform computation, to be called within another program
std::pair<double, double> SpectrumLoader::processEvent(int particleType, int absPdg, int status, double lepE_tr, double Ev) {
    // Initialize random number generator
    TRandom3 gRandom;

    // Load smearing vectors
    std::vector<double> nu_had_smear_vector;
    std::vector<double> antinu_had_smear_vector;
    std::vector<double> nu_bin_edges;
    std::vector<double> antinu_bin_edges;

    loadSmearingVectors(nu_had_smear_vector, antinu_had_smear_vector, nu_bin_edges, antinu_bin_edges);

    // Load graph errors
    std::vector<std::vector<TGraphErrors*>> graphError;
    loadGraphErrors(graphError);

    // Compute reconstructed energy and smeared lepton energy
    auto result = computeRecoE(particleType, absPdg, status, lepE_tr, Ev,                                nu_had_smear_vector, antinu_had_smear_vector,
                               nu_bin_edges, antinu_bin_edges, &gRandom, graphError);

    // Clean up
    for (auto& vec : graphError) {
        for (auto& graph : vec) {
            delete graph;
        }
    }

    return result; // Return lepE and recoE
}

// Function to process each ratioMap3D and save it as a TH2 in the output file
void SpectrumLoader::saveRatioMapsToTH2(
      const std::vector<std::vector<std::vector<double>>>& ratioMap3D,
      int pdgCode,
      const TString& outputFileName)
    {
      // Open the output ROOT file (create or update)
      TFile* outputFile = TFile::Open(outputFileName, "UPDATE");
      if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error creating or updating output file: " << outputFileName << std::endl;
        return;
      }

      // Loop over the outer layer (index for different energy bins or other layers)
      for (size_t i = 0; i < ratioMap3D.size(); ++i) {
        // Get the inner two layers for each map
        const std::vector<std::vector<double>>& innerData = ratioMap3D[i];

        // Determine the number of bins for the TH2 histogram
        int nBinsX = innerData.size();          // Number of x bins (size of the first dimension)
        int nBinsY = innerData[0].size();       // Number of y bins (size of the second dimension)

        // Create a new TH2 histogram
        TH2D* hist = new TH2D(TString::Format("ratioMap3D_%d_%lu", pdgCode, i),
                              TString::Format("Ratio Map PDG %d Index %lu", pdgCode, i),
                              nBinsX, 0, nBinsX,
                              nBinsY, 0, nBinsY);

        // Fill the histogram with data from the inner two layers
        for (int x = 0; x < nBinsX; ++x) {
            for (int y = 0; y < nBinsY; ++y) {
                hist->SetBinContent(x + 1, y + 1, innerData[x][y]);  // ROOT bins start at 1
            }
        }

        // Write the TH2 histogram to the output file
        outputFile->cd();
        hist->Write();

        // Print confirmation
        std::cout << "Saved 2D histogram for PDG " << pdgCode << " with index " << i << " to file." << std::endl;
      }

      // Close the output file
      outputFile->Close();
}



// Function to create a 3D map of ratios (total energy, lepton energy, neutron number)
std::vector<std::vector<std::vector<double>>> SpectrumLoader::processMaps(int neutrinoPDG) {
    // Open the TH3 histograms file
    TFile* histFile = TFile::Open("/exp/dune/app/users/gyang/ratpac_analyzer/weighting_output_histograms.root");

    // Energy bin range from 0.5 GeV to 8 GeV with 0.5 GeV steps
    double energyMin = 0.5;
    double energyMax = 8.0;
    double energyStep = 0.5;
    int nEnergyBins = static_cast<int>((energyMax - energyMin) / energyStep) + 1;

    // Create a 3D vector (totalEnergy, neutronNumber, leptonEnergy)
    std::vector<std::vector<std::vector<double>>> ratioMap3D(nEnergyBins);

    // Loop over total energy bins
    for (int energyBin = 0; energyBin < nEnergyBins; ++energyBin) {
        // Compute the corresponding total energy for this bin
        double totalEnergy = energyMin + energyBin * energyStep;

        // Construct histogram names for water and argon cases
        TString waterHistName = TString::Format("water_nu_%d_energy_%.1f", neutrinoPDG, totalEnergy);
        TString argonHistName = TString::Format("argon_nu_%d_energy_%.1f", neutrinoPDG, totalEnergy);

        // Retrieve the TH3 histograms
        TH3* waterHist = dynamic_cast<TH3*>(histFile->Get(waterHistName));
        TH3* argonHist = dynamic_cast<TH3*>(histFile->Get(argonHistName));

        if (!waterHist || !argonHist) {
            std::cerr << "Error: Histograms for energy " << totalEnergy << " GeV not found!" << std::endl;
            continue; // Skip this energy bin if histograms are not found
        }

        // Project the TH3 onto the x (neutron number) and y (lepton energy) axes
        TH2* waterHist2D = static_cast<TH2*>(waterHist->Project3D("yx"));
        TH2* argonHist2D = static_cast<TH2*>(argonHist->Project3D("yx"));

        // Get the number of bins for neutron number and lepton energy
        int nBinsX = waterHist2D->GetNbinsX();  // Neutron number bins
        int nBinsY = waterHist2D->GetNbinsY();  // Lepton energy bins

        // Create a 2D vector for this energy bin (neutronNumber, leptonEnergy)
        std::vector<std::vector<double>> ratioMap2D(nBinsX, std::vector<double>(nBinsY, 0));

        // Loop over all bins and compute the ratio for this total energy bin
        for (int xBin = 1; xBin <= nBinsX; ++xBin) {
            for (int yBin = 1; yBin <= nBinsY; ++yBin) {
                double waterValue = waterHist2D->GetBinContent(xBin, yBin);
                double argonValue = argonHist2D->GetBinContent(xBin, yBin);

                // Compute the ratio and handle cases where argonValue is 0
                if (argonValue != 0) {
                    ratioMap2D[xBin - 1][yBin - 1] = waterValue / argonValue;
                } else {
                    ratioMap2D[xBin - 1][yBin - 1] = 0;
                }
            }
        }

        // Store the 2D ratio map in the 3D map
        ratioMap3D[energyBin] = ratioMap2D;

        // Clean up
        delete waterHist2D;
        delete argonHist2D;
    }

    // Close the histogram file
    histFile->Close();

    // Return the 3D ratio map
    return ratioMap3D;
}


/////////////////////////////////////////////////////////////// End hacking Guang Yang
//----------------------------------------------------------------------
void SpectrumLoader::Go() {
  if (fGone) {
    std::cerr << "Error: can only call Go() once on a SpectrumLoader"
              << std::endl;
    abort();
  }
  fGone = true;

  // Find all the unique cuts
  std::set<Cut, CompareByID> cuts;
  for (auto &shiftdef : fHistDefs)
    for (auto &cutdef : shiftdef.second)
      cuts.insert(cutdef.first);
  for (const Cut &cut : cuts)
    fAllCuts.push_back(cut);

  fLivetimeByCut.resize(fAllCuts.size());
  fPOTByCut.resize(fAllCuts.size());

  const int Nfiles = NFiles();

  Progress *prog = 0;

  int fileIdx = -1;
  while (TFile *f = GetNextFile()) {
    ++fileIdx;

    if (Nfiles >= 0 && !prog)
      prog = new Progress(
          TString::Format("Filling %lu spectra from %d files matching '%s'",
                          fHistDefs.TotalSize(), Nfiles, fWildcard.c_str())
              .Data());

    HandleFile(f, Nfiles == 1 ? prog : 0);

    if (Nfiles > 1 && prog)
      prog->SetProgress((fileIdx + 1.) / Nfiles);
  } // end for fileIdx

  StoreExposures();

  if (prog) {
    prog->Done();
    delete prog;
  }

  ReportExposures();

  fHistDefs.RemoveLoader(this);
  fHistDefs.Clear();
}

//----------------------------------------------------------------------
// Helper function that can give us a friendlier error message
template <class T>
bool SetBranchChecked(TTree *tr, const std::string &bname, T *dest) {
  if (tr->FindBranch(bname.c_str())) {
    tr->SetBranchAddress(bname.c_str(), dest);
    return true;
  } else {
    std::cout << "Warning: Branch '" << bname
              << "' not found, field will not be filled" << std::endl;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////Guang's function
std::string SpectrumLoader::GetFitQunOutputFile(TFile* f) {
    if (!f || f->IsZombie()) {
        std::cerr << "Error: Input file is not valid or corrupted!" << std::endl;
        return "";
    }

    std::string keyName = f->GetName();
    std::cout << "Input file name: " << keyName << std::endl;

    std::string swapType = "";
        std::cout << "Found key: " << keyName << std::endl;
        if (keyName.find("nonswap") != std::string::npos) {
            std::cout << "Detected swap type: nonswap" << std::endl;
            swapType =  "nonswap";
        } else if (keyName.find("nueswap") != std::string::npos) {
            std::cout << "Detected swap type: nueswap" << std::endl;
            swapType =  "nueswap";
        } else if (keyName.find("tauswap") != std::string::npos) {
            std::cout << "Detected swap type: tauswap" << std::endl;
            swapType =  "tauswap";
        }
        else {
          std::cerr << "Error: Unable to determine swap type from the file!" << std::endl;
          return "";
        }

    // Construct the outputTest file path
    std::string basePath = "/pnfs/dune/scratch/users/gyang/CAFAna/";
    std::string fileName = "outputTest_extra_FHC_" + swapType + ".root";
    //return basePath + fileName;
    std::string okok = "/pnfs/dune/scratch/users/gyang/wc/outputTest_extra_allswap_allhc_root6_12.root";
    return okok;
}
/////////////////////////////////////////////////////////////////// End Guang's function


//----------------------------------------------------------------------
void SpectrumLoader::HandleFile(TFile *f, Progress *prog) {

  std::cout<<"Let's handleFile"<<std::endl;

  assert(!f->IsZombie());
  TTree *tr;
  //    if(f->GetListOfKeys()->Contains("cafmaker")){
  //      tr = (TTree*)f->Get("cafmaker/caf");
  //    }
  //    else{
  //      tr = (TTree*)f->Get("mvaselect/MVASelection");
  //    }
  tr = (TTree *)f->Get("cafCrazyFlux");
  if (!tr) {
    tr = (TTree *)f->Get("caf");
  }
  if (!tr) {
    tr = (TTree *)f->Get("cafTree");
  }

  assert(tr);

  FloatingExceptionOnNaN fpnan(false);

  std::cout<<"Let's getbranch info."<<std::endl;

  caf::StandardRecord sr;
  SetBranchChecked(tr, "Ev_reco", &sr.dune.Ev_reco);
  SetBranchChecked(tr, "Ev_reco_nue", &sr.dune.Ev_reco_nue);
  SetBranchChecked(tr, "Ev_reco_numu", &sr.dune.Ev_reco_numu);
  SetBranchChecked(tr, "Elep_reco", &sr.dune.Elep_reco);
  SetBranchChecked(tr, "theta_reco", &sr.dune.theta_reco);
  SetBranchChecked(tr, "mvaresult", &sr.dune.mvaresult);
  SetBranchChecked(tr, "mvanue", &sr.dune.mvanue);
  SetBranchChecked(tr, "mvanumu", &sr.dune.mvanumu);
  SetBranchChecked(tr, "cvnnue", &sr.dune.cvnnue);
  SetBranchChecked(tr, "cvnnumu", &sr.dune.cvnnumu);
  SetBranchChecked(tr, "numu_pid", &sr.dune.numu_pid);
  SetBranchChecked(tr, "nue_pid", &sr.dune.nue_pid);
  SetBranchChecked(tr, "reco_q", &sr.dune.reco_q);
  SetBranchChecked(tr, "RecoLepEnNue", &sr.dune.RecoLepEnNue);
  SetBranchChecked(tr, "RecoHadEnNue", &sr.dune.RecoHadEnNue);
  SetBranchChecked(tr, "RecoLepEnNumu", &sr.dune.RecoLepEnNumu);
  SetBranchChecked(tr, "RecoHadEnNumu", &sr.dune.RecoHadEnNumu);
  // ND pseudo-reconstruction flags
  SetBranchChecked(tr, "reco_numu", &sr.dune.reco_numu);
  SetBranchChecked(tr, "reco_nue", &sr.dune.reco_nue);
  SetBranchChecked(tr, "reco_nc", &sr.dune.reco_nc);
  // CW: add variables that Chris (M) wants for ND selections
  SetBranchChecked(tr, "muon_exit", &sr.dune.muon_exit);
  SetBranchChecked(tr, "muon_contained", &sr.dune.muon_contained);
  SetBranchChecked(tr, "muon_ecal", &sr.dune.muon_ecal);
  SetBranchChecked(tr, "muon_tracker", &sr.dune.muon_tracker);
  SetBranchChecked(tr, "Ehad_veto", &sr.dune.Ehad_veto);

  SetBranchChecked(tr, "Ev", &sr.dune.Ev);
  SetBranchChecked(tr, "Elep", &sr.dune.Elep);
  //    SetBranchChecked(tr, "ccnc", &sr.dune.ccnc);
  SetBranchChecked(tr, "isCC", &sr.dune.isCC);
  //    SetBranchChecked(tr, "beamPdg", &sr.dune.beamPdg);
  //    SetBranchChecked(tr, "neu", &sr.dune.neu);
  SetBranchChecked(tr, "nuPDG", &sr.dune.nuPDG);
  SetBranchChecked(tr, "nuPDGunosc", &sr.dune.nuPDGunosc);
  SetBranchChecked(tr, "LepPDG", &sr.dune.LepPDG);
  SetBranchChecked(tr, "mode", &sr.dune.mode);
  SetBranchChecked(tr, "nP", &sr.dune.nP);
  SetBranchChecked(tr, "nN", &sr.dune.nN);
  SetBranchChecked(tr, "nipi0", &sr.dune.nipi0);
  SetBranchChecked(tr, "nipip", &sr.dune.nipip);
  SetBranchChecked(tr, "nipim", &sr.dune.nipim);
  SetBranchChecked(tr, "niem", &sr.dune.niem);
  SetBranchChecked(tr, "Q2", &sr.dune.Q2);
  SetBranchChecked(tr, "W", &sr.dune.W);
  SetBranchChecked(tr, "Y", &sr.dune.Y);
  SetBranchChecked(tr, "X", &sr.dune.X);
  //    SetBranchChecked(tr, "cc", &sr.dune.cc);
  SetBranchChecked(tr, "NuMomX", &sr.dune.NuMomX);
  SetBranchChecked(tr, "NuMomY", &sr.dune.NuMomY);
  SetBranchChecked(tr, "NuMomZ", &sr.dune.NuMomZ);
  SetBranchChecked(tr, "LepMomX", &sr.dune.LepMomX);
  SetBranchChecked(tr, "LepMomY", &sr.dune.LepMomY);
  SetBranchChecked(tr, "LepMomZ", &sr.dune.LepMomZ);
  SetBranchChecked(tr, "LepE", &sr.dune.LepE);
  SetBranchChecked(tr, "LepNuAngle", &sr.dune.LepNuAngle);

  // Numu track containment flag
  SetBranchChecked(tr, "LongestTrackContNumu", &sr.dune.LongestTrackContNumu);

  SetBranchChecked(tr, "vtx_x", &sr.dune.vtx_x);
  SetBranchChecked(tr, "vtx_y", &sr.dune.vtx_y);
  SetBranchChecked(tr, "vtx_z", &sr.dune.vtx_z);

  SetBranchChecked(tr, "det_x", &sr.dune.det_x);

  SetBranchChecked(tr, "eP", &sr.dune.eP);
  SetBranchChecked(tr, "eN", &sr.dune.eN);
  SetBranchChecked(tr, "ePip", &sr.dune.ePip);
  SetBranchChecked(tr, "ePim", &sr.dune.ePim);
  SetBranchChecked(tr, "ePi0", &sr.dune.ePi0);
  SetBranchChecked(tr, "eOther", &sr.dune.eOther);
  SetBranchChecked(tr, "eRecoP", &sr.dune.eRecoP);
  SetBranchChecked(tr, "eRecoN", &sr.dune.eRecoN);
  SetBranchChecked(tr, "eRecoPip", &sr.dune.eRecoPip);
  SetBranchChecked(tr, "eRecoPim", &sr.dune.eRecoPim);
  SetBranchChecked(tr, "eRecoPi0", &sr.dune.eRecoPi0);
  SetBranchChecked(tr, "eRecoOther", &sr.dune.eRecoOther);

  SetBranchChecked(tr, "eDepP", &sr.dune.eDepP);
  SetBranchChecked(tr, "eDepN", &sr.dune.eDepN);
  SetBranchChecked(tr, "eDepPip", &sr.dune.eDepPip);
  SetBranchChecked(tr, "eDepPim", &sr.dune.eDepPim);
  SetBranchChecked(tr, "eDepPi0", &sr.dune.eDepPi0);
  SetBranchChecked(tr, "eDepOther", &sr.dune.eDepOther);

  SetBranchChecked(tr, "run", &sr.dune.run);
  SetBranchChecked(tr, "isFD", &sr.dune.isFD);
  SetBranchChecked(tr, "isFHC", &sr.dune.isFHC);

  SetBranchChecked(tr, "sigma_Ev_reco", &sr.dune.sigma_Ev_reco);
  SetBranchChecked(tr, "sigma_Elep_reco", &sr.dune.sigma_Elep_reco);
  SetBranchChecked(tr, "sigma_numu_pid", &sr.dune.sigma_numu_pid);
  SetBranchChecked(tr, "sigma_nue_pid", &sr.dune.sigma_nue_pid);

  // Get the crazy fluxes
  std::array<double, 7> crazy_tmp;
  SetBranchChecked(tr, "wgt_CrazyFlux", &crazy_tmp);

  // XSec uncertainties and CVs
  std::vector<std::array<double, 100>> XSSyst_tmp;
  std::vector<double> XSSyst_cv_tmp;
  std::vector<int> XSSyst_size_tmp;

  std::vector<std::string> const &XSSyst_names = GetAllXSecSystNames();
  XSSyst_tmp.resize(XSSyst_names.size());
  XSSyst_cv_tmp.resize(XSSyst_names.size());
  XSSyst_size_tmp.resize(XSSyst_names.size());

  sr.dune.xsSyst_wgt.resize(XSSyst_names.size());

  for (unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {
    if (!SetBranchChecked(tr, "wgt_" + XSSyst_names[syst_it],
                          &XSSyst_tmp[syst_it])) {
      std::fill_n(XSSyst_tmp[syst_it].begin(), 100, 1);
      XSSyst_cv_tmp[syst_it] = 1;
      XSSyst_size_tmp[syst_it] = 1;
      continue;
    }

    SetBranchChecked(tr, XSSyst_names[syst_it] + "_nshifts",
                     &XSSyst_size_tmp[syst_it]);
    SetBranchChecked(tr, XSSyst_names[syst_it] + "_cvwgt",
                     &XSSyst_cv_tmp[syst_it]);
  }

  int Nentries = tr->GetEntries();
  if (max_entries != 0 && max_entries < Nentries)
    Nentries = max_entries;
/*
  //////////////////////////////////////// Guang's hacking again
    // Precompute the 3D ratio maps for the four possible neutrino PDG codes
    std::vector<std::vector<std::vector<double>>> ratioMap3D_14 = processMaps(14);
    std::vector<std::vector<std::vector<double>>> ratioMap3D_12 = processMaps(12);
    std::vector<std::vector<std::vector<double>>> ratioMap3D_neg14 = processMaps(-14);
    std::vector<std::vector<std::vector<double>>> ratioMap3D_neg12 = processMaps(-12);

    // Open the histograms file to get binning information
    TFile* histFile = TFile::Open("/exp/dune/app/users/gyang/ratpac_analyzer/weighting_output_histograms.root");
    if (!histFile) {
        std::cerr << "Error: Failed to open output_histograms.root" << std::endl;
        exit(0);
    }

    // Retrieve a TH3 histogram to extract binning info for neutron number and lepton energy
    TString waterHistName = TString::Format("water_nu_14_energy_%.1f", 0.5); // Example using 0.5 GeV
    TH3* waterHist = dynamic_cast<TH3*>(histFile->Get(waterHistName));

    if (!waterHist) {
        std::cerr << "Error: Failed to retrieve TH3 histogram for binning info" << std::endl;
        exit(0);
    }

    // Project the TH3 to 2D (neutron number and lepton energy axes)
    TH2* waterHist2D = static_cast<TH2*>(waterHist->Project3D("yx"));

    // Get the binning information for neutron number (x-axis) and lepton energy (y-axis)
    int nBinsX = waterHist2D->GetNbinsX();  // Neutron number bins
    double xMin = waterHist2D->GetXaxis()->GetXmin();
    double xMax = waterHist2D->GetXaxis()->GetXmax();

    int nBinsY = waterHist2D->GetNbinsY();  // Lepton energy bins
    double yMin = waterHist2D->GetYaxis()->GetXmin();
    double yMax = waterHist2D->GetYaxis()->GetXmax();  

    std::cout<<":::::::::::::::::::::::"<<std::endl;
    std::cout<<"xMin, xMax, yMin, yMax, nBinsX, nBinsY "<<xMin<<" "<< xMax<<" "<< yMin<<" "<< yMax<<" "<< nBinsX<<" "<< nBinsY<<std::endl;
    // Output ROOT file name
    TString outputFileName = "test2d_output_ratio_maps.root";

    // Save the ratio maps for each PDG code
    saveRatioMapsToTH2(ratioMap3D_14, 14, outputFileName);
    saveRatioMapsToTH2(ratioMap3D_12, 12, outputFileName);
    saveRatioMapsToTH2(ratioMap3D_neg14, -14, outputFileName);
    saveRatioMapsToTH2(ratioMap3D_neg12, -12, outputFileName);

  ///////////////////////////////////////// Guang's hacking end
*/


///////////////////////////////////////////////  Another Guang's hacking

    std::cout<<"Let's do our fitqun hacking"<<std::endl;

    // Open the FITQUN file
    //TFile * ff = new TFile("/pnfs/dune/scratch/users/gyang/wc/outputTest_extra_allswap_allhc_root6_12.root");
    //std::string input_fitqun = GetFitQunOutputFile(ff);
    //TFile* fitqunFile = TFile::Open(input_fitqun.c_str(), "READ");

    TFile* fitqunFile = new TFile("/pnfs/dune/scratch/users/gyang/wc/outputTest_extra_allswap_allhc_root6_12.root");
    if (!fitqunFile || fitqunFile->IsZombie()) {
        std::cerr << "Error: Unable to open FITQUN file " << std::endl;
        exit(0);
    }

    //std::cout<<"1"<<std::endl;
    // Get the tree
    TTree* tree2 = static_cast<TTree*>(fitqunFile->Get("h1")); // Replace "fitqun_tree2" with the actual TTree name
    if (!tree2) {
        std::cerr << "Error: TTree 'fitqun_tree2' not found in fitqun file" << std::endl;
        fitqunFile->Close();
        exit(0);
    }

    std::cout<<"***************************************************"<<std::endl;
    std::cout<<"***************************************************"<<std::endl;
    std::cout<<"********** Doing fitqun mixing hacking ************"<<std::endl;
    //std::cout<<"*** "<< input_fitqun.c_str()<<" ***"<<std::endl;
    std::cout<<"entries in caf and fitqun: "<<Nentries<<" "<<tree2->GetEntries()<<std::endl;
    std::cout<<"***************************************************"<<std::endl;
    std::cout<<"***************************************************"<<std::endl;


    // Branch variables
    int fqmrpcflg[50];
    int fqmrnring[50];
    int fqmrpid[50][6];
    float fqmrmom[50][6];
    float fqmrdir[50][6][3];
    int ipnu[50];
    float pnu[50];
    float dirnu[3][50];
    int numnu;
    int mode2;
    int fqnmrfit;
    double fluxWeight[2] = {};          // Flux weights for FHC and RHC
    double fluxWeight2[2] = {};          // Flux weights for FHC and RHC
    double fluxWeight3[2] = {};          // Flux weights for FHC and RHC

	// Declare variables
	float EvReco_fitqun;
	float EvRecoNue_fitqun;
	float EvRecoNumu_fitqun;
	float ElepReco_fitqun;
	float ThetaReco_fitqun;
	float RecoLepEnNue_fitqun;
	float RecoHadEnNue_fitqun;
	float RecoLepEnNumu_fitqun;
	float RecoHadEnNumu_fitqun;

	float EvTrue_fitqun;
	float ElepTrue_fitqun;
	int NuPDG_fitqun;
	int LepPDG_fitqun;
	int IsCC_fitqun;

	ParticleCounts particleCounts_fitqun;
	ParticleEnergies particleEnergies_fitqun;

	float thetaAng_fitqun;
	TrueMomentums trueMomenta_fitqun;

	float Q2_fitqun;
	float W_fitqun;
	float X_fitqun;
	float Y_fitqun;

	int RunNumber_fitqun;
	int IsFD_fitqun;
	int IsFHC_fitqun;


    //std::cout<<"2nnn"<<std::endl;
    // Set branch addresses
    tree2->SetBranchAddress("fqnmrfit", &fqnmrfit);

    tree2->SetBranchAddress("fqmrpcflg", fqmrpcflg);

    tree2->SetBranchAddress("fqmrnring", fqmrnring);

    tree2->SetBranchAddress("fqmrpid", fqmrpid);
    tree2->SetBranchAddress("fqmrmom", fqmrmom);

    //tree2->SetBranchAddress("fqmrdir", fqmrdir);
    tree2->SetBranchAddress("ipnu", ipnu);

    tree2->SetBranchAddress("pnu", pnu);
    tree2->SetBranchAddress("dirnu", dirnu);
    tree2->SetBranchAddress("numnu", &numnu);
    tree2->SetBranchAddress("mode", &mode2);
    tree2->SetBranchAddress("fluxWeight", fluxWeight);
    tree2->SetBranchAddress("fluxWeight2", fluxWeight2);
    tree2->SetBranchAddress("fluxWeight3", fluxWeight3);

    int nEntries2 = tree2->GetEntries();
    //std::cout<<"3"<<std::endl;
/////////////////////////////////////////////// End of another Guang's hacking


  //for (int n = 0; n < Nentries; ++n) {
  for (int n = 0; n < 100; ++n) {
    tr->GetEntry(n);
/*
    /////////////////////////////////////// Guang's hacking again
    int neutrinoPDG = sr.dune.nuPDG;
    double totalEnergy = sr.dune.Ev;
    int neutronNumber = sr.dune.nN;
    double leptonEnergy = sr.dune.LepE;

        // Compute the total energy bin index based on the event total energy
        int energyBin = static_cast<int>((totalEnergy - 0.5) / 0.5);

        // Check if the total energy is within the valid range
        if (energyBin < 0 || energyBin >= static_cast<int>(ratioMap3D_14.size())) {
            //std::cerr << "Skipping event: Total energy " << totalEnergy << " GeV is out of range." << std::endl;
            continue;
        }

        // Select the appropriate ratio map based on the neutrino PDG
        std::vector<std::vector<std::vector<double>>>* ratioMap3D;
        switch (neutrinoPDG) {
            case 14:
                ratioMap3D = &ratioMap3D_14;
                break;
            case 12:
                ratioMap3D = &ratioMap3D_12;
                break;
            case -14:
                ratioMap3D = &ratioMap3D_neg14;
                break;
            case -12:
                ratioMap3D = &ratioMap3D_neg12;
                break;
            default:
                //std::cerr << "Skipping event: Invalid neutrino PDG " << neutrinoPDG << std::endl;
                continue;
        }

        // Get the corresponding 2D ratio map for this energy bin
        std::vector<std::vector<double>>& ratioMap2D = (*ratioMap3D)[energyBin];

        // Get the bin index for neutron number and lepton energy
        int xBin = getBinIndex(neutronNumber, nBinsX, xMin, xMax);
        int yBin = getBinIndex(leptonEnergy, nBinsY, yMin, yMax);

        double ratio = 1;
        // Check that the bin indices are valid
        if (xBin >= 0 && yBin >= 0) {
            // Get the ratio from the 2D ratio map
            ratio = ratioMap2D[xBin][yBin];

            // Output the ratio for this event
            //std::cout << "Event " << n << " - Total Energy: " << totalEnergy
            //          << " GeV, Neutron Number: " << neutronNumber
            //          << ", Lepton Energy: " << leptonEnergy
            //          << " GeV, Ratio (Water/Argon): " << ratio << " ; xBin and yBin "<< xBin<<" -- "<<yBin<<std::endl;
        } else {
            //std::cerr << "Skipping event: Bin index out of range for neutron number or lepton energy." << std::endl;
        }

 
    //sr.dune.Ev_reco = processEvent(sr.dune.nuPDG, abs(sr.dune.LepPDG) , 1, sr.dune.LepE, sr.dune.Ev).second;
    //sr.dune.Ev_reco_nue = sr.dune.Ev_reco;
    //sr.dune.Ev_reco_numu = sr.dune.Ev_reco;
    sr.dune.Ev_reco = sr.dune.Ev + gRandom->Gaus(0, sr.dune.Ev*0.02);
    sr.dune.Ev_reco_nue = sr.dune.Ev + gRandom->Gaus(0, sr.dune.Ev*0.02);
    sr.dune.Ev_reco_numu = sr.dune.Ev + gRandom->Gaus(0, sr.dune.Ev*0.02);
    //std::cout<<"have it checked, nuPDG, lepPDG, lepE, Ev and Ev_reco "<<std::endl;
    //std::cout<<"--------------- "<< sr.dune.nuPDG<<" "<<sr.dune.LepPDG<<" "<<sr.dune.LepE<<" "<<sr.dune.Ev<<" "<<sr.dune.Ev_reco<<" "<<std::endl;
    ///////////////////////////////////////////////////// Guang's hacking ends again
*/

    // Set GENIE_ScatteringMode and eRec_FromDep
    if (sr.dune.isFD) {
      sr.dune.eRec_FromDep = sr.dune.eDepP + sr.dune.eDepN + sr.dune.eDepPip +
                             sr.dune.eDepPim + sr.dune.eDepPi0 +
                             sr.dune.eDepOther + sr.dune.LepE;

      sr.dune.GENIE_ScatteringMode =
          ana::GetGENIEModeFromSimbMode(sr.dune.mode);
    } else {
      sr.dune.eRec_FromDep = sr.dune.eRecoP + sr.dune.eRecoN +
                             sr.dune.eRecoPip + sr.dune.eRecoPim +
                             sr.dune.eRecoPi0 + sr.dune.eRecoOther +
                             sr.dune.LepE;
      sr.dune.GENIE_ScatteringMode = sr.dune.mode;
    }

    // Patch up isFD which isn't set properly in FD CAFs
    if (sr.dune.isFD) {
      if (sr.dune.isFHC != 0 && sr.dune.isFHC != 1) {
        if (sr.dune.run == 20000001 || sr.dune.run == 20000002 ||
            sr.dune.run == 20000003) {
          sr.dune.isFHC = true;
          static bool once = true;
          if (once) {
            std::cout << "\nPatching up FD file to be considered FHC"
                      << std::endl;
            once = false;
          }
        } else if (sr.dune.run == 20000004 || sr.dune.run == 20000005 ||
                   sr.dune.run == 20000006) {
          sr.dune.isFHC = false;
          static bool once = true;
          if (once) {
            std::cout << "\nPatching up FD file to be considered RHC"
                      << std::endl;
            once = false;
          }
        } else {
          std::cout
              << "When patching FD CAF with unknown isFHC, saw unknown run "
              << sr.dune.run << std::endl;
          abort();
        }
      }
    } else {
      // ND
      if (sr.dune.isFHC == -1) {
        // nu-on-e files
        sr.dune.isFHC = 0;
        static bool once = true;
        if (once) {
          std::cout << "\nPatching up nu-on-e file to be considered FHC"
                    << std::endl;
          once = false;
        }
      } else if (sr.dune.isFHC != 0 && sr.dune.isFHC != 1) {
        std::cout << "isFHC not set properly in ND file: " << sr.dune.isFHC
                  << std::endl;
        abort();
      }

      double nd_smear;
      bool add_nd_smear = AddSmear(nd_smear);

      // Smear the resolution... this was added for a D1ND test, don't use it unless you know what it does
      // And if everyone's forgotten what it does, it's safe to delete it
      if (add_nd_smear){
	// sr.dune.Elep_reco is the variable that actually needs to be smeared
	// sr.dune.Ev_reco needs to be modified to match
	// sr.dune.LepE is for an alternative usage... modify it for completeness
	
	// Draw from a unit Gaussian
	static TRandom3 rnd(0);
	double this_smear = rnd.Gaus(0.0, 1.0);
	
	// Multiple by the resolution given by an environment variable
	this_smear *= nd_smear;

	// Modify the lepton energy by the appropriate amount
	double old_Elep_reco = sr.dune.Elep_reco;
	double old_LepE = sr.dune.LepE;
	double old_Ev_reco = sr.dune.Ev_reco;

	sr.dune.Elep_reco = old_Elep_reco*(1+this_smear);
	sr.dune.LepE = old_LepE*(1+this_smear);
	sr.dune.Ev_reco = old_Ev_reco - old_Elep_reco + sr.dune.Elep_reco;
      }
    }

    // Get the crazy flux info properly
    sr.dune.wgt_CrazyFlux.resize(7);
    for (int i = 0; i < 7; ++i) {
      sr.dune.wgt_CrazyFlux[i] = crazy_tmp[i];
    }

    // Reformat the genie systs
    //if (ratio != 0)
      //sr.dune.total_xsSyst_cv_wgt = 1 * ratio;
    //else
      sr.dune.total_xsSyst_cv_wgt = 1;

    static auto AnaV = GetAnaVersion();
    if (AnaV == kV3) {
      for (unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {
        const int Nuniv = XSSyst_tmp[syst_it].size();
        assert((Nuniv >= 0) && (Nuniv <= int(XSSyst_tmp[syst_it].size())));
        sr.dune.xsSyst_wgt[syst_it].resize(Nuniv);

        // Do some error checking here
        if (std::isnan(XSSyst_cv_tmp[syst_it]) ||
            std::isinf(XSSyst_cv_tmp[syst_it]) ||
            (XSSyst_cv_tmp[syst_it] == 0)) {
          std::cout << "Warning: " << XSSyst_names[syst_it]
                    << " has a bad CV of " << XSSyst_cv_tmp[syst_it]
                    << std::endl;
        } else {
          sr.dune.total_xsSyst_cv_wgt *= XSSyst_cv_tmp[syst_it];
        }

        for (int u_it = 0; u_it < Nuniv; ++u_it) {
          sr.dune.xsSyst_wgt[syst_it][u_it] = XSSyst_tmp[syst_it][u_it];
        }
      }
    } else {

      for (unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {
        const int Nuniv = XSSyst_size_tmp[syst_it];
        if (!Nuniv) {
          continue;
        }

        assert(Nuniv <= int(XSSyst_tmp[syst_it].size()));

        if (IsDoNotIncludeSyst(syst_it)) { // Multiply CV weight back into
                                           // response splines.
          if (std::isnan(XSSyst_cv_tmp[syst_it]) ||
              std::isinf(XSSyst_cv_tmp[syst_it]) ||
              XSSyst_cv_tmp[syst_it] == 0) {
            std::cout << "Warning: " << XSSyst_names[syst_it]
                      << " has a bad CV of " << XSSyst_cv_tmp[syst_it]
                      << std::endl;
          } else {
            for (int univ_it = 0; univ_it < Nuniv; ++univ_it) {
              XSSyst_tmp[syst_it][univ_it] *= XSSyst_cv_tmp[syst_it];
            }
          }
        } else { // Include CV weight in the total
          // Do some error checking here
          if (std::isnan(XSSyst_cv_tmp[syst_it]) ||
              std::isinf(XSSyst_cv_tmp[syst_it]) ||
              XSSyst_cv_tmp[syst_it] == 0) {
            std::cout << "Warning: " << XSSyst_names[syst_it]
                      << " has a bad CV of " << XSSyst_cv_tmp[syst_it]
                      << std::endl;
          } else {
            sr.dune.total_xsSyst_cv_wgt *= XSSyst_cv_tmp[syst_it];
          }
        }

        // Copy the spline in
        sr.dune.xsSyst_wgt[syst_it].clear();
        std::copy(XSSyst_tmp[syst_it].begin(), XSSyst_tmp[syst_it].end(),
                  std::back_inserter(sr.dune.xsSyst_wgt[syst_it]));
      }
    } // end version switch

////////////////////////////////////////////////////// Continue another Guang's hacking
    std::cout<<"nEntries2 "<<nEntries2<<" i "<<n<<std::endl;

    if ( n < nEntries2 ){
        tree2->GetEntry(n);

        //std::cout<<"mmmarker 5: "<<n<<std::endl;
	//std::cout<<"fqnmrfit, fqmrnring[0] "<<fqnmrfit<<" "<<fqmrnring[0]<<std::endl;
	if (fqmrnring[0] == 0 || fqnmrfit == 0) continue;
        // Reconstruction variables
        EvReco_fitqun = FitqunConverter::GetEvReco(fqmrpcflg, fqmrnring[0], fqmrmom, fqmrpid);
	//std::cout<<EvReco_fitqun<<std::endl;
        float EvRecoNue_fitqun = FitqunConverter::GetEvRecoNue(fqnmrfit, fqmrpcflg, fqmrnring, fqmrpid, fqmrmom);
        //EvRecoNue_fitqun = EvReco_fitqun;
	//std::cout<<EvRecoNue_fitqun<<std::endl;
        float EvRecoNumu_fitqun = FitqunConverter::GetEvRecoNumu(fqnmrfit, fqmrpcflg, fqmrnring, fqmrpid, fqmrmom);
        //EvRecoNumu_fitqun = EvReco_fitqun;
	//std::cout<<EvRecoNumu_fitqun<<std::endl;
        ElepReco_fitqun = FitqunConverter::GetElepReco(fqmrpcflg, fqmrnring[0], fqmrpid[0], fqmrmom[0]);
	//std::cout<<ElepReco_fitqun<<std::endl;
        //ThetaReco_fitqun = FitqunConverter::GetThetaReco(fqmrpcflg, fqmrnring[0], fqmrpid[0], fqmrdir[0], fqmrmom[0]);
        RecoLepEnNue_fitqun = FitqunConverter::GetRecoLepEnNue(fqnmrfit, fqmrpcflg, fqmrpid, fqmrmom);
        RecoHadEnNue_fitqun = FitqunConverter::GetRecoHadEnNue(EvReco_fitqun, 50, fqmrpcflg, fqmrpid, fqmrmom);
	//std::cout<<RecoHadEnNue_fitqun<<std::endl;
        RecoLepEnNumu_fitqun = FitqunConverter::GetRecoLepEnNumu(fqnmrfit, fqmrpcflg, fqmrpid, fqmrmom);
        RecoHadEnNumu_fitqun = FitqunConverter::GetRecoHadEnNumu(EvReco_fitqun, 50, fqmrpcflg, fqmrpid, fqmrmom);

	//std::cout<<RecoHadEnNumu_fitqun<<std::endl;
        // True energy and PDG values
        EvTrue_fitqun = FitqunConverter::GetEvTrue(pnu);
        ElepTrue_fitqun = FitqunConverter::GetElepTrue(pnu);
        NuPDG_fitqun = FitqunConverter::GetNuPDG(ipnu);
        LepPDG_fitqun = FitqunConverter::GetLepPDG(ipnu);

	//std::cout<<LepPDG_fitqun<<std::endl;
        // GENIE interaction mode
        IsCC_fitqun = FitqunConverter::GetIsCC(mode2);

        // Particle counts
        particleCounts_fitqun = FitqunConverter::CountOutgoingParticles(numnu, ipnu);

        // Particle energies
        particleEnergies_fitqun = FitqunConverter::CalculateParticleEnergies(numnu, ipnu, pnu);
        thetaAng_fitqun = FitqunConverter::CalculateTrueThetaAngle(dirnu, 0, 2);

	ThetaReco_fitqun = thetaAng_fitqun;

        // Derived quantities
	//std::cout<<"lep, Ev, theta "<<ElepTrue_fitqun<<" "<<EvTrue_fitqun<<" "<<thetaAng_fitqun<<std::endl;
        float Q2_fitqun = FitqunConverter::GetQ2(ElepTrue_fitqun, EvTrue_fitqun, thetaAng_fitqun);
        float W_fitqun = FitqunConverter::GetW(EvTrue_fitqun, ElepTrue_fitqun, thetaAng_fitqun);
        float X_fitqun = FitqunConverter::GetX(Q2_fitqun, EvTrue_fitqun, ElepTrue_fitqun);
        float Y_fitqun = FitqunConverter::GetY(EvTrue_fitqun, ElepTrue_fitqun);
	//std::cout<<"Q2 and W "<<Q2_fitqun<<" "<<W_fitqun<<std::endl;
	//if (W_fitqun< 1e6 && W_fitqun>-1e6) {W_fitqun = W_fitqun;}
	//else {W_fitqun = 0;}

        // True momentums
        TrueMomentums trueMomenta_fitqun = FitqunConverter::CalculateTrueMomentums(numnu, pnu, dirnu, ipnu);

        // Fixed values
        RunNumber_fitqun = FitqunConverter::GetRunNumber();
        IsFD_fitqun = FitqunConverter::GetIsFD();
        IsFHC_fitqun = FitqunConverter::GetIsFHC();

	int pid_fitqun  = FitqunConverter::CalculatePID(fqmrpcflg, fqmrnring[0], fqmrpid[0], fqmrmom[0]);

	if (abs(NuPDG_fitqun) != 12 && abs(NuPDG_fitqun) != 14 && abs(NuPDG_fitqun) != 16){
	  continue;
	}


	// Assign FitQuN variables to CAF variables
	sr.dune.Ev_reco = EvReco_fitqun;                              // Reconstructed total neutrino energy (GeV)
	sr.dune.Ev_reco_nue = EvRecoNue_fitqun;                       // Reconstructed energy assuming a νe interaction (GeV)
	sr.dune.Ev_reco_numu = EvRecoNumu_fitqun;                     // Reconstructed energy assuming a νμ interaction (GeV)
	sr.dune.Elep_reco = ElepReco_fitqun;                          // Reconstructed energy of the outgoing lepton (GeV)
	sr.dune.theta_reco = ThetaReco_fitqun;                        // Reconstructed lepton scattering angle (radians)
	sr.dune.mvaresult = -999;                                     // Multivariate analysis result score (default)
	sr.dune.mvanue = -999;                                        // MVA score for νe signal classification (default)
	sr.dune.mvanumu = -999;                                       // MVA score for νμ signal classification (default)
	sr.dune.cvnnue = (pid_fitqun == 1) ? 1.0 : -1.0; // CNN output for νe identification
	sr.dune.cvnnumu = (pid_fitqun == 2) ? 1.0 : -1.0;  // CNN output for νμ identification
	// Ignore numu_pid, nue_pid, reco_q
	sr.dune.RecoLepEnNue = RecoLepEnNue_fitqun;                   // Lepton energy for νe (GeV)
	sr.dune.RecoHadEnNue = RecoHadEnNue_fitqun;                   // Hadronic energy for νe (GeV)
	sr.dune.RecoLepEnNumu = RecoLepEnNumu_fitqun;                 // Lepton energy for νμ (GeV)
	sr.dune.RecoHadEnNumu = RecoHadEnNumu_fitqun;                 // Hadronic energy for νμ (GeV)
	// Ignore reco_numu, reco_nue, reco_nc, muon_exit, muon_contained, muon_ecal, muon_tracker, Ehad_veto
	sr.dune.Ev = EvTrue_fitqun;                                   // True neutrino energy (GeV)
	sr.dune.Elep = ElepTrue_fitqun;                               // True lepton energy (GeV)
	sr.dune.isCC = IsCC_fitqun;                                   // Is CC interaction (1 for CC, 0 for NC)
	sr.dune.nuPDG = NuPDG_fitqun;                                 // True neutrino PDG
	sr.dune.nuPDGunosc = NuPDG_fitqun;                            // True neutrino PDG before oscillation
	sr.dune.LepPDG = LepPDG_fitqun;                               // True lepton PDG
	sr.dune.mode = FitqunConverter::ConvertInteractionMode(mode2);
	sr.dune.nP = particleCounts_fitqun.nP;                        // Number of outgoing protons
	sr.dune.nN = particleCounts_fitqun.nN;                        // Number of outgoing neutrons
	sr.dune.nipi0 = particleCounts_fitqun.nipi0;                  // Number of outgoing π0 particles
	sr.dune.nipip = particleCounts_fitqun.nipip;                  // Number of outgoing π+ particles
	sr.dune.nipim = particleCounts_fitqun.nipim;                  // Number of outgoing π− particles
	sr.dune.niem = particleCounts_fitqun.niem;                    // Number of outgoing EM particles
	sr.dune.Q2 = Q2_fitqun;                                       // Four-momentum transfer squared (GeV^2)
	sr.dune.W = W_fitqun;                                         // Invariant mass of the hadronic system (GeV)
	sr.dune.Y = Y_fitqun;                                         // Inelasticity (dimensionless)
	sr.dune.X = X_fitqun;                                         // Bjorken x (dimensionless)
	sr.dune.NuMomX = trueMomenta_fitqun.NuMomX;                   // True x-component of the neutrino momentum (GeV/c)
	sr.dune.NuMomY = trueMomenta_fitqun.NuMomY;                   // True y-component of the neutrino momentum (GeV/c)
	sr.dune.NuMomZ = trueMomenta_fitqun.NuMomZ;                   // True z-component of the neutrino momentum (GeV/c)
	sr.dune.LepMomX = trueMomenta_fitqun.LepMomX;                 // True x-component of the lepton momentum (GeV/c)
	sr.dune.LepMomY = trueMomenta_fitqun.LepMomY;                 // True y-component of the lepton momentum (GeV/c)
	sr.dune.LepMomZ = trueMomenta_fitqun.LepMomZ;                 // True z-component of the lepton momentum (GeV/c)
	sr.dune.LepE = trueMomenta_fitqun.LepE;                       // True energy of the lepton (GeV)
	sr.dune.LepNuAngle = trueMomenta_fitqun.LepNuAngle;           // Angle between lepton and neutrino (radians)
	sr.dune.LongestTrackContNumu = 1;
	sr.dune.vtx_x = 0;                                            // Vertex x-coordinate
	sr.dune.vtx_y = 0;                                            // Vertex y-coordinate
	sr.dune.vtx_z = 0;                                            // Vertex z-coordinate
	// Ignore det_x
	sr.dune.eP = particleEnergies_fitqun.eP;                      // Energy of outgoing protons (GeV)
	sr.dune.eN = particleEnergies_fitqun.eN;                      // Energy of outgoing neutrons (GeV)
	sr.dune.ePip = particleEnergies_fitqun.ePip;                  // Energy of outgoing π+ (GeV)
	sr.dune.ePim = particleEnergies_fitqun.ePim;                  // Energy of outgoing π− (GeV)
	sr.dune.ePi0 = particleEnergies_fitqun.ePi0;                  // Energy of outgoing π0 (GeV)
	sr.dune.eOther = particleEnergies_fitqun.eOther;              // Energy of other particles (GeV)
	// Ignore eRecoP, eRecoN, eRecoPip, eRecoPim, eRecoPi0, eRecoOther, eDepP, eDepN, eDepPip, eDepPim, eDepPi0, eDepOther
	sr.dune.run = RunNumber_fitqun;                               // Run number
	sr.dune.isFD = IsFD_fitqun;                                   // Is Far Detector
	sr.dune.isFHC = IsFHC_fitqun;                                 // Is Forward Horn Current mode
	sr.dune.sigma_Ev_reco = FitqunConverter::GetDefaultValue();   // Default value
	sr.dune.sigma_Elep_reco = FitqunConverter::GetDefaultValue(); // Default value
	sr.dune.sigma_numu_pid = FitqunConverter::GetDefaultValue();  // Default value
	sr.dune.sigma_nue_pid = FitqunConverter::GetDefaultValue();   // Default value

	sr.dune.eRec_FromDep = EvReco_fitqun;

	// need to add flux weight
        if (sr.dune.run == 20000001)
	  sr.dune.total_xsSyst_cv_wgt = fluxWeight[0];
	if (sr.dune.run == 20000002)
	  sr.dune.total_xsSyst_cv_wgt = fluxWeight2[0];
	if (sr.dune.run == 20000003)		
	  sr.dune.total_xsSyst_cv_wgt = fluxWeight3[0];

	//if (abs(sr.dune.nuPDG) != 12 && abs(sr.dune.nuPDG) != 14 && abs(sr.dune.nuPDG) != 16){
        // Example output
/*
        std::cout << "Event " << n << ":\n"
                  << "  EvReco: " << EvReco_fitqun << " GeV\n"
                  << "  EvRecoNue: " << EvRecoNue_fitqun << " GeV\n"
                  << "  EvRecoNumu: " << EvRecoNumu_fitqun << " GeV\n"
                  << "  ElepReco: " << ElepReco_fitqun << " GeV\n"
                  << "  ThetaReco: " << ThetaReco_fitqun << " radians\n"
                  << "  RecoLepEnNue: " << RecoLepEnNue_fitqun << " GeV\n"
                  << "  RecoHadEnNue: " << RecoHadEnNue_fitqun << " GeV\n"
                  << "  RecoLepEnNumu: " << RecoLepEnNumu_fitqun << " GeV\n"
                  << "  RecoHadEnNumu: " << RecoHadEnNumu_fitqun << " GeV\n"
                  << "  EvTrue: " << EvTrue_fitqun << " GeV\n"
                  << "  ElepTrue: " << ElepTrue_fitqun << " GeV\n"
                  << "  NuPDG: " << NuPDG_fitqun << "\n"
                  << "  LepPDG: " << LepPDG_fitqun << "\n"
                  << "  IsCC: " << IsCC_fitqun << "\n"  
                  << "  Q2: " << Q2_fitqun << " GeV^2\n"
                  << "  W: " << W_fitqun << " GeV\n"
                  << "  X: " << X_fitqun << "\n"
                  << "  Y: " << Y_fitqun << "\n"
		  << "  flux weight: "<<fluxWeight[0]<<" "<<fluxWeight[1]<< std::endl;
*/
	//}

    }

///////////////////////////////////////////////////// End continue another Guang's hacking
    
    HandleRecord(&sr);

    if (prog && n % 10000 == 0)
      prog->SetProgress(double(n) / Nentries);
  } // end for n    
  fitqunFile->Close();
  std::cout<<" ------------------------ one run completed --------------------"<<std::endl;
}

//----------------------------------------------------------------------
/// Helper for \ref HandleRecord
template <class T, class U> class CutVarCache {
public:
  CutVarCache() : fVals(U::MaxID() + 1), fValsSet(U::MaxID() + 1, false) {}

  inline T Get(const U &var, const caf::StandardRecord *sr) {
    const unsigned int id = var.ID();

    if (fValsSet[id]) {
      return fVals[id];
    } else {
      const T val = var(sr);
      fVals[id] = val;
      fValsSet[id] = true;
      return val;
    }
  }

protected:
  // Seems to be faster to do this than [unordered_]map
  std::vector<T> fVals;
  std::vector<bool> fValsSet;
};

//----------------------------------------------------------------------
void SpectrumLoader::HandleRecord(caf::StandardRecord *sr) {
  // Some shifts only adjust the weight, so they're effectively nominal, but
  // aren't grouped with the other nominal histograms. Keep track of the
  // results for nominals in these caches to speed those systs up.
  CutVarCache<bool, Cut> nomCutCache;
  CutVarCache<double, Var> nomWeiCache;
  CutVarCache<double, Var> nomVarCache;

  for (auto &shiftdef : fHistDefs) {
    const SystShifts &shift = shiftdef.first;

    // Need to provide a clean slate for each new set of systematic shifts to
    // work from. Unfortunately, copying the whole StandardRecord is pretty
    // expensive. So we need to rely on this slightly dangerous "Restorer"
    // mechanism.

    // Spot checks to try and make sure no-one misses adding a variable to
    // Restorer
    static int iterationNo = 0;
    // Prime means we should get good coverage over all combinations
    const int kTestIterations = 9973;

    const TestVals *save = 0;
    if (++iterationNo % kTestIterations == 0)
      save = GetVals(sr, shiftdef.second);

    Restorer *restore = 0;
    double systWeight = 1;
    bool shifted = false;
    // Can special-case nominal to not pay cost of Shift() or Restorer
    if (!shift.IsNominal()) {
      restore = new Restorer;
      shift.Shift(*restore, sr, systWeight);
      // Did the Shift actually modify the event at all?
      shifted = !restore->Empty();
    }

    for (auto &cutdef : shiftdef.second) {
      const Cut &cut = cutdef.first;

      const bool pass = shifted ? cut(sr) : nomCutCache.Get(cut, sr);
      // Cut failed, skip all the histograms that depended on it
      if (!pass)
        continue;

      for (auto &weidef : cutdef.second) {
        const Var &weivar = weidef.first;

        double wei = shifted ? weivar(sr) : nomWeiCache.Get(weivar, sr);

        wei *= systWeight;
        if (wei == 0)
          continue;

        for (auto &vardef : weidef.second) {
          if (vardef.first.IsMulti()) {
            for (double val : vardef.first.GetMultiVar()(sr)) {
              for (Spectrum *s : vardef.second.spects)
                s->Fill(val, wei);
            }
            continue;
          }

          const Var &var = vardef.first.GetVar();

          const double val = shifted ? var(sr) : nomVarCache.Get(var, sr);

          if (std::isnan(val) || std::isinf(val)) {
            std::cerr << "Warning: Bad value: " << val
                      << " returned from a Var. The input variable(s) could "
                      << "be NaN in the CAF, or perhaps your "
                      << "Var code computed 0/0?";
            std::cout << " Not filling into this histogram for this slice."
                      << std::endl;
            continue;
          }

          for (Spectrum *s : vardef.second.spects)
            s->Fill(val, wei);

          for (ReweightableSpectrum *rw : vardef.second.rwSpects) {
            const double yval = rw->ReweightVar()(sr);

            if (std::isnan(yval) || std::isinf(yval)) {
              std::cerr << "Warning: Bad value: " << yval
                        << " for reweighting Var";
              std::cout << ". Not filling into histogram." << std::endl;
              continue;
            }

            // TODO: ignoring events with no true neutrino etc
            if (yval != 0)
              rw->fHist->Fill(val, yval, wei);
          } // end for rw
        }   // end for vardef
      }     // end for weidef
    }       // end for cutdef

    // Delete Restorer at this point and return StandardRecord to its
    // unshifted form ready for the next histogram.
    delete restore;

    // Make sure the record went back the way we found it
    if (save) {
      CheckVals(save, sr, shift.ShortName(), shiftdef.second);
      delete save;
    }
  } // end for shiftdef
}

//----------------------------------------------------------------------
void SpectrumLoader::ReportExposures() {
  // The POT member variables we use here were filled as part of
  // SpectrumLoaderBase::GetNextFile() as we looped through the input files.

  // Let's just assume no-one is using the Cut::POT() function yet, so this
  // printout remains relevant...

  std::cout << fPOT << " POT" << std::endl;
}

//----------------------------------------------------------------------
void SpectrumLoader::AccumulateExposures(const caf::SRSpill *spill) {}

//----------------------------------------------------------------------
void SpectrumLoader::StoreExposures() {
  for (auto &shiftdef : fHistDefs) {
    for (auto &cutdef : shiftdef.second) {
      for (auto &weidef : cutdef.second) {
        for (auto &vardef : weidef.second) {
          for (Spectrum *s : vardef.second.spects)
            s->fPOT += fPOT;
          for (ReweightableSpectrum *rw : vardef.second.rwSpects)
            rw->fPOT += fPOT;
        }
      }
    }
  }

  // std::map<int, double> livetime;
  // std::map<int, double> pot;

  // for(unsigned int i = 0; i < fAllCuts.size(); ++i){
  //   const int id = fAllCuts[i].ID();
  //   if(fLivetimeByCut[i] < 0){
  //     fLivetimeByCut[i] = 0;
  //     std::cout << "WARNING: no way to compute livetime for FD data spectrum.
  //     If you want a livetime you need to be applying one of the cuts from
  //     TimingCuts.h or similar. You probably should be anyway to remove bad
  //     data near the spill ends." << std::endl;
  //   }
  //   livetime.emplace(id, fLivetimeByCut[i]);
  //   pot.emplace(id, fPOTByCut[i]);
  // }

  // for(auto& shiftdef: fHistDefs){
  //   for(auto& cutdef: shiftdef.second){
  //     const Cut& cut = cutdef.first;
  //     const int id = cut.ID();

  //     for(auto& weidef: cutdef.second){
  //       for(auto& vardef: weidef.second){
  //         for(Spectrum* s: vardef.second.spects){
  //           s->fPOT += pot[id];
  //           s->fLivetime += livetime[id];
  //         }

  //         for(ReweightableSpectrum* rw: vardef.second.rwSpects){
  //           rw->fPOT += pot[id];
  //           rw->fLivetime += livetime[id];
  //         }
  //       }
  //     }
  //   }
  // }
}

//----------------------------------------------------------------------
const SpectrumLoader::TestVals *SpectrumLoader::GetVals(
    const caf::StandardRecord *sr,
    IDMap<Cut, IDMap<Var, IDMap<VarOrMultiVar, SpectList>>> &hists) const {
  TestVals *ret = new TestVals;

  // Store values for all Vars and Cuts of interest
  for (auto &cutdef : hists) {
    const bool cutval = cutdef.first(sr);
    ret->cuts.push_back(cutval);
    // Don't evaluate Vars when the Cut fails, might not be safe
    if (!cutval)
      continue;

    for (auto &weidef : cutdef.second) {
      ret->weis.push_back(weidef.first(sr));

      for (auto &vardef : weidef.second) {
        if (!vardef.first.IsMulti())
          ret->vars.push_back((vardef.first.GetVar())(sr));
      }
    }
  }

  return ret;
}

//----------------------------------------------------------------------
void SpectrumLoader::ValError(const std::string &type, const std::string &shift,
                              const std::set<std::string> & /*req*/,
                              double orig, double now) const {
  // Try and print a comprehensive error message, I imagine this might be
  // hard to track down.

  std::cerr << std::endl;

  std::cerr << "Error. Value of " << type
            << " changed after it was shifted and then restored." << std::endl;

  std::cerr << "While applying shift " << shift;

  std::cerr << " initially had value " << orig << " now has " << now
            << std::endl;

  std::cerr << "Please check your use of Restorer very carefully" << std::endl;

  abort();
}

//----------------------------------------------------------------------
void SpectrumLoader::CheckVals(
    const TestVals *v, const caf::StandardRecord *sr,
    const std::string &shiftName,
    IDMap<Cut, IDMap<Var, IDMap<VarOrMultiVar, SpectList>>> &hists) const {
  unsigned int cutIdx = 0;
  unsigned int weiIdx = 0;
  unsigned int varIdx = 0;

  // Ensure everything is as TestVals says it should be

  for (auto &cutdef : hists) {
    const bool cutval = cutdef.first(sr);

    if (cutval != v->cuts[cutIdx]) {
      ValError("Cut", shiftName, {}, v->cuts[cutIdx], cutval);
    }
    ++cutIdx;

    // Don't evaluate Vars when the Cut fails, might not be safe
    if (!cutval)
      continue;

    for (auto &weidef : cutdef.second) {
      const double weival = weidef.first(sr);
      if (!std::isnan(weival) && weival != v->weis[weiIdx]) {
        ValError("Cut", shiftName, {}, v->weis[weiIdx], weival);
      }
      ++weiIdx;

      for (auto &vardef : weidef.second) {
        if (vardef.first.IsMulti())
          continue;
        const double varval = vardef.first.GetVar()(sr);
        if (!std::isnan(varval) && varval != v->vars[varIdx]) {
          ValError("Var", shiftName, {}, v->vars[varIdx], varval);
        }
        ++varIdx;
      } // end for vardef
    }   // end for weidef
  }     // end for cutdef
}
} // namespace ana
