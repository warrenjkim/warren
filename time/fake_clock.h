#pragma once

#include <utility>

#include "warren/time/clock.h"

namespace warren {

class FakeClock final : public Clock {
 public:
  explicit FakeClock(TimePoint now) : now_(std::move(now)) {}

  inline virtual TimePoint now() const override { return now_; }

  inline void advance(const TimePoint offset) {
    now_.seconds += offset.seconds;
    now_.nanos += offset.nanos;
  }

 private:
  TimePoint now_;
};

}  // namespace warren
