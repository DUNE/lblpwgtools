/////////////////////////////////////////////////////////////////////////////////
// Sample.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, April 2021
//
// Object containing information on a particular data sample.
// Adapted from NOvA CAFAna framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// CAFAna includes
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/SystShifts.h"

// c++ includes
#include <vector>
#include <string>
#include <sstream>

namespace ana {

  // Forward declarations
  class Spectrum;
  class IPrediction;

  namespace covmx {

    // Define some useful enums
    enum Selection { kCCNumu, kCCNue, kNC, kNoSel };
    enum Polarity  { kFHC, kRHC, kNoPol };
    enum Detector  { kNearDet, kFarDet, kNoDet };

    // Sample class
    class Sample {

    public:

      // Constructors
      Sample(Selection s, Polarity p, Detector d);
      Sample(const Sample& sample);
      Sample(unsigned int id);

      // Destructor
      ~Sample();

      std::string GetSelection() const;
      std::string GetPolarity()  const;
      std::string GetDetector()  const;
      std::string GetName()      const;
      std::string GetTag()       const;
      std::string GetLatexName() const;

      void SetAxis(const HistAxis* a) { axis     = a; };
      void SetCut(const Cut* c)       { cut      = c; };
      void SetPOT(double d)           { pot      = d; };
      void SetLivetime(double l)      { livetime = l; };

      void SetPrediction(IPrediction* p);
      void SetData(Spectrum* s);

      void SetSystAlias(const ISyst* key, const ISyst* val) { systMap[key] = val; };

      const HistAxis*  GetAxis()       const;
      Binning          GetBinning()    const;
      const Cut*       GetCut()        const;
      double           GetPOT()        const;
      double           GetLivetime()   const;

      IPrediction* GetPrediction() const;
      Spectrum*    GetData()       const;

      SystShifts   GetSystShifts(SystShifts shifts) const;

      bool HasPrediction() const { return (prediction!=nullptr); };
      void ResetPrediction()     { prediction = nullptr;         };
      bool HasData() const       { return (data!=nullptr);       };
      void ResetData()           { data = nullptr;               };

      unsigned int GetID() const;
      static std::string EnsembleID(const std::vector<Sample>& samples);
      static std::vector<Sample> FromEnsembleID(std::string id);

      bool operator< (const Sample& lhs) const {
	return lhs.GetID() < this->GetID();
      }

      bool operator==(const Sample& lhs) {
        return lhs.GetID() == this->GetID();
      }

      bool operator!=(const Sample& lhs) {
	return lhs.GetID() != this->GetID();
      }

      // Mandatory member variables
      Selection selection;
      Polarity polarity;
      Detector detector;

    private:

      // Optional member variables
      const HistAxis* axis;
      const Cut* cut;
      double pot;
      double livetime;

      IPrediction* prediction;
      Spectrum* data;

      std::map<const ISyst*, const ISyst*> systMap;

    }; // class Sample

  } // namespace covmx

} // namespace ana
