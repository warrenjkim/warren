#pragma once

#include <cstdint>

namespace warren {

namespace units {

constexpr int64_t kNanosPerSecond = 1'000'000'000;

}  // namespace units

struct Duration {
  Duration() = default;

  Duration(int64_t s, uint32_t ns = 0) : seconds(s), nanos(ns) {}

  Duration& operator+=(Duration d) noexcept;

  Duration& operator-=(Duration d) noexcept;

  Duration& operator*=(int64_t d) noexcept;

  Duration& operator/=(int64_t d) noexcept;

  int64_t seconds = 0;
  uint32_t nanos = 0;
};

inline bool operator==(Duration lhs, Duration rhs) noexcept {
  return lhs.seconds == rhs.seconds && lhs.nanos == rhs.nanos;
}

inline bool operator!=(Duration lhs, Duration rhs) noexcept {
  return !(lhs == rhs);
}

inline bool operator<(Duration lhs, Duration rhs) noexcept {
  return (lhs.seconds < rhs.seconds) ||
         (lhs.seconds == rhs.seconds && lhs.nanos < rhs.nanos);
}

inline bool operator<=(Duration lhs, Duration rhs) noexcept {
  return (lhs < rhs) || (lhs == rhs);
}

inline bool operator>(Duration lhs, Duration rhs) noexcept { return rhs < lhs; }

inline bool operator>=(Duration lhs, Duration rhs) noexcept {
  return rhs <= lhs;
}

inline Duration operator+(Duration lhs, Duration rhs) noexcept {
  int64_t s = lhs.seconds + rhs.seconds;
  uint64_t ns = static_cast<uint64_t>(lhs.nanos) + rhs.nanos;
  if (ns >= units::kNanosPerSecond) {
    s++;
    ns -= units::kNanosPerSecond;
  }

  return Duration(s, static_cast<uint32_t>(ns));
}

inline Duration operator-(Duration lhs, Duration rhs) noexcept {
  int64_t s = lhs.seconds - rhs.seconds;
  int64_t ns =
      static_cast<int64_t>(lhs.nanos) - static_cast<int64_t>(rhs.nanos);
  if (ns < 0) {
    s--;
    ns += units::kNanosPerSecond;
  }

  return Duration(s, static_cast<uint32_t>(ns));
}

inline Duration operator-(Duration d) noexcept {
  if (d.nanos == 0) {
    return Duration(-d.seconds);
  }

  return Duration(-d.seconds - 1,
                  static_cast<uint32_t>(units::kNanosPerSecond - d.nanos));
}

inline Duration operator*(Duration d, int64_t scalar) noexcept {
  int64_t total_ns = scalar * (d.seconds * units::kNanosPerSecond + d.nanos);

  int64_t s = total_ns / units::kNanosPerSecond;
  int64_t ns = total_ns % units::kNanosPerSecond;

  if (ns < 0) {
    s--;
    ns += units::kNanosPerSecond;
  }

  return Duration(s, static_cast<uint32_t>(ns));
}

inline Duration operator*(int64_t scalar, Duration d) noexcept {
  return d * scalar;
}

inline Duration operator/(Duration d, int64_t divisor) noexcept {
  int64_t total_ns = (d.seconds * units::kNanosPerSecond + d.nanos) / divisor;

  int64_t s = total_ns / units::kNanosPerSecond;
  int64_t ns = total_ns % units::kNanosPerSecond;

  if (ns < 0) {
    s--;
    ns += units::kNanosPerSecond;
  }

  return Duration(s, static_cast<uint32_t>(ns));
}

inline Duration& Duration::operator+=(Duration d) noexcept {
  *this = *this + d;
  return *this;
}

inline Duration& Duration::operator-=(Duration d) noexcept {
  *this = *this - d;
  return *this;
}

inline Duration& Duration::operator*=(int64_t scalar) noexcept {
  *this = *this * scalar;
  return *this;
}

inline Duration& Duration::operator/=(int64_t divisor) noexcept {
  *this = *this / divisor;
  return *this;
}

inline Duration Seconds(int64_t s) noexcept { return Duration(s); }

}  // namespace warren
