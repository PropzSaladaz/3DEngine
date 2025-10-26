#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Compile-time log level (cut logs from the binary in release if you want)
#ifndef MGL_LOG_LEVEL
  #ifdef NDEBUG
    #define MGL_LOG_LEVEL SPDLOG_LEVEL_INFO
  #else
    #define MGL_LOG_LEVEL SPDLOG_LEVEL_DEBUG
  #endif
#endif

namespace mgl::log {
inline void init(const std::string& filename = "mgl.log") {
  // Console + rotating file
  auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto file    = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, /*max_size*/ 8*1024*1024, /*max_files*/ 3);

  std::vector<spdlog::sink_ptr> sinks{console, file};
  auto logger = std::make_shared<spdlog::logger>("mgl", sinks.begin(), sinks.end());

  // Pattern includes timestamp, level, thread id, source file:line
  logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%^%l%$] [t:%t] %s:%# %! | %v");
  logger->set_level(static_cast<spdlog::level::level_enum>(MGL_LOG_LEVEL));
  spdlog::set_default_logger(logger);
  spdlog::flush_on(spdlog::level::info);
}

// Convenience macros that carry source location without colliding with LOG()
#define MGL_TRACE(...) SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::trace, __VA_ARGS__)
#define MGL_DEBUG(...) SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::debug, __VA_ARGS__)
#define MGL_INFO(...)  SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::info,  __VA_ARGS__)
#define MGL_WARN(...)  SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::warn,  __VA_ARGS__)
#define MGL_ERROR(...) SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::err,   __VA_ARGS__)
#define MGL_CRIT(...)  SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::critical, __VA_ARGS__)
} // namespace mgl::log
