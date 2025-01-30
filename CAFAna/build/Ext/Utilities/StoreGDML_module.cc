////////////////////////////////////////////////////////////////////////
/// \brief  Store a geometry in a file
/// \author edniner@fnal.gov
/// \date
////////////////////////////////////////////////////////////////////////

// Framework includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"

// NOvA includes
#include "Geometry/Geometry.h"
#include "SummaryData/RunData.h"

namespace util {

  /// A module to store a geometry in a file
  class StoreGDML : public art::EDProducer {

  public:

    explicit StoreGDML(fhicl::ParameterSet const &pset);
    virtual ~StoreGDML();                        
    void beginJob();
    void beginRun(art::Run &run);
    void produce(art::Event& evt);
  };

  //___________________________________________________________________________
  StoreGDML::StoreGDML(fhicl::ParameterSet const& pset)
  {  
    produces< sumdata::RunData, art::InRun    >();
  }

  //___________________________________________________________________________
  StoreGDML::~StoreGDML()
  {  
  }

  //___________________________________________________________________________
  void StoreGDML::beginJob()
  {
  }

  //___________________________________________________________________________
  void StoreGDML::beginRun(art::Run& run)
  {
    // grab the geometry object to see what geometry we are using
    art::ServiceHandle<geo::Geometry> geo;

    run.put(std::make_unique<sumdata::RunData>(geo->DetId(),
                                               geo->FileBaseName(),
                                               geo->ExtractGDML()));
  }
  //___________________________________________________________________________
  void StoreGDML::produce(art::Event& event)
  {
  }

  DEFINE_ART_MODULE(StoreGDML)

} // namespace
