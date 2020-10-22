#pragma once

#include "CAFAna/Core/StanVar.h"

#include <Eigen/Dense>
namespace Eigen{
  using ArrayXstan = Eigen::Array<stan::math::var, Eigen::Dynamic, 1>;
}

namespace ana
{
  /// Variant that handles the prediction in the form of Stan vars
  stan::math::var LogLikelihood(const Eigen::ArrayXstan& exp,
                                const Eigen::ArrayXd& obs);

}
