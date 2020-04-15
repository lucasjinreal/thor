/*
 * Copyright (c) 2020 Fagang Jin.
 *
 * This file is part of thor
 * (see manaai.cn).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
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
#define LOG_INFO LogMessageThor(__FILE__, __FUNCTION__, __LINE__, "I")
#define LOG_WARNING \
  LogMessageThor(__FILE__, __FUNCTION__, __LINE__, "W")
#define LOG_FATAL \
  LogMessageThorFatal(__FILE__, __FUNCTION__, __LINE__)
#define VLOG(level) VLogMessageThor(__FILE__, __FUNCTION__, __LINE__, level).stream()

// CHECK()
// clang-format off
#ifdef NDEBUG
#define CHECK(x) LogMessageThor(__FILE__, __FUNCTION__, __LINE__).stream()
#else // NDEBUG
#define CHECK(x) if (!(x)) LogMessageThorFatal(__FILE__, __FUNCTION__, __LINE__).stream() << "Check failed: " #x << ": " // NOLINT(*)
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

// this class should be rename since it's same as GLOG which may cause conflict

// LogMessageThor
class LogMessageThor {
 public:
#ifdef NDEBUG
  LogMessageThor(...) { }
  std::ostream& stream() { return log_stream_; }
#else
  LogMessageThor(const char* file, const char* func,
             int lineno, const char* level="I")  {
    thor::log::gen_log(log_stream_, file, func, lineno, level);
  }

  ~LogMessageThor() {
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
  }

  std::ostream& stream() { return log_stream_; }
#endif // NDEBUG
 protected:
  std::stringstream log_stream_;
  LogMessageThor(const LogMessageThor&) = delete;
  void operator=(const LogMessageThor&) = delete;
};


// LogMessageThorFatal
class LogMessageThorFatal : public LogMessageThor {
 public:
#ifdef NDEBUG
  LogMessageThorFatal(...)
      : LogMessageThor() {}
  std::ostream& stream() { return log_stream_; }
#else
  LogMessageThorFatal(const char* file, const char* func,
                  int lineno, const char* level="F")
      : LogMessageThor(file, func, lineno, level) {}

  ~LogMessageThorFatal() {
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
    abort();
  }
#endif // NDEBUG
};


// VLOG
class VLogMessageThor {
 public:
#ifdef NDEBUG
  VLogMessageThor(...) {}
  std::ostream& stream() { return log_stream_; }
#else
  VLogMessageThor(const char* file, const char* func,
              int lineno, const int32_t level_int=0) {
    const char* GLOG_v = std::getenv("GLOG_v");
    GLOG_v_int = (GLOG_v && atoi(GLOG_v) > 0) ? atoi(GLOG_v) : 0;
    this->level_int = level_int;
    if (GLOG_v_int < level_int) {
      return;
    }
    const char* level = to_string(level_int).c_str();
    thor::log::gen_log(log_stream_, file, func, lineno, level);
  }

  ~VLogMessageThor() {
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

  VLogMessageThor(const VLogMessageThor&) = delete;
  void operator=(const VLogMessageThor&) = delete;
};

template <typename T>
T* CheckNotNull(const char *file, int line, const char *names, T* t) {
  if (t == NULL) {
    LogMessageThorFatal(file, names, line, "F");
  }
  return t;
}



}
}

#endif