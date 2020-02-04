/*
 * ROOTHelpers.h
 *
 *  Helper functions convenient in ROOT macros
 *
 *  Created on: Jan 18, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef UTILITIES_FUNC_ROOTHELPERS_H_
#define UTILITIES_FUNC_ROOTHELPERS_H_

#include <string>
#include <vector>

#include "TObject.h"

namespace util
{

  inline void SaveObj(const TObject * obj,
             const std::string & filenameStub,
             const std::string & dirName="",
             const std::vector<std::string> exts = {".png", ".eps", ".root"},
             bool silent=false)
  {
      std::string fullFname = (dirName.size() ? (dirName + "/") : "") + filenameStub;
      auto prevOutputFlag = gErrorIgnoreLevel;
      if (silent)
          gErrorIgnoreLevel = kWarning;
      for (const auto & ext : exts)
          obj->SaveAs( (fullFname + ext).c_str() );
      if (silent)
          gErrorIgnoreLevel = prevOutputFlag;
  }

}
#endif /* UTILITIES_FUNC_ROOTHELPERS_H_ */
