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


namespace util {
  class MergeSubruns : public art::EDFilter 
  {
  public:
    explicit MergeSubruns(fhicl::ParameterSet const& p);
    virtual ~MergeSubruns();

    virtual void reconfigure(fhicl::ParameterSet const& p);
    
    virtual bool filter(art::Event& e);

  private:
    int           fMode;        ///< 0- merge all provided files, 1 - merge only files from the same run
    unsigned int  fRun;         ///< run number for the first file
    bool          fRunStatus;   ///< Check is fRun was set.                    
  };
}


////////////////////////////////////////////////////////////////////////
namespace util {

  MergeSubruns::MergeSubruns(fhicl::ParameterSet const& p) 
  {
    this->reconfigure(p);
  }

  //......................................................................

  void MergeSubruns::reconfigure(fhicl::ParameterSet const& p)
  {
    fMode      = p.get<int>("Mode");
    fRun       = 0;
    fRunStatus = false;
  }

  //......................................................................

  MergeSubruns::~MergeSubruns() { }

  //......................................................................
    
  bool MergeSubruns::filter(art::Event& evt) 
  {

  //std::cout<<"EventID="<<evt.id()    <<std::endl;
  //std::cout<<"time=   "<<evt.time()  <<std::endl; ///time() doesn't work here
  //std::cout<<"event=  "<<evt.event() <<std::endl;
  //std::cout<<"run=    "<<evt.run()   <<std::endl;
  //std::cout<<"subRun= "<<evt.subRun()<<std::endl;

    if (fMode==1) {
      std::cout<<"Mode 1 - only files for the same run"<<std::endl;
  
      if(! fRunStatus){ 
	fRun       = evt.run();
        fRunStatus = true;
	std::cout<<"First event:"<< evt.id()<<std::endl;
	return true;
      }
      else if(evt.run() == fRun) 
	return true;
      return false;
    }

    return true; ///pass all events if fMode !=1
  }
}


////////////////////////////////////////////////////////////////////////
namespace util 
{
  DEFINE_ART_MODULE(MergeSubruns)
}
////////////////////////////////////////////////////////////////////////
