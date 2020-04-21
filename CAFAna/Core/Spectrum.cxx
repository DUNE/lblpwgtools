#include "CAFAna/Core/Spectrum.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Utilities.h"

#include "Utilities/func/MathUtil.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TObjString.h"
#include "TRandom3.h"

#include <cassert>
#include <iostream>

namespace ana {
//----------------------------------------------------------------------
// The one constructor to rule them all. (Almost) everyone forwards here
Spectrum::Spectrum(const std::vector<std::string> &labels,
                   const std::vector<Binning> &bins, ESparse sparse)
    : fHist(Hist::Uninitialized()), fPOT(0), fLivetime(0), fLabels(labels),
      fBins(bins) {
  ConstructHistogram(sparse);
}

//----------------------------------------------------------------------
// Or here...
Spectrum::Spectrum(const std::string &label, const Binning &bins,
                   ESparse sparse)
    : fHist(Hist::Uninitialized()), fPOT(0), fLivetime(0), fLabels(1, label),
      fBins(1, bins) {
  ConstructHistogram(sparse);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &label, const Binning &bins,
                   SpectrumLoaderBase &loader, const Var &var, const Cut &cut,
                   const SystShifts &shift, const Var &wei)
    : Spectrum(label, bins) {
  loader.AddSpectrum(*this, var, cut, shift, wei);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &label, const Binning &bins,
                   SpectrumLoaderBase &loader, const MultiVar &var,
                   const Cut &cut, const SystShifts &shift, const Var &wei)
    : Spectrum(label, bins) {
  loader.AddSpectrum(*this, var, cut, shift, wei);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(SpectrumLoaderBase &loader, const HistAxis &axis,
                   const Cut &cut, const SystShifts &shift, const Var &wei)
    : Spectrum(axis.GetLabels(), axis.GetBinnings()) {
  loader.AddSpectrum(*this, axis.GetMultiDVar(), cut, shift, wei);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &label, double pot, double livetime,
                   const Binning &bins)
    : Spectrum(label, bins) {
  fPOT = pot;
  fLivetime = livetime;
}

//----------------------------------------------------------------------
Spectrum::Spectrum(TH1 const *h, const std::vector<std::string> &labels,
                   const std::vector<Binning> &bins, double pot,
                   double livetime)
    : fHist(Hist::Uninitialized()), fPOT(pot), fLivetime(livetime),
      fLabels(labels), fBins(bins) {
  if (!h) {
    return;
  }

  DontAddDirectory guard;

  const TString className = h->ClassName();

  if (className == "TH1D") {
    // Shortcut if types match
    fHist = Hist::Copy((TH1D const *)h, Binning::FromTAxis(h->GetXaxis()));
  } else {
    fHist = Hist::Copy(h, Binning::FromTAxis(h->GetXaxis()));
  }
}

//----------------------------------------------------------------------
Spectrum::Spectrum(std::unique_ptr<TH1D> h,
                   const std::vector<std::string> &labels,
                   const std::vector<Binning> &bins, double pot,
                   double livetime)
    : fHist(Hist::Adopt(std::move(h), Binning::FromTAxis(h->GetXaxis()))),
      fPOT(pot), fLivetime(livetime), fLabels(labels),
      fBins(bins) // TODO is ordering here safe with the move?
{}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &label, SpectrumLoaderBase &loader,
                   const Binning &binsx, const Var &varx, const Binning &binsy,
                   const Var &vary, const Cut &cut, const SystShifts &shift,
                   const Var &wei)
    : Spectrum(label, "", loader, binsx, varx, binsy, vary, cut, shift, wei) {
  // TODO do we want this variant when there's one with a labelY just below?
}

//----------------------------------------------------------------------
Spectrum::Spectrum(SpectrumLoaderBase &loader, const HistAxis &xAxis,
                   const HistAxis &yAxis, const Cut &cut,
                   const SystShifts &shift, const Var &wei)
    : Spectrum(xAxis.GetLabels()[0], loader, xAxis.GetBinnings()[0],
               xAxis.GetVars()[0], yAxis.GetBinnings()[0], yAxis.GetVars()[0],
               cut, shift, wei) {
  // TODO - do we want to keep this variant around?
  assert(xAxis.NDimensions() == 1);
  assert(yAxis.NDimensions() == 1);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &xLabel, const std::string &yLabel,
                   SpectrumLoaderBase &loader, const Binning &binsx,
                   const Var &varx, const Binning &binsy, const Var &vary,
                   const Cut &cut, const SystShifts &shift, const Var &wei)
    : Spectrum({xLabel, yLabel}, {binsx, binsy}) {
  Var multiDVar = Var2D(varx, binsx, vary, binsy);

  loader.AddSpectrum(*this, multiDVar, cut, shift, wei);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &label, SpectrumLoaderBase &loader,
                   const Binning &binsx, const Var &varx, const Binning &binsy,
                   const Var &vary, const Binning &binsz, const Var &varz,
                   const Cut &cut, const SystShifts &shift, const Var &wei,
                   ESparse sparse)
    : Spectrum(label, "", "", loader, binsx, varx, binsy, vary, binsz, varz,
               cut, shift, wei, sparse) {
  // TODO do we want this variant when there's one with a labelY and labelZ
  // just below?
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const std::string &xLabel, const std::string &yLabel,
                   const std::string &zLabel, SpectrumLoaderBase &loader,
                   const Binning &binsx, const Var &varx, const Binning &binsy,
                   const Var &vary, const Binning &binsz, const Var &varz,
                   const Cut &cut, const SystShifts &shift, const Var &wei,
                   ESparse sparse)
    : Spectrum({xLabel, yLabel, zLabel}, {binsx, binsy, binsz}, sparse) {
  Var multiDVar = Var3D(varx, binsx, vary, binsy, varz, binsz);

  loader.AddSpectrum(*this, multiDVar, cut, shift, wei);
}

//----------------------------------------------------------------------
Spectrum::Spectrum(SpectrumLoaderBase &loader, const HistAxis &xAxis,
                   const HistAxis &yAxis, const HistAxis &zAxis, const Cut &cut,
                   const SystShifts &shift, const Var &wei, ESparse sparse)
    : Spectrum(xAxis.GetLabels()[0], loader, xAxis.GetBinnings()[0],
               xAxis.GetVars()[0], yAxis.GetBinnings()[0], yAxis.GetVars()[0],
               zAxis.GetBinnings()[0], zAxis.GetVars()[0], cut, shift, wei,
               sparse) {
  // TODO - do we want to keep this variant around?
  assert(xAxis.NDimensions() == 1);
  assert(yAxis.NDimensions() == 1);
  assert(zAxis.NDimensions() == 1);
}

//----------------------------------------------------------------------
Spectrum::~Spectrum() {
  for (SpectrumLoaderBase *loader : fLoaderCount) {
    loader->RemoveSpectrum(this);
  }
}

//----------------------------------------------------------------------
Spectrum::Spectrum(const Spectrum &rhs)
    : fHist(rhs.fHist), fPOT(rhs.fPOT), fLivetime(rhs.fLivetime),
      fLabels(rhs.fLabels), fBins(rhs.fBins) {
  assert(rhs.fLoaderCount.empty()); // Copying with pending loads is unexpected
}

//----------------------------------------------------------------------
Spectrum::Spectrum(Spectrum &&rhs)
    : fHist(std::move(rhs.fHist)), fPOT(rhs.fPOT), fLivetime(rhs.fLivetime),
      fLabels(rhs.fLabels), fBins(rhs.fBins) {
  assert(rhs.fLoaderCount.empty()); // Copying with pending loads is unexpected
}

//----------------------------------------------------------------------
Spectrum &Spectrum::operator=(const Spectrum &rhs) {
  if (this == &rhs)
    return *this;

  fHist = rhs.fHist;
  fPOT = rhs.fPOT;
  fLivetime = rhs.fLivetime;
  fLabels = rhs.fLabels;
  fBins = rhs.fBins;

  assert(fLoaderCount.empty()); // Copying with pending loads is unexpected

  return *this;
}

//----------------------------------------------------------------------
Spectrum &Spectrum::operator=(Spectrum &&rhs) {
  if (this == &rhs)
    return *this;

  fHist = std::move(rhs.fHist);
  fPOT = rhs.fPOT;
  fLivetime = rhs.fLivetime;
  fLabels = rhs.fLabels;
  fBins = rhs.fBins;

  assert(fLoaderCount.empty()); // Copying with pending loads is unexpected

  return *this;
}

//----------------------------------------------------------------------
void Spectrum::ConstructHistogram(ESparse sparse) {
  DontAddDirectory guard;

  //    assert(!fHist && !fHistSparse);

  const Binning bins1D = Bins1D();

  if (sparse) {
    assert(bins1D.IsSimple());
    const int nbins = bins1D.NBins();
    const double xmin = bins1D.Min();
    const double xmax = bins1D.Max();
    THnSparseD *h = new THnSparseD(UniqueName().c_str(), UniqueName().c_str(),
                                   1, &nbins, &xmin, &xmax);

    // Ensure errors get accumulated properly
    h->Sumw2();

    fHist = Hist::Adopt(std::unique_ptr<THnSparseD>(h), bins1D);
  } else {
    TH1D *h = HistCache::New("", bins1D);

    // Ensure errors get accumulated properly
    h->Sumw2(false);
    h->Sumw2();

    fHist = Hist::Adopt(std::unique_ptr<TH1D>(h), bins1D);
  }
}

//----------------------------------------------------------------------
TH1D *Spectrum::ToTH1(double exposure, EExposureType expotype,
                      EBinType bintype) const {

  // Could have a file temporarily open
  DontAddDirectory guard;

  TH1D *ret = fHist.ToTH1();

  if (expotype == kPOT) {
    const double pot = exposure;
    if (fPOT) {
      ret->Scale(pot / fPOT);
    } else {
      // Allow zero POT if there are also zero events
      if (ret->Integral() > 0) {
        std::cout << "Error: Spectrum with " << ret->Integral()
                  << " entries has zero POT, no way to scale to " << exposure
                  << " POT.";
        if (fLivetime > 0) {
          std::cout << " Spectrum has " << fLivetime << " seconds livetime. "
                    << "Did you mean to pass kLivetime to ToTH1()?";
        }
        std::cout << std::endl;
        abort();
      }
    }
  }
  if (expotype == kLivetime) {
    const double livetime = exposure;
    if (fLivetime) {
      ret->Scale(livetime / fLivetime);
    } else {
      // Allow zero exposure if there are also zero events
      if (ret->Integral() > 0) {
        std::cout << "Error: Spectrum with " << ret->Integral()
                  << " entries has zero livetime, no way to scale to "
                  << livetime << " seconds.";
        if (fPOT > 0) {
          std::cout << " Spectrum has " << fPOT << " POT. "
                    << "Did you mean to pass kPOT to ToTH1()?";
        }
        std::cout << std::endl;
        abort();
      }
    }
  }

  if (bintype == kBinDensity)
    ret->Scale(1, "width");

  // Allow GetMean() and friends to work even if this histogram never had any
  // explicit Fill() calls made.
  if (ret->GetEntries() == 0)
    ret->SetEntries(1);

  return ret;
}

TH1D *Spectrum::ToTH1(double exposure, Color_t col, Style_t style,
                      EExposureType expotype, EBinType bintype) const {
  // Could have a file temporarily open
  DontAddDirectory guard;

  TH1D *ret = ToTH1(exposure, expotype, bintype);

  std::string label;
  for (const std::string &l : fLabels)
    label += l + " and ";
  label.resize(label.size() - 5); // drop the last "and"
  ret->GetXaxis()->SetTitle(label.c_str());
  ret->GetYaxis()->SetTitle("Events");

  ret->SetLineColor(col);
  ret->SetMarkerColor(col);
  ret->SetLineStyle(style);

  return ret;
}

//----------------------------------------------------------------------
TH2 *Spectrum::ToTH2(double exposure, EExposureType expotype,
                     EBinType bintype) const {
  if (fBins.size() != 2) {
    std::cout << "Error: This Spectrum does not appear to be 2D." << std::endl;
    abort();
  }

  TH2 *ret = ana::ToTH2(*this, exposure, expotype, fBins[0], fBins[1]);

  ret->GetXaxis()->SetTitle(fLabels[0].c_str());
  ret->GetYaxis()->SetTitle(fLabels[1].c_str());

  if (bintype == kBinDensity)
    ret->Scale(1, "width");

  // Allow GetMean() and friends to work even if this histogram never had any
  // explicit Fill() calls made.
  if (ret->GetEntries() == 0)
    ret->SetEntries(1);

  return ret;
}

//----------------------------------------------------------------------
TH2 *Spectrum::ToTH2NormX(double exposure, EExposureType expotype) const {
  TH2 *xyhist = ToTH2(exposure, expotype);
  if (!xyhist)
    return nullptr;

  const int nbinsx = fBins[0].NBins();
  const int nbinsy = fBins[1].NBins();

  // Normalize 2D histogram to X-axis spectrum
  for (int i = 1; i <= nbinsx; ++i) {
    double norm = 0.0;
    for (int j = 1; j <= nbinsy; ++j) {
      norm += xyhist->GetBinContent(i, j);
    }
    /// If no entries in the column, skip normalization
    if (norm < 0.0000001)
      continue;

    norm = 1.0 / norm;
    for (int j = 1; j <= nbinsy; ++j) {
      xyhist->SetBinContent(i, j, xyhist->GetBinContent(i, j) * norm);
    }
  }

  // Allow GetMean() and friends to work even if this histogram never had any
  // explicit Fill() calls made.
  if (xyhist->GetEntries() == 0)
    xyhist->SetEntries(1);

  return xyhist;
}

//----------------------------------------------------------------------
TH3 *Spectrum::ToTH3(double exposure, EExposureType expotype,
                     EBinType bintype) const {
  if (fBins.size() != 3) {
    std::cout << "Error: This Spectrum does not appear to be 3D." << std::endl;
    abort();
  }

  TH3 *ret =
      ana::ToTH3(*this, exposure, expotype, fBins[0], fBins[1], fBins[2]);

  ret->GetXaxis()->SetTitle(fLabels[0].c_str());
  ret->GetYaxis()->SetTitle(fLabels[1].c_str());
  ret->GetZaxis()->SetTitle(fLabels[2].c_str());

  if (bintype == kBinDensity)
    ret->Scale(1, "width");

  // Allow GetMean() and friends to work even if this histogram never had any
  // explicit Fill() calls made.
  if (ret->GetEntries() == 0)
    ret->SetEntries(1);

  return ret;
}

//----------------------------------------------------------------------
TH1 *Spectrum::ToTHX(double exposure, bool force1D,
                     EExposureType expotype) const {
  if (force1D)
    return this->ToTH1(exposure, expotype);
  switch (fBins.size()) {
  case 1:
    return this->ToTH1(exposure, expotype);
  case 2:
    return this->ToTH2(exposure, expotype);
  case 3:
    return this->ToTH3(exposure, expotype);
  default:
    std::cout << "Error: unable to hande number of dimensions (" << fBins.size()
              << ")" << std::endl;
    abort();
  }

  return NULL;
}

//----------------------------------------------------------------------
TH1 *Spectrum::ToTH1ProjectX(double exposure, EExposureType expotype) const {
  switch (fBins.size()) {
  case 1:
    return this->ToTH1(exposure, expotype);
  case 2:
    return this->ToTH2(exposure, expotype)->ProjectionX();
  case 3:
    return this->ToTH3(exposure, expotype)->ProjectionX();
  default:
    std::cout << "Error: unable to hande number of dimensions (" << fBins.size()
              << ")" << std::endl;
    abort();
  }

  return NULL;
}

void Spectrum::FillFromHistogram(TH1 const *h) {

  if (fBins.size() != size_t(h->GetDimension())) {
    std::cout << "Error: In Spectrum::FillFromHistogram external histogram "
                 "was dimension "
              << h->GetDimension() << " but this spectrum has " << fBins.size()
              << " binnings." << std::endl;
    abort();
  }

  switch (fBins.size()) {
  case 3: {
    if (h->GetZaxis()->GetNbins() != fBins[2].NBins()) {
      std::cout << "Error: In Spectrum::FillFromHistogram Z axis of external "
                   "histogram(dim: "
                << h->GetDimension()
                << ") does not match NBins of the third binning." << std::endl;
      abort();
    }
#if defined(__GNUC__) and (__GNUC__ >= 7)
    [[gnu::fallthrough]];
#endif
  }
  case 2: {
    if (h->GetYaxis()->GetNbins() != fBins[1].NBins()) {
      std::cout << "Error: In Spectrum::FillFromHistogram X axis of external "
                   "histogram(dim: "
                << h->GetDimension()
                << ") does not match NBins of the first binning." << std::endl;
      abort();
    }
#if defined(__GNUC__) and (__GNUC__ >= 7)
    [[gnu::fallthrough]];
#endif
  }
  case 1: {
    if (h->GetXaxis()->GetNbins() != fBins[0].NBins()) {
      std::cout << "Error: In Spectrum::FillFromHistogram X axis of external "
                   "histogram(dim: "
                << h->GetDimension()
                << ") does not match NBins of the first binning (" << fLabels[0]
                << ", with " << fBins[0].NBins() << " bins)." << std::endl;
      std::cout << "\t" << h->GetXaxis()->GetTitle() << ", with "
                << h->GetXaxis()->GetNbins() << " bins from "
                << h->GetXaxis()->GetBinLowEdge(1) << " to "
                << h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetNbins())
                << std::endl;
      abort();
    }

    break;
  }
  default:
    std::cout << "Error: unable to handle number of dimensions ("
              << fBins.size() << ")" << std::endl;
    abort();
  }

