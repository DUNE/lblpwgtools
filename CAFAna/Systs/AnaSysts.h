#pragma once

#include "CAFAna/Core/ISyst.h"

class TH1D;

extern size_t NFluxParametersToAddToStatefile;

namespace ana
{

  std::vector<std::string> SplitString(std::string input, char delim = ' ');

  void KeepSysts(std::vector<const ISyst *> &systlist, std::vector<std::string> const &systsToInclude);

  void KeepSysts(std::vector<const ISyst *> &systlist, std::vector<const ISyst *> const &systsToInclude);

  void RemoveSysts(std::vector<const ISyst *> &systlist, std::vector<std::string> const &namesToRemove);

  /// Get a vector of all the analysis group systs
  std::vector<const ISyst*> getDetectorSysts(bool useFD=true, bool useND=false, bool useNueOneE=false);

#ifdef BUILD_XSEC_SYSTS
  std::vector<const ISyst*> getReducedXSecSysts(bool fluxXsecPenalties);
#endif

#ifdef BUILD_FDS_SYSTS
  std::vector<const ISyst*> getFakeDataSysts(bool fluxXsecPenaltiess);
#endif

  std::vector<const ana::ISyst *> GetListOfSysts(bool fluxsyst_Nov17 = true, bool xsecsyst = true,
						 bool detsyst = true, bool useND = true, bool useFD = true,
						 bool useNueOnE = false, bool useFakeDataDials = true,
						 bool fluxsyst_CDR = true, bool fluxsyst_Sept21 = false,
						 int NFluxSysts = NFluxParametersToAddToStatefile,
						 bool removeFDNonFitDials = false);

  std::vector<const ana::ISyst *> GetListOfSysts(std::string systString,
						 bool useND = true,
						 bool useFD = true,
						 bool useNueOnE = false);

  std::vector<const ana::ISyst *> GetListOfSysts(char const *systCString,
						 bool useND = true,
						 bool useFD = true,
						 bool useNueOnE = false);

  /// Put a list of systematics in the 'standard' order
  std::vector<const ana::ISyst *> OrderListOfSysts(std::vector<const ana::ISyst *> const &systlist);

}
