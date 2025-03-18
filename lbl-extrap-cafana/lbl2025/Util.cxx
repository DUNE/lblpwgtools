
#include "lbl2025/Util.h"

#include <DUNEStyle.h>

#include "TCanvas.h"
#include "TH1.h"

namespace lbl2025
{
	void SaveCanvas(TCanvas & c, const std::string & filestub, const std::vector<std::string> & plotext)
	{
#ifdef LBL2025_USE_DUNESTYLE
		for (TObject * obj : *c.GetListOfPrimitives())
		{
			auto h = dynamic_cast<TH1*>(obj);
			if ( !h )
				continue;
			dunestyle::CenterTitles(h);
		}
#endif

		for (const auto & ext : plotext)
			c.SaveAs( Form("%s.%s", filestub.c_str(), ext.c_str()) );
	}

}