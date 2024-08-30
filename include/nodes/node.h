#pragma once

namespace json {

class JsonVisitor;

class Node {
 public:
  virtual void accept(JsonVisitor& visitor) const = 0;
  virtual ~Node() = default;
};

}  // namespace json
