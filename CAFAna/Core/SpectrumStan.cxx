#include "CAFAna/Core/SpectrumStan.h"

#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/HistCache.h"

#include "Utilities/func/Stan.h"

#include "TH1D.h"

namespace ana
{
  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(const std::vector<std::string>& labels,
                             const std::vector<Binning>& bins,
                             double pot, double livetime)
    : fBinContents(bins.size()+2), fPOT(pot), fLivetime(livetime), fLabels(labels), fBins(bins)
  {
    this->Clear();
  }

  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(std::vector<stan::math::var> &&binContents,
                             const std::vector<std::string> &labels,
                             const std::vector<Binning> &bins,
                             double pot, double livetime)
    : fPOT(pot), fLivetime(livetime),
      fLabels(labels), fBins(bins)
  {
        // only support 1D for now
    assert (fBins.size() <= 1 && fBins.size() == fLabels.size());
    assert(int(binContents.size()) == fBins[0].NBins()+2);

    fBinContents.reserve(binContents.size());
    for (auto & binPtr : binContents)
      fBinContents.emplace_back(std::move(binPtr));
    binContents.clear();
  }


  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(double * binContents,
                             const std::vector<std::string>& labels,
                             const std::vector<Binning>& bins,
                             double pot, double livetime)
    : fPOT(pot), fLivetime(livetime),
      fLabels(labels), fBins(bins)
  {
    // only support 1D for now; 0-size is the equivalent of null constructor
    assert ((!binContents || fBins.size() == 1) && fBins.size() == fLabels.size());
    if (!binContents)
      return;

    fBinContents.reserve(bins[0].NBins()+2);
    for (int binIdx = 0; binIdx < bins[0].NBins()+2; binIdx++)
      fBinContents.emplace_back(binContents[binIdx]);
  }

  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& h,
                             const std::string& labels,
                             const Binning& bins,
                             double pot, double livetime)
    : fPOT(pot), fLivetime(livetime), fLabels({labels}), fBins({bins})
  {
    assert(h.rows() == bins.NBins()+2);
    fBinContents.reserve(bins.NBins()+2);
    for (int binIdx = 0; binIdx < bins.NBins()+2; binIdx++)
      fBinContents.emplace_back(h[binIdx]);
  }


  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(const SpectrumStan &rhs)
    : fPOT(rhs.fPOT), fLivetime(rhs.fLivetime),
      fLabels(rhs.fLabels), fBins(rhs.fBins)
  {
    fBinContents.reserve(rhs.fBinContents.size());
    fBinContents = rhs.fBinContents;
  }

  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(const Spectrum &rhs)
    : fPOT(rhs.POT()), fLivetime(rhs.Livetime()),
      fLabels(rhs.GetLabels()), fBins(rhs.GetBinnings())
  {
    assert(rhs.GetBinnings().size() == 1);
    assert( rhs.fLoaderCount.empty() );
    auto hist = rhs.ToTH1(rhs.POT());
    fBinContents.resize(rhs.GetBinnings()[0].NBins()+2);
    for (int binIdx = 0; binIdx < rhs.GetBinnings()[0].NBins()+2; binIdx++)
      fBinContents[binIdx] = stan::math::var(hist->GetBinContent(binIdx));

    HistCache::Delete(hist);
  }

  //----------------------------------------------------------------------
  SpectrumStan::SpectrumStan(ana::SpectrumStan &&rhs)
    : fBinContents(std::move(rhs.fBinContents)),
      fPOT(rhs.fPOT), fLivetime(rhs.fLivetime),
      fLabels(std::move(rhs.fLabels)), fBins(std::move(rhs.fBins))
  {}

  //----------------------------------------------------------------------
  SpectrumStan & SpectrumStan::operator+=(const SpectrumStan &rhs)
  {
    PlusEqualsHelper(rhs, +1);
    return *this;
  }

  //----------------------------------------------------------------------
  SpectrumStan SpectrumStan::operator+(const SpectrumStan &rhs) const
  {
    SpectrumStan ret(*this);
    ret += rhs;
    return std::move(ret);
  }

  //----------------------------------------------------------------------
  SpectrumStan & SpectrumStan::operator-=(const SpectrumStan &rhs)
  {
    PlusEqualsHelper(rhs, -1);
    return *this;
  }

  //----------------------------------------------------------------------
  SpectrumStan SpectrumStan::operator-(const SpectrumStan &rhs) const
  {
    SpectrumStan ret(*this);
    ret -= rhs;
    return std::move(ret);
  }

  //----------------------------------------------------------------------
  SpectrumStan& SpectrumStan::operator=(const SpectrumStan& rhs)
  {
    this->~SpectrumStan();
    new (this) SpectrumStan(rhs);
    return *this;
  }

  //----------------------------------------------------------------------
  SpectrumStan &SpectrumStan::operator=(SpectrumStan &&rhs)
  {
    this->~SpectrumStan();
    new (this) SpectrumStan(std::move(rhs));
    return *this;
  }

  //----------------------------------------------------------------------
  SpectrumStan& SpectrumStan::operator=(const Spectrum& rhs)
  {
    // abuse (?) placement new to overwrite this object using the copy constructor
    this->~SpectrumStan();
    new (this) SpectrumStan(rhs);
    return *this;
  }

  //----------------------------------------------------------------------
  void SpectrumStan::Clear()
  {
    fBinContents.resize(fBins[0].NBins()+2);
    for (auto & binValPtr : this->fBinContents)
      binValPtr = stan::math::var(0.0);
  }

  //----------------------------------------------------------------------
  std::vector<stan::math::var> SpectrumStan::ToBins(double pot) const
  {
    assert(fPOT > 0);

    std::vector<stan::math::var> ret = fBinContents;

    if (std::abs(pot - fPOT) < 1e12)  // that's approximately one proton pulse.  can't go smaller
      return ret;

    for (auto & val : ret)
      val *= pot / fPOT;

    return ret;
  }

  //----------------------------------------------------------------------
  SpectrumStan& SpectrumStan::PlusEqualsHelper(const SpectrumStan& rhs, int sign)
  {
    assert (fBinContents.size() == rhs.fBinContents.size());

    if((!fPOT && !fLivetime) || (!rhs.fPOT && !rhs.fLivetime)){
      std::cerr << "Error: can't sum SpectrumStan with no POT or livetime."
                << std::endl;
      abort();
    }

    if(!fLivetime && !rhs.fPOT){
      std::cerr << "Error: can't sum SpectrumStan with POT ("
                << fPOT << ") but no livetime and SpectrumStan with livetime ("
                << rhs.fLivetime << " sec) but no POT." << std::endl;
      abort();
    }

    if(!fPOT && !rhs.fLivetime){
      std::cout << "Error: can't sum SpectrumStan with livetime ("
                << fLivetime << " sec) but no POT and SpectrumStan with POT ("
                << rhs.fPOT << ") but no livetime." << std::endl;
      abort();
    }

    // And now there are still a bunch of good cases to consider

    if(fPOT && rhs.fPOT){
      // Scale by POT when possible
      for (std::size_t binIdx = 0; binIdx < fBinContents.size(); binIdx++)
        fBinContents[binIdx] += sign * fPOT / rhs.fPOT * rhs.fBinContents[binIdx];

      if(fLivetime && rhs.fLivetime){
        // If POT/livetime ratios match, keep regular lifetime, otherwise zero
        // it out.
        if(AlmostEqual(fLivetime*rhs.fPOT, rhs.fLivetime*fPOT))
          fLivetime = 0;
      }
      if(!fLivetime && rhs.fLivetime){
        // If the RHS has a livetime and we don't, copy it in (suitably scaled)
        fLivetime = rhs.fLivetime * fPOT/rhs.fPOT;
      }
      // Otherwise, keep our own livetime (if any)

      return *this;
    }

    if(fLivetime && rhs.fLivetime){
      // Scale by livetime, the only thing in common
      for (std::size_t binIdx = 0; binIdx < fBinContents.size(); binIdx++)
        fBinContents[binIdx] += sign * fLivetime / rhs.fLivetime * rhs.fBinContents[binIdx];

      if(!fPOT && rhs.fPOT){
        // If the RHS has a POT and we don't, copy it in (suitably scaled)
        fPOT = rhs.fPOT * fLivetime/rhs.fLivetime;
      }
      // Otherwise, keep our own POT (if any)

      return *this;
    }

    // That should have been all the cases. I definitely want to know what
    // happened if it wasn't.
    std::cout << "SpectrumStan::PlusEqualsHelper(). How did we get here? "
              << fPOT << " " << fLivetime << " "
              << rhs.fPOT << " " << rhs.fLivetime << std::endl;
    abort();
  }


}
