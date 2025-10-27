cc_library(
    name = "tracer",
    hdrs = [
        "span.h",
        "tracer.h",
    ],
    include_prefix = "warren/trace",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "attribute",
    hdrs = [
        "attribute.h",
    ],
    include_prefix = "warren/trace",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "event",
    hdrs = [
        "event.h",
    ],
    include_prefix = "warren/trace",
    visibility = ["//visibility:public"],
    deps = [
        "//time:time_point",
    ],
)
