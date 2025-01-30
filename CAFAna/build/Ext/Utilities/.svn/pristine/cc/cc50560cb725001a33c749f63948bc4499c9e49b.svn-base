#include "Utilities/func/EnvExpand.h"

#include <wordexp.h>

namespace util
{
  //......................................................................
  std::string EnvExpansion(const std::string& inString)
  {
    std::string outString = inString;
    
    wordexp_t out;
    int status = wordexp(inString.c_str(), &out, WRDE_SHOWERR);
    if(status == 0){
      outString = "";
      for (unsigned int idx = 0; idx < out.we_wordc; ++idx){
	std::string tmp(out.we_wordv[idx]);
	outString.append(tmp);
      }
    }
    wordfree(&out);
    return outString;
  }

} // namespace
