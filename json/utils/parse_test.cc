#include "warren/json/utils/parse.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace warren {
namespace json {

namespace {

using ::testing::Eq;
TEST(UtilsTest, Parse) {
  EXPECT_THAT(R"(
  {
    "key": "value",
    "other": 10
  }
      )"_json,
              Eq(parse("{\"key\": \"value\", \"other\": 10}")));
}

}  // namespace
}  // namespace json
}  // namespace warren
