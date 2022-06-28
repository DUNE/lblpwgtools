#include "CAFAna/Experiment/CountingExperiment.h"

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/IOscCalc.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH1.h"

#include <memory>

namespace ana
{
  //----------------------------------------------------------------------
  CountingExperiment::CountingExperiment(const IPrediction* p,
                                         const Spectrum& d)
    : fMC(p), fData(d)
  {
  }

  //----------------------------------------------------------------------
  CountingExperiment::~CountingExperiment()
  {
  }

  //----------------------------------------------------------------------
  double CountingExperiment::ChiSq(osc::IOscCalcAdjustable* osc,
                                   const SystShifts& syst) const
  {
    double exp = fMC->PredictSyst(osc, syst).Integral(fData.POT());
    double obs = fData.Integral(fData.POT());

    return ana::LogLikelihood(exp, obs);  // use namespace to disabiguate from method inherited from IExperiment
  }

  //----------------------------------------------------------------------
  void CountingExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = dir;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("CountingExperiment").Write("type");

    fMC->SaveTo(dir, "mc");
    fData.SaveTo(dir, "data");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<CountingExperiment> CountingExperiment::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "CountingExperiment");
    delete ptag;

    assert(dir->GetDirectory("mc"));
    assert(dir->GetDirectory("data"));
    

    const IPrediction* mc = ana::LoadFrom<IPrediction>(dir, "mc").release();
    const std::unique_ptr<Spectrum> data = Spectrum::LoadFrom(dir, "data");

    delete dir;

    auto ret = std::make_unique<CountingExperiment>(mc, *data);
    return ret;
  }
}
