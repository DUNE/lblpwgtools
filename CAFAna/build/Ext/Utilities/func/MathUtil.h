#ifndef MATHUTIL_H
#define MATHUTIL_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// \file MathUtil.h                                                     //
//                                                                      //
// Simple mathematical functions, initially to prevent the abuse of     //
// pow()                                                                //
// <bckhouse@caltech.edu>						//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <vector>

namespace util{

  /// \name Simple mathematical functions
  //@{

  /// More efficient square function than pow(x,2)
  template<class T> inline T sqr(T x){return x*x;}

  /// More efficient cube function than pow(x,3)
  template<class T> inline T cube(T x){return x*x*x;}

  /// More efficient exponentiation function than pow(x,n) for small n
  template<class T> inline T ipow(T x, unsigned int n)
  {
    T ret = 1; 
    if (n == 0) return ret;
    for(unsigned int i = 1; i <= n; ++i) ret *= x; 
    return ret;
  }


  /// 2D Euclidean distance
  inline double pythag(double x, double y)
  {
    return sqrt(sqr(x)+sqr(y));
  }

  /// 3D Euclidean distance
  inline double pythag(double x, double y, double z)
  {
    return sqrt(sqr(x)+sqr(y)+sqr(z));
  }

  /// \brief Simplified version of \ref LinFit
  ///
  /// All weights are assumed 1. In the absence of ability to weight, the
  /// returned chi-square doesn't mean much and is omitted.
  void LinFitUnweighted(const std::vector<double>& x,
                        const std::vector<double>& y,
                        double& m, double &c);

  /// \brief Find the best-fit line to a collection of points in 2-D by
  /// minimizing the squared vertical distance from the points to the line.
  ///
  /// In the common case of unweighted data with Gaussian errors, you should
  /// set the points weights to 1/sigma^2 for each point.
  ///
  /// \param x - input vector of x coordinates
  /// \param y - input vector of y coordinates
  /// \param w - input vector of weights for the points
  /// \param m - output gradient of the fit line
  /// \param c - output y-intercept of the fit line
  /// \return The chi^2 value defined by chi^2 = sum_i[w_i d^2_i]
  double LinFit(const std::vector<double>& x,
		const std::vector<double>& y,
		const std::vector<double>& w,
                double& m, double &c);
  
  /// \brief Best fit line to points using Theil-Sens median method
  ///  
  /// An implementation of the Theil-Sens straight line estimator
  /// (http://en.wikipedia.org/wiki/Theil-Sen_estimator)
  ///
  /// Theil-Sens sets the slope to be the median slope of data-point
  /// pairs and the intercept to the median value for data points
  /// computed using the median slope. Theil-Sens is more robust
  /// against the presence of noise.
  ///
  /// The weighting method is controlled by "wmode":
  /// 
  /// * wmode=0 : pairs of points will be assigned the same weights
  /// (that is, unweighted)
  ///
  /// * wmode=1 : pairs of points will be assigned weights equal to
  /// sqrt(w[i]*w[j])
  ///
  /// * wmode=2 : pairs of points will be assigned weights sqrt(w1*w1*d^2)
  /// where d is the distance between the points.
  ///
  /// \param x     - input vector of x coordinates
  /// \param y     - input vector of y coordinates
  /// \param w     - input vector of weights
  /// \param m     - on return, the slope value
  /// \param b     - on return, the intercept value
  /// \param wmode - weight mode
  ///
  void LinFitTS(const std::vector<double>& x,
		const std::vector<double>& y,
		const std::vector<double>& w,
		double& m,
		double& b,
		int     wmode);

  //@}

} // end namespace

#endif // MATHUTIL_H
