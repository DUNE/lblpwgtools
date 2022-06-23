////////////////////////////////////////////////////////////////////////
/// \brief Create a dummy spilldata product in cosmic files to set the isRHC field for downstream reco
/// \author edniner@fnal.gov
/// \date
////////////////////////////////////////////////////////////////////////

// Framework includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"

// NOvA includes
#include "Geometry/Geometry.h"
#include "SummaryData/SpillData.h"

namespace util {

  /// A module to store a geometry in a file
  class WriteHornCurrentCosmic : public art::EDProducer {

  public:

    explicit WriteHornCurrentCosmic(fhicl::ParameterSet const &pset);
    virtual ~WriteHornCurrentCosmic();                        
    void beginJob();
    void produce(art::Event& evt);
    virtual void reconfigure(fhicl::ParameterSet const& pset); 
  private:
    bool fIsRHC;

  };

  //___________________________________________________________________________
  WriteHornCurrentCosmic::WriteHornCurrentCosmic(fhicl::ParameterSet const& pset)
  {
    this->reconfigure(pset);
  
    produces< sumdata::SpillData    >();
  }

  //___________________________________________________________________________
  WriteHornCurrentCosmic::~WriteHornCurrentCosmic()
  {  
  }

  //___________________________________________________________________________
  void WriteHornCurrentCosmic::reconfigure(fhicl::ParameterSet const& pset)
  {
    fIsRHC   = pset.get<bool>("IsRHC");
  }

  //___________________________________________________________________________
  void WriteHornCurrentCosmic::beginJob()
  {
  }

  //___________________________________________________________________________
  void WriteHornCurrentCosmic::produce(art::Event& evt)
  {

     std::unique_ptr<sumdata::SpillData> sd(new sumdata::SpillData);

     //stuff dummy variables so we can fix a horn current
     sd->spillpot = 0;
     sd->goodbeam = true;
     sd->deltaspilltimensec = 0;
     sd->hornI = 0;
     sd->posx = 0;
     sd->posy = 0;
     sd->widthx = 0;
     sd->widthy = 0;
     sd->isRHC = fIsRHC;

     evt.put(std::move(sd));
  }

  DEFINE_ART_MODULE(WriteHornCurrentCosmic)

} // namespace
