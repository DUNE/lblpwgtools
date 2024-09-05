#include "CAFAna/Core/EnsembleSpectrum.h"

#include "CAFAna/Core/EnsembleRatio.h"

#include "CAFAna/Core/FitMultiverse.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Core/EnsembleSpectrum.h"
#include "CAFAna/Core/EnsembleSpectrumMerger.h"
#include <cassert>
#include <iostream>


namespace ana{

  EnsembleSpectrumMerger::EnsembleSpectrumMerger(std::vector< Spectrum> specs, 
  	const FitMultiverse* multiverse,
  	const LabelsAndBins& axis)
  
  EnsembleSpectrum EnsembleSpectrumMerger::GetEnsembleSpectrum
  : 	fMultiverse(multiverse),
      fHist(Hist::Zero((axis.GetBins1D().NBins()+2) * fMultiverse->NUniv())),
      fPOT(specs[0].POT()),
      fLivetime(specs[0].Livetime()),
      fAxis(axis)
  {

    std::cout<< "how many multiverses? "<< fMultiverse->NUniv()<<std::endl;
    // verify it is the same size
    for (auto spec : specs){
      assert(spec.GetLabels().size()==axis.size() && "one of the spectra has a different axis!");
    }
    LabelsAndBins labelsAndBins = LabelsAndBins(specs[0].GetLabels(), specs[0].GetBinnings());
    double pot = specs[0].POT();
    double livetime = specs[0].Livetime();

    long unsigned int bins = labelsAndBins.GetBins1D().NBins(); 

    // are these two extrabins overflow and underflow?
    //Hist data = Hist::Zero((labelsAndBins.GetBins1D().NBins()+2) * multiverse->NUniv());// append here all the spectra
    Eigen::ArrayXd data(bins+2, 1);
    for ( unsigned int i = 0; i<multiverse->NUniv(); i++){
      // sanity checks
      assert(specs[i].ToTH1(pot).GetNbinsX()==bins && "one of the spectra has  different nbins!");
      assert(specs[i].POT() == pot && "one of the spectra has a different POT!");
      assert(specs[i].Livetime() == livetime && "one of the spectra has a different livetime!");

      // get this universe spectra eigenarray
      Eigen::ArrayXd spec = specs[i].GetEigen();
      // copy into new array
      for (unsigned int b = 0; b <= bins ; b++){
        data[ b + (bins+2)*i ] = spec[b]; 
        fHist.Fill(b + (bins+2)*i, spec[b]);
      }
    }
  
    assert( data.rows() == fHist.GetEigen().rows() && "data and hist are diffesrent size");
      std::cout<<"now moving data\n";
  }
   
//  //----------------------------------------------------------------------
//  EnsembleSpectrumMerger::~EnsembleSpectrumMerger()
//  {
//  }

}