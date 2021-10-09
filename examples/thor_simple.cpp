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

#include <fstream>
#include <iostream>

#include "thor/det.pb.h"
#include "thor/functions.h"
#include "thor/insg.pb.h"
#include "thor/logging.h"
#include "thor/macro.h"
#include "thor/os.h"
#include "thor/tar.h"
#include "thor/timer.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::iostream;
using std::vector;

using namespace thor::log;
using namespace thor::dl;

int main() {
  PRINT_BLUE("HELLO, we are testing some basic function in thor...");

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
  LOG(INFO) << "        softmax:";
  double values[] = {-0.9381, 0.8967};
  double probs[2];
  thor::functions::softmax_1d(values, probs, 2);
  LOG(INFO) << probs[0] << " " << probs[1];

  PRINT_GREEN("4. test protos.");
  FramePossession frame_possession;
  for (int i = 0; i < 100; ++i) {
    float score_i = 0.9;
    LOG(INFO) << i << " " << score_i;
    int cls_i = 5;
    LOG(INFO) << i << " " << cls_i;
    LOG(INFO) << "at: " << i;
    if (score_i > 0.2) {
      //            InstanceSegmentation *one_ins =
      //            frame_possession.add_instances();
      Detection2D one_det;
      Box one_box;
      one_box.set_x1(123);
      one_box.set_y1(88);
      one_box.set_x2(278);
      one_box.set_y2(190);
      one_det.mutable_box()->CopyFrom(one_box);
      one_det.set_prob(static_cast<double>(score_i));
      one_det.set_cls_id(cls_i);
      //            one_ins->set_allocated_detection(&one_det);
      //            one_ins->add_mask(0.5);
      //            one_ins->add_mask(0.6);

      LOG(INFO) << "cc";
      // add mask to one_ins
      //            LOG(INFO) << one_ins->DebugString();
      //            LOG(INFO) << frame_possession.DebugString();
    }
  }
  LOG(INFO) << frame_possession.DebugString();

  PRINT_GREEN("4. test tar.");
  TarFile tr("../test.tar");
  bool is_valid_tar_file = tr.IsValidTarFile();
  if (!is_valid_tar_file) {
    cout << "not valid tar file.\n";
    return -1;
  }
  auto ns = tr.GetFileNames();
  cout << ns.size() << endl;
  for (auto a : ns) {
    cout << a << endl;
  }
}
