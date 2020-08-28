#pragma once

#include "CAFAna/Core/Spectrum.h"

namespace ana
{
  /// Represent the ratio between two spectra
  class Ratio
  {
  public:
    friend class Spectrum;

    /// \param num Numerator of the ratio
    /// \param denom Denominator of the ratio
    /// \param purOrEffErrs Does this ratio represent a purity or efficiency
    ///                     plot? If so, error bars are calculated differently.
    Ratio(const Spectrum& num, const Spectrum& denom,
	  bool purOrEffErrs = false);

    Ratio(Eigen::ArrayXd&& arr,
          const std::vector<std::string>& labels,
          const std::vector<Binning>& bins)
      : fHist(Hist::Adopt(std::move(arr))), fBins(bins), fLabels(labels)
    {
    }

    virtual ~Ratio();

    Ratio(const Ratio& rhs) = default;
    Ratio& operator=(const Ratio& rhs) = default;

    Ratio& operator*=(const Ratio& rhs);
    Ratio operator*(const Ratio& rhs) const;

    Ratio& operator/=(const Ratio& rhs);
    Ratio operator/(const Ratio& rhs) const;

    TH1D* ToTH1(Color_t col = kBlack,
                Style_t style = kSolid) const;

    bool HasStan() const {return fHist.HasStan();}
    const Eigen::ArrayXd& GetEigen() const {return fHist.GetEigen();}
    const Eigen::ArrayXstan& GetEigenStan() const {return fHist.GetEigenStan();}

  protected:
    // For derived classes
    Ratio(Hist&& h,
          std::vector<Binning>&& b,
          std::vector<std::string>&& l)
      : fHist(std::move(h)), fBins(std::move(b)), fLabels(std::move(l))
    {
    }

    Hist fHist;
    std::vector<Binning> fBins;
    std::vector<std::string> fLabels;
  };

  inline Ratio operator/(const Spectrum& lhs, const Spectrum& rhs){return Ratio(lhs, rhs);}
} // namespace
