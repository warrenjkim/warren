#include "nodes/object_node.h"

#include <utility>  // move

#include "utils/typedefs.h"  // ObjectType
#include "visitors/visitor.h"

namespace json {

void ObjectNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

ObjectNode::ObjectNode(ObjectType map) : map_(std::move(map)) {}

const ObjectType& ObjectNode::get() const { return map_; }

}  // namespace json
