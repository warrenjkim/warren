#pragma once

#include <cstddef>

#include "warren/json/parse/lexer.h"
#include "warren/json/value.h"

namespace warren {
namespace json {

class Parser {
 public:
  explicit Parser(Lexer lexer);

  Parser(Parser&&) noexcept = default;
  Parser& operator=(Parser&&) noexcept = default;

  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  Value parse();

 private:
  Value parse_value();

  nullptr_t parse_null();
  bool parse_boolean();
  std::string parse_string();
  Value parse_number();
  array_t parse_array();
  object_t parse_object();

 private:
  Lexer lexer_;
};

}  // namespace json
}  // namespace warren
