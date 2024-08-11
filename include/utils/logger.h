#pragma once

#include <boost/log/trivial.hpp>

namespace utils {

void init_logging(boost::log::trivial::severity_level min_severity);

}
