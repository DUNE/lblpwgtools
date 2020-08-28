#include <iostream>
#include <functional>

#include "TCanvas.h"
#include "TGraph.h"
#include "TH2F.h"
#include "Minuit2/StackAllocator.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TKey.h"
#include "TVectorD.h"
#include "TObjString.h"
#include "TCollection.h"

#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Fit/MinuitFitter.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ThreadPool.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/IOscCalc.h"

namespace ana
{
  //----------------------------------------------------------------------
  FrequentistSurface::FrequentistSurface(const IExperiment* expt,
                                         osc::IOscCalcAdjustable* calc,
                                         const IFitVar* xvar, int nbinsx, double xmin, double xmax,
                                         const IFitVar* yvar, int nbinsy, double ymin, double ymax,
                                         const std::vector<const IFitVar*>& profVars,
                                         const std::vector<const ISyst*>& profSysts,
                                         const SeedList& seedPts,
                                         const std::vector<SystShifts>& systSeedPts,
                                         bool parallel,
                                         MinuitFitter::FitOpts opts)
    : fParallel(parallel), fFitOpts(opts)
  {
    CreateHistograms(xvar, nbinsx, xmin, xmax,
                     yvar, nbinsy, ymin, ymax,
                     profVars, profSysts);

    for(const IFitVar* v: profVars) fSeedValues.push_back(v->GetValue( calc));

    FillSurface(expt, calc, xvar, yvar, profVars, profSysts, seedPts, systSeedPts);

    FindMinimum(expt, calc, xvar, yvar, profVars, profSysts, seedPts, systSeedPts);
  }

  //---------------------------------------------------------------------
  FrequentistSurface::~FrequentistSurface()
  {
  }

  //---------------------------------------------------------------------
  void FrequentistSurface::
  CreateHistograms(const IFitVar* xvar, int nbinsx, double xmin, double xmax,
                   const IFitVar* yvar, int nbinsy, double ymin, double ymax,
                   const std::vector<const IFitVar*>& profVars,
                   const std::vector<const ISyst*>& profSysts)
  {
    fHist = ExpandedHistogram(";"+xvar->LatexName()+";"+yvar->LatexName(),
                              nbinsx, xmin, xmax,
                              nbinsy, ymin, ymax);

    for(unsigned int i = 0; i < profVars.size()+profSysts.size(); ++i){
      std::string title;
      if(i < profVars.size())
        title = profVars[i]->LatexName();
      else
        title = profSysts[i-profVars.size()]->LatexName();

      fProfHists.push_back(ExpandedHistogram(title+";"+xvar->LatexName()+";"+yvar->LatexName(),
                                             nbinsx, xmin, xmax,
                                             nbinsy, ymin, ymax));
    }

  }

  //---------------------------------------------------------------------
  std::string FrequentistSurface::
  ProgressBarTitle(const IFitVar* xvar, const IFitVar* yvar,
                   const std::vector<const IFitVar*>& profVars,
                   const std::vector<const ISyst*>& profSysts) const
  {
    std::string title = "Filling surface for "+yvar->ShortName()+" vs "+xvar->ShortName();

    if(!profVars.empty() || !profSysts.empty()){
      title += " (profiling ";

      for(const IFitVar* v: profVars) title += v->ShortName() + ", ";
      for(const ISyst* s: profSysts)  title += s->ShortName() + ", ";

      // Always have one superfluous ", " at the end
      title.resize(title.size()-2);
      title += ")";
    }

    return title;
  }

