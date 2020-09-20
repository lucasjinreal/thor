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
// ************************************
// Copyrights by Jin Fagang
// 1/15/19-15-10
// slam
// jinfagang19@gmail.com
// ************************************

//
// Created by jintain on 1/15/19.
//

#ifndef THOR_SLAM_H
#define THOR_SLAM_H

/**
 *
 * this slam kit provide some functions such as:
 *
 * 1. feature extract
 * 2. compute Rotation and transform
 * 3. etc.
 */

#ifdef USE_OPENCV
#include "opencv2/opencv.hpp"
#include <opencv2/core/types.hpp>
#ifndef USE_OPENCV4
#include <opencv/cv.hpp>
#endif
#include "iostream"
#include "vector"

#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//#include "opencv4/opencv2/calib3d/calib3d.hpp"

using namespace std;

namespace thor {
namespace slam {

// get keypoints from 2 images
void getKeyPointsColor(const cv::Mat &img1, const cv::Mat &img2,
                       vector<cv::KeyPoint> &kp1, vector<cv::KeyPoint> &kp2);
// get good matches from 2 images
void getGoodMatchesColor(const cv::Mat &img1,
                         const cv::Mat &img2,
                         vector<cv::DMatch> &good_matches,
                         vector<cv::KeyPoint> &kp1,
                         vector<cv::KeyPoint> &kp2,
                         bool show = false);

// get rotation and transform from 2 images, we need intrinsic here
void computeR_t(vector<cv::Point3f> &pts_obj, vector<cv::Point2f> &pts_img,
                cv::Mat &camera_matrix, cv::Mat &R, cv::Mat &t, cv::Mat &inliers);
}
}
#endif


#endif //THOR_SLAM_H
