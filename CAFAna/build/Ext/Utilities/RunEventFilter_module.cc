///////////////////////////////////////////////////////////////////////////
/// \brief   Filter events based on their run/event numbers
/// \author  messier@indiana.edu
/// \date
///////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"

#include "Metadata/MetadataManager.h"
#include "RecoBase/Cluster.h"
#include "RecoBase/FilterList.h"

namespace util {
  /// Select events based on a list of run and event numbers
  ///
  /// File format is expected to be a list of run/subrun/event triplets:
  /// 
  /// # run subrun event
  /// 10796  4     102012
  /// 10796  2     121093
  /// 10796  11    133092
  /// 
  /// with comments marked by a leading '#'. By default the module
  /// looks for these to be in a file called "event-list.txt",
  /// although the filename can be reconfigured. The module can be
  /// configured to run in one of two modes. The configuration
  /// include_runeventfilter sets Mode=0 which means that the events
  /// listed in the file are to be included in the output. The
  /// configuration exclude_runeventfilter sets Mode=1 which means
  /// that the events listed are to be excluded from the output
  /// file. See RunEventFilter.fcl.
  ///
  class RunEventFilter : public art::EDFilter 
  {
  public:
    explicit RunEventFilter(fhicl::ParameterSet const& p);
    virtual ~RunEventFilter();

    virtual void reconfigure(fhicl::ParameterSet const& p);

    virtual bool beginRun(art::Run& run) override;
    virtual bool beginSubRun(art::SubRun& sr) override;
    virtual bool filter(art::Event& e) override;


  private:
    std::string               fEventFile;   ///< List of event numbers
    int                       fMode;        ///< 0=include, 1=exclude list
    bool                      fFilterSlice; ///< Apply filter at slice level
    bool                      fIsMC;        ///< Data or MC? Hence cycle number
    bool                      fUseBatchNum; ///< Batch number may be neeed for MC
    std::string               fSlicerLabel; ///< Module that produced slices

    bool                      fSkipping;
    int                       fCycle;
    int                       fBatch;

    std::set<unsigned int> fRuns;
    std::set< std::pair<unsigned int, unsigned int> > fSubRuns;
    std::set< std::tuple<unsigned int, unsigned int,
			 unsigned int> > fEvents;
    std::set< std::tuple<unsigned int, unsigned int,
			unsigned int, unsigned int> > fSlices;
    std::set< std::tuple<unsigned int, unsigned int,
			 unsigned int, unsigned int,
			 unsigned int> > fCycles;
    std::set< std::tuple<unsigned int, unsigned int,
			 unsigned int, unsigned int,
			 unsigned int, unsigned int> > fBatches;
    
  };
}


////////////////////////////////////////////////////////////////////////
namespace util
{
  //......................................................................
  RunEventFilter::RunEventFilter(fhicl::ParameterSet const& p)
    : fSkipping(false), fCycle(-5), fBatch(-5)
  {
    this->reconfigure(p);
    
    if (fFilterSlice) produces< rb::FilterList<rb::Cluster> >();
  }
    
  //......................................................................
  void RunEventFilter::reconfigure(fhicl::ParameterSet const& p)
  {
    fEventFile   = p.get<std::string>("EventFile");
    fMode        = p.get<int>("Mode");
    fFilterSlice = p.get<bool>("FilterSlice");
    fIsMC        = p.get<bool>("IsMC");
    fUseBatchNum = p.get<bool>("UseBatchNum");
    fSlicerLabel = p.get<std::string>("SlicerLabel");

    //
    // Reconfigure the run and event list given the new file name.
    //
    fRuns.    clear();
    fSubRuns. clear();
    fEvents.  clear();
    fSlices.  clear();
    fCycles.  clear();
    fBatches. clear();
    std::ifstream ifs;
    ifs.open(fEventFile.c_str());
    if (ifs.is_open()) {
      while (ifs.good()) {
	std::string buff;
	std::getline(ifs, buff);
	if (buff[0]=='#') continue;
      
	int run, subrun, event, cycle;
	// if we're filtering the slice
	if (fFilterSlice){
	  int slice;
	  // for MC we need run/subrun/cycle/event/slice
	  if(fIsMC){
	    if(fUseBatchNum){
	      int batch;
	      // for MC with batch numbers we need run/subrun/cycle/batch/event/slice
	      sscanf(buff.c_str(), "%d %d %d %d %d %d", &run, &subrun, &cycle, &batch, &event, &slice);
	      fCycles.insert(std::make_tuple(run, subrun, cycle, batch, event));
	      fBatches.insert(std::make_tuple(run, subrun, cycle, batch, event, slice));
	    }
	    else{
	      sscanf(buff.c_str(), "%d %d %d %d %d", &run, &subrun, &cycle, &event, &slice);
	      fCycles.insert(std::make_tuple(run, subrun, cycle, event, slice));
	      fSlices.insert(std::make_tuple(run, subrun, cycle, event));
	    }
	  } // fIsMC = true
	  // for Data run/subrun/event/slice
	  else{
	    sscanf(buff.c_str(), "%d %d %d %d", &run, &subrun, &event, &slice);
	    fSlices.insert(std::make_tuple(run, subrun, event, slice));
	  } // fIsMC = false
	} // if fFilterSlice
	// And if we're not filtering the slice
	else{
	  // for MC we need run/subrun/cycle/event
	  if(fIsMC){
	    if(fUseBatchNum){
	      int batch;
	      // for MC with batch numbers we need run/subrun/cycle/batch/event/slice
	      sscanf(buff.c_str(), "%d %d %d %d %d", &run, &subrun, &cycle, &batch, &event);
	      fCycles.insert(std::make_tuple(run, subrun, cycle, batch, event));
	    }
	    else{
	      
	      sscanf(buff.c_str(), "%d %d %d %d", &run, &subrun, &cycle, &event);
	      // Have to be careful here since fSlices can hold 4 tuple values
	      // and fCycles 5. In this case we only want to fill 4 fields)
	      // I'm sure there's a smarter way to do this...
	      fSlices.insert(std::make_tuple(run, subrun, cycle, event));
	    }
	  } // fIsMC = true
	  // for Data we only need run/subrun/event
	  else{
	    sscanf(buff.c_str(), "%d %d %d", &run, &subrun, &event);
	  }
	} // fFilterSlice = false
	fRuns.insert(run);
	fSubRuns.insert(std::make_pair(run, subrun));
	fEvents.insert(std::make_tuple(run, subrun, event));
      } // close while loop
      ifs.close();
    } // close input file
    else {
      std::cerr << __FILE__ << ":" << __LINE__ 
		<< " Failed to open file " << fEventFile << " for read" 
		<< std::endl;
    }
  }

