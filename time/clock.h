#pragma once

#include <cstdint>

#include "warren/time/time_point.h"

namespace warren {
class Clock {
 public:
  virtual ~Clock() = default;
  virtual TimePoint now() const = 0;
};

}  // namespace warren
