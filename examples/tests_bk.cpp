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

        float *insmask = instance_segmentation.mask().begin();
        // mask size must save with box_h * box_w
//        cv::Mat resized_mask = cv::Mat(28, 28, CV_32F, instance_segmentation.mask().begin());

//        LOG(INFO) << instance_segmentation.DebugString();
        if (instance_segmentation.mask().size() == box_h*box_w) {
            for (int r = 0; r < box_h; ++r) {
                int y_loc = r + instance_segmentation.detection().box().y1();
                for (int c = 0; c < box_w; ++c) {
                    int x_loc = c + instance_segmentation.detection().box().x1();
                    int offset = c + r * box_w;
                    LOG(INFO) << "mask " << instance_segmentation.mask().size();
                    LOG(INFO) << "offset: " << offset;
                    if (instance_segmentation.mask(offset) > 0) {
                        mask_binary.at<uchar>(y_loc, x_loc) = 255;
                        for (int channel = 0; channel < 3; channel++) {
                            float v1 = (1 - kMRTransparency);
                            float v2 =
                                    (kMRTransparency * 255.0 * kMRColorMap[color_idx + channel]);
//                            img.at<cv::Vec3b>(y_loc, x_loc)[channel] *= v1;
//                            img.at<cv::Vec3b>(y_loc, x_loc)[channel] += v2;
                            mask.at<cv::Vec3b>(y_loc, x_loc)[channel] = kCyan[channel];
                        }
                        img.at<uchar>(y_loc, x_loc, 0) = 255;
                    }
                }
            }
        }


        LOG(INFO) << "counters.";
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
//        LOG(INFO) << img.type();
//        img.convertTo(img, CV_8UC3);
        cv::findContours(mask_binary, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
        cv::Scalar color(255, 255, 255);
        cv::drawContours(img, contours, -1, color, 1, cv::LINE_AA);
    }

    cv::imshow("mask", mask);
//    cv::
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