  // Could do more checks than this on the exact binning, but I'm going to
  // trust people here.

  // Inverse of ToTH[2,3] as found in Utilities.cxx
  switch (fBins.size()) {
  case 1: {
    for (int i = 0; i < fHist.GetNbinsX(); ++i) {

      double bc = h->GetBinContent(i + 1);
      double be = h->GetBinError(i + 1);

      double obc = fHist.GetBinContent(i + 1);
      double obe = fHist.GetBinError(i + 1);

      fHist.SetBinContent(i + 1, bc + obc);
      fHist.SetBinError(i + 1, sqrt(pow(be, 2) + pow(obe, 2)));
    }
    break;
  }
  case 2: {
    int ny = fBins[1].NBins();

    for (int i = 0; i < fHist.GetNbinsX(); ++i) {
      const int ix = i / ny;
      const int iy = i % ny;

      double bc = h->GetBinContent(ix + 1, iy + 1);
      double be = h->GetBinError(ix + 1, iy + 1);

      double obc = fHist.GetBinContent(i + 1);
      double obe = fHist.GetBinError(i + 1);

      fHist.SetBinContent(i + 1, bc + obc);
      fHist.SetBinError(i + 1, sqrt(pow(be, 2) + pow(obe, 2)));
    }
    break;
  }
  case 3: {
    int ny = fBins[1].NBins();
    int nz = fBins[2].NBins();

    for (int i = 0; i < fHist.GetNbinsX(); ++i) {
      const int nynz = ny * nz;
      const int nmodnynz = i % nynz;
      const int ix = i / nynz;
      const int iy = nmodnynz / nz;
      const int iz = i % nz;

      double bc = h->GetBinContent(ix + 1, iy + 1, iz + 1);
      double be = h->GetBinError(ix + 1, iy + 1, iz + 1);

      double obc = fHist.GetBinContent(i + 1);
      double obe = fHist.GetBinError(i + 1);

      fHist.SetBinContent(i + 1, bc + obc);
      fHist.SetBinError(i + 1, sqrt(pow(be, 2) + pow(obe, 2)));
    }
    break;
  }
  }
}

