#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/async.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace xr5 {
namespace utils {

/* Singleton logger class */

class Logger {
public:
  // Disable constructor
  Logger() = delete;
  /**
   * @brief As this class should not be cloneable.
   */
  Logger(const Logger &other) = delete;

  /**
   * @brief As this class should not be assignable.
   */
  Logger &operator=(const Logger &) = delete;
  /**
   * @brief Initialize the logger
   *
   * @param[in] log_file The log file path where the logs will be written
   * @param[in] log_level An integer to set log level based on below values
   * @param[in] log_level_console An integer to set console log level based on
   * below values
   * trace = 0,
   * debug = 1,
   * info = 2,
   * warn = 3,
   * err = 4,
   * critical = 5,
   * off = 6
   *
   * @param[in] log_file the name of the log file, default value is "XR5.log"
   * @param[in] log_dir the directory where log file will be created, the
   * default value is "CMAKE_SOURCE_DIR/logs/". When cross compilation is
   * enabled the default value would be "/data/xr5/logs"
   */
  static void init(const std::string &log_file = "xr5.log",
                   const int log_level = 1, const int log_level_console = 2,
                   const std::string &logger_name = "XR5",
                   const unsigned int number_of_log_files = 3,
                   const std::string &log_dir = LOG_DIR,
                   const bool use_async_logger = true);

  /**
   * @brief Get the Logger object
   *
   * @return pointer to the spdlog object
   */
  static std::shared_ptr<spdlog::logger> getInstance();

  /**
    * @brief Set the logger level object
    * @param[in] log_level An integer to set log level based on :
        trace = 0,
        debug = 1,
        info = 2,
        warn = 3,
        err = 4,
        critical = 5,
        off = 6
   */
  static void setLoggerLevel(const int log_level);

private:
  static std::shared_ptr<spdlog::logger> logger_;
};
} // namespace utils
} // namespace xr5
#endif // LOGGER_H