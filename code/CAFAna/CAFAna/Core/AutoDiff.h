#pragma once

#include <cassert>
#include <vector>

namespace ana
{
  class DiffVar
  {
  public:
    explicit DiffVar(double val, int idx, int tot)
      : fVal(val), fDiff(tot, 0)
    {
      fDiff[idx] = 1;
    }

    DiffVar(double val) : fVal(val) // empty fDiff
    {
    }
    /*
    static DiffVar Zero(int tot)
    {
      DiffVar ret(0, 0, tot);
      ret.fDiff[0] = 0;
      return ret;
    }

    static DiffVar One(int tot)
    {
      DiffVar ret(1, 0, tot);
      ret.fDiff[0] = 0;
      return ret;
    }
    */
    explicit operator int() const
    {
      return int(fVal);
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
      // Update fVal and half of the product rule
      *this *= x.fVal;
      for(unsigned int i = 0; i < x.fDiff.size(); ++i){
        // Other half of the product rule
        fDiff[i] += fVal * x.fDiff[i];
      }
      return *this;
    }

    DiffVar operator*(const DiffVar& x) const
    {
      DiffVar ret = *this;
      ret *= x;
      return ret;
    }

    double Derivative(unsigned int varIdx)
    {
      if(fDiff.empty()) return 0;
      assert(varIdx < fDiff.size());
      return fDiff[varIdx];
    }
    //  protected:
    double fVal;
    std::vector<double> fDiff;

    /// Ensure size of 'this' is large enough for combination with 'x'
    void Match(const DiffVar& x)
    {
      assert(fDiff.empty() || x.fDiff.empty() || fDiff.size() == x.fDiff.size());
      if(fDiff.empty()) fDiff.resize(x.fDiff.size());
    }
  };

  DiffVar operator+(double x, DiffVar y){return y+x;}
  DiffVar operator-(double x, DiffVar y){return (y*-1.)+x;}
  DiffVar operator*(double x, DiffVar y){return y*x;}
}
