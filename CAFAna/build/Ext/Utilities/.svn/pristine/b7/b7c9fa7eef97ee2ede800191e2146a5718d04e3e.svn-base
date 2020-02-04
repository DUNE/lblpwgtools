#ifndef NUMITIMEPARAMS_H
#define NUMITIMEPARAMS_H

namespace util{


  // NB: if you are using files for which CalHit was run in a release from
  // before Sep 20 2015 (ie the first analysis files) you need a version of
  // this file from before Sep 20 too. See docdb 13518 for a description of the
  // bug that used to be present in the timing, fixed on this date, and the
  // workaround removed from this file.

  /*
    These cuts are for the Far Detector only.

    Events in the time window when the beam is expected to arrive (plus a small
    amount of padding) go into the analysis. Events outside of these windows
    (plus some extra padding), and also away from the starts and ends of the
    spill are the "timing sideband". These can be used to derive the expected
    cosmic background count in the spill windows. Count events in the sideband
    and then multiply by a scale factor to account for the different livetimes
    (defined as a Var at the bottom of this file, to be passed in the "wei"
    field of the Spectrum).

    A complication: before the shutdown there were problems with the timing
    system, which led to the beam sometimes appearing to be 64us late. We don't
    know exactly when this was and wasn't the case, so we open a second timing
    window, only in this time period, and hence split the sideband into three
    parts. These cuts are supposed to make this completely transparent to
    users.

    The diagram below may or may not help to clarify matters:

    +------------------------------------------------------+
    |    |<----->|  |###|  |<----->|  |###|  |<------>|    |  before shutdown
    |    |<----->|  |###|  |<------------------------>|    |   after shutdown
    +------------------------------------------------------+
    0    25    208 217 229 238   272 281 293 302     475  500us (not to scale)

    |###| = beam window
    |<->| = timing sideband
    |   | = unused
  */

  // Allow slack around the beam window because the blinding extends all the
  // way out to 208-238us.
  const int kBeamWindowPaddingMicroSec = 9;

  const double kBeamWindowMinMicroSec = 217;
  const double kBeamWindowMaxMicroSec = 229;

  // The out-of-time window before the spill window. Stay away from the very
  // start of the spill where strange things can happen.
  const double kMinTimingSidebandBeforeMicroSec = 25;
  const double kMaxTimingSidebandBeforeMicroSec = kBeamWindowMinMicroSec - kBeamWindowPaddingMicroSec;

  // The out-of-time window after the spill window. Stay away from the very
  // end of the spill where strange things can happen.
  const double kMinTimingSidebandAfterMicroSec = kBeamWindowMaxMicroSec + kBeamWindowPaddingMicroSec;
  const double kMaxTimingSidebandAfterMicroSec = 475;

  // How much the beam peak is shifted when the timing is bad.
  const double kPeakShiftMicroSec = +64;

  // Open up a second, identical, window, just later.
  const double kBeamWindowShiftedMinMicroSec = kBeamWindowMinMicroSec + kPeakShiftMicroSec;
  const double kBeamWindowShiftedMaxMicroSec = kBeamWindowMaxMicroSec + kPeakShiftMicroSec;

  // The end of the 2nd timing sideband and start of the third when we're in
  // bad-timing runs. Allow the same padding as the main beam window.
  const double kMaxTimingSidebandBeforeShiftedWindowMicroSec = kBeamWindowShiftedMinMicroSec - kBeamWindowPaddingMicroSec;
  const double kMinTimingSidebandAfterShiftedWindowMicroSec  = kBeamWindowShiftedMaxMicroSec + kBeamWindowPaddingMicroSec;

  // TCR was installed and TDU firmware was updated to a version verified to
  // not suffer from 64us time jumps at Ash River Oct 28th 2014. The first
  // good run was 17946, but here we need the last bad run.
  const int kLastBadTimingRun = 17945;


  /// How long is the beam window?
  const double kBeamWindowMicroSec = kBeamWindowMaxMicroSec - kBeamWindowMinMicroSec;
  /// How long is the part of the spill far from the start and end, and also
  /// not in the beam window?
  const double kTimingSidebandMicroSec =
    (kMaxTimingSidebandBeforeMicroSec - kMinTimingSidebandBeforeMicroSec) +
    (kMaxTimingSidebandAfterMicroSec - kMinTimingSidebandAfterMicroSec);

  /// Two windows to consider here
  const double kBeamWindowBadPeriodMicroSec =
    (kBeamWindowMaxMicroSec - kBeamWindowMinMicroSec) +
    (kBeamWindowShiftedMaxMicroSec - kBeamWindowShiftedMinMicroSec);

  /// Two beam windows to cut out here
  const double kTimingSidebandBadPeriodMicroSec =
    (kMaxTimingSidebandBeforeMicroSec - kMinTimingSidebandBeforeMicroSec) +
    (kMaxTimingSidebandBeforeShiftedWindowMicroSec - kMinTimingSidebandAfterMicroSec) +
    (kMaxTimingSidebandAfterMicroSec - kMinTimingSidebandAfterShiftedWindowMicroSec);


  static inline  bool IsInBeamWindow(const int run,
				     const double time)
  {
    // In the bad period and in the shifted window
    if(run <= kLastBadTimingRun &&
       time > 1000*kBeamWindowShiftedMinMicroSec &&
       time < 1000*kBeamWindowShiftedMaxMicroSec)
      return true;
    
    // Or in any period and in the main window
    return (time > 1000*kBeamWindowMinMicroSec &&
	    time < 1000*kBeamWindowMaxMicroSec);
  }


}// end of namespace util

#endif // NUMITIMEPARAMS_H
