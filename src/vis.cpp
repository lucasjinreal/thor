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

#include "include/vis.h"

thor::vis::RGBA thor::vis::gen_unique_color(int idx, bool is_track,
                                            double hue_step, float alpha) {
  // if idx is track id, the color should be
  if (is_track) {
    // we may have 1000+ track ids
    int track_size = 1. / hue_step;
    idx = idx % track_size;
  }
  float h = idx * hue_step - (int)(idx * hue_step);
  double v = 1.0 - (int(idx * hue_step) % 4) / 5.;

  thor::vis::RGBA rgba;
  thor::vis::hsv2rgb(rgba, h, 1, v);

  rgba.r = 255 * rgba.r;
  rgba.g = 255 * rgba.g;
  rgba.b = 255 * rgba.b;
  rgba.a = alpha;
  return rgba;
}

void thor::vis::hsv2rgb(thor::vis::RGBA &rgba, float h, float s, float v) {
  if (s == 0) {
    rgba.r = v;
    rgba.g = v;
    rgba.b = v;
    return;
  }
  int i = h * 6.0;
  float f = (h * 6.0) - i;
  float p = v * (1.0 - s);
  float q = v * (1.0 - s * f);
  float t = v * (1.0 - s * (1.0 - f));
  i = i % 6;
  switch (i) {
    case 0:
      rgba.r = v;
      rgba.g = t;
      rgba.b = p;
      break;
    case 1:
      rgba.r = q;
      rgba.g = v;
      rgba.b = p;
      break;
    case 2:
      rgba.r = p;
      rgba.g = v;
      rgba.b = t;
      break;
    case 3:
      rgba.r = p;
      rgba.g = q;
      rgba.b = v;
      break;
    case 4:
      rgba.r = t;
      rgba.g = p;
      rgba.b = v;
      break;
    case 5:
      rgba.r = v;
      rgba.g = p;
      rgba.b = q;
      break;
    default:
      break;
  }
}

void thor::vis::hsv2rgb(float &r, float &g, float &b, float h, float s,
                        float v) {
  double hh, p, q, t, ff;
  long i;
  if (s <= 0.0) {  // < is bogus, just shuts up warnings
    r = float(v);
    g = float(v);
    b = float(v);
  }
  hh = h;
  if (hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * ff));
  t = v * (1.0 - (s * (1.0 - ff)));
  switch (i) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;

    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
    default:
      r = v;
      g = p;
      b = q;
      break;
  }
}

#ifdef USE_OPENCV
cv::Scalar thor::vis::gen_unique_color_cv(int idx, bool is_track,
                                          double hue_step, float alpha) {
  RGBA cr = gen_unique_color(idx, is_track, hue_step, alpha);
  cv::Scalar c(cr.r, cr.g, cr.b);
  return c;
}

cv::Mat thor::vis::createAlpha(cv::Mat &src) {
  cv::Mat alpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
  cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
  cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.cols; j++) {
      alpha.at<uchar>(i, j) = 255 - gray.at<uchar>(i, j);
    }
  }
  return alpha;
}

int thor::vis::addAlpha(cv::Mat &src, cv::Mat &dst, cv::Mat &alpha) {
  if (src.channels() == 4) {
    return -1;
  } else if (src.channels() == 1) {
    cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
  }

  dst = cv::Mat(src.rows, src.cols, CV_8UC4);

  std::vector<cv::Mat> srcChannels;
  std::vector<cv::Mat> dstChannels;
  cv::split(src, srcChannels);

  dstChannels.push_back(srcChannels[0]);
  dstChannels.push_back(srcChannels[1]);
  dstChannels.push_back(srcChannels[2]);
  dstChannels.push_back(alpha);
  cv::merge(dstChannels, dst);
  return 0;
}
#endif

