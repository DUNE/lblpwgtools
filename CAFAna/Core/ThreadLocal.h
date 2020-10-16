#pragma once

#include <thread>
#include <shared_mutex>
#include <unordered_map>

namespace ana
{
  /// A variable that has an independent value on each thread
  ///
  /// Intended for use in implementing caches in a threadsafe manner. \ref T
  /// must be default-constructible.
  template<class T> class ThreadLocal
  {
  public:
    ~ThreadLocal()
    {
      // No good way to know a thread exited, so we can't cleanup until here
      for(auto it: fVals) delete it.second;
    }

    T* operator->()
    {
      const std::thread::id id = std::this_thread::get_id();

      {
        // Normally it's safe for there to be multiple concurrent readers
        std::shared_lock lock(fMutex);

        auto it = fVals.find(id);
        // We found the variable for this thread
        if(it != fVals.end()) return it->second;
      }

      // If we've never seen this thread before we need to create a new entry,
      // and we have to have exclusive access for that to be safe.
      std::unique_lock lock(fMutex);

      T* val = new T;
      fVals.emplace(id, val);
      return val;
    }

  protected:
    std::unordered_map<std::thread::id, T*> fVals;
    std::shared_mutex fMutex;
  };
}
