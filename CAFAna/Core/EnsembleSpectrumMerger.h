
#pragma once

#include "cafanacore/Spectrum.h"

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/IRecordSource.h"
#include "CAFAna/Core/Weight.h"
#include "CAFAna/Core/EnsembleSpectrum.h"
#include <cassert>

namespace ana{
	

  class	EnsembleSpectrumMerger//: public EnsembleSpectrum
  {
  public:
   	EnsembleSpectrumMerger(std::vector<Spectrum> specs, 
   													const FitMultiverse* multiverse, const LabelsAndBins& axis);

  // 	virtual ~EnsembleSpectrumMerger();

  protected:
    const FitMultiverse* fMultiverse;

    mutable Hist fHist;
    mutable double fPOT;
    mutable double fLivetime;
    mutable LabelsAndBins fAxis;
  	//EnsembleSpectrumMerger(){};

  };


}