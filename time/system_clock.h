#pragma once

#include <chrono>

#include "warren/time/clock.h"
#include "warren/time/time_point.h"

namespace warren {

class SystemClock final : public Clock {
 public:
  virtual TimePoint now() const override {
    std::chrono::duration epoch =
        std::chrono::system_clock::now().time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    auto nanos =
        std::chrono::duration_cast<std::chrono::nanoseconds>(epoch - seconds);

    return TimePoint(seconds.count(), static_cast<uint32_t>(nanos.count()));
  }
};

}  // namespace warren
