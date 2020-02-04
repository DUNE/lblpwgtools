////////////////////////////////////////////////////////////////////////
/// \brief   Generate unique hashes for reco products
/// \author  Justin Vasel <justin.vasel@gmail.com>
/// \date    December 2018
////////////////////////////////////////////////////////////////////////
#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include "RecoBase/CellHit.h"

namespace util {
  class HashFunctions {
  public:
    HashFunctions();
    ~HashFunctions();

    /// \brief Compute a unique hash value give a \ref rb::CellHit.
    ///
    /// \param      Any \ref rb::CellHit
    /// \return     A hash value that uniquely identifies this hit
    long Generate(rb::CellHit hit);
  };
}

#endif
