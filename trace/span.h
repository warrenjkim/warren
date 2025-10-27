#pragma once

#include <array>
#include <string_view>

namespace warren {
namespace trace {

using TraceId = std::array<uint8_t, 16>;
using SpanId = std::array<uint8_t, 8>;

struct SpanContext {
  TraceId trace_id;
  SpanId span_id;
  bool sampled = true;
};

// Spans should contain:
// * trace_id
// * span_id
// * parent_id
// * start
// * end
// * attributes
// * events
// * trace status
struct Span {
  virtual ~Span() = default;

  virtual const SpanContext& context() const = 0;

  virtual void set_attribute(std::string_view key, std::string_view value) = 0;

  virtual void set_status(std::string_view message) = 0;

  virtual void event(std::string_view event) = 0;

  virtual void end() = 0;
};

}  // namespace trace
}  // namespace warren