  //......................................................................
  RunEventFilter::~RunEventFilter() 
  { 
  }

  //......................................................................
  bool RunEventFilter::beginRun(art::Run& run)
  {
    // If we're only accepting events in the list and there were no entries
    // for this run go ahead and skip it.
    fSkipping = (fMode == 0 && !fRuns.count(run.run()));
    return !fSkipping;
  }

  //......................................................................
  bool RunEventFilter::beginSubRun(art::SubRun& sr)
  {
    // Already skipping the whole run
    if(fSkipping) return false;

    // If we're only accepting events in the list and there were no entries
    // for this subrun go ahead and skip it.
    fSkipping = (fMode == 0 && !fSubRuns.count(std::make_pair(sr.run(), sr.subRun())));
    return !fSkipping;
  }

  //......................................................................
  bool RunEventFilter::filter(art::Event& evt) 
  {
    std::unique_ptr< rb::FilterList<rb::Cluster> > filtcol(new rb::FilterList<rb::Cluster>);

    // Already skipping the whole run or subrun
    if(fSkipping){
      evt.put(std::move(filtcol));
      return false;
    }

    // We're going to pull cycle number from the metadata
    meta::MetadataManager& fManager = meta::MetadataManager::getInstance();
    // Check it exists in the metadata
    // Convert the std::string to int
    std::string cycle = fManager.GetMetadata()["simulated.cycle"];
    if(cycle != "") fCycle = std::stoi(cycle);

    std::string batch = fManager.GetMetadata()["simulated.batch"];
    if(batch != "") fBatch = std::stoi(batch);

    bool inlist = false;
    // Is this event in the list?
    if(fIsMC && fUseBatchNum)
      inlist = fCycles.count(std::make_tuple(evt.run(),
					     evt.subRun(),
					     fCycle,
					     fBatch,
					     evt.event()));
    if(fIsMC && !fUseBatchNum)
      inlist = fSlices.count(std::make_tuple(evt.run(),
					     evt.subRun(),
					     fCycle,
					     evt.event()));
    
    if(!fIsMC)
      inlist = fEvents.count(std::make_tuple(evt.run(),
					     evt.subRun(),
					     evt.event()));
    
    // If we're looking for the events in the list, and this one isn't, then
    // we can skip this event.
    if(fMode == 0 && !inlist){
      evt.put(std::move(filtcol));
      return false;
    }
    // If we're rejecting events in the list, and this event is in we can
    // reject it, so long as it's not just the slices we're supposed to be
    // rejecting.
    if(fMode == 1 && !fFilterSlice && inlist){
      evt.put(std::move(filtcol));
      return false;
    }

    if(fFilterSlice){
      art::Handle< std::vector<rb::Cluster> > slices;
      evt.getByLabel(fSlicerLabel, slices);

      // How many slices have we rejected?
      unsigned int count = 0;

      for(unsigned int slcIdx = 0; slcIdx < slices->size(); ++slcIdx){
	// This is a bit tricky. The != operator here is acting as a XOR.
	// We either want to be accepting slices in the list and to have
	// not found one, or we want to be rejecting slices in the list and
	// to have found it. In either case we filter the slice out of
	// the slices.
	bool sliceIsInList = false;
	if(fIsMC && fUseBatchNum)
	  sliceIsInList = fBatches.count(std::make_tuple(evt.run(),
							 evt.subRun(),
							 fCycle,
							 fBatch,
							 evt.event(),
							 slcIdx));
	if(fIsMC && !fUseBatchNum)
	  sliceIsInList = fCycles.count(std::make_tuple(evt.run(),
							evt.subRun(),
							fCycle,
							evt.event(),
							slcIdx));
	
	if(!fIsMC)
	  sliceIsInList = fSlices.count(std::make_tuple(evt.run(),
							evt.subRun(),
							evt.event(),
							slcIdx));
					
	
	
	if( (fMode == 1 && sliceIsInList) ||
	    (fMode == 0 && !sliceIsInList) ){
	  filtcol->Add(slices, slcIdx);
	  ++count;
	}
	
      } // end for slcIdx
      
      // If we're rejecting slices in the list, and we rejected every slice,
      // let's reject the whole event instead.
      if(fMode == 1 && count == slices->size()){
        evt.put(std::move(filtcol));
        return false;
      }
      
      evt.put(std::move(filtcol));
    } // end if fFilterSlice
    
    return true;
  }
  
} // end namespace util


////////////////////////////////////////////////////////////////////////
namespace util 
{
  DEFINE_ART_MODULE(RunEventFilter)
}
////////////////////////////////////////////////////////////////////////
