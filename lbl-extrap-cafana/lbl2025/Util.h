
#ifndef LBL2025_UTIL_H
#define LBL2025_UTIL_H

#include <string>
#include <vector>

class TCanvas;

namespace lbl2025
{
	void SaveCanvas(TCanvas & c, const std::string & filestub, const std::vector<std::string> & plotexts={"png", "svg"});
}

#endif //LBL2025_UTIL_H
