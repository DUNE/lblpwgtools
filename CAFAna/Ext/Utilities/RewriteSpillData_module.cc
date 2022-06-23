////////////////////////////////////////////////////////////////////////
/// \brief  Rewrite the SpillData object in an MC file to set the isRHC field
/// \author edniner@fnal.gov
/// \date
////////////////////////////////////////////////////////////////////////

// Framework includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"

// NOvA includes
#include "Geometry/Geometry.h"
#include "SummaryData/SpillData.h"
#include "Metadata/MetadataManager.h"

namespace util {

  /// A module to store a geometry in a file
  class RewriteSpillData : public art::EDProducer {

  public:

    explicit RewriteSpillData(fhicl::ParameterSet const &pset);
    virtual ~RewriteSpillData();                        
    void beginJob();
    void produce(art::Event& evt);
    virtual void reconfigure(fhicl::ParameterSet const& pset); 
  private:
    std::string fGeneratorLabel;

  };

  //___________________________________________________________________________
  RewriteSpillData::RewriteSpillData(fhicl::ParameterSet const& pset)
  {
    this->reconfigure(pset);
  
    produces< sumdata::SpillData    >();
  }

  //___________________________________________________________________________
  RewriteSpillData::~RewriteSpillData()
  {  
  }

  //___________________________________________________________________________
  void RewriteSpillData::reconfigure(fhicl::ParameterSet const& pset)
  {
    fGeneratorLabel   = pset.get<std::string>("GeneratorLabel");
  }

  //___________________________________________________________________________
  void RewriteSpillData::beginJob()
  {
  }

  //___________________________________________________________________________
  void RewriteSpillData::produce(art::Event& evt)
  {
     art::Handle<sumdata::SpillData> spillPot;
     evt.getByLabel(fGeneratorLabel, spillPot);

     std::unique_ptr<sumdata::SpillData> sd(new sumdata::SpillData);

     //if this is data, continue, we don't need to rewrite
     if (!evt.isRealData()){
       sd->spillpot = spillPot->spillpot;
       sd->goodbeam = spillPot->goodbeam;
       sd->deltaspilltimensec = spillPot->deltaspilltimensec;
       sd->hornI = spillPot->hornI;
       sd->posx = spillPot->posx;
       sd->posy = spillPot->posy;
       sd->widthx = spillPot->widthx;
       sd->widthy = spillPot->widthy;

       meta::MetadataManager& fManager = meta::MetadataManager::getInstance();
       std::string current = fManager.GetMetadata()["nova.hornpolarity"];
       bool isRHC = false;
       if (current == "rhc") isRHC = true;
       sd->isRHC = isRHC;
     }

     evt.put(std::move(sd));
  }

  DEFINE_ART_MODULE(RewriteSpillData)

} // namespace
