//////////////////////////////////////////////////////////////////////////
/// \brief   Collection of basic utilities for working with raw digits
/// \author  messier@indiana.edu
/// \date    June 2011
//////////////////////////////////////////////////////////////////////////
#include "Utilities/RawUtil.h"

#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "CMap/service/CMapService.h"
#include "Geometry/Geometry.h"
#include "RawData/RawDigit.h"

#include <cmath>
#include <algorithm>

//......................................................................

static bool digi_sort(const art::Ptr<rawdata::RawDigit> d1,
		      const art::Ptr<rawdata::RawDigit> d2) 
{
  return (d1->TDC()<d2->TDC());
}

//......................................................................

///
/// Sort the list of digits to be in increasing time order, earliest
/// to lattest
///
void util::TimeSort(std::vector< art::Ptr<rawdata::RawDigit> >& d)
{
  sort(d.begin(), d.end(), digi_sort);
}

//......................................................................

void util::CountXY(const std::vector< art::Ptr<rawdata::RawDigit> >& d,
		  unsigned int i1,
		  unsigned int i2,
		  unsigned int* nx,
		  unsigned int* ny)
{
  art::ServiceHandle<cmap::CMap>    cmap;
  art::ServiceHandle<geo::Geometry> geom;

  *nx = 0;
  *ny = 0;
  unsigned int i;
  for (i=i1; i<=i2; ++i) {
    unsigned int p = cmap->GetPlane(d[i].get());
    geo::View_t  v = geom->Plane(p)->View();
    if      (v==geo::kX) ++(*nx);
    else if (v==geo::kY) ++(*ny);
  }
}

//......................................................................

void util::EventBox(const std::vector< art::Ptr<rawdata::RawDigit> >& d,
		   unsigned int i1,
		   unsigned int i2,
		   unsigned int* plane1x,
		   unsigned int* plane2x,
		   unsigned int* cell1x,
		   unsigned int* cell2x,
		   unsigned int* plane1y,
		   unsigned int* plane2y,
		   unsigned int* cell1y,
		   unsigned int* cell2y) 
{
  art::ServiceHandle<cmap::CMap>    cmap;
  art::ServiceHandle<geo::Geometry> geom;

  *plane1x = 999999;
  *plane1y = 999999;
  *cell1x  = 999999;
  *cell1y  = 999999;
  *plane2x = 0;
  *cell2x  = 0;
  *plane2y = 0;
  *cell2y  = 0;
  
  unsigned int i;
  unsigned int p, c;
  geo::View_t v;
  for (i=i1; i<=i2; ++i) {
    p = cmap->GetPlane(d[i].get());
    v = geom->Plane(p)->View();
    c = cmap->GetCell(d[i].get());
    if (v==geo::kX) {
      if (p<(*plane1x)) (*plane1x) = p;
      if (p>(*plane2x)) (*plane2x) = p;
      if (c<(*cell1x))  (*cell1x)  = c;
      if (c>(*cell2x))  (*cell2x)  = c;
    }
    else if (v==geo::kY) {
      if (p<(*plane1y)) (*plane1y) = p;
      if (p>(*plane2y)) (*plane2y) = p;
      if (c<(*cell1y))  (*cell1y)  = c;
      if (c>(*cell2y))  (*cell2y)  = c;      
    }
  }
}

//......................................................................

