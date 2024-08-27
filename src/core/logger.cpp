#include "logger.h"

std::string Logger::LogLevelToString(LogLevel level) {
  switch (level) {
    case LogLevel::kTrace:
      return "\033[36mTRACE\033[0m";  // Cyan
    case LogLevel::kDebug:
      return "\033[34mDEBUG\033[0m";  // Blue
    case LogLevel::kInfo:
      return "\033[32mINFO\033[0m";  // Green
    case LogLevel::kWarn:
      return "\033[33mWARN\033[0m";  // Yellow
    case LogLevel::kError:
      return "\033[31mERROR\033[0m";  // Red
    case LogLevel::kFatal:
      return "\033[41mFATAL\033[0m";  // Red background
    default:
      return "UNKNOWN";
  }
}

std::string Logger::GetCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm tm;
  localtime_s(&tm, &now_time);

  std::stringstream ss;
  ss << "\u001b[38;2;127;255;212m" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
     << "\033[0m";  // Aquamarine timestamp
  return ss.str();
}

std::string Logger::GetColoredFileLine(const char* file, int line) {
  std::stringstream ss;
  ss << "\033[90m" << file << ":" << line << "\033[0m";  // Bright Black (Gray)
  return ss.str();
}
