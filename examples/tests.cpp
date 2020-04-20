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
#include <fstream>
#include "thor/logging.h"
#include "thor/macro.h"
#include "thor/os.h"
#include "thor/timer.h"
#include "thor/functions.h"

#include "thor/det.pb.h"
#include "thor/insg.pb.h"
#include "thor/vis.h"
#include "thor/dl.h"

#include "opencv4/opencv2/opencv.hpp"

using namespace std;
using namespace thor::log;
using namespace thor::dl;
using namespace thor::vis::color;

const int kMaskSize = 28;





int main(int argc, char** argv) {
    string pb_file = argv[1];
    string img_file = argv[2];

    LOG(INFO) << pb_file;
    LOG(INFO) << img_file;

    // read proto checkout the content
    FramePossession frame_possession;
    ifstream file(pb_file, ios::in | ios::binary);

    if (file.is_open())
    {
        if (!frame_possession.ParseFromIstream(&file))
        {
            printf("Failed to parse TextureAtlasEntry");
        }
    }
    LOG(INFO) << frame_possession.DebugString();

    // visualize the instance inside possession
    vector<InstanceSegmentation> all_instances;
//    *frame_possession.mutable_instances() = {all_instances.begin(), all_instances.end()};
    for(auto ins: frame_possession.instances()) {
        all_instances.emplace_back(ins);
    }

    LOG(INFO) << all_instances.size();

    cv::Mat img = cv::imread(img_file);
    LOG(INFO) << "img data type: " << img.type();
    auto res = thor::vis::VisualizeInstanceSegmentations(img, all_instances, thor::dl::COCO_CLASSES);
    cv::imshow("aa", img);
    cv::imshow("res", res);
    cv::waitKey(0);

    google::protobuf::ShutdownProtobufLibrary();
}