/*
 * This file implements an lightweight alternative for glog, which is more
 * friendly for mobile.
 */

#pragma once

#ifndef _THOR_LOGGING_H
#define _THOR_LOGGING_H


#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


namespace thor{

namespace log{


// LOG()
#define LOG(status) LOG_##status.stream()
#define LOG_ERROR LOG_INFO
#define LOG_INFO LogMessage(__FILE__, __FUNCTION__, __LINE__, "I")
#define LOG_WARNING \
  LogMessage(__FILE__, __FUNCTION__, __LINE__, "W")
#define LOG_FATAL \
  LogMessageFatal(__FILE__, __FUNCTION__, __LINE__)
#define VLOG(level) VLogMessage(__FILE__, __FUNCTION__, __LINE__, level).stream()

// CHECK()
// clang-format off
#ifdef NDEBUG
#define CHECK(x) LogMessage(__FILE__, __FUNCTION__, __LINE__).stream()
#else // NDEBUG
#define CHECK(x) if (!(x)) LogMessageFatal(__FILE__, __FUNCTION__, __LINE__).stream() << "Check failed: " #x << ": " // NOLINT(*)
#endif // NDEBUG

// clang-format on
#define CHECK_EQ(x, y) _CHECK_BINARY(x, ==, y)
#define CHECK_NE(x, y) _CHECK_BINARY(x, !=, y)
#define CHECK_LT(x, y) _CHECK_BINARY(x, <, y)
#define CHECK_LE(x, y) _CHECK_BINARY(x, <=, y)
#define CHECK_GT(x, y) _CHECK_BINARY(x, >, y)
#define CHECK_GE(x, y) _CHECK_BINARY(x, >=, y)
#define _CHECK_BINARY(x, cmp, y) CHECK(x cmp y) << x << "!" #cmp << y << " "
#define CHECK_NOTNULL(val) CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))


using namespace std;


void gen_log(std::ostream& log_stream_, const char* file, const char* func, int lineno, const char* level, const int kMaxLen=15);

// LogMessage
class LogMessage {
 public:
#ifdef NDEBUG
  LogMessage(...) { }
  std::ostream& stream() { return log_stream_; }
#else
  LogMessage(const char* file, const char* func,
             int lineno, const char* level="I")  {
    gen_log(log_stream_, file, func, lineno, level);
  }

  ~LogMessage() {
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
  }

  std::ostream& stream() { return log_stream_; }
#endif // NDEBUG
 protected:
  std::stringstream log_stream_;
  LogMessage(const LogMessage&) = delete;
  void operator=(const LogMessage&) = delete;
};


// LogMessageFatal
class LogMessageFatal : public LogMessage {
 public:
#ifdef NDEBUG
  LogMessageFatal(...)
      : LogMessage() {}
  std::ostream& stream() { return log_stream_; }
#else
  LogMessageFatal(const char* file, const char* func,
                  int lineno, const char* level="F")
      : LogMessage(file, func, lineno, level) {}

  ~LogMessageFatal() {
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
    abort();
  }
#endif // NDEBUG
};


// VLOG
class VLogMessage {
 public:
#ifdef NDEBUG
  VLogMessage(...) {}
  std::ostream& stream() { return log_stream_; }
#else
  VLogMessage(const char* file, const char* func,
              int lineno, const int32_t level_int=0) {
    const char* GLOG_v = std::getenv("GLOG_v");
    GLOG_v_int = (GLOG_v && atoi(GLOG_v) > 0) ? atoi(GLOG_v) : 0;
    this->level_int = level_int;
    if (GLOG_v_int < level_int) {
      return;
    }
    const char* level = to_string(level_int).c_str();
    gen_log(log_stream_, file, func, lineno, level);
  }

  ~VLogMessage() {
    if (GLOG_v_int < this->level_int) {
      return;
    }
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
  }
  std::ostream& stream() { return log_stream_; }
#endif // NDEBUG
 protected:
  std::stringstream log_stream_;
  int32_t GLOG_v_int;
  int32_t level_int;

  VLogMessage(const VLogMessage&) = delete;
  void operator=(const VLogMessage&) = delete;
};

template <typename T>
T* CheckNotNull(const char *file, int line, const char *names, T* t) {
  if (t == NULL) {
    LogMessageFatal(file, names, line, "F");
  }
  return t;
}



}
}

#endif