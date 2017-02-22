#pragma once

#include <map>
#include <string>

class TH1;
class TH1D;

namespace osc{class IOscCalculator;}

namespace ana
{
  /// Transition probability for any one channel as a function of energy
  class OscCurve
  {
  public:
    OscCurve(osc::IOscCalculator* calc, int from, int to);
    OscCurve(TH1* h);
    virtual ~OscCurve();

    OscCurve(const OscCurve& rhs);
    OscCurve& operator=(const OscCurve& rhs);

    TH1* ToTH1() const;
  protected:
    TH1D* fHist;
  };
}
