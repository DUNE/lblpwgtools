////////////////////////////////////////////////////////////////////////
/// \brief  Reject triggers that are too close to beam spills
/// \author Christopher Backhouse - bckhouse@caltech.edu
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <time.h>

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "fhiclcpp/ParameterSet.h"

#include "Geometry/Geometry.h"
#include "NovaTimingUtilities/TimingUtilities.h"
#include "RawData/RawTrigger.h"

#include "TH1.h"

#include "IFBeam_service.h"

namespace util
{
  /// Reject triggers that are too close to beam spills
  class RemoveBeamSpills: public art::EDFilter
  {
  public:
    explicit RemoveBeamSpills(const fhicl::ParameterSet& pset);
    virtual ~RemoveBeamSpills();

    void beginJob();
    bool filter(art::Event& evt);

  protected:
    std::string fRawDataLabel;
    double fVetoWindow;

    int fTotTrigs, fRejTrigs;

    std::unique_ptr<BeamFolder> fFolder;

    TH1* fClosest;
  };

  //..............................................................
  RemoveBeamSpills::RemoveBeamSpills(const fhicl::ParameterSet& pset)
    : fTotTrigs(0), fRejTrigs(0)
  {
    fRawDataLabel = pset.get<std::string>("RawDataLabel");
    fVetoWindow = pset.get<double>("VetoWindow");

    art::ServiceHandle<ifbeam_ns::IFBeam> ifbeam;
    fFolder = ifbeam->getBeamFolder(pset.get<std::string>("Bundle"),
                                    pset.get<std::string>("URL"),
                                    pset.get<double>("DBTimeWindow"));
    // Look this far back in time for a spill, in seconds
    fFolder->set_epsilon(1.5);
  }

  //......................................................................
  RemoveBeamSpills::~RemoveBeamSpills()
  {
    if(fTotTrigs)
      std::cout << "RemoveBeamSpills: of " << fTotTrigs
                << " triggers, rejected "
                << fRejTrigs << " ("
                << .01*((10000*fRejTrigs)/fTotTrigs) << "%)" << std::endl;
  }

  //......................................................................
  void RemoveBeamSpills::beginJob()
  {
    art::ServiceHandle<art::TFileService> tfs;
    fClosest = tfs->make<TH1F>("closest", ";#Deltat (sec)", 2000, -.01, +.01);
  }

  //......................................................................
  bool RemoveBeamSpills::filter(art::Event& evt)
  {
    // Doesn't apply to MC
    if(!evt.isRealData()) return true;

    ++fTotTrigs;

    // Get trigger information to extract trigger time
    art::Handle<std::vector<rawdata::RawTrigger>> trigv;
    evt.getByLabel(fRawDataLabel, trigv);
    assert(!trigv->empty());
    const rawdata::RawTrigger& trig = (*trigv)[0];

    struct timespec unixTime;
    novadaq::timeutils::convertNovaTimeToUnixTime(trig.fTriggerTimingMarker_TimeStart, unixTime);

    const double eventTime = unixTime.tv_sec + 1e-9*unixTime.tv_nsec;

    // Time since most recent $A9 (seconds)
    double ea9snc;
    try{
      // Seems to be fairly sensitive to the exact fudge factor here, 0.55
      // works. but 0.5 very occasionally fails.
      fFolder->GetNamedData(eventTime+.55, "G:EA9SNC", &ea9snc);
    }
    catch(...){
      mf::LogError("RemoveBeamSpills") << "Exception getting spill times. Aborting";
      abort();
    }

    double tof = 0; // Time-of-flight correction
    art::ServiceHandle<geo::Geometry> geom;
    if(geom->DetId() == novadaq::cnv::kFARDET) tof = 810e3/3e8; // 810km/c

    // Find DB time closest to event time
    double closest = FLT_MAX;

    // Rounding can sometimes pick the wrong second
    for(int fudge = 0; fudge <= +1; ++fudge){
      const double dt = eventTime - (round(eventTime) + fudge - ea9snc) - tof;
      if(fabs(dt) < fabs(closest)) closest = dt;
    }

    if(trig.fTriggerMask_TriggerType == 0 && fabs(closest) > fVetoWindow){
      mf::LogWarning("RemoveBeamSpills") << "This is a NuMI trigger, but the closest spill is "
                                         << closest << " seconds away.";
    }

    fClosest->Fill(closest);

    const bool reject = (fabs(closest) < fVetoWindow);
    if(reject) ++fRejTrigs;

    return !reject;
  }

  DEFINE_ART_MODULE(RemoveBeamSpills)

} // end namespace ifdb
