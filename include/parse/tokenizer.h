#pragma once

#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <utility>  // pair

#include "utils/typedefs.h"

namespace json {

class Tokenizer {
 public:
  static std::optional<std::queue<std::string>> tokenize(std::string_view json);

 private:
  static std::optional<size_t> tokenize_object(const std::string_view json,
                                               size_t index,
                                               std::queue<std::string>& tokens);
  static std::optional<size_t> tokenize_array(const std::string_view json,
                                              size_t index,
                                              std::queue<std::string>& tokens);
  static std::optional<size_t> tokenize_value(std::string_view json,
                                              size_t index,
                                              std::queue<std::string>& tokens);

 private:
  static std::optional<size_t> tokenize_string(const std::string_view json,
                                               size_t index,
                                               std::queue<std::string>& tokens);
  static std::optional<size_t> tokenize_number(const std::string_view json,
                                               size_t index,
                                               std::queue<std::string>& tokens);
  static std::optional<size_t> tokenize_key_value(
      std::string_view json, size_t index, std::queue<std::string>& tokens);

 private:
  static std::pair<size_t, std::string> tokenize_integer(
      const std::string_view json, size_t index);
  static std::optional<size_t> tokenize_logical_value(
      const std::string_view json, size_t index,
      std::queue<std::string>& tokens);
  static std::optional<std::pair<size_t, std::string>>
  tokenize_control_character(std::string_view json, size_t index,
                             std::queue<std::string>& tokens);

 private:
  static bool consume(const char actual_token, TokenType expected_token,
                      std::queue<std::string>& tokens);
  static bool consume(const std::string& actual_token,
                      const std::string& expected_token,
                      std::queue<std::string>& tokens);
  static size_t strip_whitespace(std::string_view json, size_t index);
  static bool is_hex(const char c);

 private:
  Tokenizer() = delete;
  Tokenizer(const Tokenizer&) = delete;
  Tokenizer(Tokenizer&&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;
  Tokenizer& operator=(Tokenizer&&) = delete;
  ~Tokenizer() = delete;
};

}  // namespace json
