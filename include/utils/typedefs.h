#pragma once

#include <map>
#include <memory>  // unique_ptr
#include <vector>

#include "nodes/node.h"

namespace json {

using ArrayType = std::vector<std::unique_ptr<Node>>;
using ObjectType = std::map<std::string, std::unique_ptr<Node>>;

}  // namespace json
