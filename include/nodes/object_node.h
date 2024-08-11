#pragma once

#include "node.h"
#include "utils/typedefs.h"  // ObjectType

namespace json {

class ObjectNode : public Node {
 public:
  void accept(JsonVisitor& visitor) const override;

 public:
  ObjectNode(ObjectType map);

 public:
  const ObjectType& get() const;

 private:
  ObjectType map_;

 public:
  ObjectNode() = delete;
  ~ObjectNode() = default;
};

}  // namespace json
