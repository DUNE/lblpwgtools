#pragma once

#include <tuple>
#include <map>
#include <string>

#include "CAFAna/Core/Binning.h"

class TAxis;
class TH1D;
class TH2D;

namespace ana
{
  /// \brief Helper for \ref Spectrum
  ///
  /// ROOT's handling of allocations, and especially deletions, can be very
  /// slow. It keeps everything in a big map that it then has to lookups
  /// in. This class provides a simple cache of histograms, recycling an old
  /// histogram of the same binning instead of creating a new one.
  ///
  /// Allocate new histograms with \ref New, and return them to the cache with
  /// \ref Delete. At present only TH1Ds with simple binning are supported.
  class HistCache
  {
  public:
    static TH1D* New(const std::string& title, const Binning& bins);
    static TH1D* New(const std::string& title, const TAxis* bins);
    static TH1D* Copy(TH1D* h);

    /// WARNING - all the TH2D functions can only be used with kTrueEnergyBins
    /// on the y-axis for now.
    static TH2D* NewTH2D(const std::string& title, const Binning& bins);
    static TH2D* NewTH2D(const std::string& title, const TAxis* bins);
    static TH2D* Copy(TH2D* h);

    static void Delete(TH1D*& h);
    static void Delete(TH2D*& h);
    static void PrintStats();
    static void ClearCache();
  protected:
    typedef std::tuple<int, double, double> Key_t;
    static Key_t Key(const TAxis* bins);
    static Key_t Key(const Binning& bins);

    static TH1D* New(const std::string& title, Key_t key);
    static TH2D* NewTH2D(const std::string& title, Key_t key);

    static void CheckMemoryUse();

    static std::multimap<Key_t, std::unique_ptr<TH1D>> fgMap;
    static std::multimap<Key_t, std::unique_ptr<TH2D>> fgMap2D;

    static int fgOut, fgIn;

    static long fgEstMemUsage;
  };
}