  //---------------------------------------------------------------------
  void FrequentistSurface::FillSurface(const IExperiment *expt,
                                       osc::IOscCalcAdjustable *calc,
                                       const IFitVar *xvar, const IFitVar *yvar,
                                       const std::vector<const IFitVar *> &profVars,
                                       const std::vector<const ISyst *> &profSysts,
                                       const SeedList& seedPts,
                                       const std::vector<SystShifts> &systSeedPts)
  {
    if (fParallel && !(profVars.empty() && profSysts.empty()))
    {
      // Minuit calls this at some point, and it creates a static. Which
      // doesn't like happening on multiple threads at once. So do it upfront
      // while we're still single-threaded.
      ROOT::Minuit2::StackAllocatorHolder::Get();
    }

    // Nothing created during surface filling belongs in a
    // directory. Unfortunately the local guards in Spectrum etc are racey when
    // run in parallel. But this should cover the whole lot safely.
    DontAddDirectory guard;

    const std::string progTitle = ProgressBarTitle(xvar, yvar, profVars, profSysts);

    Progress *prog = 0;
    // Difficult to keep a progress bar properly up to date when threaded
    if (!fParallel) prog = new Progress(progTitle);
    ThreadPool *pool = 0;
    if (fParallel)
    {
      pool = new ThreadPool;
      pool->ShowProgress(progTitle);
    }

    const int Nx = fHist->GetNbinsX();
    const int Ny = fHist->GetNbinsY();

    // Fill bins in "random" order so that the progress bar is accurate
    // straight away instead of possibly being misled by whatever atypical
    // points we start with. This step is a prime which guarantees we get every
    // cell.
    int step = 7919;
    // Very unlikely (Nx or Ny is a multiple of step), but just to be safe.
    if ((Nx * Ny) % step == 0) step = 1;

    int bin = 0;
    int neval = 0;

    do
    {
      const int x = bin % Nx + 1;
      const int y = bin / Nx + 1;

      const double xv = fHist->GetXaxis()->GetBinCenter(x);
      const double yv = fHist->GetYaxis()->GetBinCenter(y);

      if (xvar->Penalty(xv, calc) > 1e-10)
      {
        std::cerr << "Warning! " << xvar->ShortName() << " = " << xv
                  << " has penalty of " << xvar->Penalty(xv, calc)
                  << " that could have been applied in surface. "
                  << "This should never happen." << std::endl;
      }
      if (yvar->Penalty(yv, calc) > 1e-10)
      {
        std::cerr << "Warning! " << yvar->ShortName() << " = " << yv
                  << " has penalty of " << yvar->Penalty(yv, calc)
                  << " that could have been applied in surface. "
                  << "This should never happen." << std::endl;
      }

      if (fParallel)
      {
        pool->AddMemberTask(this, &FrequentistSurface::FillSurfacePoint,
                            expt, calc,
                            xvar, xv, yvar, yv,
                            profVars, profSysts, seedPts, systSeedPts);
      } else
      {
        FillSurfacePoint(expt, calc,
                          xvar, xv, yvar, yv,
                          profVars, profSysts, seedPts, systSeedPts);
        ++neval;
        prog->SetProgress(neval / double(Nx * Ny));
      }

      bin = (bin + step) % (Nx * Ny);
    } while (bin != 0);


    if (fParallel)
    {
      pool->Finish();
      delete pool;
    } else
    {
      prog->Done();
      delete prog;
    }
  }

  /// \brief Helper for FrequentistSurface::FillSurfacePoint
  ///
  /// The cacheing of the nominal done in PredictionInterp is not
  /// threadsafe. This is an inelegant but pragmatic way of suppressing it.
  class OscCalcNoHash: public osc::IOscCalcAdjustable
  {
  public:
    OscCalcNoHash(osc::IOscCalcAdjustable* c) : fCalc(c) {}

    osc::IOscCalcAdjustable* Copy() const override
    {
      std::cout << "FrequentistSurface::OscCalcNoHash not copyable." << std::endl;
      abort();
    }
    double P(int a, int b, double E) override {return fCalc->P(a, b, E);}
    /// Marks this calculator "unhashable" so the cacheing won't occur
    TMD5* GetParamsHash() const override {return 0;}

    // It's a shame we have to forward all the getters and setters explicitly,
    // but I don't see how to avoid it. Take away some drudgery with a macro.
#define F(v)\
    void Set##v(double x) override {fCalc->Set##v(x);}\
    double Get##v() const override {return fCalc->Get##v();}
    F(L); F(Rho);
#undef F
#define F(v)\
    void Set##v(const double& x) override {fCalc->Set##v(x);}\
    double Get##v() const override {return fCalc->Get##v();}
      F(Dmsq21); F(Dmsq32); F(Th12); F(Th13); F(Th23); F(dCP);
#undef F
  protected:
    osc::IOscCalcAdjustable* fCalc;
  };

  //----------------------------------------------------------------------
  double FrequentistSurface::FillSurfacePoint(const IExperiment* expt,
                                              osc::IOscCalcAdjustable* calc,
                                              const IFitVar* xvar, double x,
                                              const IFitVar* yvar, double y,
                                              const std::vector<const IFitVar*>& profVars,
                                              const std::vector<const ISyst*>& profSysts,
                                              const SeedList& seedPts,
                                              const std::vector<SystShifts>& systSeedPts)
  {
    osc::IOscCalcAdjustable* calcNoHash = 0; // specific to parallel mode

    if(fParallel){
      // Need to take our own copy so that we don't get overwritten by someone
      // else's changes.
      calc = calc->Copy();
    }

    xvar->SetValue(calc, x);
    yvar->SetValue(calc, y);

    if (fParallel){
      calcNoHash = new OscCalcNoHash(calc);
    }

    //Make sure that the profiled values of fitvars do not persist between steps.
    for(int i = 0; i < (int)fSeedValues.size(); ++i) profVars[i]->SetValue( calc, fSeedValues[i] );

    double chi;
    if(profVars.empty() && profSysts.empty()){
      chi = expt->ChiSq(fParallel ? calcNoHash : calc);
    }
    else{
      MinuitFitter fitter(expt, profVars, profSysts);
      fitter.SetFitOpts(fFitOpts);
      SystShifts bestSysts;
      chi = fitter.Fit(calc, bestSysts, seedPts, systSeedPts, MinuitFitter::kQuiet)->EvalMetricVal();

      for(unsigned int i = 0; i < profVars.size(); ++i){
        fProfHists[i]->Fill(x, y, profVars[i]->GetValue(calc));
      }
      for(unsigned int j = 0; j < profSysts.size(); ++j){
        fProfHists[j+profVars.size()]->Fill(x, y, bestSysts.GetShift(profSysts[j]));
      }
    }

    fHist->Fill(x, y, chi);

    if(fParallel){
      delete calc;
      delete calcNoHash;
    }
    return chi;
  }


