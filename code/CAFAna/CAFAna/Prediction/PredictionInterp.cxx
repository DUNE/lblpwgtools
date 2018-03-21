#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Utilities.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TObjString.h"
#include "TVectorD.h"

// For debug plots
#include "TGraph.h"
#include "TCanvas.h"

#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"

#include "CAFAna/Core/Loaders.h"

namespace ana
{
  //----------------------------------------------------------------------
  //DUNE constructor
  /*  PredictionInterp::PredictionInterp(std::vector<const ISyst*> systs,
                                     osc::IOscCalculator* osc,
                                     const IPredictionGenerator& predGen,
                                     DUNERunPOTSpectrumLoader& loaderBeam,
				     DUNERunPOTSpectrumLoader& loaderNue,
				     DUNERunPOTSpectrumLoader& loaderNuTau,
				     DUNERunPOTSpectrumLoader& loaderNC,
                                     const SystShifts& shiftMC)
    : fOscOrigin(osc->Copy()),
      fBinning(0, {}, {}, 0, 0)
  {
    for(const ISyst* syst: systs){
      ShiftedPreds sp;
      sp.systName = syst->ShortName();

      for(int x = -syst->PredInterpMaxNSigma(); x <= +syst->PredInterpMaxNSigma(); ++x){
        sp.shifts.push_back(x);
      }

      for(int sigma: sp.shifts){
        SystShifts shiftHere = shiftMC;
        shiftHere.SetShift(syst, sigma);
        sp.preds.push_back(predGen.Generate(loaderBeam, loaderNue, loaderNuTau, loaderNC, shiftHere).release());
      }

      fPreds.emplace(syst, sp);
    } // end for syst

    fPredNom = predGen.Generate(loaderBeam, loaderNue, loaderNuTau, loaderNC, shiftMC);

    // We need to know when all our predictions are filled so that we can
    // compute the fits
    loaders.RegisterCompletionCallback(this, &PredictionInterp::LoadedCallback);
    }*/

  //----------------------------------------------------------------------
  PredictionInterp::PredictionInterp(std::vector<const ISyst*> systs,
                                     osc::IOscCalculator* osc,
                                     const IPredictionGenerator& predGen,
                                     Loaders& loaders,
                                     const SystShifts& shiftMC)
    : fOscOrigin(osc ? osc->Copy() : 0),
      fBinning(0, {}, {}, 0, 0)
  {
    for(const ISyst* syst: systs){
      ShiftedPreds sp;
      sp.systName = syst->ShortName();

      for(int x = -syst->PredInterpMaxNSigma(); x <= +syst->PredInterpMaxNSigma(); ++x){
        sp.shifts.push_back(x);
      }

      for(int sigma: sp.shifts){
        SystShifts shiftHere = shiftMC;
        shiftHere.SetShift(syst, sigma);
        sp.preds.push_back(predGen.Generate(loaders, shiftHere).release());
      }

      fPreds.emplace(syst, sp);
    } // end for syst

    fPredNom = predGen.Generate(loaders, shiftMC);
  }

  //----------------------------------------------------------------------
  PredictionInterp::~PredictionInterp()
  {
    //    for(auto it: fPreds) for(IPrediction* p: it.second.preds) delete p;
    delete fOscOrigin;

    // It isn't really a unique ptr when we use PredictionInterpTemplates
    fPredNom.release();
  }

