#pragma once

#include <string_view>

namespace warren {

namespace trace {

struct Span {
  virtual ~Span() = default;
  virtual void annotate(std::string_view key, std::string_view value) = 0;
  virtual void event(std::string_view name) = 0;
  virtual void end() = 0;
};

}  // namespace trace

struct Tracer {
  virtual ~Tracer() = default;
  virtual trace::Span* make_span(std::string_view name) = 0;
};

}  // namespace warren
