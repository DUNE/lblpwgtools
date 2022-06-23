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
    double fVetoWindow, fEpsilon;

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
    fEpsilon = pset.get<double>("Epsilon");

    art::ServiceHandle<ifbeam_ns::IFBeam> ifbeam;
    fFolder = ifbeam->getBeamFolder(pset.get<std::string>("Bundle"),
                                    pset.get<std::string>("URL"),
                                    pset.get<double>("DBTimeWindow"));

    // Look this far back in time for a spill, in seconds
    fFolder->set_epsilon(fEpsilon);
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

    // Get trigger time in unix time
    struct timespec unixTime;
    novadaq::timeutils::convertNovaTimeToUnixTime(trig.fTriggerTimingMarker_TimeStart, unixTime);
    const double eventTime = unixTime.tv_sec + 1e-9*unixTime.tv_nsec;

    // If we're in the far detector, apply a time-of-flight correction to the trigger time
    double tof = 0;
    art::ServiceHandle<geo::Geometry> geom;
    if (geom->DetId() == novadaq::cnv::kFARDET) tof = 810e3/3e8; // 810km/c
    const double eventTimeCorrected = eventTime - tof;

    // Query the database to get NuMI trigger time. This query gives the EA9SNC value, which is
    // a value stored to the database once per second that represents the time in seconds since
    // the last trigger. Here we fetch both "ea9snc" (time since last trigger) and "time" (time
    // value was stored to the database), and so "time - ea9snc" gives us the absolute time of
    // the most recent trigger once per second.
    //
    // Note that we round the trigger time up to the closest second by calling ceil when querying
    // the database below. This is intentional and makes ifbeam queries more robust. Querying the
    // database can be a little fragile sometimes, and we can simplify it by making two assumptions:
    //
    //   1) the NuMI spill occurs at a rate less than 1Hz
    //   2) Values are written to the database once per second, on the second
    //
    // If we round up the trigger time to the next second, then we'll pull out the value from the
    // database that comes in at the _end_ of the second in which the trigger occurred. We then
    // use the ea9snc value to look backwards in time to the most recent trigger; if the value is
    // <1s then there's a chance it will coincide with the trigger and we veto it. The value stored
    // in the database at the _start_ of the second in which the trigger occurs is not going to be
    // useful to us, since the ea9snc value can only refer backwards in time to before our trigger
    // occurred.

    double ea9snc, time;
    try{
      fFolder->GetNamedData(ceil(eventTimeCorrected), "G:EA9SNC@", &ea9snc, &time);
    }
    catch(std::exception& e) {
      std::string err(e.what());
      if (err.find("variable not found")) {
        const time_t t = eventTimeCorrected;
        mf::LogError("RemoveBeamSpills") << "Couldn't find anything in ifbeam db! Timestamp: "
          << std::ctime(&t);
      } else {
        mf::LogError("RemoveBeamSpills") << "Exception encountered: " << e.what();
      }
      abort();
    }
    double deltaT = fabs(eventTimeCorrected - (time - ea9snc));
    bool reject = deltaT < fVetoWindow;
    if (reject) {
      ++fRejTrigs;
      mf::LogInfo("RemoveBeamSpills") << "Vetoed event with trigger time "
        << std::fixed << eventTimeCorrected << " due to spill at time "
        << time-ea9snc << " (deltaT = " << deltaT << ")" << std::defaultfloat;
    }
    return !reject;

  }

  DEFINE_ART_MODULE(RemoveBeamSpills)

} // end namespace ifdb
