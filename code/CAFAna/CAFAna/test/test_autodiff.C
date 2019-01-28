#include "CAFAna/Core/AutoDiff.h"
using namespace ana;

#include <iostream>

void test_autodiff()
{
  DiffVar x(1, 0, 3);
  DiffVar y(-2, 1, 3);
  DiffVar z(3.5, 2, 3);

  //  x = 1;
  //  y = -2;
  //  z = 3.5;

  const DiffVar f = (3*x*y + 2*x - 5*y*z + 12 * x * y*y*y * z - 1)*(3*x - .2 * x * y + 5);

  const double f1 = 3*1*-2 + 2*1 - 5*-2*3.5 + 12*1*-2*-2*-2*3.5-1;
  const double f2 = 3*1-.2*1*-2+5;

  const double dx1 = 3*-2 + 2 + 12*-2*-2*-2*3.5;
  const double dx2 = 3-.2*-2;

  const double dy1 = 3*1 - 5*3.5 + 12*1*3*-2*-2*3.5;
  const double dy2 = -.2*1;

  const double dz1 = -5*-2 + 12*1*-2*-2*-2;
  const double dz2 = 0;

  const double dx = f1*dx2 + f2*dx1;
  const double dy = f1*dy2 + f2*dy1;
  const double dz = f1*dz2 + f2*dz1;

  std::cout << f1*f2 << " " << dx << " " << dy << " " << dz << std::endl;

  std::cout << f.fVal << " " << f.Derivative(0) << " " << f.Derivative(1) << " " << f.Derivative(2) << std::endl;
}
