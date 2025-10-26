#pragma once

#include <string>

#include "warren/time/clock.h"

namespace warren {
namespace trace {

struct Event {
  std::string name;
  Clock::TimePoint timestamp;
};

}  // namespace trace
}  // namespace warren
