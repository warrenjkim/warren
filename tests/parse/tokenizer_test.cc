#include "parse/tokenizer.h"

#include <stdio.h>

#include <boost/log/trivial.hpp>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "utils/logger.h"

class JsonTokenizerTest : public ::testing::Test {
 protected:
  void SetUp() override { utils::init_logging(boost::log::trivial::debug); }

  void assert_tokenization(const std::string& input,
                           const std::vector<std::string>& expected_tokens) {
    auto result = json::Tokenizer::tokenize(input);
    ASSERT_TRUE(result.has_value()) << "Failed to tokenize: " << input;
    std::queue<std::string> actual_tokens = result.value();
    std::vector<std::string> actual_vector;
    while (!actual_tokens.empty()) {
      actual_vector.push_back(actual_tokens.front());
      actual_tokens.pop();
    }
    ASSERT_EQ(actual_vector, expected_tokens)
        << "Unexpected tokenization for: " << input;
  }

  void assert_invalid_input(const std::string& input) {
    auto result = json::Tokenizer::tokenize(input);
    ASSERT_FALSE(result.has_value())
        << "Incorrectly tokenized invalid input: " << input;
  }
};

TEST_F(JsonTokenizerTest, ValidIntegers) {
  assert_tokenization("[0]", {"[", "0", "]"});
  assert_tokenization("[42]", {"[", "42", "]"});
  assert_tokenization("[-0]", {"[", "-0", "]"});
  assert_tokenization("[-42]", {"[", "-42", "]"});
  assert_tokenization("[1234567890]", {"[", "1234567890", "]"});
  assert_tokenization("[9223372036854775807]",
                      {"[", "9223372036854775807", "]"});
  assert_tokenization("[-9223372036854775808]",
                      {"[", "-9223372036854775808", "]"});
}

TEST_F(JsonTokenizerTest, ValidFloats) {
  assert_tokenization("[0.0]", {"[", "0.0", "]"});
  assert_tokenization("[3.14]", {"[", "3.14", "]"});
  assert_tokenization("[-0.0]", {"[", "-0.0", "]"});
  assert_tokenization("[-3.14]", {"[", "-3.14", "]"});
  assert_tokenization("[0.123456789]", {"[", "0.123456789", "]"});
  assert_tokenization("[1.23e-45]", {"[", "1.23e-45", "]"});
  assert_tokenization("[3.402823e38]", {"[", "3.402823e38", "]"});
}

TEST_F(JsonTokenizerTest, ValidExponents) {
  assert_tokenization("[1e0]", {"[", "1e0", "]"});
  assert_tokenization("[1e+0]", {"[", "1e+0", "]"});
  assert_tokenization("[1e-0]", {"[", "1e-0", "]"});
  assert_tokenization("[1E10]", {"[", "1E10", "]"});
  assert_tokenization("[-1E-10]", {"[", "-1E-10", "]"});
  assert_tokenization("[3.14e2]", {"[", "3.14e2", "]"});
  assert_tokenization("[-3.14E-2]", {"[", "-3.14E-2", "]"});
  assert_tokenization("[1.23E+308]", {"[", "1.23E+308", "]"});
  assert_tokenization("[2.225E-308]", {"[", "2.225E-308", "]"});
}

TEST_F(JsonTokenizerTest, ValidZeroes) {
  assert_tokenization("[0]", {"[", "0", "]"});
  assert_tokenization("[-0]", {"[", "-0", "]"});
  assert_tokenization("[0.0]", {"[", "0.0", "]"});
  assert_tokenization("[-0.0]", {"[", "-0.0", "]"});
  assert_tokenization("[0e0]", {"[", "0e0", "]"});
  assert_tokenization("[-0e0]", {"[", "-0e0", "]"});
  assert_tokenization("[0e-0]", {"[", "0e-0", "]"});
  assert_tokenization("[-0e+0]", {"[", "-0e+0", "]"});
}

