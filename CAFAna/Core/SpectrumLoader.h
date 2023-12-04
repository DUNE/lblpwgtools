#pragma once

#include "CAFAna/Core/SpectrumLoaderBase.h"

#include <set>

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
    SpectrumLoader(const std::string& wildcard, int max = 0);
    SpectrumLoader(const std::vector<std::string>& fnames, int max = 0);

#ifdef WITH_SAM
    /// Named constructor for SAM projects
    static SpectrumLoader FromSAMProject(const std::string& proj,
					 int fileLimit = -1);
#endif
    virtual ~SpectrumLoader();

    virtual void Go() override;

  protected:
    SpectrumLoader();

    // Move operations
    SpectrumLoader(SpectrumLoader&&) = default;
    SpectrumLoader& operator=(SpectrumLoader&&) = default;

    // No copy operations because I don't want to deal with pointers
    SpectrumLoader(const SpectrumLoader&) = delete;
    SpectrumLoader& operator=(const SpectrumLoader&) = delete;

    virtual void HandleFile(TFile* f, Progress* prog = 0);

    virtual void HandleRecord(caf::StandardRecord* sr);

    /// Save results of AccumulateExposures into the individual spectra
    virtual void StoreExposures();

    /// Prints POT/livetime info for all spectra
    virtual void ReportExposures();

    // This is all infrasture to test that the user didn't screw up their
    // systematic shifts.
    struct TestVals
    {
      std::vector<bool> cuts;
      std::vector<double> vars, weis;
    };

    const TestVals* GetVals(const caf::SRProxy* sr,
			    IDMap<Cut, IDMap<Weight, IDMap<VarOrMultiVar, SpectList>>>& hists) const;
    void CheckVals(const TestVals* v,
                   const caf::SRProxy* sr,
                   const std::string& shiftName,
		   IDMap<Cut, IDMap<Weight, IDMap<VarOrMultiVar, SpectList>>>& hists) const;
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
