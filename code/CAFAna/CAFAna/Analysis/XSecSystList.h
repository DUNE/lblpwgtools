#pragma once

#include <map>
#include <string>
#include <vector>

namespace ana {
/// All dial names that should go into a state file
std::vector<std::string> const &GetAllXSecSystNames();
/// All dials used as freedoms in standard fits
std::vector<std::string> const &GetFitSystNames();
/// Dials which should not be used as freedoms (CV weights if they exist removed
/// in SpectrumLoader)
std::vector<std::string> const &GetDoNotIncludeSystNames();
/// Dials which have an extrapolated response outside of 0->1
std::vector<std::string> const &GetExtrapolateOffToOnSystNames();
/// Dials used to generate fake data sets when set to 1, should not be fit.
std::vector<std::string> const &GetFakeDataGenerationSystNames();

int GetXSecSystIndex(std::string const &name);
std::string GetXSecSystName(int);

/// Check if a dial name is in a list
bool SystNameIsInList(std::string const &, std::vector<std::string> const &);
/// Convenience method for checking if a dial name is
bool IsExtrapolateOffToOnSyst(std::string const &);
bool IsExtrapolateOffToOnSyst(int);
bool IsDoNotIncludeSyst(std::string const &);
bool IsDoNotIncludeSyst(int);
bool IsFakeDataGenerationSyst(std::string const &);
bool IsFakeDataGenerationSyst(int);

std::pair<double, double> GetXSecSystLimits(const std::string &name);
double GetXSecSystMax(int index);
double GetXSecSystMin(int index);

} // namespace ana
