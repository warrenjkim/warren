#pragma once

#include <utility>

#include "warren/time/clock.h"
#include "warren/time/time_point.h"

namespace warren {

class FakeClock final : public Clock {
 public:
  explicit FakeClock(TimePoint now) : now_(std::move(now)) {}

  virtual TimePoint now() const override { return now_; }

  void advance(Duration offset) { now_ += offset; }

 private:
  TimePoint now_;
};

}  // namespace warren
