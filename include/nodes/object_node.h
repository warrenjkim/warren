#pragma once

#include <cstddef>
#include <vector>

#include "node.h"
#include "nodes/key_value_node.h"

namespace json {

class JsonVisitor;

class ObjectNode : public Node {
 public:
  ~ObjectNode();

 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  void add(KeyValueNode* property);
  const std::vector<KeyValueNode*>& get() const;
  const size_t size() const;
  const bool empty() const;

 private:
  std::vector<KeyValueNode*> properties_;

 private:
  ObjectNode(const ObjectNode&) = delete;
  ObjectNode& operator=(const ObjectNode&) = delete;

 public:
  ObjectNode() = default;

 public:
  ObjectNode(ObjectNode&& other) noexcept;
  ObjectNode& operator=(ObjectNode&& other) noexcept;
};

}  // namespace json