TEST_F(JsonTokenizerTest, InvalidNumbers) {
  assert_invalid_input("[.]");
  assert_invalid_input("[+1]");
  assert_invalid_input("[01]");
  assert_invalid_input("[1.]");
  assert_invalid_input("[.1]");
  assert_invalid_input("[1e]");
  assert_invalid_input("[1e+]");
  assert_invalid_input("[1e-]");
  assert_invalid_input("[1e1.0]");
  assert_invalid_input("[-]");
  assert_invalid_input("[--1]");
  assert_invalid_input("[1-]");
  assert_invalid_input("[1.2.3]");
  assert_invalid_input("[1,000]");
  assert_invalid_input("[Infinity]");
  assert_invalid_input("[NaN]");
  assert_invalid_input("[1a]");
  assert_invalid_input("[0x123]");
  assert_invalid_input("[0b1010]");
  assert_invalid_input("[0o123]");
  assert_invalid_input("[1_000]");
  assert_invalid_input("[１２３]");
}

TEST_F(JsonTokenizerTest, ValidEdgeCases) {
  assert_tokenization("[0e0]", {"[", "0e0", "]"});
  assert_tokenization("[-0e-0]", {"[", "-0e-0", "]"});
  assert_tokenization("[1e-999]", {"[", "1e-999", "]"});
  assert_tokenization("[1e+999]", {"[", "1e+999", "]"});
  assert_tokenization("[0.0000000000000000000001]",
                      {"[", "0.0000000000000000000001", "]"});
  assert_tokenization("[9999999999999999999999]",
                      {"[", "9999999999999999999999", "]"});
  assert_tokenization("[1e-323]", {"[", "1e-323", "]"});
  assert_tokenization("[1e308]", {"[", "1e308", "]"});
}

TEST_F(JsonTokenizerTest, ValidPrecision) {
  assert_tokenization("[0.123456789012345678901234567890]",
                      {"[", "0.123456789012345678901234567890", "]"});
  assert_tokenization("[1.000000000000000005]",
                      {"[", "1.000000000000000005", "]"});
  assert_tokenization("[0.6789]", {"[", "0.6789", "]"});
  assert_tokenization("[1.23456789e-100]", {"[", "1.23456789e-100", "]"});
}

TEST_F(JsonTokenizerTest, ValidBoundaryConditions) {
  assert_tokenization("[4.940656458412465e-324]",
                      {"[", "4.940656458412465e-324", "]"});
  assert_tokenization("[-4.940656458412465e-324]",
                      {"[", "-4.940656458412465e-324", "]"});
  assert_tokenization("[1.7976931348623157e308]",
                      {"[", "1.7976931348623157e308", "]"});
  assert_tokenization("[-1.7976931348623157e308]",
                      {"[", "-1.7976931348623157e308", "]"});
}

TEST_F(JsonTokenizerTest, Arrays) {
  assert_tokenization("[]", {"[", "]"});
  assert_tokenization("[1]", {"[", "1", "]"});
  assert_tokenization("[\"string\"]", {"[", "\"", "string", "\"", "]"});
  assert_tokenization("[true]", {"[", "true", "]"});
  assert_tokenization("[null]", {"[", "null", "]"});
  assert_tokenization("[{}]", {"[", "{", "}", "]"});
  assert_tokenization("[[]]", {"[", "[", "]", "]"});
  assert_tokenization("[1, 2, 3]", {"[", "1", ",", "2", ",", "3", "]"});
  assert_tokenization(
      "[\"a\", \"b\", \"c\"]",
      {"[", "\"", "a", "\"", ",", "\"", "b", "\"", ",", "\"", "c", "\"", "]"});
  assert_tokenization("[true, false, null]",
                      {"[", "true", ",", "false", ",", "null", "]"});
  assert_tokenization("[[1, 2], [3, 4]]", {"[", "[", "1", ",", "2", "]", ",",
                                           "[", "3", ",", "4", "]", "]"});
}

TEST_F(JsonTokenizerTest, Objects) {
  assert_tokenization("{}", {"{", "}"});
  assert_tokenization("{\"key\": \"value\"}",
                      {"{", "\"", "key", "\"", ":", "\"", "value", "\"", "}"});
  assert_tokenization(
      "{\"a\": 1, \"b\": true}",
      {"{", "\"", "a", "\"", ":", "1", ",", "\"", "b", "\"", ":", "true", "}"});
}

