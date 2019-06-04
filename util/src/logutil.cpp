#include <util/logutil.h>

#include <boost/log/utility/setup.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/string_literal.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <boost/shared_ptr.hpp>

namespace cc
{
namespace util
{

namespace
{

typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > sink_t;
typedef boost::log::sinks::synchronous_sink< boost::log::sinks::basic_text_ostream_backend<char> > sink_c;
static boost::shared_ptr< sink_t > g_file_sink;
static boost::shared_ptr< sink_c > g_console_sink;
static std::string compassRoot_;

void logFormatter(
  const boost::log::record_view& rec, boost::log::formatting_ostream& strm)
{
  auto severity = rec[boost::log::trivial::severity];

  if (severity)
  {
    // Set the color
    switch (severity.get())
    {
      case boost::log::trivial::debug:
        strm << "\033[32m";
        break;
      case boost::log::trivial::warning:
        strm << "\033[33m";
        break;
      case boost::log::trivial::error:
      case boost::log::trivial::fatal:
        strm << "\033[31m";
        break;
      default:
        break;
    }
  }

  std::string sLevel = boost::log::trivial::to_string(severity.get());
  std::transform(sLevel.begin(), sLevel.end(), sLevel.begin(), ::toupper);

  strm << "[" << sLevel << "] " << rec[boost::log::expressions::smessage];

  // Restore the default color
  if (severity)
  {
    strm << "\033[0m";
  }
}

}

boost::log::trivial::severity_level getSeverityLevel()
{
 return boost::log::attribute_cast<
   boost::log::attributes::mutable_constant<boost::log::trivial::severity_level>>(
     boost::log::core::get()->get_global_attributes()["Severity"]).get();
}

void initFileLogger() {
  g_file_sink = boost::log::add_file_log(
    boost::log::keywords::file_name = compassRoot_ + "/logs/communication_%N.log"
    ,boost::log::keywords::rotation_size = 10 * 1024 * 1024
    ,boost::log::keywords::auto_flush = true
    ,boost::log::keywords::open_mode = std::ios_base::app
    );
  g_file_sink->set_formatter(&logFormatter);
}

void initConsoleLogger() {
   g_console_sink = boost::log::add_console_log(
    std::cout,
    boost::log::keywords::auto_flush = true);
  g_console_sink->set_formatter(&logFormatter);
}

void initLogger(const std::string& compassRoot)
{
  compassRoot_ = compassRoot;
  initConsoleLogger();
}

void openLogFileStream() {
  boost::log::core::get()->remove_sink(g_console_sink);
  //g_console_sink.reset();
  initFileLogger();
}

void closeLogFileStream() {
  boost::log::core::get()->remove_sink(g_file_sink);
  //g_file_sink.reset();
  initConsoleLogger();
}

} // util
} // cc



