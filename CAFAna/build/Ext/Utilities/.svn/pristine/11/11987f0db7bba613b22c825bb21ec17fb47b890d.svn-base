///
/// \file    RawUtil.h 
/// \brief   Functions for getting information about a collection of raw hits.
/// \version $Id: RawUtil.h,v 1.3 2012-08-16 01:28:29 bckhouse Exp $
/// \author  messier@indiana.edu
///
#ifndef UTIL_RAWUTIL_H
#define UTIL_RAWUTIL_H
#include <vector>
#include "canvas/Persistency/Common/Ptr.h"
namespace rawdata { class RawDigit; }

namespace util
{
  /// \name Utilities acting on RawDigits
  /// Used by Cana
  //@{

  ///
  /// A raw slice is just two indices: the index of the first hit in
  /// the slice, and the index of the last hit in the slice, inclusive.
  ///
  typedef std::pair<unsigned int, unsigned int> RawSlice;
  
  //....................................................................
  
  ///
  /// \brief Arrange the list of raw hits in time order (early to late)
  ///
  /// @param d : The list of digits to be sorted. Sorted on output.
  ///
  void TimeSort(std::vector< art::Ptr<rawdata::RawDigit> >& d);
  
  ///
  /// \brief Count the number of digits in each detector view
  /// 
  /// @param d   : The complete list of raw digits to be considered
  /// @param i1  : Index of the first digit to be considered
  /// @param i2  : Index of the last digit to be considered (inclusive)
  /// @param nx  : On output, the number of hits in the x view
  /// @param ny  : On output, the number of hits in the y view
  ///
  void CountXY(const std::vector< art::Ptr<rawdata::RawDigit> >& d,
	       unsigned int i1,
	       unsigned int i2,
	       unsigned int* nx,
	       unsigned int* ny);
  ///
  /// \brief Find boxes in plane/cell units that contain all the hits
  ///
  /// @param d  : The complete list of raw digits to be considered
  /// @param i1 : Index of the first hit to be considered
  /// @param i2 : Index of the last hit to be considered (inclusive)
  /// @param plane1x : On output, the lowest plane number in the x view
  /// @param plane2x : On output, the highest plane number in the x view
  /// @param cell1x  : On output, the lowest cell number in the x view
  /// @param cellx   : On output, the highest cell number in the x view
  /// @param plane1y : On output, the lowest plane number in the y view
  /// @param plane2y : On output, the highest plane number in the y view
  /// @param cell1y  : On output, the lowest cell number in the y view
  /// @param celly   : On output, the highest cell number in the y view
  ///
  void EventBox(const std::vector< art::Ptr<rawdata::RawDigit> >& d,
		unsigned int i1,
		unsigned int i2,
		unsigned int* plane1x,
		unsigned int* plane2x,
		unsigned int* cell1x,
		unsigned int* cell2x,
		unsigned int* plane1y,
		unsigned int* plane2y,
		unsigned int* cell1y,
		unsigned int* cell2y);
  ///
  /// \brief Find windows in time that have significant activity in the detector
  ///
  /// @param d      - The full raw digit list to consider
  /// @param dt_tdc - Sliding window size in NOvA clock ticks (64 MHz)
  /// @param nhit   - Total number of hits to look for in window
  /// @param nhitx  - Number of hits required in x view
  /// @param nhity  - Number of hits required in y view
  /// @param slice  - On output, the start and end indicies of the time windows
  ///
  /// Note: d must be time sorted on input. See "TimeSort" above.
  ///
  void TimeSlice(const std::vector< art::Ptr<rawdata::RawDigit> >& d,
		 unsigned int dt_tdc,
		 unsigned int nhit,
		 unsigned int nhitx,
		 unsigned int nhity,
		 std::vector<RawSlice>& slice);
  ///
  /// \brief Filter hits that are "FEB flash" candidates; that is,
  /// they occur within a specified time of an FEB hit in saturation
  ///
  /// @param rd - Input list of raw digits. On output it will be time
  ///             sorted and have had FEB flash hits removed.
  ///
  /// @param adc_sat - ADC value to flag a raw hit as being in
  /// saturation, recommended value is 3400 counts
  ///
  /// @param dt_tdc - time difference between time of saturation and
  /// time of candidate "flash" hit in units of TDC
  /// counts. Recommended value is 1280 (20 usec)
  /// Note: 1920 (30 usec) is also a useful optional value. 
  /// @returns the new size of the rd raw hit list
  ///
  /// Note that the vector rd will be returned time sorted and equal
  /// or shorter than its original length
  ///
  unsigned int
    FilterFEBFlash(std::vector< art::Ptr<rawdata::RawDigit> >& rd,
		   int adc_sat=3400,
		   int dt_tdc=1280);
    
  //@}
}

#endif

////////////////////////////////////////////////////////////////////////
