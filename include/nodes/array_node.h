#pragma once

#include "node.h"
#include "utils/typedefs.h"  // ArrayType

namespace json {

class ArrayNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  ArrayNode(ArrayType arr);

 public:
  const ArrayType& get() const;

 private:
  ArrayType arr_;

 public:
  ArrayNode() = delete;
  ~ArrayNode() = default;
};

}  // namespace json
