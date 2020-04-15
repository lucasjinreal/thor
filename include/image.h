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
#pragma once
#include <opencv2/core/mat.hpp>
#include "opencv2/imgproc.hpp"


namespace thor{
namespace image {

typedef struct {
  int w;
  int h;
  int c;
  float *data;
} Image;

float* Normalize(cv::Mat img);

/**
 * this function using for convert cv::Mat default HWC data to CHW while doing
 *
 *    cv::Mat resizedImage = cv::Mat::zeros(INPUT_H, INPUT_W, CV_32FC3);
      cv::resize(frame, resizedImage, cv::Size(INPUT_W, INPUT_H));
      data = HWC2CHW(resizedImage, kMeans);
 *
 * @param img
 * @param kMeans
 * @param kStds
 * @return
 */
float* HWC2CHW(cv::Mat img, const float kMeans[3], const float kStds[3]);
float* HWC2CHW_fast(cv::Mat img, const float kMeans[3], const float kStds[3]);

cv::Mat read2mat(float * prob, cv::Mat out);
cv::Mat map2threeunchar(cv::Mat real_out, cv::Mat real_out_);


/**
 * Some resize function will be added,
 * such as resizeAlongShortest etc.
 *
 */
cv::Mat resizeAlongShortest(cv::Mat img, int target_w, int target_h);



}
}


