////////////////////////////////////////////////////////////////////////
/// \brief  Fake the SpillData object in an MC file to set the isRHC field
/// \author bckhouse@fnal.gov
/// \date
////////////////////////////////////////////////////////////////////////

// Framework includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"

// NOvA includes
#include "SummaryData/SpillData.h"
#include "Metadata/MetadataManager.h"

namespace util {
  class FakeSpillData : public art::EDProducer
  {
  public:
    explicit FakeSpillData(const fhicl::ParameterSet& pset);
    virtual ~FakeSpillData();                        
    void produce(art::Event& evt);

  protected:
    bool fForcePolarityForward;
    bool fForcePolarityReverse;
  };

  //___________________________________________________________________________
  FakeSpillData::FakeSpillData(const fhicl::ParameterSet& pset)
    : fForcePolarityForward(pset.get<bool>("ForcePolarityForward")),
      fForcePolarityReverse(pset.get<bool>("ForcePolarityReverse"))
  {
    produces< sumdata::SpillData >();

    assert(!(fForcePolarityForward && fForcePolarityReverse));
  }

  //___________________________________________________________________________
  FakeSpillData::~FakeSpillData()
  {  
  }

  //___________________________________________________________________________
  void FakeSpillData::produce(art::Event& evt)
  {
    std::unique_ptr<sumdata::SpillData> sd(new sumdata::SpillData);

    // Fill in some typical values to hopefully keep good beam cuts happy
    sd->spillpot = 0;
    sd->goodbeam = true;
    sd->deltaspilltimensec = 0;
    sd->hornI = -199.5;
    sd->posx = 0.9;
    sd->posy = 1.3;
    sd->widthx = 1.2;
    sd->widthy = 1.2;

    if(fForcePolarityForward){
      sd->isRHC = false;
    }
    else if(fForcePolarityReverse){
      sd->isRHC = true;
    }
    else{
      meta::MetadataManager& fManager = meta::MetadataManager::getInstance();
      const std::string current = fManager.GetMetadata()["nova.hornpolarity"];
      sd->isRHC = (current == "rhc");
    }

    evt.put(std::move(sd));
  }

  DEFINE_ART_MODULE(FakeSpillData)

} // namespace