namespace thor {
namespace vis {

#ifdef USE_OPENCV
/**
 * Human pose order should in OpenPose order
 *
 * @param poses
 * @param image
 */
void renderHumanPose(std::vector<HumanPose> &poses, cv::Mat &image) {
  // drawing HumanPoses on image
  CV_Assert(image.type() == CV_8UC3);
  const std::vector<cv::Scalar> colors = {
      cv::Scalar(255, 0, 0),   cv::Scalar(255, 85, 0),  cv::Scalar(255, 170, 0),
      cv::Scalar(255, 255, 0), cv::Scalar(170, 255, 0), cv::Scalar(85, 255, 0),
      cv::Scalar(0, 255, 0),   cv::Scalar(0, 255, 85),  cv::Scalar(0, 255, 170),
      cv::Scalar(0, 255, 255), cv::Scalar(0, 170, 255), cv::Scalar(0, 85, 255),
      cv::Scalar(0, 0, 255),   cv::Scalar(85, 0, 255),  cv::Scalar(170, 0, 255),
      cv::Scalar(255, 0, 255), cv::Scalar(255, 0, 170), cv::Scalar(255, 0, 85)};
  const std::vector<std::pair<int, int>> limbKeypointsIds = {
      {1, 2}, {1, 5},  {2, 3},   {3, 4},  {5, 6},   {6, 7},
      {1, 8}, {8, 9},  {9, 10},  {1, 11}, {11, 12}, {12, 13},
      {1, 0}, {0, 14}, {14, 16}, {0, 15}, {15, 17}};

  const int stickWidth = 3;
  const cv::Point2f absentKeypoint(-1.0f, -1.0f);
  for (const auto &pose : poses) {
    // we only support 18 keypoints
    CV_Assert(pose.keypoints.size() == 18);

    for (size_t keypointIdx = 0; keypointIdx < pose.keypoints.size();
         keypointIdx++) {
      if (pose.keypoints[keypointIdx] != absentKeypoint) {
        cv::circle(image, pose.keypoints[keypointIdx], 3, colors[keypointIdx],
                   -1);
      }
    }
  }
  cv::Mat pane = image.clone();
  for (auto &pose : poses) {
    for (const auto &limbKeypointsId : limbKeypointsIds) {
      std::pair<cv::Point2f, cv::Point2f> limbKeypoints(
          pose.keypoints[limbKeypointsId.first],
          pose.keypoints[limbKeypointsId.second]);
      if (limbKeypoints.first == absentKeypoint ||
          limbKeypoints.second == absentKeypoint) {
        continue;
      }

      float meanX = (limbKeypoints.first.x + limbKeypoints.second.x) / 2;
      float meanY = (limbKeypoints.first.y + limbKeypoints.second.y) / 2;
      cv::Point difference = limbKeypoints.first - limbKeypoints.second;
      double length =
          std::sqrt(difference.x * difference.x + difference.y * difference.y);
      int angle = static_cast<int>(std::atan2(difference.y, difference.x) *
                                   180 / CV_PI);
      std::vector<cv::Point> polygon;
      cv::ellipse2Poly(cv::Point2d(meanX, meanY),
                       cv::Size2d(length / 2, stickWidth), angle, 0, 360, 1,
                       polygon);
      cv::fillConvexPoly(pane, polygon, colors[limbKeypointsId.second]);
    }
    // for every pose, if pose has pose_id, means it is tracked
    if (pose.pose_id != -1) {
      // we draw this id
      Box b = pose.to_box();
      cv::putText(image, to_string(pose.pose_id), Point2f(b.xmin, b.ymin),
                  FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 255));
      cv::rectangle(image, Point2f(b.xmin, b.ymin), Point2f(b.xmax, b.ymax),
                    Scalar(255, 0, 0), 1);
    }
  }
  cv::addWeighted(image, 0.6, pane, 0.7, 0, image);
}

void renderHumanPoseSimple(std::vector<HumanPose> &poses, cv::Mat &image) {
  // a more simple render of human pose estimation
  // we can define our own limbKeypoints to unlink some joint
  const std::vector<std::pair<int, int>> limbKeypointsIds = {
      {1, 2}, {1, 5},  {2, 3},   {3, 4},  {5, 6},   {6, 7},
      {1, 8}, {8, 9},  {9, 10},  {1, 11}, {11, 12}, {12, 13},
      {1, 0}, {0, 14}, {14, 16}, {0, 15}, {15, 17}};
  const cv::Point2f absentKeypoint(-1.0f, -1.0f);
  for (auto &pose : poses) {
    for (const auto &limbKeypointsId : limbKeypointsIds) {
      std::pair<cv::Point2f, cv::Point2f> limbKeypoints(
          pose.keypoints[limbKeypointsId.first],
          pose.keypoints[limbKeypointsId.second]);
      if (limbKeypoints.first == absentKeypoint ||
          limbKeypoints.second == absentKeypoint) {
        continue;
      }
      cv::line(image, limbKeypoints.first, limbKeypoints.second,
               Scalar(255, 255, 255), 1);
    }
    if (pose.pose_id != -1) {
      // we draw this id
      Box b = pose.to_box();
      cv::putText(image, to_string(pose.pose_id), Point2f(b.xmin, b.ymin),
                  FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255));
      cv::rectangle(image, Point2f(b.xmin, b.ymin), Point2f(b.xmax, b.ymax),
                    Scalar(255, 255, 255), 1);
    }
  }
}

