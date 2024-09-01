#pragma once
#include <boost/log/trivial.hpp>

#define TRACE(func, msg, level)                                          \
  BOOST_LOG_TRIVIAL(trace) << std::string(level * 2, ' ') << "[" << func \
                           << "] " << msg
#define DEBUG(func, msg, level)          \
  BOOST_LOG_TRIVIAL(debug) <<            \
      [](const size_t l) {               \
        std::string indent;              \
        indent.reserve(l * 4);           \
        for (size_t i = 0; i < l; i++) { \
          indent += "|   ";              \
        }                                \
        return indent;                   \
      }(level) << "[" << func            \
                           << "] " << msg

#define INFO(msg) BOOST_LOG_TRIVIAL(info) << msg

#define WARN(msg) BOOST_LOG_TRIVIAL(warn) << msg

#define ERROR(func, msg, level)                                          \
  BOOST_LOG_TRIVIAL(error) << std::string(level * 2, ' ') << "[" << func \
                           << "] " << msg

#define FATAL(msg) BOOST_LOG_TRIVIAL(fatal) << msg
