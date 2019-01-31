#pragma once

#include <cassert>
#include <cmath>
#include <vector>

namespace ana
{
  class DiffVar
  {
  public:
    static DiffVar Variable(double val, int idx, int tot)
    {
      DiffVar ret;
      ret.fVal = val;
      ret.fDiff.resize(tot);
      ret.fDiff[idx] = 1;
      return ret;
    }

    static DiffVar Constant(double x)
    {
      DiffVar ret;
      ret.fVal = x;
      return ret;
    }

    bool operator==(double x) const {return fVal == x;}
    bool operator<(const DiffVar& x) const {return fVal < x.fVal;}

    DiffVar& operator+=(double x){fVal += x; return *this;}
    DiffVar& operator-=(double x){fVal -= x; return *this;}
    DiffVar& operator*=(double x)
    {
      fVal *= x;
      for(double& y: fDiff) y *= x;
      return *this;
    }
    DiffVar& operator/=(double x){*this *= 1./x; return *this;}

    DiffVar operator+(double x) const {DiffVar ret = *this; ret += x; return ret;}
    DiffVar operator-(double x) const {DiffVar ret = *this; ret -= x; return ret;}
    DiffVar operator*(double x) const {DiffVar ret = *this; ret *= x; return ret;}
    DiffVar operator/(double x) const {DiffVar ret = *this; ret /= x; return ret;}

    DiffVar& operator+=(const DiffVar& x)
    {
      Match(x);
      fVal += x.fVal;
      for(unsigned int i = 0; i < x.fDiff.size(); ++i) fDiff[i] += x.fDiff[i];
      return *this;
    }

    DiffVar operator+(const DiffVar& x) const
    {
      DiffVar ret = *this;
      ret += x;
      return ret;
    }

    DiffVar& operator-=(const DiffVar& x)
    {
      Match(x);
      fVal -= x.fVal;
      for(unsigned int i = 0; i < x.fDiff.size(); ++i) fDiff[i] -= x.fDiff[i];
      return *this;
    }

    DiffVar operator-(const DiffVar& x) const
    {
      DiffVar ret = *this;
      ret -= x;
      return ret;
    }

    DiffVar& operator*=(const DiffVar& x)
    {
      Match(x);

      // Product rule
      for(unsigned int i = 0; i < x.fDiff.size(); ++i){
        fDiff[i] = x.fVal * fDiff[i] + fVal * x.fDiff[i];
      }

      fVal *= x.fVal;

      return *this;
    }

    DiffVar operator*(const DiffVar& x) const
    {
      DiffVar ret = *this;
      ret *= x;
      return ret;
    }

    double Derivative(unsigned int varIdx) const
    {
      if(fDiff.empty()) return 0;
      assert(varIdx < fDiff.size());
      return fDiff[varIdx];
    }

    //  protected:
    double fVal;
    std::vector<double> fDiff;

  private:
    DiffVar(){}

    /// Ensure size of 'this' is large enough for combination with 'x'
    void Match(const DiffVar& x)
    {
      assert(fDiff.empty() || x.fDiff.empty() || fDiff.size() == x.fDiff.size());
      if(fDiff.empty()) fDiff.resize(x.fDiff.size());
    }
  };

  DiffVar operator+(double x, const DiffVar& y){return y+x;}
  DiffVar operator-(double x, const DiffVar& y){return (y*-1.)+x;}
  DiffVar operator*(double x, const DiffVar& y){return y*x;}

  bool operator<(const ana::DiffVar& a, double b){return a.fVal < b;}
  bool operator>(const ana::DiffVar& a, double b){return a.fVal > b;}
}

namespace std
{
  ana::DiffVar min(const ana::DiffVar& a, const ana::DiffVar& b){return (a.fVal < b.fVal) ? a : b;}
  ana::DiffVar max(const ana::DiffVar& a, const ana::DiffVar& b){return (a.fVal > b.fVal) ? a : b;}

  int floor(const ana::DiffVar& a){return std::floor(a.fVal);}
}
