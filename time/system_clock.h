#pragma once

#include <chrono>

#include "warren/time/clock.h"

namespace warren {

class SystemClock final : public Clock {
 public:
  inline virtual TimePoint now() const override {
    std::chrono::duration epoch =
        std::chrono::system_clock::now().time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    auto nanos =
        std::chrono::duration_cast<std::chrono::nanoseconds>(epoch - seconds);

    return TimePoint{.seconds = static_cast<uint64_t>(seconds.count()),
                     .nanos = static_cast<uint64_t>(nanos.count())};
  }
};

}  // namespace warren
