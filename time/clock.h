#pragma once

#include <cstdint>

namespace warren {

class Clock {
 public:
  struct TimePoint {
    uint64_t seconds = 0;
    uint64_t nanos = 0;

    bool operator==(const Clock::TimePoint& other) const noexcept = default;
  };

  virtual ~Clock() = default;
  virtual TimePoint now() = 0;
};

}  // namespace warren
