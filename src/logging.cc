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

#include "thor/logging.h"

namespace thor {

namespace log {

void gen_log(std::ostream& log_stream_, const char* file, const char* func,
             int lineno, const char* level, const int kMaxLen) {
  const int len = strlen(file);
  std::string time_str;
  struct tm tm_time;  // Time of creation of LogMessageThor
  time_t timestamp = time(NULL);
  localtime_r(&timestamp, &tm_time);
  struct timeval tv;
  gettimeofday(&tv, NULL);

  // print date / time
  log_stream_ << level << ' ' << std::setw(1) << 1 + tm_time.tm_mon << '/'
              << tm_time.tm_mday << ' ' << tm_time.tm_hour << ':'
              << std::setw(2) << tm_time.tm_min << ':' << std::setw(2)
              << tm_time.tm_sec << '.' << std::setw(3) << tv.tv_usec / 1000
              << " ";

  if (len > kMaxLen) {
    log_stream_ << "..." << file + len - kMaxLen << " " << func << ":" << lineno
                << "] ";
  } else {
    log_stream_ << file << " " << func << ":" << lineno << "] ";
  }
}

}  // namespace log
}  // namespace thor
