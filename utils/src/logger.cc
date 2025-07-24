#include "utils/logger.h"

namespace xr5 {
namespace utils {

std::shared_ptr<spdlog::logger> Logger::logger_;

std::shared_ptr<spdlog::logger> Logger::GetInstance() {
  if (logger_ == nullptr) {
    init();
    logger_->warn(
        "[Logger::GetInstance] Created a new logger with default values");
  }
  return logger_;
}

void Logger::setLoggerLevel(const int log_level) {
  logger_->set_level(spdlog::level::level_enum(log_level));
}

void Logger::init(const std::string &log_file, const int log_level,
                  const int log_level_console, const std::string &logger_name,
                  const unsigned int number_of_log_files,
                  const std::string &log_dir, const bool use_async_logger) {
  if (logger_ != nullptr) {
    logger_->warn("[Logger::initLogger] Logger is already initialized");
    return;
  }
  int log_level_console_allowed = log_level_console;
  int log_level_allowed = log_level;
  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  stdout_sink->set_level(spdlog::level::level_enum(log_level_console_allowed));
  auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      log_dir + log_file, 1024 * 1024 * 10, number_of_log_files);
  rotating_sink->set_level(spdlog::level::level_enum(log_level_allowed));

  std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};

  if (use_async_logger) {
    spdlog::init_thread_pool(8192, 1);
    logger_ = std::make_shared<spdlog::async_logger>(
        logger_name, sinks.begin(), sinks.end(), spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);
  } else {
    logger_ = std::make_shared<spdlog::logger>(logger_name, sinks.begin(),
                                               sinks.end());
  }

  logger_->set_level(spdlog::level::level_enum(
      std::min(log_level_console_allowed, log_level_allowed)));
  logger_->flush_on(spdlog::level::level_enum(log_level_allowed));
  logger_->info("[Logger::initLogger] Constructed logger with log_level as: {} "
                "and log_level_console as: {}",
                log_level_allowed, log_level_console_allowed);
}
} // namespace utils
} // namespace xr5