// Draw boxes
cv::Mat VisualizeDetection(cv::Mat &img, vector<vector<float>> detections,
                           vector<string> classes_names, bool enable_mask,
                           float confidence_threshold, bool normalized) {
  // for visualize
  const int font = cv::FONT_HERSHEY_TRIPLEX;
  const float font_scale = 0.6;
  const int font_thickness = 2;
  cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
  for (int i = 0; i < detections.size(); ++i) {
    const vector<float> &d = detections[i];
    // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
    // CHECK_EQ(d.size(), 7);
    int offset = 0;
    if (d.size() == 7) {
      offset = 1;
    }
    // cout << "l: " << d[offset + 0] << " score: " << d[offset + 1] << endl;
    const float score = d[1 + offset];
    if (score >= confidence_threshold) {
      cv::Point pt1, pt2;
      if (normalized) {
        pt1.x = (img.cols * d[2 + offset]);
        pt1.y = (img.rows * d[3 + offset]);
        pt2.x = (img.cols * d[4 + offset]);
        pt2.y = (img.rows * d[5 + offset]);
      } else {
        pt1.x = d[2 + offset];
        pt1.y = d[3 + offset];
        pt2.x = d[4 + offset];
        pt2.y = d[5 + offset];
      }

      cv::Scalar u_c = thor::vis::gen_unique_color_cv(d[offset]);
      cv::rectangle(img, pt1, pt2, u_c, 2, 8, 0);
      cv::rectangle(mask, pt1, pt2, u_c, cv::FILLED, 0);

      char score_str[256];
      sprintf(score_str, "%.2f", score);
      std::string label_text =
          classes_names[d[offset]] + " " + string(score_str);
      int base_line = 0;
      cv::Point text_origin = cv::Point(pt1.x - 2, pt1.y - 3);
      cv::Size text_size = cv::getTextSize(label_text, font, font_scale,
                                           font_thickness, &base_line);
      cv::rectangle(img, cv::Point(text_origin.x, text_origin.y + 5),
                    cv::Point(text_origin.x + text_size.width,
                              text_origin.y - text_size.height - 5),
                    u_c, -1, 0);
      cv::putText(img, label_text, text_origin, font, font_scale,
                  cv::Scalar(0, 0, 0), font_thickness);
    }
  }
  cv::Mat combined;
  cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
  // maybe combine a mask img back later
  return combined;
}

cv::Mat VisualizeDetection(cv::Mat &img, vector<thor::Box> detections,
                           vector<string> classes_names, bool enable_mask,
                           float confidence_threshold, bool normalized) {
  // for visualize
  const int font = cv::FONT_HERSHEY_SIMPLEX;
  const float font_scale = 0.6;
  const int font_thickness = 2;
  cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
  for (int i = 0; i < detections.size(); ++i) {
    thor::Box box = detections[i];
    box.to_xyxy();
    const float score = box.score;
    if (score >= confidence_threshold) {
      cv::Point pt1, pt2;
      if (normalized) {
        pt1.x = (img.cols * box.xmin);
        pt1.y = (img.rows * box.ymin);
        pt2.x = (img.cols * box.xmax);
        pt2.y = (img.rows * box.ymax);
      } else {
        pt1.x = box.xmin;
        pt1.y = box.ymin;
        pt2.x = box.xmax;
        pt2.y = box.ymax;
      }

      cv::Scalar u_c = thor::vis::gen_unique_color_cv(box.idx);
      cv::rectangle(img, pt1, pt2, u_c, 2, 8, 0);
      cv::rectangle(mask, pt1, pt2, u_c, cv::FILLED, 0);

      char score_str[256];
      sprintf(score_str, "%.2f", score);
      std::string label_text = classes_names[box.idx] + " " + string(score_str);
      int base_line = 0;
      cv::Point text_origin = cv::Point(pt1.x - 2, pt1.y - 3);
      cv::Size text_size = cv::getTextSize(label_text, font, font_scale,
                                           font_thickness, &base_line);
      cv::rectangle(img, cv::Point(text_origin.x, text_origin.y + 5),
                    cv::Point(text_origin.x + text_size.width,
                              text_origin.y - text_size.height - 5),
                    u_c, -1, 0);
      cv::putText(img, label_text, text_origin, font, font_scale,
                  cv::Scalar(255, 255, 255), font_thickness);
    }
  }
  cv::Mat combined;
  cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
  // maybe combine a mask img back later
  return combined;
}

