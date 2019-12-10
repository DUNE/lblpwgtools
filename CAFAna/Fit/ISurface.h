#pragma once

#include <limits>
#include <map>
#include <vector>

#include "TAttMarker.h"

#include "CAFAna/Core/FwdDeclare.h"

class TGraph;
class TH2;
class TH2F;

namespace ana
{
  class ISurface
  {
    public:
      ISurface()
        : fBestLikelihood(std::numeric_limits<double>::signaling_NaN()),
          fBestFitX(std::numeric_limits<double>::signaling_NaN()),
          fBestFitY(std::numeric_limits<double>::signaling_NaN()),
          fHist(nullptr)
      {}

      virtual ~ISurface() {};

      double BestLikelihood() const {return fBestLikelihood;}
      double GetBestFitX() const {return fBestFitX;}
      double GetBestFitY() const {return fBestFitY;}

      /// Draw the surface itself
      void Draw() const;

      /// Draw the best fit point
      void DrawBestFit(Color_t color, Int_t marker= kFullCircle) const;

      /// \param fc Surface to compare against for this significance level
      /// \param style Line style for TAttLine, solid, dotted, dashed etc
      /// \param color Line color for TAttLine
      /// \param minchi \f$\chi^2\f$ of best fit to compare against.
      ///               Default: best fit from this surface.
      void DrawContour(TH2* fc, Style_t style, Color_t color,
                       double minchi = -1);

      /// For expert use, custom painting of contours
      std::vector<TGraph*> GetGraphs(TH2* fc, double minchi = -1);


      TH2* ToTH2(double minchi = -1) const;

      void SetTitle(const char* str);

    protected:
      void SaveTo(TDirectory * dir) const;
      static void FillSurfObj(ISurface & surf, TDirectory * dir);

      double fBestLikelihood;
      double fBestFitX;
      double fBestFitY;
      TH2F* fHist;
      std::vector<double> fSeedValues;

      void EnsureAxes() const;
  };

  /// Helper function for the gaussian approximation surfaces
  TH2* Flat(double level, const ISurface& s);
}

