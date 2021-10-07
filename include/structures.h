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
#ifndef COMMON_STRUCTURE_H
#define COMMON_STRUCTURE_H

#include <algorithm>

#include "iostream"

//#ifdef USE_OPENCV
// #ifndef USE_OPENCV4
// #include "opencv/cv.hpp"
// #endif
#include "opencv2/opencv.hpp"
//#endif

using namespace std;

/**
 *
 * We start defining all useful structures here
 * currently lots of usage need declaration of structures
 * while some of them can be standardized and make it public
 * as a common usage
 *
 * such as:
 *
 * BoundingBoxes
 * HumanFaces
 * HumanPoses
 * ....
 *
 * (We do not using protobuf as there no need to add
 * more dependencies to thor)
 *
 */

namespace thor {
enum BoxFormat {
  XYXY,
  TLWH,
  BOTH,
};

struct Bbox {
  float xmin;
  float ymin;
  float xmax;
  float ymax;
  float score;
  int cid;
};
struct InstanceSegmentation {
  // Bbox box;

  float xmin;
  float ymin;
  float xmax;
  float ymax;
  float score;
  int cid;  // mask size uncertain
  float *mask;
  // vector<float> mask_vec;
  // id for ith position in mem data of masks
  int idx;
};

// simple box declaration
struct Box {
  // left, top, right, bottom
  // left top is the origin
  int top;
  int left;
  int w;
  int h;

  // box score, only for prediction
  float score;
  // label index
  int idx;

  int xmin;
  int ymin;
  int xmax;
  int ymax;

  BoxFormat format;

  Box() {}
  Box(int a, int b, int c, int d, int format = BoxFormat::XYXY) {
    switch (format) {
      case XYXY:
        xmin = a;
        ymin = b;
        xmax = c;
        ymax = d;
        this->format = XYXY;
        break;
      case TLWH:
        top = a;
        left = b;
        w = c;
        h = d;
        this->format = TLWH;
        break;
      default:
        break;
    }
  }

  void to_tlwh() {
    // suppose we have xyxy already
    if (format == XYXY) {
      this->top = xmin;
      this->left = ymin;
      this->w = xmax - xmin;
      this->h = ymax - ymin;
      format = BOTH;
    }
  }
  void to_xyxy() {
    if (format == TLWH) {
      this->xmin = left;
      this->ymin = top;
      this->xmax = left + w;
      this->ymax = top + h;
      format = BOTH;
    }
  }

  float area() {
    switch (this->format) {
      case XYXY:
        return (this->xmax - this->xmin) * (this->ymax - this->xmax);
        break;
      case TLWH:
        return this->w * this->h;
        break;
      default:
        return (this->xmax - this->xmin) * (this->ymax - this->xmax);
        break;
    }
  }
  void print() {
    switch (this->format) {
      case XYXY:
        std::cout << "x1:" << this->xmin << ",y1:" << this->ymin
                  << ",x2:" << this->xmax << ",y2:" << this->ymax
                  << ",id:" << this->idx << ",score:" << this->score
                  << std::endl;
        break;
      case TLWH:
        std::cout << "top:" << this->top << ",left:" << this->left
                  << ",w:" << this->w << ",h:" << this->h << ",id:" << this->idx
                  << ",score:" << this->score << std::endl;
        break;
      default:
        std::cout << "x1:" << this->xmin << ",y1:" << this->ymin
                  << ",x2:" << this->xmax << ",y2:" << this->ymax
                  << ",id:" << this->idx << ",score:" << this->score
                  << std::endl;
        break;
    }
  }
};

struct BoxSimple {
  float x1;
  float y1;
  float x2;
  float y2;
};

struct landmarks {
  float x;
  float y;
};

// detection supports multiple usages
struct Detection {
  // x1 y1 x2 y2
  BoxSimple bbox;
  // float objectness;
  landmarks marks[5];
  int classId;
  float prob;
};

//#ifdef USE_OPENCV
// human pose decalaration
struct HumanPose {
  std::vector<cv::Point2f> keypoints;
  float score;
  // semantic pose type such as:
  // 1. waving hands; 2. stop; 3. turn left; 4. turn right
  // 5. follow me; 0. null
  int pose_type;
  // indicates same pose through video series, using for pose tracking
  int pose_id = -1;
  // if confirm it is face, this flag will on
  bool is_face = false;

  HumanPose(
      const std::vector<cv::Point2f> &keypoints = std::vector<cv::Point2f>(),
      const float &score = 0)
      : keypoints(keypoints), score(score){};
  Box to_box() {
    const cv::Point2f absentKeypoint(-1.0f, -1.0f);
    // converts human pose to a bounding box
    vector<float> xss;
    vector<float> yss;
    for (auto const &p : keypoints) {
      // don't count absent points
      if (p == absentKeypoint) {
        continue;
      }
      xss.push_back(p.x);
      yss.push_back(p.y);
      //	  cout << "(" << p.x << "," << p.y << ")" << "\n";
    }

    auto xminmax = std::minmax_element(xss.begin(), xss.end());
    auto yminmax = std::minmax_element(yss.begin(), yss.end());
    Box b((int)*(xminmax.first), (int)*(yminmax.first), (int)*(xminmax.second),
          (int)*(yminmax.second), BoxFormat::XYXY);
    //	b.print();
    return b;
  }
};

// todo: HumanFaces

// camera intrinsics decalaration
struct CameraIntrinsicParams {
  double fx, fy, cx, cy, scale;
  inline void print() {
    std::cout << "fx: " << fx << " fy: " << fy << " cx: " << cx << " cy:" << cy
              << " scale: " << scale << endl;
  };
  inline cv::Mat toCameraMatrix() {
    // convert to a cv::Mat matrix
    double camera_matrix_data[3][4] = {{fx, 0, cx}, {0, fy, cy}, {0, 0, 1}};
    cv::Mat camera_mat(3, 3, CV_64F, camera_matrix_data);
    return camera_mat;
  }
};
//#endif

}  // namespace thor

#endif  // COMMON_STRUCTURE_H
