#pragma once

#include <string>

#include "warren/json/parse/lexer.h"
#include "warren/json/parse/parser.h"
#include "warren/json/value.h"

namespace warren {
namespace json {

inline Value operator""_json(const char* json, size_t len) {
  return Parser(Lexer(std::string(json, len))).parse();
}

inline Value parse(std::string json) {
  return Parser(Lexer(std::move(json))).parse();
}

}  // namespace json
}  // namespace warren
