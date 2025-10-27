#include "warren/time/fake_clock.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "warren/time/time_point.h"

namespace warren {

namespace {

using ::testing::Eq;

TEST(FakeClockTest, Advance) {
  FakeClock clock(TimePoint{});
  ASSERT_THAT(clock.now(), Eq(TimePoint()));

  clock.advance(seconds(10));
  EXPECT_THAT(clock.now(), Eq(TimePoint(/*seconds=*/10)));
}

}  // namespace

}  // namespace warren
