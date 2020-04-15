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
#include "include/timer.h"




namespace thor{
Timer::Timer(const float& hz)
    : hz_(hz),
      start_time_(),
      target_duration_(1.0 / hz),
      busy_(false) {}


Timer::Timer(): hz_(20),
                start_time_(),
                target_duration_(1.0/20),
                busy_(false){
}

void Timer::on() {
  start_time_ = std::chrono::system_clock::now();
  last_update_time_ = start_time_;
  busy_ = true;
}

double Timer::off() {
  std::chrono::duration<double> time_cost = std::chrono::system_clock::now() - start_time_;
  if (time_cost < target_duration_) {
    std::this_thread::sleep_for(target_duration_ - time_cost);
  }
  std::chrono::duration<double> total_time = std::chrono::system_clock::now() - start_time_;
  busy_ = false;
  return total_time.count();
}

//void Timer::press() {
//  last_update_time_ = std::chrono::system_clock::now();
//}

double Timer::lap() {
  // return as seconds
  // we only need seconds as readable
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::duration<double > duration = now - last_update_time_;
  last_update_time_ = now;
//  return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
// this should return as seconds
  return duration.count();
}

}