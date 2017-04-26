#include "CAFAna/Prediction/PredictionScaleComp.h"

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH1.h"
#include "TVectorD.h"

#include <cassert>

namespace ana
{
  namespace
  {
    // We need some way to identify what components the user wants to vary in
    // the fit. We just create N copies of this class, each one identifying one
    // component. They're distinguished by their pointer values.
    class DummyScaleCompSyst : public ana::ISyst
    {
    public:
      DummyScaleCompSyst(int i) : fN(i) {}

      std::set<std::string> Requires() const override { return {}; }

      std::string ShortName() const override
      {
        return "CompNormShift_" + std::to_string(fN);
      }

      std::string LatexName() const override
      {
        return "Component Normalization Shift " + std::to_string(fN);
      }

      void Shift(double sigma, ana::Restorer&, caf::StandardRecord* sr,
                 double& weight) const override
      {
      }
    private:
      int fN;
    };
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(SpectrumLoaderBase& loader,
                      const HistAxis& axis,
                      Cut cut,
                      const std::vector<const SystComponentScale*>& systs,
                      const SystShifts& shift,
                      const Var& wei)
    : fSysts(systs)
  {
    Cut complementCut = kNoCut;

    assert(!systs.empty() && "Please give at least one systematic.");
    for(const SystComponentScale* syst: systs){
      fPreds.push_back(new PredictionNoOsc(loader, axis,
                                           cut && syst->GetCut(), shift, wei));
      complementCut = complementCut && !syst->GetCut();
    }

    // The idea is that if truthcuts are exhaustive, this Spectrum should wind
    // up empty
    fComplement = new PredictionNoOsc(loader, axis,
                                      cut && complementCut, shift, wei);
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(DUNERunPOTSpectrumLoader& loaderBeam,
                      DUNERunPOTSpectrumLoader& loaderNue,
                      DUNERunPOTSpectrumLoader& loaderNuTau,
                      DUNERunPOTSpectrumLoader& loaderNC,
                      const HistAxis&     axis,
                      Cut                 cut,
                      const std::vector<const SystComponentScale*>& systs,
                      const SystShifts&   shift,
                      const Var&          wei)
  {
    Cut complementCut = kNoCut;

    assert(!systs.empty() && "Please give at least one systematic.");
    for(const SystComponentScale* syst: systs){
      fPreds.push_back(new PredictionNoExtrap(loaderBeam, loaderNue, loaderNuTau, loaderNC,
                                              axis, cut && syst->GetCut(), shift, wei));
      complementCut = complementCut && !syst->GetCut();
    }

    // This is the set of events that didn't wind up in any of the scaleable
    // categories.
    fComplement = new PredictionNoExtrap(loaderBeam, loaderNue, loaderNuTau, loaderNC,
                                         axis, cut && complementCut, shift, wei);
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(SpectrumLoaderBase& loader,
                      const HistAxis& axis1,
                      const HistAxis& axis2,
                      Cut cut,
                      const std::vector<const SystComponentScale*>& systs,
                      const SystShifts& shift,
                      const Var& wei)
  {
    assert(0 && "unimplemented");

    // TODO TODO TODO
    /*
    assert(truthcuts.size()>0 && "Please give at least one truth selection.");
    for(unsigned int i = 0; i < truthcuts.size(); ++i){
      fPreds.push_back(new PredictionNoOsc(loader, axis1, axis2,
                                           cut && truthcuts[i], shift, wei));
      fSysts.push_back(new DummyScaleCompSyst(i));
      fComplementCut = fComplementCut && !truthcuts[i];
    }

    // The idea is that if truthcuts are exhaustive, this Spectrum should wind
    // up empty
    fComplement = new Spectrum(loader, axis1, axis2,
                               cut && fComplementCut, shift, wei);
    */
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::
  PredictionScaleComp(const IPrediction* complement,
                      const std::vector<const IPrediction*>& preds,
                      const std::vector<const SystComponentScale*>& systs)
    : fSysts(systs),
      fPreds(preds),
      fComplement(complement)
  {
  }

  //----------------------------------------------------------------------
  PredictionScaleComp::~PredictionScaleComp()
  {
    for(const ISyst* s: fSysts) delete s;
    for(const IPrediction* p: fPreds) delete p;
    delete fComplement;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionScaleComp::Predict(osc::IOscCalculator* calc) const
  {
    Spectrum ret = fComplement->Predict(calc);

    for(const IPrediction* p: fPreds) ret += p->Predict(calc);

    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionScaleComp::PredictSyst(osc::IOscCalculator* calc,
                                            const SystShifts&    shift) const
  {
    SystShifts shiftClean = shift;
    for(const ISyst* s: fSysts) shiftClean.SetShift(s, 0);

    Spectrum ret = fComplement->PredictSyst(calc, shiftClean);

    for(unsigned int i = 0; i < fPreds.size(); ++i){
      Spectrum si = fPreds[i]->PredictSyst(calc, shiftClean);
      si.Scale(1 + shift.GetShift(fSysts[i]));

      si.Scale(pow(1+fSysts[i].OneSigmaScale(), shift.GetShift(fSysts[i])));

      ret += si;
    }

    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionScaleComp::SaveTo(TDirectory* dir) const
  {
    assert(0 && "No solution for serializing Systs...");

    TDirectory* tmp = gDirectory;
    dir->cd();

    TObjString("PredictionScaleComp").Write("type");

    fComplement->SaveTo(dir->mkdir("complement"));

    for(unsigned int i = 0; i < fPreds.size(); ++i){
      fPreds[i]->SaveTo(dir->mkdir(("pred"+std::to_string(i)).c_str()));
    }

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionScaleComp> PredictionScaleComp::LoadFrom(TDirectory* dir)
  {
    assert(0 && "No solution for serializing Systs...");

    IPrediction* complement = ana::LoadFrom<IPrediction>(dir->GetDirectory("complement")).release();

    std::vector<const IPrediction*> preds;

    for(unsigned int i = 0; ; ++i){
      const IPrediction* p = ana::LoadFrom<IPrediction>(dir->GetDirectory(("pred"+std::to_string(i)).c_str())).release();
      if(!p) break; // We got all the predictions

      preds.push_back(p);
    }

    return std::unique_ptr<PredictionScaleComp>(new PredictionScaleComp(complement, preds, {}));
  }

}
