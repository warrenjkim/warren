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

TEST_F(JsonTokenizerTest, ValidEmptyAndWhitespace) {
  assert_tokenization("", {});
  assert_tokenization(" [ ] ", {"[", "]"});
  assert_tokenization("[\n  1,\n  2,\n  3\n]",
                      {"[", "1", ",", "2", ",", "3", "]"});
}

TEST_F(JsonTokenizerTest, ValidBasicStructures) {
  assert_tokenization("[]", {"[", "]"});
  assert_tokenization("{}", {"{", "}"});
  assert_tokenization("[{}]", {"[", "{", "}", "]"});
  assert_tokenization("{\"\":\"\"}",
                      {"{", "\"", "", "\"", ":", "\"", "", "\"", "}"});
}

TEST_F(JsonTokenizerTest, ValidNumbers) {
  assert_tokenization(
      "[0,42,-0,-42,3.14,-3.14,1e10,1e-10,1.23e+10,-1.23E-10]",
      {"[",   "0",     ",",    "42",       ",",     "-0",        ",",
       "-42", ",",     "3.14", ",",        "-3.14", ",",         "1e10",
       ",",   "1e-10", ",",    "1.23e+10", ",",     "-1.23E-10", "]"});
  assert_tokenization("[0e0,-0e-0,1e-999,1e+999,1e308]",
                      {"[", "0e0", ",", "-0e-0", ",", "1e-999", ",", "1e+999",
                       ",", "1e308", "]"});
  assert_tokenization(
      "[0.0000000000000000000001,9999999999999999999999]",
      {"[", "0.0000000000000000000001", ",", "9999999999999999999999", "]"});
}

TEST_F(JsonTokenizerTest, ValidStrings) {
  assert_tokenization(
      "[\"Hello, world!\",\"Escaped \\\" "
      "quote\",\"\\u0041\",\"\\n\\t\\r\\b\\f\"]",
      {"[", "\"", "Hello, world!", "\"", ",", "\"", "Escaped \\\" quote", "\"",
       ",", "\"", "\\u0041", "\"", ",", "\"", "\\n\\t\\r\\b\\f", "\"", "]"});
  assert_tokenization("[\"\\u0000\",\"\\\\\\\"\",\"line1\\nline2\\nline3\"]",
                      {"[", "\"", "\\u0000", "\"", ",", "\"", "\\\\\\\"", "\"",
                       ",", "\"", "line1\\nline2\\nline3", "\"", "]"});
}

TEST_F(JsonTokenizerTest, ValidUnicode) {
  assert_tokenization("[\"\\u0001\",\"\\uFFFF\",\"😀\",\"🌈\",\"🚀\"]",
                      {"[",  "\"", "\\u0001", "\"", ",",  "\"", "\\uFFFF",
                       "\"", ",",  "\"",      "😀", "\"", ",",  "\"",
                       "🌈", "\"", ",",       "\"", "🚀", "\"", "]"});
  assert_tokenization(
      "{\"\\u00A9\":\"copyright\"}",
      {"{", "\"", "\\u00A9", "\"", ":", "\"", "copyright", "\"", "}"});
}

TEST_F(JsonTokenizerTest, ValidArrays) {
  assert_tokenization(
      "[1,\"two\",3.14,true,null,{},[],-1]",
      {"[",    "1", ",", "\"", "two", "\"", ",", "3.14", ",",  "true", ",",
       "null", ",", "{", "}",  ",",   "[",  "]", ",",    "-1", "]"});
}

TEST_F(JsonTokenizerTest, ValidObjects) {
  assert_tokenization(
      "{\"key\":\"value\",\"nested\":{\"array\":[1,2,3]}}",
      {"{",  "\"",     "key", "\"", ":", "\"", "value", "\"", ",",
       "\"", "nested", "\"",  ":",  "{", "\"", "array", "\"", ":",
       "[",  "1",      ",",   "2",  ",", "3",  "]",     "}",  "}"});
}

TEST_F(JsonTokenizerTest, ValidNestedStructures) {
  assert_tokenization("[[[[]]]]", {"[", "[", "[", "[", "]", "]", "]", "]"});
  assert_tokenization("{\"a\":{\"b\":{\"c\":{}}}}",
                      {"{", "\"", "a", "\"", ":", "{", "\"", "b", "\"", ":",
                       "{", "\"", "c", "\"", ":", "{", "}",  "}", "}",  "}"});
}

TEST_F(JsonTokenizerTest, ValidComplexJson) {
  assert_tokenization(
      "{\"menu\":{\"id\":\"file\",\"value\":\"File\",\"popup\":{\"menuitem\":["
      "{\"value\":\"New\",\"onclick\":\"CreateNewDoc()\"},"
      "{\"value\":\"Open\",\"onclick\":\"OpenDoc()\"},"
      "{\"value\":\"Close\",\"onclick\":\"CloseDoc()\"}]}}}",
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

TEST_F(JsonTokenizerTest, ValidLogicalValues) {
  assert_tokenization("[true]", {"[", "true", "]"});
  assert_tokenization("[false]", {"[", "false", "]"});
  assert_tokenization("[null]", {"[", "null", "]"});
}

TEST_F(JsonTokenizerTest, InvalidExtraTokens) {
  assert_invalid_input("{}}");
  assert_invalid_input("{{");
  assert_invalid_input("{");
  assert_invalid_input("{\"\":1,");
  assert_invalid_input("{\"\":1{");
  assert_invalid_input("{ ");
  assert_invalid_input("[]]");
  assert_invalid_input("[[");
  assert_invalid_input("[ ");
  assert_invalid_input("[10, ");
}

TEST_F(JsonTokenizerTest, InvalidLogicalValues) {
  assert_invalid_input("[treu]");
  assert_invalid_input("[fals]");
  assert_invalid_input("[nul]");
}

TEST_F(JsonTokenizerTest, InvalidControlCharacters) {
  assert_invalid_input("[\"\\u123k5\"]");
  assert_invalid_input("[\"\\u");
  assert_invalid_input("[\"\\u123");
  assert_invalid_input("[\"\\");
  assert_invalid_input("[\"\\ \"]");
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

TEST_F(JsonTokenizerTest, InvalidStructures) {
  assert_invalid_input("[1,2,]");
  assert_invalid_input("{\"a\":1,}");
  assert_invalid_input("[1,2");
  assert_invalid_input("{\"a\":1");
  assert_invalid_input("\"unclosed string");
  assert_invalid_input("{a:1}");
  assert_invalid_input("[,]");
  assert_invalid_input("{,}");
  assert_invalid_input("{\"a\":}");
  assert_invalid_input("{:1}");
}

TEST_F(JsonTokenizerTest, InvalidKeyValues) {
  assert_invalid_input("{\"\"");
  assert_invalid_input("{\"\"a");
  assert_invalid_input("{\"\":");
}
