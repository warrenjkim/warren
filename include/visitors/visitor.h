#pragma once

namespace json {

class JsonNode;
class ArrayNode;
class BooleanNode;
class NullNode;
class NumberNode;
class ObjectNode;
class StringNode;

class JsonVisitor {
 public:
  void visit(const ArrayNode& node);
  void visit(const BooleanNode& node);
  void visit(const NullNode& node);
  void visit(const NumberNode& node);
  void visit(const ObjectNode& node);
  void visit(const StringNode& node);
};

}  // namespace json
