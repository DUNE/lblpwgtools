/////////////////////////////////////////////////////////////////////////////////
// NDOscillatableSpectrum.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Object containing Spectrum with the ability to be oscillated (also contains
// true energy information).  Similar to OscillatableSpectrum but for ND.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Core/NDOscillatableSpectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/NDOscCurve.h"
#include "CAFAna/Core/Utilities.h"
#include "StandardRecord/SRProxy.h"
#include "OscLib/IOscCalc.h"

// root
#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

// stl
#include <cassert>
#include <memory>

namespace ana {

  const Var kTrueLOverTrueE([](const caf::SRProxy* sr) {
      const double trueE = sr->Ev;
      // TODO needs to be replaced with the true baseline
      // when this information is available in StandardRecord
      // const double trueL = 0.574; // km -- ND baseline
      const double trueL = 0.383; // km -- 2/3 of ND baseline (a more accurate number for the decay of the mesons)
      return trueL/trueE;
    });

  //----------------------------------------------------------------------
  NDOscillatableSpectrum::NDOscillatableSpectrum(const std::string& label,
						 const Binning& bins,
						 SpectrumLoaderBase& loader,
						 const Var& var,
						 const Cut& cut,
						 const SystShifts& shift,
						 const Var& wei)
    : ReweightableSpectrum(loader,
			   HistAxis(label, bins, var),
			   HistAxis("True Energy (GeV)", kTrueLOverTrueEBins, kTrueLOverTrueE),
			   cut, shift, wei) {
  }

  //----------------------------------------------------------------------
  NDOscillatableSpectrum::NDOscillatableSpectrum(SpectrumLoaderBase& loader,
                                                 const HistAxis& axis,
                                                 const Cut& cut,
                                                 const SystShifts& shift,
                                                 const Var& wei)
    : ReweightableSpectrum(loader, axis,
			   HistAxis("True Energy (GeV)", kTrueLOverTrueEBins, kTrueLOverTrueE),
			   cut, shift, wei) {
  }

  //----------------------------------------------------------------------
  NDOscillatableSpectrum::~NDOscillatableSpectrum() {
  }

  //----------------------------------------------------------------------
  NDOscillatableSpectrum::NDOscillatableSpectrum(const NDOscillatableSpectrum& rhs)
    : ReweightableSpectrum(rhs) {
      assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  NDOscillatableSpectrum::NDOscillatableSpectrum(NDOscillatableSpectrum&& rhs)
    : ReweightableSpectrum(rhs) {
    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
  }

  //----------------------------------------------------------------------
  NDOscillatableSpectrum& NDOscillatableSpectrum::operator=(const NDOscillatableSpectrum& rhs) {
    if (this == &rhs) return *this;
    ReweightableSpectrum::operator=(rhs);
    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
    assert( fReferences.empty() ); // Copying with pending loads is unexpected
    return *this;
  }

  //----------------------------------------------------------------------
  NDOscillatableSpectrum& NDOscillatableSpectrum::operator=(NDOscillatableSpectrum&& rhs) {
    if (this == &rhs) return *this;
    ReweightableSpectrum::operator=(rhs);
    assert( rhs.fReferences.empty() ); // Copying with pending loads is unexpected
    assert( fReferences.empty() ); // Copying with pending loads is unexpected
    return *this;
  }

  //----------------------------------------------------------------------
  Spectrum NDOscillatableSpectrum::Oscillated(osc::IOscCalc* calc,
                                              int from, int to) const {
    const NDOscCurve curve(calc, from, to);
    return WeightedBy(curve);
  }

  //----------------------------------------------------------------------
  void NDOscillatableSpectrum::SaveTo(TDirectory* dir, const std::string& name) const {
    _SaveTo(dir, name, "NDOscillatableSpectrum");
  }

  //----------------------------------------------------------------------
  std::unique_ptr<NDOscillatableSpectrum> NDOscillatableSpectrum::LoadFrom(TDirectory* dir, const std::string& name) {

    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    DontAddDirectory guard;

    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "NDOscillatableSpectrum");
    delete tag;

    TH2D* spect = (TH2D*)dir->Get("hist");
    assert(spect);
    TH1* hPot = (TH1*)dir->Get("pot");
    assert(hPot);
    TH1* hLivetime = (TH1*)dir->Get("livetime");
    assert(hLivetime);

    std::vector<std::string> labels;
    std::vector<Binning> bins;

    for (int i = 0; ; ++i) {
      const std::string subname = TString::Format("bins%d", i).Data();
      TDirectory* subdir = dir->GetDirectory(subname.c_str());
      if (!subdir) break;
      delete subdir;
      bins.push_back(*Binning::LoadFrom(dir, subname));
      TObjString* label = (TObjString*)dir->Get(TString::Format("label%d", i));
      labels.push_back(label ? label->GetString().Data() : "");
      delete label;
    }
    delete dir;

    auto ret = std::make_unique<NDOscillatableSpectrum>(kNullLoader,
							HistAxis(labels, bins),
							kNoCut);

    // ROOT histogram storage is row-major, but Eigen is column-major by
    // default
    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen:: Dynamic, Eigen::RowMajor> MatRowMajor;
    ret->fMat = Eigen::Map<MatRowMajor>(spect->GetArray(),
					ret->fMat.rows(),
					ret->fMat.cols());

    delete spect;

    ret->fPOT = hPot->Integral(0, -1);
    ret->fLivetime = hLivetime->Integral(0, -1);

    delete hPot;
    delete hLivetime;

    return ret;
  }

}