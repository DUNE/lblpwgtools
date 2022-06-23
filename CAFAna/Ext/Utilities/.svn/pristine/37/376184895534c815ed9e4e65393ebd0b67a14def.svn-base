/////////////////////////////////////////////////////////////////////////
/// \brief   Concatenate subrun files from same run
/// \author  nowak@physics.umn.edu	
/// \date
/////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Framework includes
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/ParameterSet.h"

#include "StandardRecord/StandardRecord.h"


namespace util {
  namespace
  {
    struct HSRParams
    {
      template<class T> using Atom = fhicl::Atom<T>;
      using Comment = fhicl::Comment;
      using Name = fhicl::Name;
      
      Atom<std::string> standardRecordLabel
      {
        Name("StandardRecordLabel"),
        Comment("Module which produced the StandardRecord (likely cafMaker)")
      };
    };
  }
}


namespace util {
  class HasStandardRecord : public art::EDFilter 
  {
  public:
    using Parameters = art::EDFilter::Table<HSRParams>;
    using vSR = std::vector<caf::StandardRecord>;
    
    explicit HasStandardRecord(const Parameters &params);
    virtual ~HasStandardRecord();
    virtual bool filter(art::Event& e);

  protected:
    HSRParams fParams;
    const art::ProductToken<vSR> fSRToken;
    
  };
}


////////////////////////////////////////////////////////////////////////
namespace util {

  HasStandardRecord::HasStandardRecord(const Parameters& params)
    : fParams(params()),
      fSRToken(consumes<vSR>(fParams.standardRecordLabel()))
  {
    
  }

  //......................................................................

  HasStandardRecord::~HasStandardRecord() { }

  //......................................................................
    
  bool HasStandardRecord::filter(art::Event& evt) 
  {

    art::Handle<vSR> srHandle;
    evt.getByToken(fSRToken, srHandle);

    // Return true if we have found StandardRecords
    return srHandle.isValid();
  }
}


////////////////////////////////////////////////////////////////////////
namespace util 
{
  DEFINE_ART_MODULE(HasStandardRecord)
}
////////////////////////////////////////////////////////////////////////
