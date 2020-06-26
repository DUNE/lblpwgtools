#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Systs/Systs.h"

class TH1D;

namespace ana
{

	/// Get a vector of all the analysis group systs
	std::vector<const ISyst*> getDetectorSysts(bool useFD=true, bool useND=false, bool useNueOneE=false);

}