#pragma once

namespace json {

enum TokenType : char {
  L_BRACE = '{',
  R_BRACE = '}',
  L_BRACKET = '[',
  R_BRACKET = ']',
  COMMA = ',',
  COLON = ':',
  QUOTE = '"',
  SOLIDUS = '\\',
  PLUS = '+',
  MINUS = '-',
  ZERO = '0',
  PERIOD = '.',
  EXPONENT = 'e',
};

}  // namespace json