TEST_F(JsonTokenizerTest, MixedTypes) {
  assert_tokenization(
      "[1, \"string\", true, null, {\"key\": \"value\"}, [1, 2]]",
      {"[",    "1", ",", "\"", "string", "\"", ",", "true", ",",
       "null", ",", "{", "\"", "key",    "\"", ":", "\"",   "value",
       "\"",   "}", ",", "[",  "1",      ",",  "2", "]",    "]"});
}

TEST_F(JsonTokenizerTest, WhitespaceHandling) {
  assert_tokenization(" [ ] ", {"[", "]"});
  assert_tokenization("[ 1 , 2 , 3 ]", {"[", "1", ",", "2", ",", "3", "]"});
  assert_tokenization("[\n  1,\n  2,\n  3\n]",
                      {"[", "1", ",", "2", ",", "3", "]"});
  assert_tokenization("{ \"a\" : 1 }", {"{", "\"", "a", "\"", ":", "1", "}"});
}

TEST_F(JsonTokenizerTest, Strings) {
  assert_tokenization("[\"Hello, world!\"]",
                      {"[", "\"", "Hello, world!", "\"", "]"});
  assert_tokenization("[\"Escaped \\\" quote\"]",
                      {"[", "\"", "Escaped \\\" quote", "\"", "]"});
  assert_tokenization("[\"\\u0041\"]", {"[", "\"", "\\u0041", "\"", "]"});
  assert_tokenization("[\"\\n\\t\\r\\b\\f\"]",
                      {"[", "\"", "\\n\\t\\r\\b\\f", "\"", "]"});
}

TEST_F(JsonTokenizerTest, ComplexJson) {
  assert_tokenization(
      "{\"menu\": {"
      "  \"id\": \"file\","
      "  \"value\": \"File\","
      "  \"popup\": {"
      "    \"menuitem\": ["
      "      {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},"
      "      {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},"
      "      {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}"
      "    ]"
      "  }"
      "}}",
      {"{",
       "\"",
       "menu",
       "\"",
       ":",
       "{",
       "\"",
       "id",
       "\"",
       ":",
       "\"",
       "file",
       "\"",
       ",",
       "\"",
       "value",
       "\"",
       ":",
       "\"",
       "File",
       "\"",
       ",",
       "\"",
       "popup",
       "\"",
       ":",
       "{",
       "\"",
       "menuitem",
       "\"",
       ":",
       "[",
       "{",
       "\"",
       "value",
       "\"",
       ":",
       "\"",
       "New",
       "\"",
       ",",
       "\"",
       "onclick",
       "\"",
       ":",
       "\"",
       "CreateNewDoc()",
       "\"",
       "}",
       ",",
       "{",
       "\"",
       "value",
       "\"",
       ":",
       "\"",
       "Open",
       "\"",
       ",",
       "\"",
       "onclick",
       "\"",
       ":",
       "\"",
       "OpenDoc()",
       "\"",
       "}",
       ",",
       "{",
       "\"",
       "value",
       "\"",
       ":",
       "\"",
       "Close",
       "\"",
       ",",
       "\"",
       "onclick",
       "\"",
       ":",
       "\"",
       "CloseDoc()",
       "\"",
       "}",
       "]",
       "}",
       "}",
       "}"});
}

TEST_F(JsonTokenizerTest, ValidNestedStructures) {
  assert_tokenization("[[[[]]]]", {"[", "[", "[", "[", "]", "]", "]", "]"});
  assert_tokenization("{\"a\":{\"b\":{\"c\":{}}}}",
                      {"{", "\"", "a", "\"", ":", "{", "\"", "b", "\"", ":",
                       "{", "\"", "c", "\"", ":", "{", "}",  "}", "}",  "}"});
  assert_tokenization("[{\"a\":[1,{\"b\":2}]}]",
                      {"[", "{", "\"", "a", "\"", ":", "[", "1", ",", "{", "\"",
                       "b", "\"", ":", "2", "}", "]", "}", "]"});
}

TEST_F(JsonTokenizerTest, ValidEmptyStructures) {
  assert_tokenization("[]", {"[", "]"});
  assert_tokenization("{}", {"{", "}"});
  assert_tokenization("[{}]", {"[", "{", "}", "]"});
  assert_tokenization("{\"\":\"\"}",
                      {"{", "\"", "", "\"", ":", "\"", "", "\"", "}"});
}

