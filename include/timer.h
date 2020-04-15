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
//
// Created by fagangjin on 8/5/19.
//

/**
 *
 *
 * Our timer enables us count any time
 * whenever you want
 *
 *
 */

#ifndef THOR_INCLUDE_TIMER_H_
#define THOR_INCLUDE_TIMER_H_




#include <time.h>
#include <chrono>
#include <thread>

/**********************************
  Timer class
**********************************/


namespace thor {

class Timer {
 public:
  Timer(const float& hz);
  Timer();
  ~Timer(){};
  void on();
  double off();
//  /// Start measuring time
//  void press();
  double lap();

 private:
  float hz_;
  std::chrono::system_clock::time_point start_time_;
  std::chrono::system_clock::time_point last_update_time_;
  std::chrono::duration<double> target_duration_;
  bool busy_;
};

}



#endif //THOR_INCLUDE_TIMER_H_
