/////////////////////////////////////////////////////////////////////////////////
// Sample.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, April 2021
//
// Object containing information on a particular data sample.
// Adapted from NOvA CAFAna framework.
/////////////////////////////////////////////////////////////////////////////////

// CAFAna includes
#include "CAFAna/Core/Sample.h"
#include "CAFAna/Core/Registry.h"

// c++ includes
#include <iostream>
#include <cassert>

using std::ostringstream;
using std::string;
using std::vector;

namespace ana {

  namespace covmx {

    //-------------------------------------------------------------------------
    Sample::Sample(Selection s, Polarity p, Detector d)
      : selection(s), polarity(p), detector(d), axis(nullptr), cut(nullptr),
	pot(-1), livetime(-1),
	prediction(nullptr), data(nullptr) {
    }

    //-------------------------------------------------------------------------
    Sample::Sample(Sample const& sample)
      : selection(sample.selection), polarity(sample.polarity),
	detector(sample.detector), axis(sample.axis), cut(sample.cut),
	pot(sample.pot), livetime(sample.livetime),
	prediction(sample.prediction), data(sample.data),
	systMap(sample.systMap) {
    }

    //-------------------------------------------------------------------------
    Sample::Sample(unsigned int id)
      : axis(nullptr), cut(nullptr), pot(-1), livetime(-1),
	prediction(nullptr), data(nullptr) {
      selection = (covmx::Selection)((id & (3 << 7)) >> 7);
      polarity  = (covmx::Polarity) ((id & (3 << 5)) >> 5);
      detector  = (covmx::Detector) ((id & (3 << 3)) >> 3);
    }

    //-------------------------------------------------------------------------
    Sample::~Sample() {
    }

    //-------------------------------------------------------------------------
    string Sample::GetSelection() const {
      vector<string> sel = { "numusel", "nuesel", "ncsel" };
      return sel[selection];
    }

    //-------------------------------------------------------------------------
    string Sample::GetPolarity() const {
      vector<string> pol = { "fhc", "rhc" };
      return pol[polarity];
    }

    //-------------------------------------------------------------------------
    string Sample::GetDetector() const {
      vector<string> pol = { "neardet", "fardet" };
      return pol[detector];
    }

    //-------------------------------------------------------------------------
    string Sample::GetName() const {
      ostringstream name;
      name << GetSelection() << " " << GetPolarity() << " " << GetDetector();
      return name.str();
    }

    //-------------------------------------------------------------------------
    string Sample::GetLatexName() const {
      ostringstream name;
      vector<string> sel = { "CC $\\nu_{\\mu}$", "CC $\\nu_{e}$", "NC" };
      name << sel[selection];
      vector<string> pol = { "FHC", "RHC" };
      name << " " << pol[polarity];
      vector<string> det = { "ND", "FD" };
      name << " " << det[detector];
      return name.str();
    }

    //-------------------------------------------------------------------------
    string Sample::GetTag() const {
      ostringstream tag;
      tag << GetSelection() << "_" << GetPolarity() << "_" << GetDetector();
      return tag.str();
    }

    //-------------------------------------------------------------------------
    void Sample::SetPrediction(IPrediction* p) {
      prediction = p;
    }

    //-------------------------------------------------------------------------
    void Sample::SetData(Spectrum* s) {
      data = s;
    }

    //-------------------------------------------------------------------------
    const HistAxis* Sample::GetAxis() const {
      if (!axis) assert(false && "Axis not set in sample!");
      return axis;
    }

    //-------------------------------------------------------------------------
    Binning Sample::GetBinning() const {
      if (!axis) assert(false && "Axis not set in sample!");
      return axis->GetBinnings()[0];
    }

    //-------------------------------------------------------------------------
    const Cut* Sample::GetCut() const {
      if (!cut) assert(false && "Cut not set in sample!");
      return cut;
    }

    //-------------------------------------------------------------------------
    double Sample::GetPOT() const {
      if (pot == -1) assert(false && "POT not set in sample!");
      return pot;
    }

    //-------------------------------------------------------------------------
    double Sample::GetLivetime() const {
      if (livetime == -1) assert(false && "Livetime not set in sample!");
      return livetime;
    }

    //-------------------------------------------------------------------------
    IPrediction* Sample::GetPrediction() const {
      // For what it's worth, I know having a const member function that
      // returns a bare pointer to a member is extremely not good. If you
      // feel strongly about this, please come yell at me and perhaps you'll
      // inspire me to be better
      if (!prediction) assert(false && "Prediction not set in sample!");
      return prediction;
    }

    //-------------------------------------------------------------------------
    Spectrum* Sample::GetData() const {
      if (!data) assert(false && "Data spectrum not set in sample!");
      return data;
    }

    //-------------------------------------------------------------------------
    SystShifts Sample::GetSystShifts(SystShifts shifts) const {
      SystShifts ret;
      for (const ISyst* syst : shifts.ActiveSysts()) {
	if (systMap.count(syst)) {
	  if (systMap.at(syst)) { // If there's a nullptr here, skip it
	    ret.SetShift(systMap.at(syst), shifts.GetShift(syst));
	  }
	} else {
	  ret.SetShift(syst, shifts.GetShift(syst));
	}
      }
      return ret;
    } // function Sample::GetSystShifts

    //-------------------------------------------------------------------------
    unsigned int Sample::GetID() const {
      unsigned int id = selection;
      id <<= 2;
      id += polarity;
      id <<= 2;
      id += detector;
      return id;
    } // function Sample::GetID

    //-------------------------------------------------------------------------
    string Sample::EnsembleID(const vector<Sample>& samples) {
      ostringstream oss;
      oss << "id";
      for (const Sample& s : samples)
	oss << "_" << s.GetID();
      return oss.str();
    } // function Sample::EnsembleID

    //-------------------------------------------------------------------------
    vector<Sample> Sample::FromEnsembleID(string id) {
      vector<Sample> ret;
      size_t start = 3;
      while (true) {
	size_t end = id.find("_", start);
	unsigned int val = stoi(id.substr(start, end));
	ret.push_back(Sample(val));
	if (end == string::npos) break;
	start = end + 1;
      }
      return ret;
    } // function Sample::FromEnsembleID

  } // namespace covmx

} // namespace ana