//----------------------------------------------------------------------
void Spectrum::Scale(double c) { fHist.Scale(c); }

//----------------------------------------------------------------------
double Spectrum::Integral(double exposure, double *err,
                          EExposureType expotype) const {
  const double ratio =
      (expotype == kPOT) ? exposure / fPOT : exposure / fLivetime;

  if (err) {
    *err = 0;

    for (int i = 0; i < fHist.GetNbinsX() + 2; ++i) {
      *err += util::sqr(fHist.GetBinError(i));
    }
    *err = sqrt(*err) * ratio;
  }

  return fHist.Integral(0, -1) * ratio;
}

//----------------------------------------------------------------------
double Spectrum::Mean() const { return fHist.GetMean(); }

//----------------------------------------------------------------------
void Spectrum::Fill(double x, double w) { fHist.Fill(x, w); }

//----------------------------------------------------------------------
Spectrum Spectrum::MockData(double pot, bool makethrow, int seed) const {
  Spectrum ret = FakeData(pot);

  if (!makethrow)
    return ret;

  TRandom3 rnd(seed); // zero seeds randomly

  for (int i = 0; i < ret.fHist.GetNbinsX() + 2; ++i) {
    ret.fHist.SetBinContent(i, rnd.Poisson(ret.fHist.GetBinContent(i)));
  }

  // Drop old errors, which are based on the MC statistics, and create new
  // ones that are based on the prediction for the data
  ret.fHist.ResetErrors();

  return ret;
}

