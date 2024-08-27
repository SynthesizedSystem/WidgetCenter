#pragma once

#define LOG_TRACE(format, ...)                                             \
  Logger::LogMessage(Logger::LogLevel::kTrace, __FILE__, __LINE__, format, \
                     ##__VA_ARGS__)
#define LOG_DEBUG(format, ...)                                             \
  Logger::LogMessage(Logger::LogLevel::kDebug, __FILE__, __LINE__, format, \
                     ##__VA_ARGS__)
#define LOG_INFO(format, ...)                                             \
  Logger::LogMessage(Logger::LogLevel::kInfo, __FILE__, __LINE__, format, \
                     ##__VA_ARGS__)
#define LOG_WARN(format, ...)                                             \
  Logger::LogMessage(Logger::LogLevel::kWarn, __FILE__, __LINE__, format, \
                     ##__VA_ARGS__)
#define LOG_ERROR(format, ...)                                             \
  Logger::LogMessage(Logger::LogLevel::kError, __FILE__, __LINE__, format, \
                     ##__VA_ARGS__)
#define LOG_FATAL(format, ...)                                             \
  Logger::LogMessage(Logger::LogLevel::kFatal, __FILE__, __LINE__, format, \
                     ##__VA_ARGS__)

class Logger {
 public:
  enum class LogLevel { kTrace, kDebug, kInfo, kWarn, kError, kFatal };

  static std::string LogLevelToString(LogLevel level);
  static std::string GetCurrentTimestamp();
  static std::string GetColoredFileLine(const char* file, int line);

  template <typename... Args>
  static void LogMessage(LogLevel level, const char* file, int line,
                         const char* format, Args... args);

 private:
#ifdef _TRACE
  static const LogLevel current_log_level = LogLevel::kTrace;
#elif _DEBUG
  static const LogLevel current_log_level = LogLevel::kDebug;
#else
  static const LogLevel current_log_level = LogLevel::kInfo;
#endif
};

template <typename... Args>
inline void Logger::LogMessage(LogLevel level, const char* file, int line,
                               const char* format, Args... args) {
  if (level < current_log_level) {
    return;
  }

  constexpr size_t buffer_size = 1024;
  char buffer[buffer_size];

  std::snprintf(buffer, buffer_size, format, args...);

  std::stringstream log_stream;
  log_stream << GetCurrentTimestamp() << " " << GetColoredFileLine(file, line)
             << " " << LogLevelToString(level) << " " << buffer << std::endl;

  std::cout << log_stream.str();
}
