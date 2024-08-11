#include "nodes/array_node.h"

#include <utility>  // move

#include "utils/typedefs.h"  // ArrayType
#include "visitors/visitor.h"

namespace json {

void ArrayNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

ArrayNode::ArrayNode(ArrayType arr) : arr_(std::move(arr)) {}

const ArrayType& ArrayNode::get() const { return arr_; }

}  // namespace json