//----------------------------------------------------------------------
Spectrum Spectrum::FakeData(double pot) const {
  Spectrum ret = *this;
  if (fPOT > 0)
    ret.fHist.Scale(pot / fPOT);
  ret.fPOT = pot;

  // Drop old errors, which are based on the MC statistics, and create new
  // ones that are based on the prediction for the data
  ret.fHist.ResetErrors();

  return ret;
}

//----------------------------------------------------------------------
void Spectrum::Clear() { fHist.Reset(); }

//----------------------------------------------------------------------
void Spectrum::RemoveLoader(SpectrumLoaderBase *p) { fLoaderCount.erase(p); }

//----------------------------------------------------------------------
void Spectrum::AddLoader(SpectrumLoaderBase *p) { fLoaderCount.insert(p); }

//----------------------------------------------------------------------
Spectrum &Spectrum::PlusEqualsHelper(const Spectrum &rhs, int sign) {
  // In this case it would be OK to have no POT/livetime
  if (rhs.fHist.Initialized() && rhs.fHist.Integral(0, -1) == 0)
    return *this;

  if ((!fPOT && !fLivetime) || (!rhs.fPOT && !rhs.fLivetime)) {
    std::cout << "Error: can't sum Spectrum with no POT or livetime." << fPOT
              << " " << rhs.fPOT << std::endl;
    abort();
  }

  if (!fLivetime && !rhs.fPOT) {
    std::cout << "Error: can't sum Spectrum with POT (" << fPOT
              << ") but no livetime and Spectrum with livetime ("
              << rhs.fLivetime << " sec) but no POT." << std::endl;
    abort();
  }

  if (!fPOT && !rhs.fLivetime) {
    std::cout << "Error: can't sum Spectrum with livetime (" << fLivetime
              << " sec) but no POT and Spectrum with POT (" << rhs.fPOT
              << ") but no livetime." << std::endl;
    abort();
  }

  // And now there are still a bunch of good cases to consider

  if (fPOT && rhs.fPOT) {
    // Scale by POT when possible
    fHist.Add(rhs.fHist, sign * fPOT / rhs.fPOT);

    if (fLivetime && rhs.fLivetime) {
      // If POT/livetime ratios match, keep regular lifetime, otherwise zero
      // it out.
      if (AlmostEqual(fLivetime * rhs.fPOT, rhs.fLivetime * fPOT))
        fLivetime = 0;
    }
    if (!fLivetime && rhs.fLivetime) {
      // If the RHS has a livetime and we don't, copy it in (suitably scaled)
      fLivetime = rhs.fLivetime * fPOT / rhs.fPOT;
    }
    // Otherwise, keep our own livetime (if any)

    return *this;
  }

  if (fLivetime && rhs.fLivetime) {
    // Scale by livetime, the only thing in common
    fHist.Add(rhs.fHist, sign * fLivetime / rhs.fLivetime);

    if (!fPOT && rhs.fPOT) {
      // If the RHS has a POT and we don't, copy it in (suitably scaled)
      fPOT = rhs.fPOT * fLivetime / rhs.fLivetime;
    }
    // Otherwise, keep our own POT (if any)

    return *this;
  }

  // That should have been all the cases. I definitely want to know what
  // happened if it wasn't.
  std::cout << "Spectrum::operator+=(). How did we get here? " << fPOT << " "
            << fLivetime << " " << rhs.fPOT << " " << rhs.fLivetime
            << std::endl;
  abort();
}

