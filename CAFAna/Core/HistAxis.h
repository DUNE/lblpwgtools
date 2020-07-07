#pragma once

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include <string>

namespace ana
{
  /// \brief Collect information describing the x-axis of an analysis histogram
  ///
  /// That is, what it should be labelled, what the binning should be, and what
  /// variable will be being filled into it.
  template<class T> class GenericHistAxis
  {
  public:
    // The one constructor to rule them all
    GenericHistAxis(const std::vector<std::string>& labels,
                    const std::vector<Binning>& bins,
                    const std::vector<T>& vars = {});

    // Forwards
    GenericHistAxis(const std::string& label,
                    const Binning& bins,
                    const T& var)
      : GenericHistAxis(std::vector<std::string>(1, label),
                        std::vector<Binning>(1, bins),
                        std::vector<T>(1, var))
    {
    }

    GenericHistAxis(const std::string& label,
                    const Binning& bins)
      : GenericHistAxis(std::vector<std::string>(1, label),
                        std::vector<Binning>(1, bins))
    {
    }

    GenericHistAxis(const std::string& labelX,
                    const Binning& binsX,
                    const T& varX,
                    const std::string& labelY,
                    const Binning& binsY,
                    const T& varY)
      : GenericHistAxis({labelX, labelY}, {binsX, binsY}, {varX, varY})
    {
    }

    /// Shortcut for simple binnings
    GenericHistAxis(const std::string& label,
                    int nx, double x0, double x1,
                    const T& var)
      : GenericHistAxis(label, Binning::Simple(nx, x0, x1), var)
    {
    }

    /// Shortcut for simple binnings
    GenericHistAxis(const std::string& labelX,
                    int nx, double x0, double x1,
                    const T& varX,
                    const std::string& labelY,
                    int ny, double y0, double y1,
                    const T& varY)
      :  GenericHistAxis(labelX, Binning::Simple(nx, x0, x1), varX,
                         labelY, Binning::Simple(ny, y0, y1), varY)
    {
    }

    GenericHistAxis(const GenericHistAxis<T>& ax) = default;

    GenericHistAxis(const std::vector<GenericHistAxis<T>>& axes);

    GenericHistAxis(const GenericHistAxis<T>& xax,
                    const GenericHistAxis<T>& yax)
      : GenericHistAxis(std::vector<GenericHistAxis<T>>{xax, yax})
    {
    }

    GenericHistAxis(const GenericHistAxis<T>& xax,
                    const GenericHistAxis<T>& yax,
                    const GenericHistAxis<T>& zax)
      : GenericHistAxis(std::vector<GenericHistAxis<T>>{xax, yax, zax})
    {
    }

    unsigned int NDimensions() const{return fLabels.size();}

    const std::vector<std::string>& GetLabels() const {return fLabels;}
    const std::vector<Binning>& GetBinnings() const {return fBins;}
    const std::vector<T>& GetVars() const {return fVars;}

    /// A variable "flattening" all the dimensions into one 1D value. Use
    /// sparingly.
    T GetVar1D() const;

    /// Appropriate binning and labelling for that 1D Var
    const Binning& GetBins1D() const;
    const std::string& GetLabel1D() const;

  protected:
    std::vector<std::string> fLabels;
    std::vector<Binning> fBins;
    std::vector<T> fVars;

    mutable std::optional<Binning> fBins1D;
    mutable std::optional<std::string> fLabel1D;
  };

  typedef GenericHistAxis<Var> HistAxis;
  typedef GenericHistAxis<SpillVar> SpillHistAxis;
  typedef GenericHistAxis<SpillTruthVar> SpillTruthHistAxis;
}
