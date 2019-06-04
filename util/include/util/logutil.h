#ifndef CC_UTIL_LOGUTIL_H
#define CC_UTIL_LOGUTIL_H

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>

#define LOG(lvl) BOOST_LOG_TRIVIAL(lvl)

namespace cc 
{
namespace util 
{

void initLogger(const std::string& compassRoot);

void openLogFileStream();

void closeLogFileStream();

boost::log::trivial::severity_level getSeverityLevel();

} // util
} // cc

#endif /* CC_UTIL_LOGUTIL_H */
