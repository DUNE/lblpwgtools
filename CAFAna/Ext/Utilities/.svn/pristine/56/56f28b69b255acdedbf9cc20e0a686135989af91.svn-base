////////////////////////////////////////////////////////////////////////
/// \brief   Generate unique hashes for reco products
/// \author  Justin Vasel <justin.vasel@gmail.com>
/// \date    December 2018
////////////////////////////////////////////////////////////////////////

#include <cstdint>

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
uint64_t util::HashFunctions::Generate(rawdata::RawDigit h)
{
  return (uint64_t)h.TDC() + ((uint64_t)h.DaqChannel() << 32);
}

// ............................................................................
uint64_t util::HashFunctions::Generate(rb::CellHit h)
{
  // First 32 bits are daq channel, second 32 bits are TDC:
  // TDC        ____ ____ xxxx xxxx
  // DaqChannel xxxx xxxx ____ ____
  return (uint64_t)h.TDC() + ((uint64_t)h.DaqChannel() << 32);
}
