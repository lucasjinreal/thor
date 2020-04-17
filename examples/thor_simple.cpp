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

#include <zconf.h>
#include <iostream>
#include "thor/logging.h"
#include "thor/macro.h"
#include "thor/os.h"
#include "thor/timer.h"
#include "thor/functions.h"

#include "thor/det.pb.h"
#include "thor/insg.pb.h"


using namespace std;
using namespace thor::log;
using namespace thor::dl;


int main() {
  PRINT_RED("hello, we are testing some basic function in thor...");

  PRINT_GREEN("1. test a timer.");
  thor::Timer timer(20);
  timer.on();
  LOG(INFO) << " count start..";
  for (int i = 0; i < 2; ++i) {
    sleep(1);
  }
  LOG(INFO) << " cost time: " << timer.lap();
  for (int i = 0; i < 2; ++i) {
    sleep(1);
  }
  LOG(INFO) << "another time: " << timer.lap();

  PRINT_GREEN("2. test datum.");
  if (thor::os::suffix("hhhaaa.mp4") == "mp4") {
    LOG(INFO) << "this is an video file.";
  }

  LOG(INFO) << "creating some folder recursively.";
  thor::os::makedirs("data/pointcloud", 07777);


  PRINT_GREEN("3. test softmax.");
  LOG(INFO) << "test for functions...";
  LOG(INFO)  << "        softmax:";
  double values[] = {-0.9381,  0.8967};
  double probs[2];
  thor::functions::softmax_1d(values, probs, 2);
  LOG(INFO) << probs[0] << " " << probs[1];

  PRINT_GREEN("4. test protos.");
  Detection2D det1;
  Box box;
  box.set_x1(23);
  box.set_y1(89);
  box.set_x2(99);
  box.set_y2(156);
  det1.set_allocated_box(&box);
  det1.set_cls_id(9);
  det1.set_prob(0.9);

  InstanceSegmentation seg1;
  seg1.set_allocated_detection(&det1);
  // float32
  seg1.add_mask(2.3);
  seg1.add_mask(2.3);
  seg1.add_mask(2.3);
  seg1.add_mask(2.3);

  LOG(INFO) << seg1.DebugString();

}