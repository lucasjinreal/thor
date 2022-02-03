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
// Created by jintian on 18-1-12.
//

#pragma once

/**
 *  Several Compute Vision Algorithm
 *
 *  we using cao::cv:: do many things such as:
 *  nms
 *  crfs
 */

#include "generic.h"
#include "iostream"
#include "numeric"
#include "string"
#include "structures.h"
#include "vector"

#ifdef USE_PROTOBUF
#include "proto/det.pb.h"
#endif

namespace thor {
namespace dl {

using std::vector;

#ifdef USE_PROTOBUF
using thor::dl::Detection2D;
#endif

// add several CLASSES here
vector<string> VOC_CLASSES = {"__background__",
                              "aeroplane",
                              "bicycle",
                              "bird",
                              "boat",
                              "bottle",
                              "bus",
                              "car",
                              "cat",
                              "chair",
                              "cow",
                              "diningtable",
                              "dog",
                              "horse",
                              "motorbike",
                              "person",
                              "pottedplant",
                              "sheep",
                              "sofa",
                              "train",
                              "tvmonitor"};

vector<string> VOC_CLASSES_NO_BK = {
    "aeroplane",   "bicycle", "bird",  "boat",      "bottle",
    "bus",         "car",     "cat",   "chair",     "cow",
    "diningtable", "dog",     "horse", "motorbike", "person",
    "pottedplant", "sheep",   "sofa",  "train",     "tvmonitor"};

vector<string> COCO_CLASSES = {"__background__",
                               "person",
                               "bicycle",
                               "car",
                               "motorcycle",
                               "airplane",
                               "bus",
                               "train",
                               "truck",
                               "boat",
                               "traffic light",
                               "fire hydrant",
                               "stop sign",
                               "parking meter",
                               "bench",
                               "bird",
                               "cat",
                               "dog",
                               "horse",
                               "sheep",
                               "cow",
                               "elephant",
                               "bear",
                               "zebra",
                               "giraffe",
                               "backpack",
                               "umbrella",
                               "handbag",
                               "tie",
                               "suitcase",
                               "frisbee",
                               "skis",
                               "snowboard",
                               "sports ball",
                               "kite",
                               "baseball bat",
                               "baseball glove",
                               "skateboard",
                               "surfboard",
                               "tennis racket",
                               "bottle",
                               "wine glass",
                               "cup",
                               "fork",
                               "knife",
                               "spoon",
                               "bowl",
                               "banana",
                               "apple",
                               "sandwich",
                               "orange",
                               "broccoli",
                               "carrot",
                               "hot dog",
                               "pizza",
                               "donut",
                               "cake",
                               "chair",
                               "couch",
                               "potted plant",
                               "bed",
                               "dining table",
                               "toilet",
                               "tv",
                               "laptop",
                               "mouse",
                               "remote",
                               "keyboard",
                               "cell phone",
                               "microwave",
                               "oven",
                               "toaster",
                               "sink",
                               "refrigerator",
                               "book",
                               "clock",
                               "vase",
                               "scissors",
                               "teddy bear",
                               "hair drier",
                               "toothbrush"};

vector<string> COCO_CLASSES_NO_BK = {
    "person",        "bicycle",      "car",
    "motorcycle",    "airplane",     "bus",
    "train",         "truck",        "boat",
    "traffic light", "fire hydrant", "stop sign",
    "parking meter", "bench",        "bird",
    "cat",           "dog",          "horse",
    "sheep",         "cow",          "elephant",
    "bear",          "zebra",        "giraffe",
    "backpack",      "umbrella",     "handbag",
    "tie",           "suitcase",     "frisbee",
    "skis",          "snowboard",    "sports ball",
    "kite",          "baseball bat", "baseball glove",
    "skateboard",    "surfboard",    "tennis racket",
    "bottle",        "wine glass",   "cup",
    "fork",          "knife",        "spoon",
    "bowl",          "banana",       "apple",
    "sandwich",      "orange",       "broccoli",
    "carrot",        "hot dog",      "pizza",
    "donut",         "cake",         "chair",
    "couch",         "potted plant", "bed",
    "dining table",  "toilet",       "tv",
    "laptop",        "mouse",        "remote",
    "keyboard",      "cell phone",   "microwave",
    "oven",          "toaster",      "sink",
    "refrigerator",  "book",         "clock",
    "vase",          "scissors",     "teddy bear",
    "hair drier",    "toothbrush"};

#ifdef USE_PROTOBUF
bool ClassAgnosticNonMaximumSuppression(const std::vector<Detection2D> &input,
                                        const double nms_threshold,
                                        std::vector<Detection2D> *output);

bool ClassAgnosticNonMaximumSuppression(const std::vector<Detection2D> &input,
                                        const double nms_threshold,
                                        std::vector<bool> *mask);
#endif

}  // namespace dl
}  // namespace thor
