#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <functional>
#include <limits>
#include <chrono>
#include <set>

#include "TCanvas.h"
#include "TFile.h"
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

#include "CAFAna/Core/IFittable.h"
#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Fit/MinuitFitter.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ThreadPool.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Fit/FitAxis.h"

#include "OscLib/IOscCalc.h"

namespace ana
{

  //----------------------------------------------------------------------
  FrequentistSurface::FrequentistSurface(osc::IOscCalcAdjustable* calc,
                                         const FitAxis& xax,
                                         const FitAxis& yax,
                                         const std::vector<const IFitVar*>& profVars,
                                         const std::vector<const ISyst*>& profSysts,
                                         bool parallel,
                                         MinuitFitter::FitOpts opts,
                                         EGlobalMinOpt minopt)
    : fParallel(parallel), fFitOpts(opts), fMinOpt(minopt)
  {
    fLogX = xax.islog;
    fLogY = yax.islog;

    CreateHistograms(xax, yax, profVars, profSysts);

    for(const IFitVar* v: profVars) fSeedValues.push_back(v->GetValue( calc));
  }

  //----------------------------------------------------------------------
  FrequentistSurface::FrequentistSurface(const IExperiment* expt,
                                         osc::IOscCalcAdjustable* calc,
                                         const FitAxis& xax,
                                         const FitAxis& yax,
                                         const std::vector<const IFitVar*>& profVars,
                                         const std::vector<const ISyst*>& profSysts,
                                         const SeedList& seedPts,
                                         const std::vector<SystShifts>& systSeedPts,
                                         bool parallel,
                                         MinuitFitter::FitOpts opts,
                                         EGlobalMinOpt minopt)
    : FrequentistSurface(calc, xax, yax, profVars, profSysts, parallel, opts, minopt)
  {
    FillSurface(expt, calc, xax.var, yax.var, profVars, profSysts, seedPts, systSeedPts);
    FindMinimum(expt, calc, xax.var, yax.var, profVars, profSysts, seedPts, systSeedPts);
  }

  //---------------------------------------------------------------------
  FrequentistSurface::~FrequentistSurface()
  {
  }

