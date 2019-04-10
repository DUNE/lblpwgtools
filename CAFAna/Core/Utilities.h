#pragma once

#include "CAFAnaCore/CAFAna/Core/UtilsExt.h"

#include <cxxabi.h>
#include <cfenv>
#include <map>
#include <set>
#include <string>

// these are templated types.
// can't forward-declare them here
// because compiler errors result
// when the templates are introduced
#include "TMatrixD.h"

#include <Eigen/Dense>

class TArrayD;
class TDirectory;
class TH1;
class TH2;
class TH3;
class TF1;
class TH1D;
class TH2F;
class TH2D;
class TH3D;

#include "CAFAna/Core/MathUtil.h"

namespace ana
{
  class Binning;
  class Spectrum;
  class Ratio;

  /// \brief Alter floating-point exception flag
  ///
  /// Upon going out of scope, restores the previous setting
  class FloatingExceptionOnNaN
  {
  public:
    FloatingExceptionOnNaN(bool enable = true);
    ~FloatingExceptionOnNaN();
  protected:
    fexcept_t fBackup;
  };

  /** \brief Compute bin-to-bin covariance matrix from a collection of sets of bin contents.

      \param binSets   Collection of sets of bins from which covariances should be calculated
      \param firstBin  The first bin that should be considered (inclusive)
      \param lastBin   The last bin that should be considered (inclusive).  -1 means "last in set"

      \returns  unique_ptr to TMatrixD containing computed covariance matrix unless binSets.size() < 2,
                in which case the unique_ptr's target is nullptr.

      Note TH1D is a child class of TArrayD -- so you can pass a vector
      of TH1D* to this method.
  **/
  std::unique_ptr<TMatrixD> CalcCovMx(const std::vector<TArrayD*> & binSets, int firstBin=0, int lastBin=-1);

  class LLPerBinFracSystErr
  {
  public:
    static void SetError(double e) {fgErr = e;}
    static double GetError() {return std::max(fgErr, 0.);}
  protected:
    static double fgErr;
  };

  /** \brief The log-likelihood formula from the PDG.

      \param exp The expected spectrum
      \param obs The corresponding observed spectrum

      \returns The log-likelihood formula from the PDG
      \f[ \chi^2=2\sum_i^{\rm bins}\left(e_i-o_i+o_i\ln\left({o_i\over e_i}\right)\right) \f]

      Includes underflow bin and an option for
      overflow bin (off by default) and handles
      zero observed or expected events correctly.
  **/
  double LogLikelihood(const TH1* exp, const TH1* obs, bool useOverflow = false);

  /** \brief The log-likelihood formula from the PDG.

      \param exp The expected spectrum
      \param obs The corresponding observed spectrum

      \returns The log-likelihood formula from the PDG
      \f[ \chi^2=2\sum_i^{\rm bins}\left(e_i-o_i+o_i\ln\left({o_i\over e_i}\right)\right) \f]

      Includes underflow bin and an option for
      overflow bin (off by default) and handles
      zero observed or expected events correctly.
  **/
  double LogLikelihood(const Eigen::ArrayXd& exp, const Eigen::ArrayXd& obs, bool useOverflow = false);

  /** \brief The log-likelihood formula for a single bin

      \param exp Expected count
      \param obs Observed count

      \returns The log-likelihood formula from the PDG
      \f[ \chi^2=2\left(e-o+o\ln\left({o\over e}\right)\right) \f]

      Handles zero observed or expected events correctly.
      Templated so that it can handle usage with Stan vars and other numeric types.
      (The horible third template parameter ensures this function can only be used
       with types that accept conversion from double -- which means you can't pass
       it a TH1* or a std::vector<stan::math::var>&, removing the ambiguity with
       those other versions of LogLikelihood). The return type promotes to
       stan::math::var if either T or U are.
  **/
  template <typename T, typename U,
            typename std::enable_if_t<std::is_convertible_v<double, T> && std::is_convertible_v<double, U>, int> = 0>
  decltype(T(0) - U(0)) LogLikelihood(T exp, U obs)
  {
    // http://www.wolframalpha.com/input/?i=d%2Fds+m*(1%2Bs)+-d+%2B+d*ln(d%2F(m*(1%2Bs)))%2Bs%5E2%2FS%5E2%3D0
    // http://www.wolframalpha.com/input/?i=solve+-d%2F(s%2B1)%2Bm%2B2*s%2FS%5E2%3D0+for+s
    const auto S = LLPerBinFracSystErr::GetError();
    if(S > 0){
      const auto S2 = util::sqr(S);
      const auto s = .25*(sqrt(8*obs*S2+util::sqr(exp*S2-2))-exp*S2-2);
      exp *= 1+s;
    }

    // With this value, negative expected events and one observed
    // event gives a chisq from this one bin of 182.
    const double minexp = 1e-40; // Don't let expectation go lower than this

    if(obs*1000 > exp){
      // This strange form is for numerical stability when exp ~ obs
      return 2*obs*((exp-obs)/obs + log1p((obs-exp)/exp));
    }
    else{
      // But log1p doesn't like arguments near -1 (observation much smaller
      // than expectation), and it's better to use the usual formula in that
      // case.
      if(obs){
        return 2*(exp-obs + obs*log(obs/exp));
      }
      else{
        return 2*exp;
      }
    }
  }