//----------------------------------------------------------------------
Spectrum &Spectrum::operator+=(const Spectrum &rhs) {
  return PlusEqualsHelper(rhs, +1);
}

//----------------------------------------------------------------------
Spectrum Spectrum::operator+(const Spectrum &rhs) const {
  Spectrum ret = *this;
  ret += rhs;
  return ret;
}

//----------------------------------------------------------------------
Spectrum &Spectrum::operator-=(const Spectrum &rhs) {
  return PlusEqualsHelper(rhs, -1);
}

//----------------------------------------------------------------------
Spectrum Spectrum::operator-(const Spectrum &rhs) const {
  Spectrum ret = *this;
  ret -= rhs;
  return ret;
}

//----------------------------------------------------------------------
Spectrum &Spectrum::operator*=(const Ratio &rhs) {
  fHist.Multiply(rhs.fHist);
  return *this;
}

//----------------------------------------------------------------------
Spectrum Spectrum::operator*(const Ratio &rhs) const {
  Spectrum ret = *this;
  ret *= rhs;
  return ret;
}

//----------------------------------------------------------------------
Spectrum &Spectrum::operator/=(const Ratio &rhs) {
  fHist.Divide(rhs.fHist);
  return *this;
}

//----------------------------------------------------------------------
Spectrum Spectrum::operator/(const Ratio &rhs) const {
  Spectrum ret = *this;
  ret /= rhs;
  return ret;
}

