#include "nodes/object_node.h"

#include <vector>

#include "nodes/key_value_node.h"
#include "visitors/visitor.h"

namespace json {

ObjectNode::~ObjectNode() {
  for (KeyValueNode* property : properties_) {
    delete property;
  }
}

void ObjectNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

void ObjectNode::add(KeyValueNode* property) {
  properties_.push_back(property);
}

const std::vector<KeyValueNode*>& ObjectNode::get() const {
  return properties_;
}

const size_t ObjectNode::size() const {
  return properties_.size();
}

const bool ObjectNode::empty() const {
  return properties_.empty();
}

ObjectNode::ObjectNode(ObjectNode&& other) noexcept
    : properties_(std::move(other.properties_)) {}

ObjectNode& ObjectNode::operator=(ObjectNode&& other) noexcept {
  if (this != &other) {
    for (KeyValueNode* property : properties_) {
      delete property;
    }

    properties_ = std::move(other.properties_);
  }

  return *this;
}

}  // namespace json
