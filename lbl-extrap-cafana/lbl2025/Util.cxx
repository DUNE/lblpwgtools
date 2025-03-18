
#include "lbl2025/Util.h"

#include "TCanvas.h"

namespace lbl2025
{
	void SaveCanvas(TCanvas & c, const std::string & filestub, const std::vector<std::string> & plotext)
	{
		for (const auto & ext : plotext)
			c.SaveAs( Form("%s.%s", filestub.c_str(), ext.c_str()) );
	}

}