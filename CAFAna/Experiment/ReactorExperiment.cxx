#include "CAFAna/Experiment/ReactorExperiment.h"

#include "CAFAna/Vars/FitVars.h"

#include "Utilities/func/MathUtil.h"

#include "TDirectory.h"
#include "TH1.h"
#include "TObjString.h"

#include <cassert>

namespace ana
{
  //----------------------------------------------------------------------
  double ReactorExperiment::ChiSq(osc::IOscCalcAdjustable* osc,
                                  const SystShifts& /*syst*/) const
  {
    return util::sqr((kFitSinSq2Theta13.GetValue(osc)-fBestFit)/fSigma);
  }

  //----------------------------------------------------------------------
  double ReactorExperiment::SSTh13(osc::IOscCalcAdjustable* osc) const
  {
    return kFitSinSq2Theta13.GetValue(osc);
  }

  //----------------------------------------------------------------------
  void ReactorExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = dir;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("ReactorExperiment").Write("type");

    TH1D params("", "", 2, 0, 2);
    params.SetBinContent(1, fBestFit);
    params.SetBinContent(2, fSigma);
    params.Write("params");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<ReactorExperiment> ReactorExperiment::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "ReactorExperiment");
    delete tag;

    TH1* params = (TH1*)dir->Get("params");
    assert(params);

    const double bestFit = params->GetBinContent(1);
    const double sigma   = params->GetBinContent(2);

    delete dir;

    return std::unique_ptr<ReactorExperiment>(new ReactorExperiment(bestFit,
                                                                    sigma));
  }

  //----------------------------------------------------------------------
  const ReactorExperiment* ReactorConstraintNuFit2018(bool normal_ordering, bool sk_data)
  {
    // NuFit November 2018
    double this_val = 0.;
    double this_err = 0.;

    if(sk_data){
      if(normal_ordering){
        this_val = 0.088;
        this_err = 0.003;
      }
      else{
        this_val = 0.088;
        this_err = 0.003;        
      }
    } // use sk atm data
    else{
      if(normal_ordering){
        this_val = 0.088;
        this_err = 0.003;
      }
      else{
        this_val = 0.089;
        this_err = 0.003;        
      }
    } // no sk atm data

    return new ReactorExperiment(this_val, this_err);
  }

  //----------------------------------------------------------------------
  const ReactorExperiment* ReactorConstraintNuFit2019(bool normal_ordering, bool sk_data)
  {
    // NuFit July 2019
    double this_val = 0.;
    double this_err = 0.;

    if(sk_data){
      if(normal_ordering){
        this_val = 0.087;
        this_err = 0.003;
      }
      else{
        this_val = 0.088;
        this_err = 0.003;        
      }
    } // use sk atm data
    else{
      if(normal_ordering){
        this_val = 0.088
        this_err = 0.003;
      }
      else{
        this_val = 0.088;
        this_err = 0.003;        
      }
    } // no sk atm data

    return new ReactorExperiment(this_val, this_err);
  }

  //----------------------------------------------------------------------
  const ReactorExperiment* ReactorConstraintPDG2019()
  {
    // http://pdg.lbl.gov/2019/tables/rpp2019-sum-leptons.pdf
    // ssth13=0.0218+/-0.0007 -> ss2th13=0.085+/-0.003
    return new ReactorExperiment(0.085, 0.003);
  }
}