cv::Mat VisualizeDetections(cv::Mat &img, vector<thor::Detection> detections,
                            const vector<string> classes_names,
                            const vector<cv::Scalar> *colors,
                            const float line_thickness, const float font_scale,
                            const bool fancy, const float confidence_threshold,
                            const bool enable_mask, const bool normalized) {
  const int font = cv::FONT_HERSHEY_SIMPLEX;
  const int font_thickness = 1;
  cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
  for (int i = 0; i < detections.size(); ++i) {
    thor::Detection det = detections[i];
    const auto score = (float)det.prob;
    if (score >= confidence_threshold) {
      cv::Point pt1, pt2;
      if (normalized) {
        pt1.x = (img.cols * det.bbox.x1);
        pt1.y = (img.rows * det.bbox.y1);
        pt2.x = (img.cols * det.bbox.x2);
        pt2.y = (img.rows * det.bbox.y2);
      } else {
        pt1.x = det.bbox.x1;
        pt1.y = det.bbox.y1;
        pt2.x = det.bbox.x2;
        pt2.y = det.bbox.y2;
      }

      cv::Scalar u_c;
      if (colors != nullptr) {
        u_c = (*colors)[det.classId];
      } else {
        u_c = thor::vis::gen_unique_color_cv(det.classId);
      }
      cv::rectangle(img, pt1, pt2, u_c, line_thickness, cv::LINE_4, 0);
      cv::rectangle(mask, pt1, pt2, u_c, cv::FILLED, 0);

      // CV_FONT_HERSHEY_DUPLEX

      char score_str[256];
      sprintf(score_str, "%.1f", score * 100);
      std::string label_text =
          classes_names[det.classId] + " " + string(score_str) + "%";
      int base_line = 4;
      cv::Point text_origin = cv::Point(pt1.x + 2, pt1.y - base_line);
      cv::Size text_size = cv::getTextSize(label_text, font, font_scale,
                                           font_thickness, &base_line);
      cv::rectangle(
          mask, cv::Point(pt1.x, text_origin.y - text_size.height - base_line),
          cv::Point(text_origin.x + text_size.width + 2, pt1.y), u_c, -1, 0);
      cv::putText(img, label_text, text_origin, font, font_scale,
                  cv::Scalar(255, 255, 255), font_thickness, cv::LINE_AA);
    }
  }
  cv::Mat combined;
  cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
  // maybe combine a mask img back later
  return combined;
}

cv::Mat VisualizeDetections(cv::Mat &img,
                            vector<thor::dl::Detection2D> detections,
                            const vector<string> classes_names,
                            const vector<cv::Scalar> *colors,
                            const float line_thickness, const float font_scale,
                            const bool fancy, const float confidence_threshold,
                            const bool enable_mask, const bool normalized) {
  // for visualize
  // const int font = cv::FONT_HERSHEY_DUPLEX;
  const int font = cv::FONT_HERSHEY_SIMPLEX;
  const int font_thickness = 1;
  cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
  for (int i = 0; i < detections.size(); ++i) {
    thor::dl::Detection2D det = detections[i];
    const thor::dl::Box &box = det.box();
    const auto score = (float)det.prob();
    if (score >= confidence_threshold) {
      cv::Point pt1, pt2;
      if (normalized) {
        pt1.x = (img.cols * box.x1());
        pt1.y = (img.rows * box.y1());
        pt2.x = (img.cols * box.x2());
        pt2.y = (img.rows * box.y2());
      } else {
        pt1.x = box.x1();
        pt1.y = box.y1();
        pt2.x = box.x2();
        pt2.y = box.y2();
      }

      cv::Scalar u_c;
      if (colors != nullptr) {
        u_c = (*colors)[det.cls_id()];
      } else {
        u_c = thor::vis::gen_unique_color_cv(det.cls_id());
      }
      cv::rectangle(img, pt1, pt2, u_c, line_thickness, cv::LINE_4, 0);
      cv::rectangle(mask, pt1, pt2, u_c, cv::FILLED, 0);

      // CV_FONT_HERSHEY_DUPLEX

      char score_str[256];
      sprintf(score_str, "%.1f", score * 100);
      std::string label_text =
          classes_names[det.cls_id()] + " " + string(score_str) + "%";
      int base_line = 4;
      cv::Point text_origin = cv::Point(pt1.x + 2, pt1.y - base_line);
      cv::Size text_size = cv::getTextSize(label_text, font, font_scale,
                                           font_thickness, &base_line);
      cv::rectangle(
          mask, cv::Point(pt1.x, text_origin.y - text_size.height - base_line),
          cv::Point(text_origin.x + text_size.width + 2, pt1.y), u_c, -1, 0);
      cv::putText(img, label_text, text_origin, font, font_scale,
                  cv::Scalar(255, 255, 255), font_thickness, cv::LINE_AA);
    }
  }
  cv::Mat combined;
  cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
  // maybe combine a mask img back later
  return combined;
}