  Eigen::MatrixXd EigenMatrixXdFromTMatrixD(const TMatrixD* mat);

  TMatrixD TMatrixDFromEigenMatrixXd(const Eigen::MatrixXd& mat);

  /**  \brief Chi-squared calculation using a covariance matrix.

       \param exp   Expected bin counts
       \param obs   Observed bin counts
       \param covmxinv Inverse of covariance matrix.  Must have same dimensions as exp and obs

       \returns The chi^2 calculated according to the formula from the PDG:
       \f[ \chi^2 = (\vec{o}-\vec{e})^{T} V^{-1} (\vec{o} - \vec{e}) \]

       Note that this implicitly assumes Gaussian statistics for the bin counts!
  **/
  double Chi2CovMx(const Eigen::ArrayXd& exp, const Eigen::ArrayXd& obs, const Eigen::MatrixXd& covmxinv);

  /// \brief For use with low-statistics data in combination with a MC
  /// prediction whose bins have a correlated uncertainty.
  ///
  /// \param exp The nominal expectation
  /// \param obs The observed data
  /// \param covmxinv The inverse of the correlation matrix between MC bins.
  ///                 DO NOT include any contribution for the statistical
  ///                 uncertainty on the data.
  /// \param hint Optional. It's a substantial optimization to pass the same
  ///             vector here each time so that we can learn from a previous
  ///             similar problem.
  ///
  /// The matrix must be symmetric and have dimension equal to the number of
  /// non-overflow bins in the histograms.
  double LogLikelihoodCovMx(const Eigen::ArrayXd& exp, const Eigen::ArrayXd& obs, const Eigen::MatrixXd& covmxinv, std::vector<double>* hint = 0);


  /// \brief Internal helper for \ref Surface and \ref FCSurface
  ///
  /// Creates a histogram having bins \em centred at the min and max
  /// coordinates
  TH2F* ExpandedHistogram(const std::string& title,
                          int nbinsx, double xmin, double xmax,
                          int nbinsy, double ymin, double ymax);

  /// \brief Invert a symmetric matrix with possibly empty rows/columns.
  ///
  /// Invert a symmetric matrix that may have empty rows/columns,
  /// which (strictly speaking) make it impossible to invert the matrix.
  /// (This often arises when computing covariance matrices for predictions
  ///  which have empty bins; the covariance is 0 for the entire row/column
  ///  in that case.)
  /// Since those rows/cols are not useful, we can sidestep the problem
  /// by removing them (and the corresponding columns)
  /// from the matrix, inverting that, then re-inserting
  /// the null rows/columns.
  std::unique_ptr<TMatrixD> SymmMxInverse(const TMatrixD& mx);

  /// This is $SRT_PRIVATE_CONTEXT if a CAFAna directory exists there,
  /// otherwise $SRT_PUBLIC_CONTEXT
  std::string FindCAFAnaDir();

  /// Read list of input files from a text file, one per line
  std::vector<std::string> LoadFileList(const std::string& listfile);

  /// \brief Extract map of metadata parameters from a CAF file
  ///
  /// \param dir The "meta" directory from the CAF file
  /// \return    A map from metadata field name to metadata value
  std::map<std::string, std::string> GetCAFMetadata(TDirectory* dir);

  /// \brief \a base += \a add
  ///
  /// \param base The original source of strings, will be altered
  /// \param add  Strings to add to \a base if missing
  /// \param mask Fields for which there was a conflict, will be altered
  void CombineMetadata(std::map<std::string, std::string>& base,
                       const std::map<std::string, std::string>& add,
                       std::set<std::string>& mask);

  /// \brief Write map of metadata parameters into a CAF file
  ///
  /// \param dir  The "meta" directory of the CAF file
  /// \param meta Map from metadata field name to metadata value
  void WriteCAFMetadata(TDirectory* dir,
                        const std::map<std::string, std::string>& meta);

  /// Is this a grid (condor) job?
  bool RunningOnGrid();

  // Calling this function will return a Fourier series, fit to the input
  // histogram.  Assumes x-axis covers one period
  class FitToFourier
  {
  public:
    FitToFourier(TH1* h, double xlo, double xhi, int NOsc);
    ~FitToFourier();
    TF1* Fit() const;
    double operator()(double *x, double *par) const;
  private:

    const TH1*   fHist; // Histogram to fit
    const double fxlo;  // Lower bound
    const double fxhi;  // Upper bound - assumed to be 1 osc from the low end
    const int    fNOsc; // Highest harmonic to include

  };

  void EnsurePositiveDefinite(TH2* mat);

  /// \brief Returns a masking histogram based on axis limits
  ///
  /// This mask *does* include entries for underflow and overflow bins
  Eigen::ArrayXd GetMaskArray(const Spectrum& s,
                              double xmin=0, double xmax=-1,
                              double ymin=0, double ymax=-1);
}
