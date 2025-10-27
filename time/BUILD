test_suite(
    name = "tests",
    tests = [
        ":fake_clock_test",
    ],
)

cc_library(
    name = "time",
    visibility = ["//visibility:public"],
    deps = [
        ":clock",
        ":duration",
        ":fake_clock",
        ":system_clock",
    ],
)

cc_library(
    name = "duration",
    hdrs = [
        "duration.h",
    ],
    include_prefix = "warren/time",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "time_point",
    hdrs = [
        "time_point.h",
    ],
    include_prefix = "warren/time",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":duration",
    ],
)

cc_library(
    name = "clock",
    hdrs = [
        "clock.h",
    ],
    include_prefix = "warren/time",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":time_point",
    ],
)

cc_library(
    name = "system_clock",
    hdrs = [
        "system_clock.h",
    ],
    include_prefix = "warren/time",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":clock",
        ":time_point",
    ],
)

cc_library(
    name = "fake_clock",
    hdrs = [
        "fake_clock.h",
    ],
    include_prefix = "warren/time",
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":clock",
        ":time_point",
    ],
)

cc_test(
    name = "fake_clock_test",
    srcs = ["fake_clock_test.cc"],
    deps = [
        ":fake_clock",
        ":time_point",
        "@googletest//:gtest_main",
    ],
)
