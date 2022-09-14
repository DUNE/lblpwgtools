#pragma once

#include "CAFAna/Core/SpectrumLoaderBase.h"

#include <set>

class TFile;

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

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

    virtual void HandleRecord(caf::SRProxy* sr);

    /// Save results of AccumulateExposures into the individual spectra
    virtual void StoreExposures();

    /// Prints POT/livetime info for all spectra
    virtual void ReportExposures();

    /// All unique cuts contained in fHistDefs
    std::vector<Cut> fAllCuts;
    std::vector<double> fLivetimeByCut; ///< Indexing matches fAllCuts
    std::vector<double> fPOTByCut;      ///< Indexing matches fAllCuts
    int max_entries;

  };
}
