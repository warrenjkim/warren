cc_library(
    name = "json",
    visibility = ["//:__subpackages__"],
    deps = [
        "//json/parse:lexer",
        "//json/parse:parser",
        "//json/parse:reader",
        "//json/parse:token",
        "//json/utils:exception",
        "//json/utils:parse",
        "//json/utils:to_string",
        "//json/value",
    ],
)

test_suite(
    name = "tests",
    tests = [
        "//json/parse:tests",
        "//json/utils:tests",
        "//json/value:value_test",
    ],
)
