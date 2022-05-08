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
// 1/15/19-15-11
// slam
// jinfagang19@gmail.com
// CTI Robotics
// ************************************


//
// Created by jintain on 1/15/19.
//

#ifdef USE_OPENCV
#include "thor/slam.h"

using namespace thor;

void slam::getKeyPointsColor(const cv::Mat &img1, const cv::Mat &img2, vector <cv::KeyPoint> &kp1,
                             vector <cv::KeyPoint> &kp2) {
  // calculate keypoints
  /**
   *  "FAST" – FastFeatureDetector
      "STAR" – StarFeatureDetector
      "SIFT" – SIFT (nonfree module)
      "SURF" – SURF (nonfree module)
      "ORB" – ORB
      "MSER" – MSER
      "GFTT" – GoodFeaturesToTrackDetector
      "HARRIS" – GoodFeaturesToTrackDetector with Harris detector enabled
      "Dense" – DenseFeatureDetector
      "SimpleBlob" – SimpleBlobDetector
   */
   cerr << "this function not implemented, using getGoodMatchesColor() directly.\n";
}

void slam::getGoodMatchesColor(const cv::Mat &img1, const cv::Mat &img2,
    vector<cv::DMatch> &good_matches, vector <cv::KeyPoint> &kp1,
    vector <cv::KeyPoint> &kp2, bool show) {

  // ORB is the best feature extractor for now, but not work with (Scale-invariant)
  cv::Ptr<cv::ORB> detector = cv::ORB::create();
  cv::Ptr<cv::FlannBasedMatcher> matcher;


  vector<cv::DMatch> all_matches;
  cv::Mat descriptor1, descriptor2;
  detector->detectAndCompute(img1, cv::Mat(), kp1, descriptor1);
  detector->detectAndCompute(img2, cv::Mat(), kp2, descriptor2);

  matcher->match(descriptor1, descriptor2, all_matches);
  if(descriptor1.type()!=CV_32F) {
    descriptor1.convertTo(descriptor1, CV_32F);
  }

  if(descriptor2.type()!=CV_32F) {
    descriptor2.convertTo(descriptor2, CV_32F);
  }

  // filter good matches
  double min_dist = 9999;
  for (int i = 0; i < all_matches.size(); ++i) {
    if ( all_matches[i].distance < min_dist ){
      min_dist = all_matches[i].distance;
    }
  }

  for (int j = 0; j < all_matches.size(); ++j) {
    if (all_matches[j].distance < 4*min_dist) {
      good_matches.push_back(all_matches[j]);
    }
  }
  // now we get all matches
  if (show) {
    cv::Mat match_res;
    cv::drawMatches(img1, kp1, img2, kp2, good_matches, match_res);
    cv::imshow( "good matches", match_res);
    cv::waitKey(0);
  }
}

#endif


