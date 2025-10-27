#pragma once

#include <cstdint>

#include "warren/time/duration.h"

namespace warren {

struct TimePoint {
  TimePoint() = default;

  TimePoint(int64_t s, uint32_t ns = 0) : seconds(s), nanos(ns) {}

  TimePoint& operator+=(Duration d) noexcept;

  TimePoint& operator-=(Duration d) noexcept;

  int64_t seconds = 0;
  uint32_t nanos = 0;
};

inline bool operator==(TimePoint lhs, TimePoint rhs) noexcept {
  return lhs.seconds == rhs.seconds && lhs.nanos == rhs.nanos;
}

inline bool operator!=(TimePoint lhs, TimePoint rhs) noexcept {
  return !(lhs == rhs);
}

inline bool operator<(TimePoint lhs, TimePoint rhs) noexcept {
  return (lhs.seconds < rhs.seconds) ||
         (lhs.seconds == rhs.seconds && lhs.nanos < rhs.nanos);
}

inline bool operator<=(TimePoint lhs, TimePoint rhs) noexcept {
  return (lhs < rhs) || (lhs == rhs);
}

inline bool operator>(TimePoint lhs, TimePoint rhs) noexcept {
  return rhs < lhs;
}

inline bool operator>=(TimePoint lhs, TimePoint rhs) noexcept {
  return rhs <= lhs;
}

inline TimePoint operator+(TimePoint tp, Duration d) noexcept {
  int64_t s = tp.seconds + d.seconds;
  int64_t ns = static_cast<int64_t>(tp.nanos) + static_cast<int64_t>(d.nanos);
  if (ns >= units::kNanosPerSecond) {
    s++;
    ns -= units::kNanosPerSecond;
  }

  return TimePoint(s, static_cast<uint32_t>(ns));
}

inline TimePoint operator+(Duration d, TimePoint tp) noexcept { return tp + d; }

inline Duration operator-(TimePoint lhs, TimePoint rhs) noexcept {
  int64_t s = lhs.seconds - rhs.seconds;
  int64_t ns =
      static_cast<int64_t>(lhs.nanos) - static_cast<int64_t>(rhs.nanos);
  if (ns < 0) {
    s--;
    ns += units::kNanosPerSecond;
  }

  return Duration(s, static_cast<uint32_t>(ns));
}

inline TimePoint operator-(TimePoint tp, Duration d) noexcept {
  return tp + (-d);
}

inline TimePoint& TimePoint::operator+=(Duration d) noexcept {
  *this = *this + d;
  return *this;
}

inline TimePoint& TimePoint::operator-=(Duration d) noexcept {
  *this = *this - d;
  return *this;
}

}  // namespace warren
