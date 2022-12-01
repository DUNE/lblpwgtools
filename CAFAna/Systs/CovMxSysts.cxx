#include "CAFAna/Systs/CovMxSysts.h"

// root
#include "TKey.h"
#include "TVectorD.h"

// stl
#include <iostream>
#include <sstream>

namespace ana {
  using std::cout;
  using std::endl;
  using std::make_pair;
  using std::make_unique;
  using std::map;
  using std::ostringstream;
  using std::pair;
  using std::runtime_error;
  using std::string;
  using std::unique_ptr;

  // -------------------------------------------------------------------------------------
  NuISyst::NuISyst(string name, string label, string sampleName,
		   map<int, pair<TH1*, TH1*>> shifts) :
    ISyst(name, label),
    fSampleName(sampleName),
    fShifts(shifts) {
  }

  // -------------------------------------------------------------------------------------
  NuISyst::~NuISyst() {
    // Clean up the weight histograms
    for (auto shift : fShifts) {
      if (shift.second.first)  delete shift.second.first;
      if (shift.second.second) delete shift.second.second;
    }
  } // NuISyst destructor

  // -------------------------------------------------------------------------------------
  /// Return base name of systematic
  string NuISyst::BaseName() const {
    if (fSampleName.empty()) return ShortName(); // If there's no sample prefix to remove
    return ShortName().substr(fSampleName.size()+1);
  }

  // -------------------------------------------------------------------------------------
  /// SaveTo implementation for NuISyst
  void NuISyst::SaveTo(TDirectory* dir, string type) const {

    cout << "NuISyst::SaveTo implementation is a temporary hack that should be replaced once CommonAna migration is complete." << endl;

    TDirectory* tmp = gDirectory;
    dir->cd();
    TObjString(type.c_str()).Write("type");
    TObjString(ShortName().c_str()).Write("name");
    TObjString(LatexName().c_str()).Write("label");
    TObjString(fSampleName.c_str()).Write("samplename");
    TDirectory* sigmasDir = dir->mkdir("Sigmas");

    // Save each shift
    for (auto const& sigma : fShifts) {

      TDirectory* sigmaDir = sigmasDir->mkdir(Form("Sigma%d", sigma.first));

      // Save sigma value
      TVectorD vSigma(1);
      vSigma[0] = sigma.first;
      sigmaDir->WriteTObject(&vSigma, "sigma");

      // Save shift histograms
      sigmaDir->WriteTObject(sigma.second.first, "sig");
      sigmaDir->WriteTObject(sigma.second.second, "bkg");

    }

    tmp->cd();

  } // function NuISyst::SaveTo

  // -------------------------------------------------------------------------------------
  /// Get weight for a given energy, channel and sigma shift
  double NuISyst::WeightFor(SystChannel channel, double sigma, double calE) const {
    // If it's a one-sided shift (ie. no -1 sigma weights) then we want to return
    // 1 for a negative sigma
    if (sigma < 0 and not fShifts.count(-1)) return 1;
    const int bin = fShifts.at(0).first->FindBin(calE);

    auto low = fShifts.begin();
    auto end = fShifts.end();
    std::advance(end, -2);
    if (sigma < fShifts.begin()->first)
      low = fShifts.begin();
    else if (sigma >= end->first)
      low = end;
    else {
      for (auto it = fShifts.begin(); it != end; ++it) {
	if (sigma >= it->first) {
	  low = it;
	  break;
	}
      }
    }

    auto high = low;
    ++high;

    // // Why would we have templates differing by more than 1 sigma?
    // // fracpart below assumes this
    assert(high->first - low->first == 1);
    TH1* hLow = nullptr;
    TH1* hHigh = nullptr;

    if (channel == SystChannel::kSig) {
      hLow = low->second.first;
      hHigh = high->second.first;
    }
    else {
      hLow = low->second.second;
      hHigh = high->second.second;
    }

    const double fracpart = sigma - low->first;
    const double ret = (fracpart*hHigh->GetBinContent(bin)) +
      ((1-fracpart)*hLow->GetBinContent(bin));

    return std::max(0., ret); // Keep the LL from blowing up

  } // function NuISyst::WeightFor

  // -------------------------------------------------------------------------------------
  KeySyst::KeySyst(string name, string label)
    : ISyst(name, label), fOneSided(false) {
  }

  // -------------------------------------------------------------------------------------
  void covmx::NormSyst::Shift(double sigma, caf::SRProxy* sr, double& weight) const {
    weight *= 1 + (sigma * fScale);
    if (weight < 0) weight = 0;
  }

} // namespace ana
