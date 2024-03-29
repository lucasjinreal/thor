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
#include <string>
#include <vector>

#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "os.h"

using cv::imread;
using cv::VideoCapture;
using std::string;
using std::vector;

namespace thor {
namespace image {

typedef struct {
  int w;
  int h;
  int c;
  float *data;
} Image;

float *Normalize(cv::Mat img);

float *HWC2CHW(cv::Mat img, const float kMeans[3], const float kStds[3]);
float *HWC2CHW_fast(cv::Mat img, const float kMeans[3], const float kStds[3]);

cv::Mat read2mat(float *prob, cv::Mat out);
cv::Mat map2threeunchar(cv::Mat real_out, cv::Mat real_out_);

cv::Mat resizeAlongShortest(cv::Mat img, int target_w, int target_h);

} // namespace image

namespace iter {

template <class Item> class SourceIterator {
public:
  virtual Item *next() = 0;

public:
  // members
  std::vector<Item> item_pool;
  bool is_video_mode;
  int crt;
  cv::VideoCapture cap;
};

template <class Item> class ImageSourceIter : public SourceIterator<Item> {
public:
  explicit ImageSourceIter(std::string source);
  ~ImageSourceIter();

  Item *next();
};

template <class Item> ImageSourceIter<Item>::ImageSourceIter(string source) {
  // judge if this is directory, image file, or video file
  if (thor::os::isfile(source)) {
    // judge if it's video or image file
    if (thor::os::suffix(source) == "mp4" ||
        thor::os::suffix(source) == "avi") {
      this->is_video_mode = true;
      auto res = this->cap.open(source);
      if (!res) {
        std::cerr << "open video error! " << source << std::endl;
      }
    } else {
      cv::Mat itm = cv::imread(source);
      this->item_pool.push_back(itm);
    }
  } else if (thor::os::isdir(source)) {
  }
}

template <class Item> ImageSourceIter<Item>::~ImageSourceIter() {
  this->cap.release();
}

template <class Item> Item *ImageSourceIter<Item>::next() {
  if (this->is_video_mode) {
    cv::Mat a;
    this->cap >> a;
    return &a;
  } else {
    if (this->crt > this->item_pool.size()) {
      return NULL;
    } else {
      Item a = this->item_pool[this->crt];
      this->crt += 1;
      return &a;
    }
  }
}

} // namespace iter

} // namespace thor
