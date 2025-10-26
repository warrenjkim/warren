#include "warren/time/fake_clock.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace warren {

namespace {

using ::testing::Eq;

TEST(FakeClockTest, Advance) {
  FakeClock clock(Clock::TimePoint{});
  ASSERT_THAT(clock.now(), Eq(Clock::TimePoint{}));

  clock.advance(Clock::TimePoint{.seconds = 10});
  EXPECT_THAT(clock.now(), Eq(Clock::TimePoint{.seconds = 10}));
}

}  // namespace

}  // namespace warren
