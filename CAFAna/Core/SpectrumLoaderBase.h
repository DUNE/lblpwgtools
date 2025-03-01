#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "CAFAnaCore/CAFAna/Core/IFileSource.h"

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/MultiVar.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Weight.h"

class TDirectoryFile;
class TFile;
class TH1;
class TTree;

namespace ana
{
  class Spectrum;
  class ReweightableSpectrum;

  /// Base class for the various types of spectrum loader
  class SpectrumLoaderBase
  {
  public:
// Dow we need these friends??
//    friend class ReweightableSpectrum;
//    friend class NDOscillatableSpectrum;
//    friend class OscillatableSpectrum;
//    friend class Spectrum;

    virtual ~SpectrumLoaderBase();

    /// Load all the registered spectra
    virtual void Go() = 0;

    /// Indicate whether or not \ref Go has been called
    virtual bool Gone() const {return fGone;}

  protected:
    /// Component of other constructors
    SpectrumLoaderBase();
    /// Construct from a filename, wildcard, SAM definition, or SAM query
    SpectrumLoaderBase(const std::string& wildcard, const std::string& pathInFile="");
    /// Construct from an explicit list of files
    SpectrumLoaderBase(const std::vector<std::string>& fnames, const std::string& pathInFile="");

    // Move operations
    SpectrumLoaderBase(SpectrumLoaderBase&&) = default;
    SpectrumLoaderBase& operator=(SpectrumLoaderBase&&) = default;

    // No copy operations because I don't want to deal with pointers
    SpectrumLoaderBase(const SpectrumLoaderBase&) = delete;
    SpectrumLoaderBase& operator=(const SpectrumLoaderBase&) = delete;

    /// Figure out if \a str is a wildcard or SAM query and return a source
    IFileSource* WildcardOrSAMQuery(const std::string& str) const;

    /// Forwards to \ref fFileSource
    int NFiles() const;

    /// Forwards to \ref fFileSource but also accumulates POT and livetime
    TFile* GetNextFile();

    /// the relevant trees inside a CAF may be nested in a subdir.
    /// this is a simple utility function to centralize that
    TDirectoryFile *GetDirectoryFile(TFile *f) const;

    std::string fWildcard;
    std::unique_ptr<IFileSource> fFileSource;
    std::string fPathInFile;  ///< are the various trees we need in some sub-folder?

    bool fGone; ///< Has Go() been called? Can't add more histograms after that

    double fPOT; ///< Accumulated by calls to \ref GetNextFile


    // new additions 
    double fPOTFromHist; ///< Accumulated by calls to \ref GetNextFile
    // Do we really need this?
    int fNReadouts; 
  };

}
