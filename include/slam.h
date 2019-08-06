// ************************************
// Copyrights by Jin Fagang
// 1/15/19-15-10
// slam
// jinfagang19@gmail.com
// CTI Robotics
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
#include <opencv/cv.hpp>
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
