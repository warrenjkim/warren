#pragma once
#include <boost/log/trivial.hpp>

#define TRACE(msg) BOOST_LOG_TRIVIAL(trace) << msg;
#define DEBUG(msg) BOOST_LOG_TRIVIAL(debug) << msg;
#define INFO(msg) BOOST_LOG_TRIVIAL(info) << msg;
#define WARN(msg) BOOST_LOG_TRIVIAL(warn) << msg;
#define ERROR(msg) BOOST_LOG_TRIVIAL(error) << msg;
#define FATAL(msg) BOOST_LOG_TRIVIAL(fatal) << msg;
