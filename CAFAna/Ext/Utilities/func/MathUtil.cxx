#include "Utilities/func/MathUtil.h"

#include <cassert>
#include <algorithm>

namespace util
{
  //......................................................................
  void LinFitUnweighted(const std::vector<double>& x,
                        const std::vector<double>& y,
                        double& m, double& c)
  {
    // Before going ahead, make sure we have sensible arrays
    assert(x.size() == y.size());
    assert(x.size() >= 2);

    // Accumulate the sums for the fit
    double Sx  = 0;
    double Sy  = 0;
    double Sxy = 0;
    double Sy2 = 0;
    double Sx2 = 0;
    const unsigned int I = x.size();
    for(unsigned int i = 0; i < I; ++i) {
      Sx  += x[i];
      Sy  += y[i];
      Sx2 += x[i]*x[i];
      Sxy += x[i]*y[i];
      Sy2 += y[i]*y[i];
    }
    const double d = I*Sx2 - Sx*Sx;
    m = (I*Sxy  - Sx*Sy)/d;
    c = (Sy*Sx2 - Sx*Sxy)/d;
  }

  //......................................................................
  double LinFit(const std::vector<double>& x,
		const std::vector<double>& y,
		const std::vector<double>& w,
                double& m, double& c)
  {
    // Before going ahead, make sure we have sensible arrays
    assert(x.size() == y.size());
    assert(x.size() == w.size());
    assert(x.size() >= 2);

    // Accumulate the sums for the fit
    double Sw   = 0;
    double Swx  = 0;
    double Swy  = 0;
    double Swxy = 0;
    double Swy2 = 0;
    double Swx2 = 0;
    for(unsigned int i = 0; i < w.size(); ++i) {
      Sw   += w[i];
      Swx  += w[i]*x[i];
      Swy  += w[i]*y[i];
      Swx2 += w[i]*x[i]*x[i];
      Swxy += w[i]*x[i]*y[i];
      Swy2 += w[i]*y[i]*y[i];
    }
    const double d = Sw*Swx2 - Swx*Swx;
    m = (Sw*Swxy  - Swx*Swy)/d;
    c = (Swy*Swx2 - Swx*Swxy)/d;

    const double chi2 =
      Swy2 - 2.0*m*Swxy - 2.0*c*Swy + 2.0*m*c*Swx +
      c*c*Sw + m*m*Swx2;

    return chi2;
  }

  //......................................................................
  
  static bool sort_pair_by_first(const std::pair<double,double>& a,
				 const std::pair<double,double>& b) 
  {
    return (a.first<b.first);
  }
  
  //......................................................................
  
  static double find_median(const std::vector< std::pair<double,double> >& a)
  {
    unsigned int i;
    for (i=0; i<a.size(); ++i) {
      if (a[i].second>0.5) break;
    }
    if (i>1) {
      //
      // Linearly interpolate along weights to find 0.5
      //
      double x1 = a[i-1].second;
      double x2 = a[i].  second;
      double y1 = a[i-1].first;
      double y2 = a[i].  first;
      return y1 + (0.5-x1)*(y2-y1)/(x2-x1);
    }
    return a[i].first;
  }

  //......................................................................

  void LinFitTS(const std::vector<double>& x,
		const std::vector<double>& y,
		const std::vector<double>& wt,
		double& m,
		double& b,
		int     wmode)
  {
    unsigned int i, j;
  
    //
    // Check that we have at least two points and that the x and y
    // vectors sizes match
    //
    unsigned int n = x.size();
    assert(n>1);
    assert(n==y.size());
    
    //
    // Reserve a vector of slope,weight pairs. Reserve the space we need
    // up front to improve efficiency
    //
    std::vector<std::pair<double,double> > mw;
    mw.reserve(n*(n-1)-1);

    //
    // Find the slopes of all pairs of points x,y. Depending on calling
    // options, weight the pairs by their separation distance.
    //
    double dx, dy, w;
    double wnorm = 0.0;
    for (i=0; (i+1)<n; ++i) {
      for (j=i+1; j<n; ++j) {
	dx = x[j]-x[i];
	dy = y[j]-y[i];
	if (dx!=0.0) {
	  m = dy/dx;
	  switch (wmode) {
	  case 1:  w = sqrt(wt[i]*wt[j]);               break;
	  case 2:  w = sqrt(wt[i]*wt[j]*(dx*dx+dy*dy)); break;
	  default: w = 1.0;                             break;
	  }
	  if (w>0.0) {
	    mw.push_back( std::pair<double,double>(m,w) );
	    wnorm += w;
	  }
	}
      }
    }
    std::sort(mw.begin(), mw.end(), sort_pair_by_first);

    //
    // Normalize weights to accumulate along the interval between 0 and 1
    //
    double sumw = 0.0;
    wnorm = 1.0/wnorm;
    for (i=0; i<mw.size(); ++i) {
      mw[i].second = wnorm*mw[i].second + sumw;
      sumw = mw[i].second;
    }
    
    m = find_median(mw);
    
    //
    // Build a vector or intercept, weight pairs
    //
    std::vector<std::pair<double,double> > bw;
    bw.reserve(n);

    //
    // Now compute the intercepts
    //
    wnorm = 0.0;
    w     = 1.0;
    for (i=0; i<n; ++i) {
      b = y[i] - m*x[i];
      bw.push_back( std::pair<double,double>(b,w) );
      wnorm += w;
    }
    std::sort(bw.begin(), bw.end(), sort_pair_by_first);
  
    //
    // Normalize the weights to accumulate along a range between 0 and
    // 1
    //
    sumw = 0.0;
    wnorm = 1.0/wnorm;
    for (i=0; i<bw.size(); ++i) {
      bw[i].second = wnorm*bw[i].second + sumw;
      sumw = bw[i].second;
    }
  
    b = find_median(bw); 
  }

} // namespace
