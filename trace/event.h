#pragma once

#include <string>

#include "warren/time/time_point.h"

namespace warren {
namespace trace {

struct Event {
  std::string event;
  TimePoint timestamp;
};

}  // namespace trace
}  // namespace warren
