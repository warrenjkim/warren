#pragma once

#include "visitors/visitor.h"

namespace json {

class Node {
 public:
  virtual void accept(JsonVisitor& visitor) const = 0;
  virtual ~Node() = default;
};

}  // namespace json
