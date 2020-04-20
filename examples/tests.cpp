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


cv::Mat VisualizeInstanceSegmentations(cv::Mat &img,
                                       vector<thor::dl::InstanceSegmentation> instances,
                                       const vector<string> classes_names) {

    cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
    cv::Mat mask_binary = cv::Mat::zeros(img.size(), CV_8UC1);
    for (const auto &instance_segmentation : instances) {
        int category_id = instance_segmentation.detection().cls_id();
        std::string cat = classes_names[category_id];
        float confidence = instance_segmentation.detection().prob();
        char label[20];
        sprintf(label, "%s:%.2f", cat.c_str(), confidence);
        LOG(INFO) << label;
        cv::rectangle(img,
                      cv::Point(instance_segmentation.detection().box().x1(), instance_segmentation.detection().box().y1()),
                      cv::Point(instance_segmentation.detection().box().x2(), instance_segmentation.detection().box().y2()),
                      cv::Scalar(255, 0, 0));
        cv::putText(img, label,
                    cv::Point2i(instance_segmentation.detection().box().x1(), instance_segmentation.detection().box().y1()),
                    cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0));
        int color_idx = (category_id % kMRNumColors) * 3;
        int box_w = instance_segmentation.detection().box().x2() - instance_segmentation.detection().box().x1();
        int box_h = instance_segmentation.detection().box().y2() - instance_segmentation.detection().box().y1();

        LOG(INFO) << "mask " << instance_segmentation.mask_size() << " " << box_h*box_w;

        // mask size must save with box_h * box_w
        cv::Mat resized_mask(instance_segmentation.mask_h(), instance_segmentation.mask_w(), CV_32F, (float*) instance_segmentation.mask().begin());
        cv::resize(resized_mask, resized_mask, cv::Point(box_w, box_h));

        cv::Rect box(instance_segmentation.detection().box().x1(), instance_segmentation.detection().box().y1(),
                box_w, box_h);
        cv::Mat mask = (resized_mask > 0.1);
        Scalar color = thor::vis::gen_unique_color_cv(category_id);
        Mat coloredRoi = (0.3 * color + 0.7 * img(box));
        coloredRoi.convertTo(coloredRoi, CV_8UC3);

        vector<Mat> contours;
        Mat hierarchy;
        mask.convertTo(mask, CV_8U);
        findContours(mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        drawContours(coloredRoi, contours, -1, color, 5, LINE_8, hierarchy, 100);
        coloredRoi.copyTo(img(box), mask);
    }

    return img;
}


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
    auto res = VisualizeInstanceSegmentations(img, all_instances, thor::dl::COCO_CLASSES);
    cv::imshow("aa", img);
    cv::imshow("res", res);
    cv::waitKey(0);

    google::protobuf::ShutdownProtobufLibrary();
}