  //----------------------------------------------------------------------
  std::vector<std::vector<PredictionInterp::Coeffs>> PredictionInterp::
  FitRatios(const std::vector<double>& shifts,
            const std::vector<std::unique_ptr<TH1>>& ratios) const
  {
    assert(shifts.size() == ratios.size());

    std::vector<std::vector<Coeffs>> ret;

    const int binMax = ratios[0]->GetNbinsX();

    // Linear interpolation
    if(ratios.size() == 2){
      for(int binIdx = 0; binIdx < binMax+2; ++binIdx){
	ret.push_back({});
	const double y1 = ratios[0]->GetBinContent(binIdx);
        const double y2 = ratios[1]->GetBinContent(binIdx);
	ret.back().emplace_back(0, 0, y2-y1, y1);
      }
      return ret;
    }

    for(int binIdx = 0; binIdx < binMax+2; ++binIdx){
      ret.push_back({});

      // This is cubic interpolation. For each adjacent set of four points we
      // determine coefficients for a cubic which will be the curve between the
      // center two. We constrain the function to match the two center points
      // and to have the right mean gradient at them. This causes this patch to
      // match smoothly with the next one along. The resulting function is
      // continuous and first and second differentiable. At the ends of the
      // range we fit a quadratic instead with only one constraint on the
      // slope. The coordinate conventions are that point y1 sits at x=0 and y2
      // at x=1. The matrices are simply the inverses of writing out the
      // constraints expressed above.

      {
        const double y1 = ratios[0]->GetBinContent(binIdx);
        const double y2 = ratios[1]->GetBinContent(binIdx);
        const double y3 = ratios[2]->GetBinContent(binIdx);
        const double v[3] = {y1, y2, (y3-y1)/2};
        const double m[9] = { 1, -1,  1,
                             -2,  2, -1,
                              1,  0,  0};
        const TVectorD res = TMatrixD(3, 3, m) * TVectorD(3, v);
        ret.back().emplace_back(0, res(0), res(1), res(2));
      }

      // We're assuming here that the shifts are separated by exactly 1 sigma.
      for(unsigned int shiftIdx = 1; shiftIdx < ratios.size()-2; ++shiftIdx){
        const double y0 = ratios[shiftIdx-1]->GetBinContent(binIdx);
        const double y1 = ratios[shiftIdx  ]->GetBinContent(binIdx);
        const double y2 = ratios[shiftIdx+1]->GetBinContent(binIdx);
        const double y3 = ratios[shiftIdx+2]->GetBinContent(binIdx);

        const double v[4] = {y1, y2, (y2-y0)/2, (y3-y1)/2};
        const double m[16] = { 2, -2,  1,  1,
                              -3,  3, -2, -1,
                               0,  0,  1,  0,
                               1,  0,  0,  0};
        const TVectorD res = TMatrixD(4, 4, m) * TVectorD(4, v);
        ret.back().emplace_back(res(0), res(1), res(2), res(3));
      } // end for shiftIdx

      {
        const int N = ratios.size()-3;
        const double y0 = ratios[N  ]->GetBinContent(binIdx);
        const double y1 = ratios[N+1]->GetBinContent(binIdx);
        const double y2 = ratios[N+2]->GetBinContent(binIdx);
        const double v[3] = {y1, y2, (y2-y0)/2};
        const double m[9] = {-1,  1, -1,
                              0,  0,  1,
                              1,  0,  0};
        const TVectorD res = TMatrixD(3, 3, m) * TVectorD(3, v);
        ret.back().emplace_back(0, res(0), res(1), res(2));
      }
    } // end for binIdx

    return ret;
  }

  //----------------------------------------------------------------------
  std::vector<std::vector<PredictionInterp::Coeffs>> PredictionInterp::
  FitComponent(const std::vector<double>& shifts,
               const std::vector<IPrediction*>& preds,
               Flavors::Flavors_t flav,
               Current::Current_t curr,
               Sign::Sign_t sign) const
  {
    IPrediction* pNom = 0;
    for(unsigned int i = 0; i < shifts.size(); ++i){
      if(shifts[i] == 0) pNom = preds[i];
    }
    assert(pNom);

    // Do it this way rather than via fPredNom so that systematics evaluated
    // relative to some alternate nominal (eg Birks C where the appropriate
    // nominal is no-rock) can work.
    const Spectrum nom = pNom->PredictComponent(fOscOrigin,
						flav, curr, sign);

    std::vector<std::unique_ptr<TH1>> ratios;
    for(auto& p: preds){
      ratios.emplace_back(Ratio(p->PredictComponent(fOscOrigin,
                                                    flav, curr, sign),
                                nom).ToTH1());

      // Check none of the ratio values is utterly crazy
      std::unique_ptr<TH1>& r = ratios.back();
      for(int i = 0; i < r->GetNbinsX()+2; ++i){
	const double y = r->GetBinContent(i);
	if(y > 500){
	  std::cout << "PredictionInterp: WARNING, ratio in bin "
		    << i << " is " << y << ". Ignoring." << std::endl;
	  r->SetBinContent(i, 1);
	}
      }
    }

    return FitRatios(shifts, ratios);
  }