  //---------------------------------------------------------------------
  void FrequentistSurface::FindMinimum(const IExperiment* expt,
                                       osc::IOscCalcAdjustable* calc,
                                       const IFitVar* xvar, const IFitVar* yvar,
                                       const std::vector<const IFitVar*>& profVars,
                                       const std::vector<const ISyst*>& profSysts,
                                       const SeedList& seedPts,
                                       const std::vector<SystShifts>& systSeedPts)
  {
    // Location of the best minimum found from filled surface
    double minchi = 1e10;
    int minx = fHist->GetNbinsX()/2;
    int miny = fHist->GetNbinsY()/2;
    for(int x = 1; x <= fHist->GetNbinsX(); ++x){
      for(int y = 1; y <= fHist->GetNbinsY(); ++y){
        const double chi = fHist->GetBinContent(x, y);
        if(chi < minchi){
          minchi = chi;
          minx = x;
          miny = y;
        }
      }
    }

    std::vector<const IFitVar*> allVars = {xvar, yvar};
    allVars.insert(allVars.end(), profVars.begin(), profVars.end());
    MinuitFitter fit(expt, allVars, profSysts);
    fit.SetFitOpts(fFitOpts);
    // Seed from best grid point
    xvar->SetValue(calc, fHist->GetXaxis()->GetBinCenter(minx));
    yvar->SetValue(calc, fHist->GetYaxis()->GetBinCenter(miny));
    for(int i = 0; i < (int)fSeedValues.size(); ++i) profVars[i]->SetValue( calc, fSeedValues[i] );
    SystShifts systSeed = SystShifts::Nominal();
    fBestLikelihood = fit.Fit(calc, systSeed, seedPts)->EvalMetricVal();
    fBestFitX = xvar->GetValue(calc);
    fBestFitY = yvar->GetValue(calc);

    for(int x = 0; x < fHist->GetNbinsX()+2; ++x){
      for(int y = 0; y < fHist->GetNbinsY()+2; ++y){
        fHist->SetBinContent(x, y, fHist->GetBinContent(x, y)-fBestLikelihood);
      }
    }

    fHist->SetMinimum(0);
  }

  //----------------------------------------------------------------------
  void FrequentistSurface::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory *tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("FrequentistSurface").Write("type");

    ISurface::SaveToHelper(dir);

    TDirectory *profDir = dir->mkdir("profHists");
    int idx = 0;
    for (auto it: FrequentistSurface::fProfHists)
    {
      profDir->cd();
      it->Write(TString::Format("hist%d", idx++));
    }

    dir->Write();
    delete dir;

    tmp->cd();
  }

//----------------------------------------------------------------------
  std::unique_ptr<FrequentistSurface> FrequentistSurface::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    DontAddDirectory guard;

    TObjString *tag = (TObjString *) dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "FrequentistSurface");
    delete tag;

    std::unique_ptr<FrequentistSurface> surf(new FrequentistSurface);
    ISurface::FillSurfObj(*surf, dir);

    for (std::size_t idx = 0; idx < surf->fSeedValues.size(); ++idx)
    {
      // Search for old "marg" name here too for backwards compatibility
      TH2 *h = (TH2 *) dir->Get(TString::Format("profHists/hist%lu", idx));
      if (h)
        surf->fProfHists.push_back(h);
      else
        surf->fProfHists.push_back((TH2 *) dir->Get(TString::Format("margHists/hist%lu", idx)));
    }

    delete dir;

    return surf;
  }


  // See eg the statistics section of the PDG
  TH2* Gaussian68Percent2D(const FrequentistSurface& s){return Flat(2.30, s);}
  TH2* Gaussian90Percent2D(const FrequentistSurface& s){return Flat(4.61, s);}
  TH2* Gaussian95Percent2D(const FrequentistSurface& s){return Flat(5.99, s);}
  TH2* Gaussian2Sigma2D   (const FrequentistSurface& s){return Flat(6.18, s);}
  TH2* Gaussian99Percent2D(const FrequentistSurface& s){return Flat(9.21, s);}
  TH2* Gaussian3Sigma2D   (const FrequentistSurface& s){return Flat(11.83, s);}

  TH2* Gaussian68Percent1D(const FrequentistSurface& s){return Flat(1.00, s);}
  TH2* Gaussian90Percent1D(const FrequentistSurface& s){return Flat(2.71, s);}
  TH2* Gaussian95Percent1D(const FrequentistSurface& s){return Flat(3.84, s);}
  TH2* Gaussian2Sigma1D   (const FrequentistSurface& s){return Flat(4.00, s);}
  TH2* Gaussian99Percent1D(const FrequentistSurface& s){return Flat(6.63, s);}
  TH2* Gaussian3Sigma1D   (const FrequentistSurface& s){return Flat(9.00, s);}


} // namespace