cv::Mat VisualizeInstanceSegmentations(
    cv::Mat &img, vector<thor::dl::InstanceSegmentation> instances,
    const vector<string> classes_names, const vector<cv::Scalar> *colors,
    const float line_thickness, const float font_scale, const bool fancy,
    const float confidence_threshold, const bool enable_mask,
    const bool normalized) {
  cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
  cv::Mat mask_binary = cv::Mat::zeros(img.size(), CV_8UC1);
  for (const auto &instance_segmentation : instances) {
    int category_id = instance_segmentation.detection().cls_id();
    std::string cat = classes_names[category_id];
    float confidence = instance_segmentation.detection().prob();
    char label[20];
    sprintf(label, "%s:%.2f", cat.c_str(), confidence);
    cv::rectangle(img,
                  cv::Point(instance_segmentation.detection().box().x1(),
                            instance_segmentation.detection().box().y1()),
                  cv::Point(instance_segmentation.detection().box().x2(),
                            instance_segmentation.detection().box().y2()),
                  cv::Scalar(255, 0, 0));
    cv::putText(img, label,
                cv::Point2i(instance_segmentation.detection().box().x1(),
                            instance_segmentation.detection().box().y1()),
                cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0));
    int box_w = instance_segmentation.detection().box().x2() -
                instance_segmentation.detection().box().x1();
    int box_h = instance_segmentation.detection().box().y2() -
                instance_segmentation.detection().box().y1();

    // mask size must save with box_h * box_w
    cv::Mat resized_mask(instance_segmentation.mask_h(),
                         instance_segmentation.mask_w(), CV_32F,
                         (float *)instance_segmentation.mask().begin());

    int l = instance_segmentation.detection().box().x1() < 0
                ? 0
                : instance_segmentation.detection().box().x1();
    int t = instance_segmentation.detection().box().y1() < 0
                ? 0
                : instance_segmentation.detection().box().y1();
    box_w = l + box_w > img.cols ? img.cols : box_w;
    box_h = t + box_h > img.rows ? img.rows : box_h;
    cv::Rect box(l, t, box_w, box_h);
    cv::resize(resized_mask, resized_mask, cv::Point(box_w, box_h));
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

cv::Mat VisualizeDetectionStyleDetectron2(
    cv::Mat &img, vector<thor::Box> detections, vector<string> classes_names,
    bool enable_mask, float confidence_threshold, bool normalized) {
  // for detectron2 style drawing bounding boxes
  const int font = cv::FONT_HERSHEY_SIMPLEX;
  const float font_scale = 0.35;
  const int font_thickness = 1;
  cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
  for (int i = 0; i < detections.size(); ++i) {
    thor::Box box = detections[i];
    box.to_xyxy();
    const float score = box.score;
    if (score >= confidence_threshold) {
      cv::Point pt1, pt2;
      if (normalized) {
        pt1.x = (img.cols * box.xmin);
        pt1.y = (img.rows * box.ymin);
        pt2.x = (img.cols * box.xmax);
        pt2.y = (img.rows * box.ymax);
      } else {
        pt1.x = box.xmin;
        pt1.y = box.ymin;
        pt2.x = box.xmax;
        pt2.y = box.ymax;
      }

      cv::Scalar u_c = thor::vis::gen_unique_color_cv(box.idx);
      cv::rectangle(img, pt1, pt2, u_c, 1, 8, 0);
      cv::rectangle(mask, pt1, pt2, u_c, cv::FILLED, 0);

      char score_str[256];
      sprintf(score_str, "%.1f", score);
      std::string label_text = classes_names[box.idx] + ":" + string(score_str);
      int base_line = 0;
      cv::Point text_origin = cv::Point(pt1.x, pt1.y - 2);
      cv::Size text_size = cv::getTextSize(label_text, font, font_scale,
                                           font_thickness, &base_line);
      cv::rectangle(img, cv::Point(text_origin.x, text_origin.y),
                    cv::Point(text_origin.x + text_size.width,
                              text_origin.y - text_size.height),
                    cv::Scalar(0, 0, 0), -1, 0);
      cv::putText(img, label_text, text_origin, font, font_scale,
                  cv::Scalar(255, 255, 255), font_thickness);
    }
  }
  cv::Mat combined;
  cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
  // maybe combine a mask img back later
  return combined;
};

#endif

}  // namespace vis
}  // namespace thor
