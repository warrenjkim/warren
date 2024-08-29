#include "parse/tokenizer.h"

#include <cctype>
#include <cstdio>
#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <utility>  // pair

#include "utils/macros.h"
#include "utils/typedefs.h"

namespace json {

std::optional<std::queue<std::string>> Tokenizer::tokenize(
    std::string_view json) {
  std::queue<std::string> tokens;
  DEBUG("[tokenize] string to parse: " << json);

  size_t index = strip_whitespace(json, 0);
  if (index >= json.length()) {
    return tokens;
  }

  std::optional<size_t> new_index;
  switch (json[index]) {
    case L_BRACE:
      new_index = tokenize_object(json, index, tokens);
      break;
    case L_BRACKET:
      new_index = tokenize_array(json, index, tokens);
      break;
    default:
      return std::nullopt;
  }

  if (!new_index.has_value()) {
    return std::nullopt;
  }

  index = strip_whitespace(json, *new_index);
  if (index < json.length()) {
    return std::nullopt;
  }

  return tokens;
}

std::optional<size_t> Tokenizer::tokenize_object(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  char c = json[index];
  consume(c, L_BRACE, tokens);

  index = strip_whitespace(json, index + 1);
  if (index >= json.length()) {
    return std::nullopt;
  }

  if (consume(json[index], R_BRACE, tokens)) {
    return strip_whitespace(json, index + 1);
  }

  while (true) {
    std::optional<size_t> new_index = tokenize_key_value(json, index, tokens);
    if (!new_index.has_value()) {
      return std::nullopt;
    }

    index = strip_whitespace(json, *new_index);
    if (index >= json.length()) {
      return std::nullopt;
    }

    c = json[index];
    DEBUG("[tokenize_object] (Current character, Index): (" << c << ", "
                                                            << index << ")");

    if (consume(c, R_BRACE, tokens)) {
      return strip_whitespace(json, index + 1);
    }

    if (!consume(c, COMMA, tokens)) {
      return std::nullopt;
    }

    index = strip_whitespace(json, index + 1);
    if (index >= json.length()) {
      return std::nullopt;
    }
  }
}

std::optional<size_t> Tokenizer::tokenize_array(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  char c = json[index];
  consume(c, L_BRACKET, tokens);

  index = strip_whitespace(json, index + 1);
  if (index >= json.length()) {
    return std::nullopt;
  }

  if (consume(json[index], R_BRACKET, tokens)) {
    return strip_whitespace(json, index + 1);
  }

  while (true) {
    std::optional<size_t> new_index = tokenize_value(json, index, tokens);
    if (!new_index.has_value()) {
      return std::nullopt;
    }

    index = strip_whitespace(json, *new_index);
    if (index >= json.length()) {
      return std::nullopt;
    }

    c = json[index];
    DEBUG("[tokenize_array] (Current character, Index): (" << c << ", " << index
                                                           << ")");

    if (consume(c, R_BRACKET, tokens)) {
      return strip_whitespace(json, index + 1);
    }

    if (!consume(c, COMMA, tokens)) {
      return std::nullopt;
    }

    index = strip_whitespace(json, index + 1);
    if (index >= json.length()) {
      return std::nullopt;
    }
  }
}

std::optional<size_t> Tokenizer::tokenize_value(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  char c = json[index];
  DEBUG("[tokenize_value] (Current character, Index): (" << c << ", " << index
                                                         << ")");
  if (c == QUOTE) {
    return tokenize_string(json, index, tokens);
  } else if (c == L_BRACE) {
    return tokenize_object(json, index, tokens);
  } else if (c == L_BRACKET) {
    return tokenize_array(json, index, tokens);
  } else if (isdigit(c) || c == MINUS) {
    return tokenize_number(json, index, tokens);
  } else if (tolower(c) == 't' || tolower(c) == 'f' || tolower(c) == 'n') {
    return tokenize_logical_value(json, index, tokens);
  }

  return std::nullopt;
}

std::optional<size_t> Tokenizer::tokenize_string(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  char c = json[index++];
  consume(c, QUOTE, tokens);

  std::string token = "";
  while (true) {
    if (index >= json.length()) {
      return std::nullopt;
    }

    c = json[index++];
    DEBUG("[tokenize_string] (Current character, Index): (" << c << ", "
                                                            << index << ")");

    if (c == QUOTE) {
      tokens.emplace(token);
      consume(c, QUOTE, tokens);

      return strip_whitespace(json, index);
    }

    if (c == SOLIDUS) {
      auto ctrl_char_result = tokenize_control_character(json, index, tokens);
      if (!ctrl_char_result.has_value()) {
        return std::nullopt;
      }

      index = ctrl_char_result->first;
      token += ctrl_char_result->second;
    } else {
      token += c;
    }
  }

  return std::nullopt;
}

std::optional<size_t> Tokenizer::tokenize_number(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  std::string token = "";
  if (json[index] == MINUS) {
    token += MINUS;
    if (++index >= json.length() || !isdigit(json[index])) {
      return std::nullopt;
    }
  }

  DEBUG("[tokenize_number -- Sign] (Token, Index): (" << token << ", " << index
                                                      << ")");

  auto [new_index, integer] = tokenize_integer(json, index);
  token += integer;
  index = strip_whitespace(json, new_index);

  DEBUG("[tokenize_number -- Integer] (Token, Index): (" << token << ", "
                                                         << index << ")");

  if (index < json.length() && json[index] == PERIOD) {
    token += PERIOD;
    auto [new_index, fraction] = tokenize_integer(json, index + 1);
    if (fraction.empty()) {
      return std::nullopt;
    }

    token += fraction;
    index = strip_whitespace(json, new_index);
    DEBUG("[tokenize_number -- Fraction] (Token, Index): (" << token << ", "
                                                            << index << ")");
  }

  if (index < json.length() && tolower(json[index]) == EXPONENT) {
    token += json[index++];
    if (index < json.length() &&
        (json[index] == PLUS || json[index] == MINUS)) {
      token += json[index++];
    }
    DEBUG("[tokenize_number -- Exponent Sign] (Token, Index): ("
          << token << ", " << index << ")");

    auto [new_index, exponent] = tokenize_integer(json, index);
    if (exponent.empty()) {
      return std::nullopt;
    }

    token += exponent;
    index = strip_whitespace(json, new_index);
    DEBUG("[tokenize_number -- Exponent Integer] (Token, Index): ("
          << token << ", " << index << ")");
  }

  if (token.front() == ZERO && token.length() > 1 &&
      (token.at(1) != PERIOD && token.at(1) != EXPONENT)) {
    return std::nullopt;
  }

  tokens.emplace(token);
  return index;
}

std::optional<size_t> Tokenizer::tokenize_key_value(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  std::optional<size_t> new_index = tokenize_string(json, index, tokens);
  if (!new_index.has_value()) {
    return std::nullopt;
  }

  index = strip_whitespace(json, *new_index);
  if (index >= json.length()) {
    return std::nullopt;
  }

  char c = json[index];
  if (!consume(c, COLON, tokens)) {
    return std::nullopt;
  }

  index = strip_whitespace(json, index + 1);
  if (index >= json.length()) {
    return std::nullopt;
  }

  new_index = tokenize_value(json, index, tokens);
  if (!new_index.has_value()) {
    return std::nullopt;
  }

  return strip_whitespace(json, *new_index);
}

std::pair<size_t, std::string> Tokenizer::tokenize_integer(
    const std::string_view json, size_t index) {
  std::string token = "";
  char c = json[index];
  while (isdigit(c)) {
    token += c;
    if (++index >= json.length()) {
      break;
    }

    c = json[index];
  }

  return std::make_pair(index, token);
}

std::optional<size_t> Tokenizer::tokenize_logical_value(
    const std::string_view json, size_t index,
    std::queue<std::string>& tokens) {
  switch (json[index]) {
    case 't':
      if (!consume(std::string(json.substr(index, 4)), "true", tokens)) {
        return std::nullopt;
      }

      return strip_whitespace(json, index + 4);
    case 'f':
      if (!consume(std::string(json.substr(index, 5)), "false", tokens)) {
        return std::nullopt;
      }

      return strip_whitespace(json, index + 5);
    case 'n':
      if (!consume(std::string(json.substr(index, 4)), "null", tokens)) {
        return std::nullopt;
      }

      return strip_whitespace(json, index + 4);
  }

  return std::nullopt;
}

std::optional<std::pair<size_t, std::string>>
Tokenizer::tokenize_control_character(const std::string_view json, size_t index,
                                      std::queue<std::string>& tokens) {
  if (index >= json.length()) {
    return std::nullopt;
  }

  char c = json[index];
  DEBUG("[tokenize_control_character] (Current character, Index): ("
        << c << ", " << index << ")");
  std::string token = "\\";
  const size_t HEX_CODE_LENGTH = 4;
  switch (c) {
    case 'u':
      token += c;
      if (++index >= json.length()) {
        return std::nullopt;
      }

      for (size_t i = 0; i < HEX_CODE_LENGTH; i++) {
        char hex_digit = json[index++];
        DEBUG("[tokenize_control_character] (Current character, Index): ("
              << hex_digit << ", " << index << ")");
        if (!is_hex(hex_digit) || index >= json.length()) {
          return std::nullopt;
        }

        token += hex_digit;
      }

      return std::make_pair(index, token);
    case '"':
    case '\\':
    case '/':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
      token += c;
      return std::make_pair(index + 1, token);
    default:
      return std::nullopt;
  }

  return std::nullopt;
}

bool Tokenizer::consume(const std::string& actual_token,
                        const std::string& expected_token,
                        std::queue<std::string>& tokens) {
  if (actual_token != expected_token) {
    return false;
  }

  tokens.emplace(actual_token);

  return true;
}

bool Tokenizer::consume(const char actual_token, TokenType expected_token,
                        std::queue<std::string>& tokens) {
  if (actual_token != expected_token) {
    return false;
  }

  tokens.emplace(std::string(1, actual_token));

  return true;
}

size_t Tokenizer::strip_whitespace(std::string_view json, size_t index) {
  while (index < json.length() && isspace(json[index])) {
    index++;
  }

  return index;
}

bool Tokenizer::is_hex(const char c) {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      return true;
    default:
      return false;
  }
}

}  // namespace json
