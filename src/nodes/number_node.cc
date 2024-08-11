#include "nodes/number_node.h"

#include <cstdint>
#include <variant>

#include "visitors/visitor.h"

namespace json {

void NumberNode::accept(JsonVisitor& visitor) const { visitor.visit(*this); }

NumberNode::NumberNode(const int64_t value) : value_(value) {}

NumberNode::NumberNode(const double value) : value_(value) {}

const std::variant<int64_t, double> NumberNode::get() const {
  return std::holds_alternative<int64_t>(value_) ? std::get<int64_t>(value_)
                                                 : std::get<double>(value_);
}

}  // namespace json