  //----------------------------------------------------------------------
  void PredictionInterp::InitFits() const
  {
    if(fPreds.empty() ||
       !fPreds.begin()->second.fits[0].empty()) return; // Already initialized

    for(auto& it: fPreds){
      ShiftedPreds& sp = it.second;

      sp.fits[kNueApp]   = FitComponent(sp.shifts, sp.preds, Flavors::kNuMuToNuE,  Current::kCC, Sign::kBoth);
      sp.fits[kNueSurv]  = FitComponent(sp.shifts, sp.preds, Flavors::kNuEToNuE,   Current::kCC, Sign::kBoth);
      sp.fits[kNumuSurv] = FitComponent(sp.shifts, sp.preds, Flavors::kNuMuToNuMu, Current::kCC, Sign::kBoth);
      sp.fits[kNC] = FitComponent(sp.shifts, sp.preds, Flavors::kAll, Current::kNC, Sign::kBoth);

      sp.fits[kOther] = FitComponent(sp.shifts, sp.preds, Flavors::kNuEToNuMu | Flavors::kAllNuTau, Current::kCC, Sign::kBoth);
    }

    // Predict something, anything, so that we can know what binning to use
    fBinning = fPredNom->Predict(fOscOrigin);
    fBinning.Clear();
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::Predict(osc::IOscCalculator* calc) const
  {
    return fPredNom->Predict(calc);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictComponent(osc::IOscCalculator* calc,
                                              Flavors::Flavors_t flav,
                                              Current::Current_t curr,
                                              Sign::Sign_t sign) const
  {
    return fPredNom->PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictSyst(osc::IOscCalculator* calc,
                                         const SystShifts& shift) const
  {
    InitFits();
    return PredictComponentSyst(calc, shift,
                                Flavors::kAll,
                                Current::kBoth,
                                Sign::kBoth);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::
  ShiftSpectrum(const Spectrum& s,
                CoeffsType type,
                const SystShifts& shift) const
  {
    // TODO histogram operations could be too slow
    TH1D* h = s.ToTH1(s.POT());

    const int N = h->GetNbinsX()+2;
    double corr[N];
    for(int i = 0; i < N; ++i) corr[i] = 1;

    for(const ISyst* syst: shift.ActiveSysts()){
      auto it = fPreds.find(syst);
      assert(it != fPreds.end());
      const ShiftedPreds& sp = it->second;

      double x = shift.GetShift(syst);

      int shiftBin = x - sp.shifts[0];
      if(shiftBin < 0) shiftBin = 0;
      if(shiftBin >= int(sp.fits[0][0].size())) shiftBin = sp.fits[0][0].size()-1;

      x -= sp.shifts[shiftBin];

      for(int n = 0; n < N; ++n){
        const Coeffs f = sp.fits[type][n][shiftBin];
        corr[n] *= f.a*util::cube(x) + f.b*util::sqr(x) + f.c*x + f.d;
      } // end for n
    } // end for syst

    for(int n = 0; n < N; ++n){
      h->SetBinContent(n, std::max(h->GetBinContent(n)*corr[n], 0.));
    }

    return Spectrum(std::unique_ptr<TH1D>(h), s.GetLabels(), s.GetBinnings(), s.POT(), s.Livetime());
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::
  ShiftedComponent(osc::IOscCalculator* calc,
                   const TMD5* hash,
                   const SystShifts& shift,
                   Flavors::Flavors_t flav,
                   Current::Current_t curr,
                   Sign::Sign_t sign,
                   CoeffsType type) const
  {
    // Must be the base case of the recursion to use the cache. Otherwise we
    // can cache systematically shifted versions of our children, which is
    // wrong. Also, some calculators won't hash themselves.
    const bool canCache = (hash != 0);

    const Key_t key = {flav, curr, sign};
    auto it = fNomCache.find(key);

    // We have the nominal for this exact combination of flav, curr, sign, calc
    // stored.  Shift it and return.
    if(canCache && it != fNomCache.end() && it->second.hash == *hash){
      return ShiftSpectrum(it->second.nom, type, shift);
    }

    // We need to compute the nominal again for whatever reason
    const Spectrum nom = fPredNom->PredictComponent(calc, flav, curr, sign);

    if(canCache){
      // Insert into the cache if not already there, or update if there but
      // with old oscillation parameters.
      if(it == fNomCache.end())
        fNomCache.emplace(key, Val_t({*hash, nom}));
      else
        it->second = {*hash, nom};
    }

    Spectrum temp = ShiftSpectrum(nom, type, shift);

    return ShiftSpectrum(nom, type, shift);
  }

  //----------------------------------------------------------------------
  Spectrum PredictionInterp::PredictComponentSyst(osc::IOscCalculator* calc,
                                                  const SystShifts& shift,
                                                  Flavors::Flavors_t flav,
                                                  Current::Current_t curr,
                                                  Sign::Sign_t sign) const
  {
    InitFits();

    //    Spectrum& ret = fBinning;
    Spectrum ret = fPredNom->Predict(fOscOrigin);
    ret.Clear();

    if(ret.POT()==0) ret.OverridePOT(1e24);

    // Check that we're able to handle all the systs we were passed
    for(const ISyst* syst: shift.ActiveSysts()){
      if(fPreds.find(syst) == fPreds.end()){
        // If we don't find it we may recently have been loaded from a file.
        bool found = false;
        for(auto it: fPreds){
          // Check all the systs we know about to see if the name matches
          if(it.second.systName == syst->ShortName()){
            // And if so rewrite fPreds to use the newly-discovered pointer
            fPreds.emplace(syst, it.second);
            fPreds.erase(fPreds.find(it.first));
            found = true;
            break;
          }
        } // end for it
        if(!found){
          // We couldn't find this syst even after searching by name
          std::cerr << "This PredictionInterp is not set up to handle the requested systematic: " << syst->ShortName() << std::endl;
          abort();
        }
      }
    } // end for syst

    const TMD5* hash = calc ? calc->GetParamsHash() : 0;


    if(curr & Current::kCC){
      if(flav & Flavors::kNuEToNuE)    ret += ShiftedComponent(calc, hash, shift, Flavors::kNuEToNuE,    Current::kCC, sign, kNueSurv);
      if(flav & Flavors::kNuEToNuMu)   ret += ShiftedComponent(calc, hash, shift, Flavors::kNuEToNuMu,   Current::kCC, sign, kOther  );
      if(flav & Flavors::kNuEToNuTau)  ret += ShiftedComponent(calc, hash, shift, Flavors::kNuEToNuTau,  Current::kCC, sign, kOther  );
      if(flav & Flavors::kNuMuToNuE)   ret += ShiftedComponent(calc, hash, shift, Flavors::kNuMuToNuE,   Current::kCC, sign, kNueApp  );
      if(flav & Flavors::kNuMuToNuMu)  ret += ShiftedComponent(calc, hash, shift, Flavors::kNuMuToNuMu,  Current::kCC, sign, kNumuSurv);
      if(flav & Flavors::kNuMuToNuTau) ret += ShiftedComponent(calc, hash, shift, Flavors::kNuMuToNuTau, Current::kCC, sign, kOther   );
    }
    if(curr & Current::kNC){
      assert(flav == Flavors::kAll); // Don't know how to calculate anything else
      assert(sign == Sign::kBoth);   // Why would you want to split NCs out by sign?
      
      ret += ShiftedComponent(calc, hash, shift, Flavors::kAll, Current::kNC, Sign::kBoth, kNC);
    }

    delete hash;

    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionInterp::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;

    dir->cd();
    TObjString("PredictionInterp").Write("type");


    fPredNom->SaveTo(dir->mkdir("pred_nom"));


    for(auto it: fPreds){
      const ShiftedPreds& sp = it.second;

      for(unsigned int i = 0; i < sp.shifts.size(); ++i){
        sp.preds[i]->SaveTo(dir->mkdir(TString::Format("pred_%s_%+d",
                                                       sp.systName.c_str(),
                                                       int(sp.shifts[i])).Data()));
      } // end for i
    } // end for it

    if(fOscOrigin) ana::SaveTo(*fOscOrigin, dir->mkdir("osc_origin"));

    if(!fPreds.empty()){
      TH1F hSystNames("syst_names", ";Syst names", fPreds.size(), 0, fPreds.size());
      int binIdx = 1;
      for(auto it: fPreds){
        hSystNames.GetXaxis()->SetBinLabel(binIdx++, it.second.systName.c_str());
      }
      hSystNames.Write("syst_names");
    }


    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionInterp> PredictionInterp::LoadFrom(TDirectory* dir)
  {
    TObjString* tag = (TObjString*)dir->Get("type");
    assert(tag);
    assert(tag->GetString() == "PredictionInterp" ||
           tag->GetString() == "PredictionInterp2"); // backwards compatibility

    std::unique_ptr<PredictionInterp> ret(new PredictionInterp);

    LoadFromBody(dir, ret.get());

    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionInterp::LoadFromBody(TDirectory* dir, PredictionInterp* ret,
				      std::vector<const ISyst*> veto)
  {
    ret->fPredNom = ana::LoadFrom<IPrediction>(dir->GetDirectory("pred_nom"));

    TH1* hSystNames = (TH1*)dir->Get("syst_names");
    if(hSystNames){
      for(int systIdx = 0; systIdx < hSystNames->GetNbinsX(); ++systIdx){
        ShiftedPreds sp;
        sp.systName = hSystNames->GetXaxis()->GetBinLabel(systIdx+1);

	bool tocont = false;
	for(const ISyst* v: veto){
	  if(sp.systName == v->ShortName()) tocont = true;
	}
	if(tocont) continue;

        for(int shift = -3; shift <= +3; ++shift){
          TDirectory* preddir = dir->GetDirectory(TString::Format("pred_%s_%+d", sp.systName.c_str(), shift).Data());
          if(!preddir) continue; // Can happen for genie systs

          IPrediction* pred = ana::LoadFrom<IPrediction>(preddir).release();

          sp.shifts.push_back(shift);
          sp.preds.push_back(pred);
        } // end for shift

        // We can't figure out the real syst this name refers to just
        // yet. Store under a dummy syst that won't match any existing pointer.
        class DummySyst: public ISyst
        {
          virtual std::string ShortName() const {return "";}
          virtual std::string LatexName() const {return "";}
          virtual void Shift(double sigma, Restorer& restore,
                             caf::StandardRecord* sr, double& weight) const {}
        };

        ret->fPreds.emplace(new DummySyst, sp);
      } // end for systIdx
    } // end if hSystNames

    TDirectory* oscDir = dir->GetDirectory("osc_origin");
    if(oscDir) ret->fOscOrigin = oscDir ? ana::LoadFrom<osc::IOscCalculator>(oscDir).release() : 0;

    // Recalculate sp.fits and fBinning from information in the file
    ret->InitFits();
  }

  //----------------------------------------------------------------------
  void PredictionInterp::DebugPlots(osc::IOscCalculator* calc,
				    const std::string& savePattern,
				    Flavors::Flavors_t flav,
				    Current::Current_t curr,
				    Sign::Sign_t sign) const
  {
    InitFits();

    std::unique_ptr<TH1> nom(fPredNom->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));
    const int nbins = nom->GetNbinsX();

    TGraph* curves[nbins];
    TGraph* points[nbins];

    for(auto it: fPreds){
      for(int i = 0; i <= 80; ++i){
        const double x = .1*i-4;
        const SystShifts ss(it.first, x);
        std::unique_ptr<TH1> h(PredictComponentSyst(calc, ss, flav, curr, sign).ToTH1(18e20));

        for(int bin = 0; bin < nbins; ++bin){
          if(i == 0){
            curves[bin] = new TGraph;
            points[bin] = new TGraph;
          }

          const double ratio = h->GetBinContent(bin+1)/nom->GetBinContent(bin+1);

          if(!isnan(ratio) && !isinf(ratio))
            curves[bin]->SetPoint(curves[bin]->GetN(), x, ratio);
        } // end for bin
      } // end for i (x)

      // As elswhere, to allow BirksC etc that need a different nominal to plot
      // right.
      IPrediction* pNom = 0;
      for(unsigned int shiftIdx = 0; shiftIdx < it.second.shifts.size(); ++shiftIdx){
	if(it.second.shifts[shiftIdx] == 0) pNom = it.second.preds[shiftIdx];
      }
      assert(pNom);
      std::unique_ptr<TH1> hnom(pNom->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));

      for(unsigned int shiftIdx = 0; shiftIdx < it.second.shifts.size(); ++shiftIdx){
	std::unique_ptr<TH1> h(it.second.preds[shiftIdx]->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));

        for(int bin = 0; bin < nbins; ++bin){
	  const double ratio = h->GetBinContent(bin+1)/hnom->GetBinContent(bin+1);
	  points[bin]->SetPoint(points[bin]->GetN(), it.second.shifts[shiftIdx], ratio);
	}
      } // end for shiftIdx


      int nx = int(sqrt(nbins));
      int ny = int(sqrt(nbins));
      if(nx*ny < nbins) ++nx;
      if(nx*ny < nbins) ++ny;

      TCanvas* c = new TCanvas;
      c->Divide(nx, ny);

      for(int bin = 0; bin < nbins; ++bin){
        c->cd(bin+1);
        (new TH2F("",
                  TString::Format("%s %g < %s < %g;Shift;Ratio",
                                  it.second.systName.c_str(),
                                  nom->GetXaxis()->GetBinLowEdge(bin+1),
                                  nom->GetXaxis()->GetTitle(),
                                  nom->GetXaxis()->GetBinUpEdge(bin+1)),
                  100, -4, +4, 100, .5, 1.5))->Draw();
        if(curves[bin]->GetN() > 0) curves[bin]->Draw("l same");
        points[bin]->SetMarkerStyle(kFullCircle);
        if(points[bin]->GetN() > 0) points[bin]->Draw("p same");
      } // end for bin

      c->cd(0);

      if(!savePattern.empty()){
	assert(savePattern.find("%s") != 0);
	gPad->Print(TString::Format(savePattern.c_str(), it.second.systName.c_str()).Data());
      }
    } // end for it
  }

  //----------------------------------------------------------------------
  void PredictionInterp::DebugPlotsColz(osc::IOscCalculator* calc,
                                        const std::string& savePattern,
                                        Flavors::Flavors_t flav,
                                        Current::Current_t curr,
                                        Sign::Sign_t sign) const
  {
    InitFits();

    std::unique_ptr<TH1> nom(fPredNom->PredictComponent(calc, flav, curr, sign).ToTH1(18e20));
    const int nbins = nom->GetNbinsX();

    for(auto it: fPreds){
      new TCanvas;
      TH2* h2 = new TH2F("", ";;#sigma",
                         nbins, nom->GetXaxis()->GetXmin(), nom->GetXaxis()->GetXmax(),
                         80, -4, +4);
      h2->GetXaxis()->SetTitle(nom->GetXaxis()->GetTitle());

      for(int i = 1; i <= 80; ++i){
        const double y = h2->GetYaxis()->GetBinCenter(i);
        const SystShifts ss(it.first, y);
        std::unique_ptr<TH1> h(PredictComponentSyst(calc, ss, flav, curr, sign).ToTH1(18e20));

        for(int bin = 0; bin < nbins; ++bin){
          const double ratio = h->GetBinContent(bin+1)/nom->GetBinContent(bin+1);

          if(!isnan(ratio) && !isinf(ratio))
            h2->Fill(h2->GetXaxis()->GetBinCenter(bin), y, ratio);
        } // end for bin
      } // end for i (x)

      h2->Draw("colz");
      h2->SetMinimum(0.5);
      h2->SetMaximum(1.5);

      if(!savePattern.empty()){
	assert(savePattern.find("%s") != 0);
	gPad->Print(TString::Format(savePattern.c_str(), it.second.systName.c_str()).Data());
      }
    } // end for it
  }

} // namespace
