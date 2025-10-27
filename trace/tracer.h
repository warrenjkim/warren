#pragma once

#include <memory>
#include <string_view>

#include "warren/trace/span.h"

namespace warren {

struct Tracer {
  virtual ~Tracer() = default;

  [[nodiscard]] virtual std::unique_ptr<trace::Span> make_span(
      std::string_view name) const = 0;

  [[nodiscard]] virtual std::unique_ptr<trace::Span> make_span(
      std::string_view name, const trace::SpanContext& context) const = 0;
};

}  // namespace warren
