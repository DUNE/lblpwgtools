#include "CAFAna/Experiment/ICovarianceMatrix.h"
#include <cassert>

namespace ana
{
  //----------------------------------------------------------------------
  void ICovarianceMatrix::ApplyMask(Eigen::ArrayXd& a, Eigen::ArrayXd& b) const
  {
    if(fMaskA.size() == 0) return;

    assert(a.size() == fMaskA.size());
    assert(b.size() == fMaskA.size());

    // Arrays mean we get bin-by-bin operations
    a *= fMaskA;
    b *= fMaskA;
  }
}