TEST_F(JsonTokenizerTest, ValidUnicodeCharacters) {
  assert_tokenization(
      "[\"\\u0001\",\"\\uFFFF\"]",
      {"[", "\"", "\\u0001", "\"", ",", "\"", "\\uFFFF", "\"", "]"});
  assert_tokenization(
      "{\"\\u00A9\":\"copyright\"}",
      {"{", "\"", "\\u00A9", "\"", ":", "\"", "copyright", "\"", "}"});
  assert_tokenization("[\"😀\",\"🌈\",\"🚀\"]",
                      {"[", "\"", "😀", "\"", ",", "\"", "🌈", "\"", ",", "\"",
                       "🚀", "\"", "]"});
}

TEST_F(JsonTokenizerTest, ValidEscapedCharacters) {
  assert_tokenization("[\"\\b\\f\\n\\r\\t\"]",
                      {"[", "\"", "\\b\\f\\n\\r\\t", "\"", "]"});
  assert_tokenization("[\"\\\\\\\"\\//\"]",
                      {"[", "\"", "\\\\\\\"\\//", "\"", "]"});
  assert_tokenization(
      "{\"key\\tname\":\"value\\nhere\"}",
      {"{", "\"", "key\\tname", "\"", ":", "\"", "value\\nhere", "\"", "}"});
}

TEST_F(JsonTokenizerTest, ValidNumbers) {
  assert_tokenization(
      "[0,42,-0,-42,3.14,-3.14,1e10,1e-10,1.23e+10,-1.23E-10]",
      {"[",   "0",     ",",    "42",       ",",     "-0",        ",",
       "-42", ",",     "3.14", ",",        "-3.14", ",",         "1e10",
       ",",   "1e-10", ",",    "1.23e+10", ",",     "-1.23E-10", "]"});
}

TEST_F(JsonTokenizerTest, ValidMixedArrayTypes) {
  assert_tokenization(
      "[1,\"two\",3.14,true,null,{},[],-1]",
      {"[",    "1", ",", "\"", "two", "\"", ",", "3.14", ",",  "true", ",",
       "null", ",", "{", "}",  ",",   "[",  "]", ",",    "-1", "]"});
}

TEST_F(JsonTokenizerTest, ValidComplexObjectStructures) {
  assert_tokenization(
      "{\"a\":[1,2],\"b\":{\"c\":3,\"d\":[true,false]}}",
      {"{",  "\"", "a",  "\"", ":", "[",    "1", ",",     "2", "]", ",",
       "\"", "b",  "\"", ":",  "{", "\"",   "c", "\"",    ":", "3", ",",
       "\"", "d",  "\"", ":",  "[", "true", ",", "false", "]", "}", "}"});
}

TEST_F(JsonTokenizerTest, InvalidJSONStructures) {
  assert_invalid_input("[1,2,]");
  assert_invalid_input("{\"a\":1,}");
  assert_invalid_input("[1,2");
  assert_invalid_input("{\"a\":1");
  assert_invalid_input("\"unclosed string");
  assert_invalid_input("{a:1}");
  assert_invalid_input("[+1]");
  assert_invalid_input("[.1]");
  assert_invalid_input("[1.]");
  assert_invalid_input("[,]");
  assert_invalid_input("{,}");
  assert_invalid_input("{\"a\":}");
  assert_invalid_input("{:1}");
  assert_invalid_input("[01]");
  assert_invalid_input("[1e]");
  assert_invalid_input("[\"\\u123\"]");
}

TEST_F(JsonTokenizerTest, ValidEdgeCaseStrings) {
  assert_tokenization("[\"\\u0000\"]", {"[", "\"", "\\u0000", "\"", "]"});
  assert_tokenization("[\"\\\\\\\"\"]", {"[", "\"", "\\\\\\\"", "\"", "]"});
  assert_tokenization("[\"line1\\nline2\\nline3\"]",
                      {"[", "\"", "line1\\nline2\\nline3", "\"", "]"});
}
