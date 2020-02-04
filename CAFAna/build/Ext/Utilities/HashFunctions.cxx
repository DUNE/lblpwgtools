////////////////////////////////////////////////////////////////////////
/// \brief   Generate unique hashes for reco products
/// \author  Justin Vasel <justin.vasel@gmail.com>
/// \date    December 2018
////////////////////////////////////////////////////////////////////////

#include "Utilities/HashFunctions.h"

// ............................................................................
util::HashFunctions::HashFunctions()
{
}

// ............................................................................
util::HashFunctions::~HashFunctions()
{
}

// ............................................................................
long util::HashFunctions::Generate(rb::CellHit hit)
{
  long hash = 0;
  hash += (long)((std::abs(hit.TDC())+1)) * 1e3;
  hash *= (long)(hit.ADC()+1)             * 1e2;
  hash += (long)(hit.Plane()+1)           * 1e1;
  hash *= (long)(hit.Cell()+1)            * 1e0;

  return hash;
}
