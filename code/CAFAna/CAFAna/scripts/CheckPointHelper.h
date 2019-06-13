#pragma once

#include <chrono>
#include <cstdlib>

class CheckPointHelper {

  std::chrono::time_point<std::chrono::system_clock> fBegin;
  std::chrono::time_point<std::chrono::system_clock> fLimit;
  std::chrono::time_point<std::chrono::system_clock> fLastCheckPoint;

  std::chrono::minutes fSafeUnitDuration;
  std::chrono::minutes fCheckpointDuration;

public:
  CheckPointHelper()
      : fBegin(std::chrono::system_clock::now()), fSafeUnitDuration(0),
        fCheckpointDuration(60) {

    char const *total_duration_m = getenv("CAFANA_TOTALDURATION_MIN");
    char const *safe_unit_duration_m = getenv("CAFANA_SAFEUNITDURATION_MIN");

    if (total_duration_m && safe_unit_duration_m) {
      fSafeUnitDuration = std::chrono::minutes(atoi(safe_unit_duration_m));
      fLimit = fBegin + std::chrono::minutes(atoi(total_duration_m));
      std::cout << "[CHK]: Time for an estimated "
                << (atoi(total_duration_m) / atoi(safe_unit_duration_m))
                << " work units." << std::endl;

      char const *chk_duration_m = getenv("CAFANA_CHKDURATION_MIN");
      if (chk_duration_m) {
        fCheckpointDuration = std::chrono::minutes(atoi(chk_duration_m));
      }
      std::cout << "[CHK]: Will checkpoint every "
                << fCheckpointDuration.count() << " minutes." << std::endl;
    }

    NotifyCheckpoint();
  }

  bool IsCounting() { return fSafeUnitDuration.count(); }

  bool IsSafeToStartNewUnit() {
    if (!IsCounting()) {
      return true;
    }

    return (fLimit - std::chrono::system_clock::now()) > fSafeUnitDuration;
  }

  bool ShouldCheckpoint() {
    if (!IsCounting()) {
      return false;
    }

    std::cout << "[CHK]: Time since last checkpoint = "
              << std::chrono::duration_cast<std::chrono::minutes>(
                     std::chrono::system_clock::now() - fLastCheckPoint)
                     .count()
              << " minutes." << std::endl;

    bool sc = ((std::chrono::system_clock::now() - fLastCheckPoint) >=
               fCheckpointDuration);
    return sc;
  }

  void NotifyCheckpoint() {
    fLastCheckPoint = std::chrono::system_clock::now();
  }
};
