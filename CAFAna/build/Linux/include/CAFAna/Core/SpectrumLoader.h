#pragma once

#include "CAFAna/Core/SpectrumLoaderBase.h"

#include <TGraphErrors.h>
#include <TRandom3.h>
#include <utility> // For std::pair
#include "TH3.h"
#include "CAFAna/Core/FitqunConverter.h"
class TFile;

namespace ana
{
  class Progress;

  /// \brief Collaborates with \ref Spectrum and \ref OscillatableSpectrum to
  /// fill spectra from CAF files.
  ///
  /// The outside user should just pass a filename, wildcard, SAM dataset name,
  /// or query to the constructor. Then construct all the spectra you
  /// need. They will register with this loader. Finally, calling \ref Go will
  /// cause all the spectra to be filled at once. After this the loader may not
  /// be used again.
  class SpectrumLoader: public SpectrumLoaderBase
  {
  public:
    SpectrumLoader(const std::string& wildcard, DataSource src = kBeam, int max = 0);
    SpectrumLoader(const std::vector<std::string>& fnames,
                   DataSource src = kBeam, int max = 0);

#ifndef DONT_USE_SAM
    /// Named constructor for SAM projects
    static SpectrumLoader FromSAMProject(const std::string& proj,
					 DataSource src = kBeam,
					 int fileLimit = -1);
#endif
    virtual ~SpectrumLoader();

    virtual void Go() override;

    // Function to sample from histogram data based on its bin contents
    double sampleFromHistogram(const std::vector<double>& histogram, const std::vector<double>& binEdges, TRandom3* gRandom);

    // Function to load smearing histograms into vectors and create CDFs
    void loadSmearingVectors(std::vector<double>& nu_had_smear_vector, std::vector<double>& antinu_had_smear_vector, std::vector<double>& nu_bin_edges, std::vector<double>& antinu_bin_edges);

    // Function to load graph errors from external files
    void loadGraphErrors(std::vector<std::vector<TGraphErrors*>>& graphError);

    // Function to compute reconstructed energy based on lepton and hadron energies
    std::pair<double, double> computeRecoE(int particleType, int absPdg, int status, double lepE_tr, double Ev, const std::vector<double>& nu_had_smear_vector, const std::vector<double>& antinu_had_smear_vector,const std::vector<double>& nu_bin_edges, const std::vector<double>& antinu_bin_edges,TRandom3* gRandom, std::vector<std::vector<TGraphErrors*>>& graphError, double res_emu_factor = 1.25);

    // Function to process an event and compute energy values
    std::pair<double, double> processEvent(int particleType, int absPdg, int status, double lepE_tr, double Ev);

    // Helper function to get bin index for a value
    int getBinIndex(double value, int nBins, double binLowEdge, double binHighEdge) {
      if (value < binLowEdge || value > binHighEdge) return -1;
      double binWidth = (binHighEdge - binLowEdge) / nBins;
      int binIndex = static_cast<int>((value - binLowEdge) / binWidth);
      return (binIndex < 0 || binIndex >= nBins) ? -1 : binIndex;
    }
    // Function to create a 3D map of ratios (total energy, lepton energy, neutron number)
    std::vector<std::vector<std::vector<double>>> processMaps(int neutrinoPDG);

    // Function to process each ratioMap3D and save it as a TH2 in the output file
    void saveRatioMapsToTH2(
      const std::vector<std::vector<std::vector<double>>>& ratioMap3D, 
      int pdgCode, 
      const TString& outputFileName); 

    std::string GetFitQunOutputFile(TFile* f);

  protected:
    SpectrumLoader(DataSource src = kBeam);

    // Move operations
    SpectrumLoader(SpectrumLoader&&) = default;
    SpectrumLoader& operator=(SpectrumLoader&&) = default;

    // No copy operations because I don't want to deal with pointers
    SpectrumLoader(const SpectrumLoader&) = delete;
    SpectrumLoader& operator=(const SpectrumLoader&) = delete;

    void AccumulateExposures(const caf::SRSpill* spill) override;

    virtual void HandleFile(TFile* f, Progress* prog = 0);

    virtual void HandleRecord(caf::StandardRecord* sr);

    /// Save results of AccumulateExposures into the individual spectra
    virtual void StoreExposures();

    /// Prints POT/livetime info for all spectra
    virtual void ReportExposures();

    bool AddSmear(double &smear_val);

    // This is all infrasture to test that the user didn't screw up their
    // systematic shifts.
    struct TestVals
    {
      std::vector<bool> cuts;
      std::vector<double> vars, weis;
    };

    const TestVals* GetVals(const caf::StandardRecord* sr,
			    IDMap<Cut, IDMap<Var, IDMap<VarOrMultiVar, SpectList>>>& hists) const;
    void CheckVals(const TestVals* v,
                   const caf::StandardRecord* sr,
                   const std::string& shiftName,
		   IDMap<Cut, IDMap<Var, IDMap<VarOrMultiVar, SpectList>>>& hists) const;
    void ValError(const std::string& type,
                  const std::string& shift,
                  const std::set<std::string>& req,
                  double orig, double now) const;


    /// All unique cuts contained in fHistDefs
    std::vector<Cut> fAllCuts;
    std::vector<double> fLivetimeByCut; ///< Indexing matches fAllCuts
    std::vector<double> fPOTByCut;      ///< Indexing matches fAllCuts
    int max_entries;
  };
}