///
/// Find time windows with significant hit activity
///
void util::TimeSlice(const std::vector< art::Ptr<rawdata::RawDigit> >& d,
		    unsigned int tdcwindow,
		    unsigned int nhit,
		    unsigned int nhitx,
		    unsigned int nhity,
		    std::vector<RawSlice>& slice) 
{
  //
  // If the list can't meet our minimum hit requirements, we're done
  // before we start. These checks also guarantee that the assumptions
  // we make later regarding unsigned int arithmetic is correct
  //
  if (nhit<1 || nhit<(nhitx+nhity) || d.size()<nhit) return;
  
  unsigned int i, j;
  std::vector<RawSlice> tmpslice;
  for (i=0; i<d.size(); ++i) {
    //
    // Slide forward through the hits until we pass the limit of the
    // time window
    //
    unsigned int dt = 0;
    for (j=i+1; j<d.size(); ++j) {
      dt = d[j]->TDC() - d[i]->TDC();
      if (dt>=tdcwindow) {
	//
	// The last hit will have exceeded the time window so back up by
	// one so that j is the index of the last hit to be included
	//
	--j;
	//
	// Do we have enough total hits?
	//
	if (j>=i && (j-i+1)>=nhit) {
	  //
	  // Do we have enough in the x and y views?
	  //
	  unsigned int nx=0, ny=0;
	  util::CountXY(d, i, j, &nx, &ny);
	  //
	  // Slice meets requirements. Put it in the list.
	  //
	  if (nx>=nhitx && ny>=nhity) {
	    RawSlice rs;
	    rs.first  = i;
	    rs.second = j;
	    tmpslice.push_back(rs);
	  }
	}
	break; // Jump out of loop on j
      } // if (dt>=tdcwindow)
    } // for (j=...
    //
    // If we've bumped into the end of the digit list, we're done
    //
    if (j==d.size()) break;
  }
  //
  // Merge overlaping slices
  //
  for (i=0; i<tmpslice.size(); ++i) {
    if (tmpslice[i].first==0 && tmpslice[i].second==0) continue;
    for (j=i+1; j<tmpslice.size(); ++j) {
      if (tmpslice[j].first <= tmpslice[i].second) {
	tmpslice[i].second = tmpslice[j].second;
	tmpslice[j].first = 0;
	tmpslice[j].second= 0;
      }
    }
  }
  //
  // Push which ever slices survived to the output
  //
  for (unsigned int i=0; i<tmpslice.size(); ++i) {
    if (!(tmpslice[i].first==0 && tmpslice[i].second==0)) {
      slice.push_back(tmpslice[i]);
    }
  }
}

//......................................................................

unsigned int
util::FilterFEBFlash(std::vector< art::Ptr<rawdata::RawDigit> >& rd,
	       int adc_sat,
	       int dt_tdc)
{
  unsigned int i, j;

  //
  // For efficiency, time sort the hits in advance
  //
  util::TimeSort(rd);
      
  //
  // Flag all hits as good, look for ones that might be bad
  //
  std::vector<bool> isgood(rd.size());
  for (i=0; i<rd.size(); ++i) isgood[i] = true;
  for (i=0; (i+1)<rd.size(); ++i) {
    //
    // If the hit i is not in saturation, we can skip to the next
    // hit
    //
    if (rd[i]->ADC()<adc_sat) continue;
    //
    // Get ID number for the FEB ID that has the hit in saturation
    //
    unsigned int chi      = rd[i]->DaqChannel();
    unsigned int diblocki = (chi&0x0FC00000)>>22;
    unsigned int dcmi     = (chi&0x003F0000)>>16;
    unsigned int febi     = (chi&0x0000FF00)>>8;
    for (j=i+1; j<rd.size(); ++j) {
      //
      // Don't need to look beyond the time limit
      //
      if ((rd[j]->TDC()-rd[i]->TDC())>dt_tdc) break;
      //
      // Hit j is in time range of another hit which is in
      // saturation. Check if hit j comes from the same FEB as hit
      // i
      //
      unsigned int chj      = rd[j]->DaqChannel();
      unsigned int diblockj = (chj&0x0FC00000)>>22;
      unsigned int dcmj     = (chj&0x003F0000)>>16;
      unsigned int febj     = (chj&0x0000FF00)>>8;
      bool sameFEB = 
	(diblocki==diblockj) && (dcmi==dcmj) && (febi==febj);
      if (sameFEB) isgood[j] = false;
    }
  }
  //
  // Push all the good hits into the final list
  //
  std::vector< art::Ptr<rawdata::RawDigit> > good;
  good.reserve(rd.size());
  for (i=0; i<rd.size(); ++i) {
    if (isgood[i]) good.push_back(rd[i]);
  }
  rd.swap(good);
  return rd.size();
}

////////////////////////////////////////////////////////////////////////
