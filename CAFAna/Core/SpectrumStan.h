#pragma once

#include <string>
#include <vector>

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/StanTypedefs.h"

#include <Eigen/Dense>

namespace ana
{
  class Spectrum;

  class SpectrumStan
  {
  public:
    SpectrumStan(const std::vector<std::string>& labels,
                 const std::vector<Binning>& bins,
                 double pot, double livetime);

    SpectrumStan(std::vector<stan::math::var> &&binContents,
                 const std::vector<std::string> &labels,
                 const std::vector<Binning> &bins,
                 double pot, double livetime);

    SpectrumStan(double * binContents,
                 const std::vector<std::string>& labels,
                 const std::vector<Binning>& bins,
                 double pot, double livetime);

    /// Makes a spectrum from an eigen3 vector
    SpectrumStan(const Eigen::Matrix<stan::math::var,Eigen::Dynamic,1>& h,  // like VectorXd but with stan::math::var type instead of double
                 const std::string& labels,
                 const Binning& bins,
                 double pot,
                 double livetime);

    SpectrumStan(const SpectrumStan &rhs);

    SpectrumStan(SpectrumStan && rhs);

    /// Copy conversion from the normal kind of spectrum
    SpectrumStan(const Spectrum& rhs);

    SpectrumStan& operator=(const SpectrumStan& rhs);

    SpectrumStan& operator=(SpectrumStan&& rhs);

    void Clear();

    const std::vector<Binning>& GetBinnings() const { return fBins; }
    const std::vector<std::string>& GetLabels() const { return fLabels; }

    double Livetime() const { return fLivetime; }
    double POT() const { return fPOT; }


    std::vector<stan::math::var> ToBins(double pot) const;

    // simple arithmetic operators...
    SpectrumStan& operator+=(const SpectrumStan& rhs);
    SpectrumStan operator+(const SpectrumStan& rhs) const;

    SpectrumStan& operator-=(const SpectrumStan& rhs);
    SpectrumStan operator-(const SpectrumStan& rhs) const;

    /// Assignment conversion from the normal kind of spectrum
    SpectrumStan& operator=(const Spectrum& rhs);

  private:
    /// Helper for operator+= and operator-=
    SpectrumStan& PlusEqualsHelper(const SpectrumStan& rhs, int sign);

    /// Contents of the bins.
    std::vector<stan::math::var> fBinContents;

    double fPOT;
    double fLivetime;

    std::vector<std::string> fLabels;
    std::vector<Binning> fBins;
  };
}