  //---------------------------------------------------------------------
  void FrequentistSurface::
  CreateHistograms(const FitAxis& xax, const FitAxis& yax,
                   const std::vector<const IFitVar*>& profVars,
                   const std::vector<const ISyst*>& profSysts)
  {
    fHist = ExpandedHistogram(";"+xax.var->LatexName()+";"+yax.var->LatexName(),
                              xax.nbins, xax.min, xax.max, xax.islog,
                              yax.nbins, yax.min, yax.max, yax.islog);

    for(unsigned int i = 0; i < profVars.size()+profSysts.size(); ++i){
      std::string title;
      if(i < profVars.size())
        title = profVars[i]->LatexName();
      else
        title = profSysts[i-profVars.size()]->LatexName();

      fProfHists.push_back(ExpandedHistogram(title+";"+xax.var->LatexName()+";"+yax.var->LatexName(),
                                             xax.nbins, xax.min, xax.max, xax.islog,
                                             yax.nbins, yax.min, yax.max, yax.islog));
    }

    fTimeHist = ExpandedHistogram("Fit time;"+xax.var->LatexName()+";"+yax.var->LatexName(),
                                  xax.nbins, xax.min, xax.max, xax.islog,
                                  yax.nbins, yax.min, yax.max, yax.islog);

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
    // Nothing created during surface filling belongs in a
    // directory. Unfortunately the local guards in Spectrum etc are racey when
    // run in parallel. But this should cover the whole lot safely.
    DontAddDirectory guard;

    const std::string progTitle = ProgressBarTitle(xvar, yvar, profVars, profSysts);

    Progress *prog = 0;
    // Difficult to keep a progress bar properly up to date when threaded
    if (!fParallel) prog = new Progress(progTitle);
    ThreadPool *pool = 0;

    if(fParallel){
      // A hack/workaround needed for parallel running:
      //
      // Give all the constituents of the Prediction a chance to do lazy
      // initialization, before they race themselves trying to do it in
      // parallel.
      expt->ChiSq(calc);

      pool = new ThreadPool;
      pool->ShowProgress(progTitle + TString::Format(" using %d threads", pool->NThreads()).Data());
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

    // Allow the surface to be parallelised across multiple jobs by splitting up
    // the full surface into patches, and only running bins that fall inside a
    // certain patch
    int first = 0, last = Nx * Ny;
    if (RunningOnGrid() && NumJobs()>1) {
      int n = NumJobs();
      int i = JobNumber();
      if (fMinOpt == kStandalone && !i) {
        first = 0;
        last = 0;
      } // if first standalone job
      else {
        if (fMinOpt == kStandalone) {
          n -= 1;
          i -= 1;
        }
        double stride = double(Nx*Ny) / double(n);
        first = i * stride;
        last = (i+1) * stride;
      } // if not first standalone job
    } // if running on grid

    do{
      if (neval < first) {
        ++neval;
        bin = (bin + step) % (Nx * Ny);
        continue;
      } else if (neval >= last) {
        break;
      }

      const int x = bin % Nx + 1;
      const int y = bin / Nx + 1;

      const double xv = BinCenterX(x);
      const double yv = BinCenterY(y);

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

      ThreadPool::func_t task = [=](){
        FillSurfacePoint(expt, calc,
                         xvar, xv, yvar, yv,
                         profVars, profSysts, seedPts, systSeedPts);
      };

      ++neval;
      if(fParallel){
        pool->AddTask(task);
      }
      else{
        task(); // Just do it straight away
        prog->SetProgress(double(neval-first) / double(last-first));
      }

      if (RunningOnGrid() && NumJobs()>1) fBinMask.push_back(bin);

      bin = (bin + step) % (Nx * Ny);
    } while (bin != 0);


    if(fParallel){
      pool->Finish();
      delete pool;
    }
    else{
      prog->Done();
      delete prog;
    }
  }

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
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    if (fMinOpt == kStandalone) {
      std::cerr << "Standalone global fit option not implemented in "
                << "FrequentistSurface. Exiting" << std::endl;
      exit(1);
    }

    auto start = high_resolution_clock::now();

    if(fParallel){
      // Need to take our own copy so that we don't get overwritten by someone
      // else's changes.
      calc = calc->Copy();
    }

    xvar->SetValue(calc, x);
    yvar->SetValue(calc, y);

    //Make sure that the profiled values of fitvars do not persist between steps.
    for(int i = 0; i < (int)fSeedValues.size(); ++i) profVars[i]->SetValue( calc, fSeedValues[i] );

    expt->Reset();

    double chi;
    if(profVars.empty() && profSysts.empty()){
      chi = expt->ChiSq(calc);
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

    if(fParallel) delete calc;

    auto end = high_resolution_clock::now();
    microseconds ms = duration_cast<microseconds>(end-start);
    fTimeHist->Fill(x, y, ms.count() * microseconds::period::num / microseconds::period::den);

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
    if (fMinOpt == kStandalone) {
      std::cerr << "Standalone global fit option not implemented in "
                << "FrequentistSurface. Exiting" << std::endl;
      exit(1);
    }

    // Location of the best minimum found from filled surface
    double minchi = 1e10;
    int minx = fHist->GetNbinsX()/2;
    int miny = fHist->GetNbinsY()/2;
    for(int x = 1; x <= fHist->GetNbinsX(); ++x){
      for(int y = 1; y <= fHist->GetNbinsY(); ++y){
        int bin = ((y-1)*fHist->GetNbinsX())+(x-1);
        const double chi = fHist->GetBinContent(x, y);
        if(RunningOnGrid() && NumJobs()>1
          && !std::count(fBinMask.begin(), fBinMask.end(), bin)) continue;
        if (chi < minchi){
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
    expt->Reset();
    // Seed from best grid point
    xvar->SetValue(calc, BinCenterX(minx));
    yvar->SetValue(calc, BinCenterY(miny));
    for(int i = 0; i < (int)fSeedValues.size(); ++i) profVars[i]->SetValue( calc, fSeedValues[i] );
    SystShifts systSeed = SystShifts::Nominal();

    if(fMinOpt==kSeeded) {
      fBestLikelihood = fit.Fit(calc, systSeed, seedPts)->EvalMetricVal();
      fBestFitX       = xvar->GetValue(calc);
      fBestFitY       = yvar->GetValue(calc);
      fBestFitVals[xvar] = fBestFitX;
      fBestFitVals[yvar] = fBestFitY;
      for (const IFitVar* v : profVars)
        fBestFitVals[v] = v->GetValue(calc);
      for (const ISyst* s : profSysts)
        fBestFitVals[s] = systSeed.GetShift(s);
      for(int x = 0; x < fHist->GetNbinsX()+2; ++x){
        for(int y = 0; y < fHist->GetNbinsY()+2; ++y){
          fHist->SetBinContent(x, y, fHist->GetBinContent(x, y)-fBestLikelihood);
        }
      }
    }
    else {
      fBestLikelihood = NAN;
      fBestFitX       = NAN;
      fBestFitY       = NAN;
      fBestFitVals.clear();
    }

    fHist->SetMinimum(0);
  }

  //----------------------------------------------------------------------
  double FrequentistSurface::BestFitValue(const IFittable* named)
  {
    if (fBestFitVals.count(named)) {
      return fBestFitVals[named];
    } else {
      std::cerr << "Error: no best fit value for " << named->ShortName() << std::endl;
      abort();
    }
  }

  //----------------------------------------------------------------------
  void FrequentistSurface::DrawProfHist(size_t idx) const
  {
    if (idx >= fProfHists.size()) {
      std::cerr << "Error: index " << idx << " exceeds size of profiled hist vector ("
                << fProfHists.size() << ")" << std::endl;
      abort();
    }
    // Could have a file temporarily open
    DontAddDirectory guard;

    EnsureAxes(fProfHists[idx]);
    fProfHists[idx]->Draw("colz same");
    gPad->RedrawAxis();
  }

  //----------------------------------------------------------------------
  TH2* FrequentistSurface::GetTimeHist()
  {
    if (!fTimeHist) assert(false && "Time histogram not set in surface!");
    return fTimeHist;
  }

  //----------------------------------------------------------------------
  double FrequentistSurface::Time()
  {
    if (!fTimeHist) assert(false && "Time histogram not set in surface!");
    return fTimeHist->Integral();
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

    dir->cd();
    if (!fBestFitVals.empty()) {
      TH1D hBestFit("", "", fBestFitVals.size(), 0, fBestFitVals.size());
      double centre = 0.5;
      for (auto const& [named, val] : fBestFitVals) {
        hBestFit.Fill(centre, val);
        hBestFit.GetXaxis()->SetBinLabel(std::ceil(centre), named->ShortName().c_str());
        centre += 1.;
      } // for named
      hBestFit.Write("bestfitvals");
    }

    if (fTimeHist) fTimeHist->Write("time");

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
    assert(tag->GetString() == "FrequentistSurface" || tag->GetString() == "Surface");
    delete tag;

    std::unique_ptr<FrequentistSurface> surf(new FrequentistSurface);
    ISurface::FillSurfObj(*surf, dir);

    for(std::size_t idx = 0; ; ++idx){
      TH2* h = (TH2*)dir->Get(TString::Format("profHists/hist%lu", idx));
      if(h) surf->fProfHists.push_back(h); else break;
    }

    TH1* hBestFit = (TH1*)dir->Get("bestfitvals");
    if (hBestFit) {
      for (size_t i = 1; i <= (size_t)hBestFit->GetNbinsX(); ++i) {
        std::string name = hBestFit->GetXaxis()->GetBinLabel(i);
        double val = hBestFit->GetBinContent(i);
        auto var = Registry<IFitVar>::ShortNameToPtr(name, true);
        if (var) {
          surf->fBestFitVals[var] = val;
          continue;
        }
        auto syst = Registry<ISyst>::ShortNameToPtr(name, true);
        if (syst) {
          surf->fBestFitVals[syst] = val;
          continue;
        }
        std::cerr << "warning: name \"" << name << "\" does not match any fit var or syst." << std::endl;
      } // for var
    } // if best fit values stored

    TH2* h = (TH2*)dir->Get("time");
    surf->fTimeHist = h ? h : nullptr; // allow failure, but set to nullptr

    delete dir;

    return surf;
  }

  //----------------------------------------------------------------------
  std::unique_ptr<FrequentistSurface> FrequentistSurface::LoadFromMulti(
    const std::vector<TFile*> files, const std::string& label,
    bool allowFail, int nSeparateSeedPoints)
  {
    std::vector<std::unique_ptr<FrequentistSurface>> surfs;
    for (TFile* f : files) {
      surfs.push_back(FrequentistSurface::LoadFrom(f, label));
    }

    if (surfs.empty()) {
      if (allowFail) return nullptr;
      else assert(false && "No input files provided");
    }

    int Nx = surfs[0]->fHist->GetNbinsX();
    int Ny = surfs[0]->fHist->GetNbinsY();
    size_t nbins = Nx * Ny;
    std::map<int,int> binMask;

    // Loop over the surfaces to calculate the number of patches per
    // bin. The number of patches per bin should be equal to the number
    // of separately seeded points.
    for (size_t surf = 0; surf < surfs.size(); ++surf) {
      for (int bin : surfs[surf]->fBinMask) {
        binMask[bin]++;
      }
    }
    // Check that all bins are accounted for for at least one seed point.
    bool bin_mask_to_file_mismatch = binMask.size() != nbins;
    // Now check that each bin was seeded at the same number of points.
    for (auto bin : binMask) {
      if (bin.second != nSeparateSeedPoints) {
        bin_mask_to_file_mismatch = true;
        std::cout << "Got " << bin.second << " patches for bin " << bin.first
          << " but expected " << nSeparateSeedPoints << ". How"
          << " many separate seed points are you combining?" << std::endl;
        break;
      }
    }
    // If either of these conditions are not met, we have a mismatch.
    if (bin_mask_to_file_mismatch) {
      if (allowFail) return nullptr;
      else {
        std::cout << "The number of patches found is inconsistent with the"
          << " surface you are trying to make. Is the file list"
          << " correct for this surface?"
          << std::endl;
        assert(false);
      }
    }

    DontAddDirectory guard;

    // Create return surface and initialise members with first in list.
    std::unique_ptr<FrequentistSurface> ret(new FrequentistSurface);

    ret->fLogX = surfs[0]->fLogX;
    ret->fLogY = surfs[0]->fLogY;
    // Potentially need the seed points from each individually seeded surface but don't
    // duplicate.
    // Also a good point to build the time histogram.
    TH2* time_hist = surfs[0]->fTimeHist; // could be null pointer.
    if (time_hist) {
      ret->fTimeHist = (TH2*)time_hist->Clone();
      ret->fTimeHist->Reset();
    }
    else ret->fTimeHist = time_hist;
    std::set<double> all_seed_values;
    for (unsigned int surf = 0; surf < surfs.size(); surf++) {
      if (ret->fTimeHist) ret->fTimeHist->Add(surfs[surf]->fTimeHist);
      for (auto seed : surfs[surf]->fSeedValues) all_seed_values.insert(seed);
    }
    ret->fSeedValues.assign(all_seed_values.begin(), all_seed_values.end());

    // If best fit was found independently of the patch files, fBestLikelihood will be NAN and
    // we want it to remain that way so that the info can be added later by the user. If it wasn't
    // found independently, the true best fit will be stored in one of the surfaces. Loop and find
    // it.
    //
    // In the same loop, for each bin update to the "global minimum" (minimum over the seed points)
    // for that bin and set the pulls in the corresponding bins in the profile plots.
    //
    // First, setup the return fHist and fProfHists.
    ret->fHist = (TH2F*)surfs[0]->ToTH2(0)->Clone();
    // Reset the histogram, set each bin to a big number for easy bookkeeping.
    ret->fHist->Reset();
    for(int x = 0; x < ret->fHist->GetNbinsX()+2; ++x){
      for(int y = 0; y < ret->fHist->GetNbinsY()+2; ++y){
        ret->fHist->SetBinContent(x, y, std::numeric_limits<double>::max());
      }
    }
    for (TH2* h : surfs[0]->fProfHists) {
      ret->fProfHists.push_back((TH2F*)h->Clone());
      ret->fProfHists.back()->Reset();
    }
    ret->fBestLikelihood = NAN;
    ret->fBestFitX = NAN;
    ret->fBestFitY = NAN;
    ret->fBestFitVals.clear();

    for (size_t surf = 0; surf < surfs.size(); ++surf) {
      TH2F* hist = (TH2F*)surfs[surf]->ToTH2(0);
      double this_best_likelihood = surfs[surf]->fBestLikelihood;
      if (!std::isnan(this_best_likelihood) &&
          (std::isnan(ret->fBestLikelihood) || this_best_likelihood < ret->fBestLikelihood)) {
        ret->fBestLikelihood = this_best_likelihood;
        ret->fBestFitX = surfs[surf]->fBestFitX;
        ret->fBestFitY = surfs[surf]->fBestFitY;
        ret->fBestFitVals = surfs[surf]->fBestFitVals;
      }
      for (size_t bin : surfs[surf]->fBinMask) {
        const int x = bin % Nx + 1;
        const int y = bin / Nx + 1;
        double val = hist->GetBinContent(x, y);
        if (val < ret->fHist->GetBinContent(x, y)) {
          ret->fHist->SetBinContent(x, y, val);
          for (size_t phist = 0; phist < ret->fProfHists.size(); ++phist) {
            ret->fProfHists[phist]->SetBinContent(x, y, surfs[surf]->fProfHists[phist]->GetBinContent(x, y));
          }
        }
      }
    }

    // Scale hist by global minimum.
    // If the best fit was found independently of the patch files, fBestLikelihood has not been
    // added to the bins of fHist when ToTH2 was called in this function, therefore, we don't need
    // to subtract it.
    if (!std::isnan(ret->fBestLikelihood)) {
      for(int x = 0; x < ret->fHist->GetNbinsX()+2; ++x){
        for(int y = 0; y < ret->fHist->GetNbinsY()+2; ++y){
          ret->fHist->SetBinContent(x, y, ret->fHist->GetBinContent(x, y)-ret->fBestLikelihood);
        }
      }
    }

    ret->fHist->SetMinimum(0);

    return ret;
  }

  //----------------------------------------------------------------------
  std::unique_ptr<FrequentistSurface> FrequentistSurface::LoadFromMulti(
    const std::vector<std::string>& files, const std::string& label,
    bool allowFail, int nSeparateSeedPoints)
  {
    std::vector<TFile*> tfiles;
    std::vector<std::unique_ptr<TFile>> owned;
    for (const std::string& fname : files) {
      owned.push_back(ROOTFile(fname));
      tfiles.push_back(owned.back().get());
    }

    return LoadFromMulti(tfiles, label, allowFail, nSeparateSeedPoints);
  }

  //----------------------------------------------------------------------
  std::unique_ptr<FrequentistSurface> FrequentistSurface::LoadFromMulti(
    const std::string& wildcard, const std::string& label,
    bool allowFail, int nSeparateSeedPoints)
  {
    return LoadFromMulti(Wildcard(wildcard), label, allowFail,
                         nSeparateSeedPoints);
  }

  //----------------------------------------------------------------------
  void FrequentistSurface::SetBestFitInformation(double likelihood,
                                                 double bestFitX,
                                                 double bestFitY,
                                                 std::map<const IFittable*, double> bestFitVals)
  {
    // If best fit information is already set, loop over the surface and make it an absolute surface
    // before subtracting the new best likelihood to make it a relative surface again. Warn user this
    // is what is being done.
    double shift = -likelihood;

    if (!std::isnan(fBestLikelihood)) {
      std::cout << "\n\n\n WARNING! The function SetBestFitInformation is being called but";
      std::cout << " the best fit information was already set in the surface.\n";
      std::cout << " fBestLikelihood, fBestFitX and fBestFitY will be set to the values";
      std::cout << " that have been passed and the resulting surface will be relative to that likelihood." << std::endl;
      shift += fBestLikelihood;
    }

    for(int x = 0; x < fHist->GetNbinsX()+2; ++x){
      for(int y = 0; y < fHist->GetNbinsY()+2; ++y){
        fHist->SetBinContent(x, y, fHist->GetBinContent(x, y)+shift);
      }
    }
    fBestLikelihood = likelihood;
    fBestFitX = bestFitX;
    fBestFitY = bestFitY;
    fBestFitVals = bestFitVals;

    fHist->SetMinimum(0);
  }

  // See eg the statistics section of the PDG
  TH2* Gaussian68Percent2D(const FrequentistSurface& s){return Flat(2.30, s);}
  TH2* Gaussian90Percent2D(const FrequentistSurface& s){return Flat(4.61, s);}
  TH2* Gaussian95Percent2D(const FrequentistSurface& s){return Flat(5.99, s);}
  TH2* Gaussian2Sigma2D   (const FrequentistSurface& s){return Flat(6.18, s);}
  TH2* Gaussian99Percent2D(const FrequentistSurface& s){return Flat(9.21, s);}
  TH2* Gaussian3Sigma2D   (const FrequentistSurface& s){return Flat(11.83, s);}
  TH2* Gaussian4Sigma2D   (const FrequentistSurface& s){return Flat(19.33, s);}
  TH2* Gaussian5Sigma2D   (const FrequentistSurface& s){return Flat(28.74, s);}

  TH2* Gaussian68Percent1D(const FrequentistSurface& s){return Flat(1.00, s);}
  TH2* Gaussian90Percent1D(const FrequentistSurface& s){return Flat(2.71, s);}
  TH2* Gaussian95Percent1D(const FrequentistSurface& s){return Flat(3.84, s);}
  TH2* Gaussian2Sigma1D   (const FrequentistSurface& s){return Flat(4.00, s);}
  TH2* Gaussian99Percent1D(const FrequentistSurface& s){return Flat(6.63, s);}
  TH2* Gaussian3Sigma1D   (const FrequentistSurface& s){return Flat(9.00, s);}
  TH2* Gaussian4Sigma1D   (const FrequentistSurface& s){return Flat(16.00, s);}
  TH2* Gaussian5Sigma1D   (const FrequentistSurface& s){return Flat(25.00, s);}


} // namespace