//----------------------------------------------------------------------
void Spectrum::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;
  dir->cd();

  TObjString("Spectrum").Write("type");

  fHist.Write();
  TH1D hPot("", "", 1, 0, 1);
  hPot.Fill(.5, fPOT);
  hPot.Write("pot");
  TH1D hLivetime("", "", 1, 0, 1);
  hLivetime.Fill(.5, fLivetime);
  hLivetime.Write("livetime");

  for (unsigned int i = 0; i < fBins.size(); ++i) {
    TObjString(fLabels[i].c_str()).Write(TString::Format("label%d", i).Data());
    fBins[i].SaveTo(dir->mkdir(TString::Format("bins%d", i)));
  }

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<Spectrum> Spectrum::LoadFrom(TDirectory *dir) {
  DontAddDirectory guard;

  TObjString *tag = (TObjString *)dir->Get("type");
  assert(tag);
  assert(tag->GetString() == "Spectrum");
  delete tag;

  TH1 *hPot = (TH1 *)dir->Get("pot");
  assert(hPot);
  TH1 *hLivetime = (TH1 *)dir->Get("livetime");
  assert(hLivetime);

  std::vector<std::string> labels;
  std::vector<Binning> bins;
  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("bins%d", i));
    if (!subdir)
      break;
    bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label = (TObjString *)dir->Get(TString::Format("label%d", i));
    labels.push_back(label ? label->GetString().Data() : "");
    delete subdir;
    delete label;
  }

  if (bins.empty() && labels.empty()) {
    abort();
    /*
    // Must be an old file. Make an attempt at backwards compatibility.
    if(spect){
      bins.push_back(Binning::FromTAxis(spect->GetXaxis()));
      labels.push_back(spect->GetXaxis()->GetTitle());
    }
    else{
      bins.push_back(Binning::FromTAxis(spectSparse->GetAxis(0)));
      labels.push_back(spectSparse->GetAxis(0)->GetTitle());
    }
    */
  }

  std::unique_ptr<Spectrum> ret =
      std::make_unique<Spectrum>((TH1 *)0, labels, bins, hPot->GetBinContent(1),
                                 hLivetime->GetBinContent(1));
  ret->fHist = Hist::FromDirectory(dir, ret->Bins1D());

  delete hPot;
  delete hLivetime;

  return ret;
}

//----------------------------------------------------------------------
Binning Spectrum::Bins1D() const {
  assert(!fBins.empty());

  Binning bins1D = fBins[0];
  if (fBins.size() > 1) {
    int n = 1;
    for (const Binning &b : fBins)
      n *= b.NBins();
    bins1D = Binning::Simple(n, 0, n);
  }
  return bins1D;
}
} // namespace ana
