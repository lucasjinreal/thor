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
 *  Vision module in cao
 *  the module provide some utility methods to draw bboxes
 */
#include <sys/stat.h>

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "cmath"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "structures.h"

#ifdef USE_PROTOBUF
#include "proto/det.pb.h"
#include "proto/insg.pb.h"
#endif

using cv::Mat;
using cv::Point2f;
using cv::Scalar;

namespace thor {
namespace vis {

struct RGB {
  float r, g, b;
};

struct RGBA {
  float r, g, b, a;

  inline void print() {
    std::cout << "r:" << r << " g:" << g << " b:" << b << " a:" << a << endl;
  }
};

// conversions
cv::Scalar toCvColor(const unsigned char *c) {
  return cv::Scalar(c[0], c[1], c[2]);
}

// adding default values for better calling
thor::vis::RGBA gen_unique_color(int idx, bool is_track = false,
                                 double hue_step = 0.41, float alpha = 0.7);

void hsv2rgb(float &r, float &g, float &b, float h, float s, float v);

void hsv2rgb(thor::vis::RGBA &rgba, float h, float s, float v);

//#ifdef USE_OPENCV
// tracking color is slightly different, they are all close
cv::Scalar gen_unique_color_cv(int idx, bool is_track = false,
                               double hue_step = 0.41, float alpha = 0.7);

cv::Mat createAlpha(cv::Mat &src);

int addAlpha(cv::Mat &src, cv::Mat &dst, cv::Mat &alpha);

// draw detections
cv::Mat VisualizeDetection(cv::Mat &img, vector<vector<float>> detections,
                           vector<string> classes_names,
                           bool enable_mask = true,
                           float confidence_threshold = 0.02,
                           bool normalized = false);

cv::Mat VisualizeDetection(cv::Mat &img, vector<thor::Box> detections,
                           vector<string> classes_names,
                           bool enable_mask = true,
                           float confidence_threshold = 0.02,
                           bool normalized = false);

cv::Mat VisualizeDetectionStyleDetectron2(cv::Mat &img,
                                          vector<thor::Box> detections,
                                          vector<string> classes_names,
                                          bool enable_mask = true,
                                          float confidence_threshold = 0.02,
                                          bool normalized = false);

cv::Mat VisualizeDetectionStyleDetectron2(cv::Mat &img,
                                          vector<thor::Bbox> detections,
                                          vector<string> classes_names,
                                          bool enable_mask = true,
                                          float confidence_threshold = 0.02,
                                          bool normalized = false);

// More modern API which same as alfred
cv::Mat VisualizeDetections(
    cv::Mat &img, vector<thor::Detection> detections,
    const vector<string> classes_names, const vector<cv::Scalar> *colors = NULL,
    const float line_thickness = 1, const float font_scale = 0.38,
    const bool fancy = false, const float confidence_threshold = 0.02,
    const bool enable_mask = false, const bool normalized = false);

cv::Mat VisualizeDetectionsWithOverrideColors(
    cv::Mat &img, vector<thor::Detection> detections,
    const vector<string> classes_names,
    const std::map<int, cv::Scalar> *override_colors = NULL,
    const float line_thickness = 1, const bool with_text = true,
    const float font_scale = 0.38, const bool fancy = false,
    const float confidence_threshold = 0.02, const bool enable_mask = false,
    const bool normalized = false);

///////////////////// Visualize protobuf messages ///////////////////////

#ifdef USE_PROTOBUF
cv::Mat VisualizeDetections(
    cv::Mat &img, vector<thor::dl::Detection2D> detections,
    const vector<string> classes_names, const vector<cv::Scalar> *colors = NULL,
    const float line_thickness = 1.7, const float font_scale = 0.3,
    const bool fancy = false, const float confidence_threshold = 0.02,
    const bool enable_mask = false, const bool normalized = false);

cv::Mat VisualizeInstanceSegmentations(
    cv::Mat &img, vector<thor::dl::InstanceSegmentation> instances,
    const vector<string> classes_names, const vector<cv::Scalar> *colors = NULL,
    const float line_thickness = 1, const float font_scale = 0.38,
    const bool fancy = false, const float confidence_threshold = 0.02,
    const bool enable_mask = false, const bool normalized = false);
#endif

/////////////////// Visualize Lane ///////////////////////
cv::Mat VisualizeLanes(cv::Mat &img, const vector<vector<cv::Point>> &lanes,
                       const vector<cv::Scalar> *colors = NULL,
                       const float line_thickness = 12, const float alpha = 1.0,
                       const bool guide_line = true);

// adding render HumanPose on image
void renderHumanPose(std::vector<HumanPose> &poses, cv::Mat &image);

void renderHumanPoseSimple(std::vector<HumanPose> &poses, cv::Mat &image);
//#endif

// separate color module
namespace color {
// Note: color value is in BGR order.
// Color reference: https://en.wikipedia.org/wiki/Web_colors#X11_color_names

constexpr unsigned char kAliceBlue[3] = {255, 248, 240};
constexpr unsigned char kAntiqueWhite[3] = {215, 235, 250};
constexpr unsigned char kAqua[3] = {255, 255, 0};
constexpr unsigned char kAquamarine[3] = {212, 255, 127};
constexpr unsigned char kAzure[3] = {255, 255, 240};
constexpr unsigned char kBeige[3] = {220, 245, 245};
constexpr unsigned char kBisque[3] = {196, 228, 255};
constexpr unsigned char kBlack[3] = {0, 0, 0};
constexpr unsigned char kBlanchedAlmond[3] = {205, 235, 255};
constexpr unsigned char kBlue[3] = {255, 0, 0};
constexpr unsigned char kBlueViolet[3] = {226, 43, 138};
constexpr unsigned char kBrown[3] = {42, 42, 165};
constexpr unsigned char kBurlyWood[3] = {135, 184, 222};
constexpr unsigned char kCadetBlue[3] = {160, 158, 95};
constexpr unsigned char kChartreuse[3] = {0, 255, 127};
constexpr unsigned char kChocolate[3] = {30, 105, 210};
constexpr unsigned char kCoral[3] = {80, 127, 255};
constexpr unsigned char kCornflowerBlue[3] = {237, 149, 100};
constexpr unsigned char kCornsilk[3] = {220, 248, 255};
constexpr unsigned char kCrimson[3] = {60, 20, 220};
constexpr unsigned char kCyan[3] = {255, 255, 0};
constexpr unsigned char kDarkBlue[3] = {139, 0, 0};
constexpr unsigned char kDarkCyan[3] = {139, 139, 0};
constexpr unsigned char kDarkGoldenrod[3] = {11, 134, 184};
constexpr unsigned char kDarkGray[3] = {169, 169, 169};
constexpr unsigned char kDarkGreen[3] = {0, 100, 0};
constexpr unsigned char kDarkGrey[3] = {169, 169, 169};
constexpr unsigned char kDarkKhaki[3] = {107, 183, 189};
constexpr unsigned char kDarkMagenta[3] = {139, 0, 139};
constexpr unsigned char kDarkOlivegreen[3] = {47, 107, 85};
constexpr unsigned char kDarkOrange[3] = {0, 140, 255};
constexpr unsigned char kDarkOrchid[3] = {204, 50, 153};
constexpr unsigned char kDarkRed[3] = {0, 0, 139};
constexpr unsigned char kDarkSalmon[3] = {122, 150, 233};
constexpr unsigned char kDarkSeaGreen[3] = {143, 188, 143};
constexpr unsigned char kDarkSlateBlue[3] = {139, 61, 72};
constexpr unsigned char kDarkSlateGray[3] = {79, 79, 47};
constexpr unsigned char kDarkSlateGrey[3] = {79, 79, 47};
constexpr unsigned char kDarkTurquoise[3] = {209, 206, 0};
constexpr unsigned char kDarkViolet[3] = {211, 0, 148};
constexpr unsigned char kDeepPink[3] = {147, 20, 255};
constexpr unsigned char kDeepSkyBlue[3] = {255, 191, 0};
constexpr unsigned char kDimGray[3] = {105, 105, 105};
constexpr unsigned char kDimGrey[3] = {105, 105, 105};
constexpr unsigned char kDodgerBlue[3] = {255, 144, 30};
constexpr unsigned char kFireBrick[3] = {34, 34, 178};
constexpr unsigned char kFloralWhite[3] = {240, 250, 255};
constexpr unsigned char kForestGreen[3] = {34, 139, 34};
constexpr unsigned char kFuchsia[3] = {255, 0, 255};
constexpr unsigned char kGainsboro[3] = {220, 220, 220};
constexpr unsigned char kGhostWhite[3] = {255, 248, 248};
constexpr unsigned char kGold[3] = {0, 215, 255};
constexpr unsigned char kGoldenrod[3] = {32, 165, 218};
constexpr unsigned char kGray[3] = {128, 128, 128};
constexpr unsigned char kGreen[3] = {0, 128, 0};
constexpr unsigned char kGreenYellow[3] = {47, 255, 173};
constexpr unsigned char kGrey[3] = {128, 128, 128};
constexpr unsigned char kHoneydew[3] = {240, 255, 240};
constexpr unsigned char kHotPink[3] = {180, 105, 255};
constexpr unsigned char kIndianRed[3] = {92, 92, 205};
constexpr unsigned char kIndigo[3] = {130, 0, 75};
constexpr unsigned char kIvory[3] = {240, 255, 255};
constexpr unsigned char kKhaki[3] = {140, 230, 240};
constexpr unsigned char kLavender[3] = {250, 230, 230};
constexpr unsigned char kLavenderBlush[3] = {245, 240, 255};
constexpr unsigned char kLawnGreen[3] = {0, 252, 124};
constexpr unsigned char kLemonChiffon[3] = {205, 250, 255};
constexpr unsigned char kLightBlue[3] = {230, 216, 173};
constexpr unsigned char kLightCoral[3] = {128, 128, 240};
constexpr unsigned char kLightCyan[3] = {255, 255, 224};
constexpr unsigned char kLightGoldenrodYellow[3] = {210, 250, 250};
constexpr unsigned char kLightGray[3] = {211, 211, 211};
constexpr unsigned char kLightGreen[3] = {144, 238, 144};
constexpr unsigned char kLightGrey[3] = {211, 211, 211};
constexpr unsigned char kLightPink[3] = {193, 182, 255};
constexpr unsigned char kLightSalmon[3] = {122, 160, 255};
constexpr unsigned char kLightSeaGreen[3] = {170, 178, 32};
constexpr unsigned char kLightSkyBlue[3] = {250, 206, 135};
constexpr unsigned char kLightSlateGray[3] = {153, 136, 119};
constexpr unsigned char kLightSlateGrey[3] = {153, 136, 119};
constexpr unsigned char kLightSteelBlue[3] = {222, 196, 176};
constexpr unsigned char kLightYellow[3] = {224, 255, 255};
constexpr unsigned char kLime[3] = {0, 255, 0};
constexpr unsigned char kLimeGreen[3] = {50, 205, 50};
constexpr unsigned char kLinen[3] = {230, 240, 250};
constexpr unsigned char kMagenta[3] = {255, 0, 255};
constexpr unsigned char kMaroon[3] = {0, 0, 128};
constexpr unsigned char kMediumAquamarine[3] = {170, 205, 102};
constexpr unsigned char kMediumBlue[3] = {205, 0, 0};
constexpr unsigned char kMediumOrchid[3] = {211, 85, 186};
constexpr unsigned char kMediumPurple[3] = {219, 112, 147};
constexpr unsigned char kMediumSeaGreen[3] = {113, 179, 60};
constexpr unsigned char kMediumSlateBlue[3] = {238, 104, 123};
constexpr unsigned char kMediumSpringGreen[3] = {154, 250, 0};
constexpr unsigned char kMediumTurquoise[3] = {204, 209, 72};
constexpr unsigned char kMediumVioletRed[3] = {133, 21, 199};
constexpr unsigned char kMidnightBlue[3] = {112, 25, 25};
constexpr unsigned char kMintCream[3] = {250, 255, 245};
constexpr unsigned char kMistyRose[3] = {225, 228, 255};
constexpr unsigned char kMoccasin[3] = {181, 228, 255};
constexpr float kMRTransparency = 0.4f;
constexpr int kMRNumColors = 79;
constexpr unsigned char kNavajoWhite[3] = {173, 222, 255};
constexpr unsigned char kNavy[3] = {128, 0, 0};
constexpr unsigned char kOldLace[3] = {230, 245, 253};
constexpr unsigned char kOlive[3] = {0, 128, 128};
constexpr unsigned char kOliveDrab[3] = {35, 142, 107};
constexpr unsigned char kOrange[3] = {0, 165, 255};
constexpr unsigned char kOrangeRed[3] = {0, 69, 255};
constexpr unsigned char kOrchid[3] = {214, 112, 218};
constexpr unsigned char kPaleGoldenrod[3] = {170, 232, 238};
constexpr unsigned char kPaleGreen[3] = {152, 251, 152};
constexpr unsigned char kPaleTurquoise[3] = {238, 238, 175};
constexpr unsigned char kPaleVioletRed[3] = {147, 112, 219};
constexpr unsigned char kPapayawhip[3] = {213, 239, 255};
constexpr unsigned char kPeachPuff[3] = {185, 218, 255};
constexpr unsigned char kPeru[3] = {63, 133, 205};
constexpr unsigned char kPink[3] = {203, 192, 255};
constexpr unsigned char kPlum[3] = {221, 160, 221};
constexpr unsigned char kPowderBlue[3] = {230, 224, 176};
constexpr unsigned char kPurple[3] = {128, 0, 128};
constexpr unsigned char kRed[3] = {0, 0, 255};
constexpr unsigned char kRosyBrown[3] = {143, 143, 188};
constexpr unsigned char kRoyalBlue[3] = {225, 105, 65};
constexpr unsigned char kSaddleBrown[3] = {19, 69, 139};
constexpr unsigned char kSalmon[3] = {114, 128, 250};
constexpr unsigned char kSandyBrown[3] = {96, 164, 244};
constexpr unsigned char kSeaGreen[3] = {87, 139, 46};
constexpr unsigned char kSeashell[3] = {238, 245, 255};
constexpr unsigned char kSienna[3] = {45, 82, 160};
constexpr unsigned char kSilver[3] = {192, 192, 192};
constexpr unsigned char kSkyBlue[3] = {235, 206, 135};
constexpr unsigned char kSlateBlue[3] = {205, 90, 106};
constexpr unsigned char kSlateGray[3] = {144, 128, 112};
constexpr unsigned char kSlateGrey[3] = {144, 128, 112};
constexpr unsigned char kSnow[3] = {250, 250, 255};
constexpr unsigned char kSpringGreen[3] = {127, 255, 0};
constexpr unsigned char kSteelBlue[3] = {180, 130, 70};
constexpr unsigned char kTan[3] = {140, 180, 210};
constexpr unsigned char kTeal[3] = {128, 128, 0};
constexpr unsigned char kThistle[3] = {216, 191, 216};
constexpr unsigned char kTomato[3] = {71, 99, 255};
constexpr unsigned char kTurquoise[3] = {208, 224, 64};
constexpr unsigned char kViolet[3] = {238, 130, 238};
constexpr unsigned char kWheat[3] = {179, 222, 245};
constexpr unsigned char kWhite[3] = {255, 255, 255};
constexpr unsigned char kWhiteSmoke[3] = {245, 245, 245};
constexpr unsigned char kYellow[3] = {0, 255, 255};
constexpr unsigned char kYellowGreen[3] = {50, 205, 154};

// All following colormaps are 256 * 3 row major matrices
// Each row represents a color in BGR order
// Reference:
// https://docs.opencv.org/2.4/modules/contrib/doc/facerec/colormaps.html

constexpr unsigned char kAutumnColormap[] = {
    0, 0,   255, 0, 1,   255, 0, 2,   255, 0, 3,   255, 0, 4,   255,
    0, 5,   255, 0, 6,   255, 0, 7,   255, 0, 8,   255, 0, 9,   255,
    0, 10,  255, 0, 11,  255, 0, 12,  255, 0, 13,  255, 0, 14,  255,
    0, 15,  255, 0, 16,  255, 0, 17,  255, 0, 18,  255, 0, 19,  255,
    0, 20,  255, 0, 21,  255, 0, 22,  255, 0, 23,  255, 0, 24,  255,
    0, 25,  255, 0, 26,  255, 0, 27,  255, 0, 28,  255, 0, 29,  255,
    0, 30,  255, 0, 31,  255, 0, 32,  255, 0, 33,  255, 0, 34,  255,
    0, 35,  255, 0, 36,  255, 0, 37,  255, 0, 38,  255, 0, 39,  255,
    0, 40,  255, 0, 41,  255, 0, 42,  255, 0, 43,  255, 0, 44,  255,
    0, 45,  255, 0, 46,  255, 0, 47,  255, 0, 48,  255, 0, 49,  255,
    0, 50,  255, 0, 51,  255, 0, 52,  255, 0, 53,  255, 0, 54,  255,
    0, 55,  255, 0, 56,  255, 0, 57,  255, 0, 58,  255, 0, 59,  255,
    0, 60,  255, 0, 61,  255, 0, 62,  255, 0, 63,  255, 0, 64,  255,
    0, 65,  255, 0, 66,  255, 0, 67,  255, 0, 68,  255, 0, 69,  255,
    0, 70,  255, 0, 71,  255, 0, 72,  255, 0, 73,  255, 0, 74,  255,
    0, 75,  255, 0, 76,  255, 0, 77,  255, 0, 78,  255, 0, 79,  255,
    0, 80,  255, 0, 81,  255, 0, 82,  255, 0, 83,  255, 0, 84,  255,
    0, 85,  255, 0, 86,  255, 0, 87,  255, 0, 88,  255, 0, 89,  255,
    0, 90,  255, 0, 91,  255, 0, 92,  255, 0, 93,  255, 0, 94,  255,
    0, 95,  255, 0, 96,  255, 0, 97,  255, 0, 98,  255, 0, 99,  255,
    0, 100, 255, 0, 101, 255, 0, 102, 255, 0, 103, 255, 0, 104, 255,
    0, 105, 255, 0, 106, 255, 0, 107, 255, 0, 108, 255, 0, 109, 255,
    0, 110, 255, 0, 111, 255, 0, 112, 255, 0, 113, 255, 0, 114, 255,
    0, 115, 255, 0, 116, 255, 0, 117, 255, 0, 118, 255, 0, 119, 255,
    0, 120, 255, 0, 121, 255, 0, 122, 255, 0, 123, 255, 0, 124, 255,
    0, 125, 255, 0, 126, 255, 0, 127, 255, 0, 128, 255, 0, 129, 255,
    0, 130, 255, 0, 131, 255, 0, 132, 255, 0, 133, 255, 0, 134, 255,
    0, 135, 255, 0, 136, 255, 0, 137, 255, 0, 138, 255, 0, 139, 255,
    0, 140, 255, 0, 141, 255, 0, 142, 255, 0, 143, 255, 0, 144, 255,
    0, 145, 255, 0, 146, 255, 0, 147, 255, 0, 148, 255, 0, 149, 255,
    0, 150, 255, 0, 151, 255, 0, 152, 255, 0, 153, 255, 0, 154, 255,
    0, 155, 255, 0, 156, 255, 0, 157, 255, 0, 158, 255, 0, 159, 255,
    0, 160, 255, 0, 161, 255, 0, 162, 255, 0, 163, 255, 0, 164, 255,
    0, 165, 255, 0, 166, 255, 0, 167, 255, 0, 168, 255, 0, 169, 255,
    0, 170, 255, 0, 171, 255, 0, 172, 255, 0, 173, 255, 0, 174, 255,
    0, 175, 255, 0, 176, 255, 0, 177, 255, 0, 178, 255, 0, 179, 255,
    0, 180, 255, 0, 181, 255, 0, 182, 255, 0, 183, 255, 0, 184, 255,
    0, 185, 255, 0, 186, 255, 0, 187, 255, 0, 188, 255, 0, 189, 255,
    0, 190, 255, 0, 191, 255, 0, 192, 255, 0, 193, 255, 0, 194, 255,
    0, 195, 255, 0, 196, 255, 0, 197, 255, 0, 198, 255, 0, 199, 255,
    0, 200, 255, 0, 201, 255, 0, 202, 255, 0, 203, 255, 0, 204, 255,
    0, 205, 255, 0, 206, 255, 0, 207, 255, 0, 208, 255, 0, 209, 255,
    0, 210, 255, 0, 211, 255, 0, 212, 255, 0, 213, 255, 0, 214, 255,
    0, 215, 255, 0, 216, 255, 0, 217, 255, 0, 218, 255, 0, 219, 255,
    0, 220, 255, 0, 221, 255, 0, 222, 255, 0, 223, 255, 0, 224, 255,
    0, 225, 255, 0, 226, 255, 0, 227, 255, 0, 228, 255, 0, 229, 255,
    0, 230, 255, 0, 231, 255, 0, 232, 255, 0, 233, 255, 0, 234, 255,
    0, 235, 255, 0, 236, 255, 0, 237, 255, 0, 238, 255, 0, 239, 255,
    0, 240, 255, 0, 241, 255, 0, 242, 255, 0, 243, 255, 0, 244, 255,
    0, 245, 255, 0, 246, 255, 0, 247, 255, 0, 248, 255, 0, 249, 255,
    0, 250, 255, 0, 251, 255, 0, 252, 255, 0, 253, 255, 0, 254, 255,
    0, 255, 255,
};

constexpr unsigned char kBoneColormap[] = {
    0,   0,   0,   2,   1,   1,   3,   2,   2,   4,   3,   3,   5,   4,   4,
    6,   4,   4,   8,   5,   5,   9,   6,   6,   10,  7,   7,   11,  8,   8,
    12,  9,   9,   14,  10,  10,  15,  11,  11,  16,  11,  11,  17,  12,  12,
    18,  13,  13,  20,  14,  14,  21,  15,  15,  22,  16,  16,  23,  17,  17,
    24,  18,  18,  26,  18,  18,  27,  19,  19,  28,  20,  20,  29,  21,  21,
    31,  22,  22,  32,  23,  23,  33,  24,  24,  34,  24,  24,  35,  25,  25,
    37,  26,  26,  38,  27,  27,  39,  28,  28,  40,  29,  29,  41,  30,  30,
    43,  31,  31,  44,  32,  32,  45,  32,  32,  46,  33,  33,  47,  34,  34,
    49,  35,  35,  50,  36,  36,  51,  37,  37,  52,  38,  38,  53,  39,  39,
    55,  39,  39,  56,  40,  40,  57,  41,  41,  58,  42,  42,  59,  43,  43,
    61,  44,  44,  62,  45,  45,  63,  46,  46,  64,  46,  46,  66,  47,  47,
    67,  48,  48,  68,  49,  49,  69,  50,  50,  70,  51,  51,  72,  52,  52,
    73,  53,  53,  74,  53,  53,  75,  54,  54,  76,  55,  55,  78,  56,  56,
    79,  57,  57,  80,  58,  58,  81,  59,  59,  82,  60,  60,  84,  60,  60,
    85,  61,  61,  86,  62,  62,  87,  63,  63,  88,  64,  64,  90,  65,  65,
    91,  66,  66,  92,  67,  67,  93,  67,  67,  94,  68,  68,  96,  69,  69,
    97,  70,  70,  98,  71,  71,  99,  72,  72,  101, 73,  73,  102, 74,  74,
    103, 74,  74,  104, 75,  75,  105, 76,  76,  107, 77,  77,  108, 78,  78,
    109, 79,  79,  110, 80,  80,  111, 81,  81,  113, 81,  81,  114, 82,  82,
    115, 83,  83,  116, 84,  84,  117, 86,  85,  118, 87,  86,  119, 88,  87,
    119, 89,  88,  120, 90,  88,  121, 92,  89,  122, 93,  90,  123, 94,  91,
    124, 95,  92,  125, 96,  93,  126, 98,  94,  126, 99,  95,  127, 100, 95,
    128, 101, 96,  129, 102, 97,  130, 104, 98,  131, 105, 99,  132, 106, 100,
    132, 107, 101, 133, 108, 102, 134, 110, 102, 135, 111, 103, 136, 112, 104,
    137, 113, 105, 138, 115, 106, 139, 116, 107, 140, 117, 108, 140, 118, 109,
    141, 119, 109, 142, 121, 110, 143, 122, 111, 144, 123, 112, 145, 124, 113,
    146, 125, 114, 147, 127, 115, 147, 128, 116, 148, 129, 116, 149, 130, 117,
    150, 131, 118, 151, 133, 119, 152, 134, 120, 153, 135, 121, 154, 136, 122,
    154, 137, 123, 155, 139, 123, 156, 140, 124, 157, 141, 125, 158, 142, 126,
    159, 143, 127, 160, 145, 128, 161, 146, 129, 161, 147, 130, 162, 148, 130,
    163, 150, 131, 164, 151, 132, 165, 152, 133, 166, 153, 134, 167, 154, 135,
    168, 156, 136, 168, 157, 137, 169, 158, 137, 170, 159, 138, 171, 160, 139,
    172, 162, 140, 173, 163, 141, 174, 164, 142, 175, 165, 143, 175, 166, 144,
    176, 168, 144, 177, 169, 145, 178, 170, 146, 179, 171, 147, 180, 172, 148,
    181, 174, 149, 181, 175, 150, 182, 176, 151, 183, 177, 151, 184, 178, 152,
    185, 180, 153, 186, 181, 154, 187, 182, 155, 188, 183, 156, 188, 185, 157,
    189, 186, 158, 190, 187, 158, 191, 188, 159, 192, 189, 160, 193, 191, 161,
    194, 192, 162, 195, 193, 163, 195, 194, 164, 196, 195, 165, 197, 197, 165,
    198, 198, 166, 199, 199, 167, 200, 200, 168, 201, 201, 170, 202, 202, 171,
    203, 203, 173, 203, 203, 174, 204, 204, 175, 205, 205, 177, 206, 206, 178,
    207, 207, 179, 208, 208, 181, 209, 209, 182, 210, 210, 184, 210, 210, 185,
    211, 211, 186, 212, 212, 188, 213, 213, 189, 214, 214, 190, 215, 215, 192,
    216, 216, 193, 217, 217, 195, 217, 217, 196, 218, 218, 197, 219, 219, 199,
    220, 220, 200, 221, 221, 201, 222, 222, 203, 223, 223, 204, 224, 224, 206,
    224, 224, 207, 225, 225, 208, 226, 226, 210, 227, 227, 211, 228, 228, 212,
    229, 229, 214, 230, 230, 215, 230, 230, 217, 231, 231, 218, 232, 232, 219,
    233, 233, 221, 234, 234, 222, 235, 235, 223, 236, 236, 225, 237, 237, 226,
    237, 237, 228, 238, 238, 229, 239, 239, 230, 240, 240, 232, 241, 241, 233,
    242, 242, 234, 243, 243, 236, 244, 244, 237, 244, 244, 239, 245, 245, 240,
    246, 246, 241, 247, 247, 243, 248, 248, 244, 249, 249, 245, 250, 250, 247,
    251, 251, 248, 251, 251, 250, 252, 252, 251, 253, 253, 252, 254, 254, 254,
    255, 255, 255,
};

constexpr unsigned char kCoolColormap[] = {
    255, 255, 0,   255, 254, 1,   255, 253, 2,   255, 252, 3,   255, 251, 4,
    255, 250, 5,   255, 249, 6,   255, 248, 7,   255, 247, 8,   255, 246, 9,
    255, 245, 10,  255, 244, 11,  255, 243, 12,  255, 242, 13,  255, 241, 14,
    255, 240, 15,  255, 239, 16,  255, 238, 17,  255, 237, 18,  255, 236, 19,
    255, 235, 20,  255, 234, 21,  255, 233, 22,  255, 232, 23,  255, 231, 24,
    255, 230, 25,  255, 229, 26,  255, 228, 27,  255, 227, 28,  255, 226, 29,
    255, 225, 30,  255, 224, 31,  255, 223, 32,  255, 222, 33,  255, 221, 34,
    255, 220, 35,  255, 219, 36,  255, 218, 37,  255, 217, 38,  255, 216, 39,
    255, 215, 40,  255, 214, 41,  255, 213, 42,  255, 212, 43,  255, 211, 44,
    255, 210, 45,  255, 209, 46,  255, 208, 47,  255, 207, 48,  255, 206, 49,
    255, 205, 50,  255, 204, 51,  255, 203, 52,  255, 202, 53,  255, 201, 54,
    255, 200, 55,  255, 199, 56,  255, 198, 57,  255, 197, 58,  255, 196, 59,
    255, 195, 60,  255, 194, 61,  255, 193, 62,  255, 192, 63,  255, 191, 64,
    255, 190, 65,  255, 189, 66,  255, 188, 67,  255, 187, 68,  255, 186, 69,
    255, 185, 70,  255, 184, 71,  255, 183, 72,  255, 182, 73,  255, 181, 74,
    255, 180, 75,  255, 179, 76,  255, 178, 77,  255, 177, 78,  255, 176, 79,
    255, 175, 80,  255, 174, 81,  255, 173, 82,  255, 172, 83,  255, 171, 84,
    255, 170, 85,  255, 169, 86,  255, 168, 87,  255, 167, 88,  255, 166, 89,
    255, 165, 90,  255, 164, 91,  255, 163, 92,  255, 162, 93,  255, 161, 94,
    255, 160, 95,  255, 159, 96,  255, 158, 97,  255, 157, 98,  255, 156, 99,
    255, 155, 100, 255, 154, 101, 255, 153, 102, 255, 152, 103, 255, 151, 104,
    255, 150, 105, 255, 149, 106, 255, 148, 107, 255, 147, 108, 255, 146, 109,
    255, 145, 110, 255, 144, 111, 255, 143, 112, 255, 142, 113, 255, 141, 114,
    255, 140, 115, 255, 139, 116, 255, 138, 117, 255, 137, 118, 255, 136, 119,
    255, 135, 120, 255, 134, 121, 255, 133, 122, 255, 132, 123, 255, 131, 124,
    255, 130, 125, 255, 129, 126, 255, 128, 127, 255, 127, 128, 255, 126, 129,
    255, 125, 130, 255, 124, 131, 255, 123, 132, 255, 122, 133, 255, 121, 134,
    255, 120, 135, 255, 119, 136, 255, 118, 137, 255, 117, 138, 255, 116, 139,
    255, 115, 140, 255, 114, 141, 255, 113, 142, 255, 112, 143, 255, 111, 144,
    255, 110, 145, 255, 109, 146, 255, 108, 147, 255, 107, 148, 255, 106, 149,
    255, 105, 150, 255, 104, 151, 255, 103, 152, 255, 102, 153, 255, 101, 154,
    255, 100, 155, 255, 99,  156, 255, 98,  157, 255, 97,  158, 255, 96,  159,
    255, 95,  160, 255, 94,  161, 255, 93,  162, 255, 92,  163, 255, 91,  164,
    255, 90,  165, 255, 89,  166, 255, 88,  167, 255, 87,  168, 255, 86,  169,
    255, 85,  170, 255, 84,  171, 255, 83,  172, 255, 82,  173, 255, 81,  174,
    255, 80,  175, 255, 79,  176, 255, 78,  177, 255, 77,  178, 255, 76,  179,
    255, 75,  180, 255, 74,  181, 255, 73,  182, 255, 72,  183, 255, 71,  184,
    255, 70,  185, 255, 69,  186, 255, 68,  187, 255, 67,  188, 255, 66,  189,
    255, 65,  190, 255, 64,  191, 255, 63,  192, 255, 62,  193, 255, 61,  194,
    255, 60,  195, 255, 59,  196, 255, 58,  197, 255, 57,  198, 255, 56,  199,
    255, 55,  200, 255, 54,  201, 255, 53,  202, 255, 52,  203, 255, 51,  204,
    255, 50,  205, 255, 49,  206, 255, 48,  207, 255, 47,  208, 255, 46,  209,
    255, 45,  210, 255, 44,  211, 255, 43,  212, 255, 42,  213, 255, 41,  214,
    255, 40,  215, 255, 39,  216, 255, 38,  217, 255, 37,  218, 255, 36,  219,
    255, 35,  220, 255, 34,  221, 255, 33,  222, 255, 32,  223, 255, 31,  224,
    255, 30,  225, 255, 29,  226, 255, 28,  227, 255, 27,  228, 255, 26,  229,
    255, 25,  230, 255, 24,  231, 255, 23,  232, 255, 22,  233, 255, 21,  234,
    255, 20,  235, 255, 19,  236, 255, 18,  237, 255, 17,  238, 255, 16,  239,
    255, 15,  240, 255, 14,  241, 255, 13,  242, 255, 12,  243, 255, 11,  244,
    255, 10,  245, 255, 9,   246, 255, 8,   247, 255, 7,   248, 255, 6,   249,
    255, 5,   250, 255, 4,   251, 255, 3,   252, 255, 2,   253, 255, 1,   254,
    255, 0,   255,
};

constexpr unsigned char kHotColormap[] = {
    0,   0,   3,   0,   0,   5,   0,   0,   8,   0,   0,   11,  0,   0,   13,
    0,   0,   16,  0,   0,   19,  0,   0,   21,  0,   0,   24,  0,   0,   27,
    0,   0,   29,  0,   0,   32,  0,   0,   35,  0,   0,   37,  0,   0,   40,
    0,   0,   43,  0,   0,   45,  0,   0,   48,  0,   0,   50,  0,   0,   53,
    0,   0,   56,  0,   0,   58,  0,   0,   61,  0,   0,   64,  0,   0,   66,
    0,   0,   69,  0,   0,   72,  0,   0,   74,  0,   0,   77,  0,   0,   80,
    0,   0,   82,  0,   0,   85,  0,   0,   88,  0,   0,   90,  0,   0,   93,
    0,   0,   96,  0,   0,   98,  0,   0,   101, 0,   0,   104, 0,   0,   106,
    0,   0,   109, 0,   0,   112, 0,   0,   114, 0,   0,   117, 0,   0,   120,
    0,   0,   122, 0,   0,   125, 0,   0,   128, 0,   0,   130, 0,   0,   133,
    0,   0,   135, 0,   0,   138, 0,   0,   141, 0,   0,   143, 0,   0,   146,
    0,   0,   149, 0,   0,   151, 0,   0,   154, 0,   0,   157, 0,   0,   159,
    0,   0,   162, 0,   0,   165, 0,   0,   167, 0,   0,   170, 0,   0,   173,
    0,   0,   175, 0,   0,   178, 0,   0,   181, 0,   0,   183, 0,   0,   186,
    0,   0,   189, 0,   0,   191, 0,   0,   194, 0,   0,   197, 0,   0,   199,
    0,   0,   202, 0,   0,   205, 0,   0,   207, 0,   0,   210, 0,   0,   213,
    0,   0,   215, 0,   0,   218, 0,   0,   220, 0,   0,   223, 0,   0,   226,
    0,   0,   228, 0,   0,   231, 0,   0,   234, 0,   0,   236, 0,   0,   239,
    0,   0,   242, 0,   0,   244, 0,   0,   247, 0,   0,   250, 0,   0,   252,
    0,   0,   255, 0,   3,   255, 0,   5,   255, 0,   8,   255, 0,   11,  255,
    0,   13,  255, 0,   16,  255, 0,   19,  255, 0,   21,  255, 0,   24,  255,
    0,   27,  255, 0,   29,  255, 0,   32,  255, 0,   35,  255, 0,   37,  255,
    0,   40,  255, 0,   43,  255, 0,   45,  255, 0,   48,  255, 0,   50,  255,
    0,   53,  255, 0,   56,  255, 0,   58,  255, 0,   61,  255, 0,   64,  255,
    0,   66,  255, 0,   69,  255, 0,   72,  255, 0,   74,  255, 0,   77,  255,
    0,   80,  255, 0,   82,  255, 0,   85,  255, 0,   88,  255, 0,   90,  255,
    0,   93,  255, 0,   96,  255, 0,   98,  255, 0,   101, 255, 0,   104, 255,
    0,   106, 255, 0,   109, 255, 0,   112, 255, 0,   114, 255, 0,   117, 255,
    0,   120, 255, 0,   122, 255, 0,   125, 255, 0,   128, 255, 0,   130, 255,
    0,   133, 255, 0,   135, 255, 0,   138, 255, 0,   141, 255, 0,   143, 255,
    0,   146, 255, 0,   149, 255, 0,   151, 255, 0,   154, 255, 0,   157, 255,
    0,   159, 255, 0,   162, 255, 0,   165, 255, 0,   167, 255, 0,   170, 255,
    0,   173, 255, 0,   175, 255, 0,   178, 255, 0,   181, 255, 0,   183, 255,
    0,   186, 255, 0,   189, 255, 0,   191, 255, 0,   194, 255, 0,   197, 255,
    0,   199, 255, 0,   202, 255, 0,   205, 255, 0,   207, 255, 0,   210, 255,
    0,   213, 255, 0,   215, 255, 0,   218, 255, 0,   220, 255, 0,   223, 255,
    0,   226, 255, 0,   228, 255, 0,   231, 255, 0,   234, 255, 0,   236, 255,
    0,   239, 255, 0,   242, 255, 0,   244, 255, 0,   247, 255, 0,   250, 255,
    0,   252, 255, 0,   255, 255, 4,   255, 255, 8,   255, 255, 12,  255, 255,
    16,  255, 255, 20,  255, 255, 24,  255, 255, 28,  255, 255, 32,  255, 255,
    36,  255, 255, 40,  255, 255, 44,  255, 255, 48,  255, 255, 52,  255, 255,
    56,  255, 255, 60,  255, 255, 64,  255, 255, 68,  255, 255, 72,  255, 255,
    76,  255, 255, 80,  255, 255, 84,  255, 255, 88,  255, 255, 92,  255, 255,
    96,  255, 255, 100, 255, 255, 104, 255, 255, 108, 255, 255, 112, 255, 255,
    116, 255, 255, 120, 255, 255, 124, 255, 255, 128, 255, 255, 131, 255, 255,
    135, 255, 255, 139, 255, 255, 143, 255, 255, 147, 255, 255, 151, 255, 255,
    155, 255, 255, 159, 255, 255, 163, 255, 255, 167, 255, 255, 171, 255, 255,
    175, 255, 255, 179, 255, 255, 183, 255, 255, 187, 255, 255, 191, 255, 255,
    195, 255, 255, 199, 255, 255, 203, 255, 255, 207, 255, 255, 211, 255, 255,
    215, 255, 255, 219, 255, 255, 223, 255, 255, 227, 255, 255, 231, 255, 255,
    235, 255, 255, 239, 255, 255, 243, 255, 255, 247, 255, 255, 251, 255, 255,
    255, 255, 255,
};

constexpr unsigned char kHsvColormap[] = {
    0,   0,   255, 0,   6,   255, 0,   12,  255, 0,   18,  255, 0,   24,  255,
    0,   30,  255, 0,   36,  255, 0,   42,  255, 0,   48,  255, 0,   54,  255,
    0,   60,  255, 0,   66,  255, 0,   72,  255, 0,   78,  255, 0,   84,  255,
    0,   90,  255, 0,   96,  255, 0,   102, 255, 0,   108, 255, 0,   114, 255,
    0,   120, 255, 0,   126, 255, 0,   131, 255, 0,   137, 255, 0,   143, 255,
    0,   149, 255, 0,   155, 255, 0,   161, 255, 0,   167, 255, 0,   173, 255,
    0,   179, 255, 0,   185, 255, 0,   191, 255, 0,   197, 255, 0,   203, 255,
    0,   209, 255, 0,   215, 255, 0,   221, 255, 0,   227, 255, 0,   233, 255,
    0,   239, 255, 0,   245, 255, 0,   251, 255, 0,   255, 253, 0,   255, 247,
    0,   255, 241, 0,   255, 235, 0,   255, 229, 0,   255, 223, 0,   255, 217,
    0,   255, 211, 0,   255, 205, 0,   255, 199, 0,   255, 193, 0,   255, 187,
    0,   255, 181, 0,   255, 175, 0,   255, 169, 0,   255, 163, 0,   255, 157,
    0,   255, 151, 0,   255, 145, 0,   255, 139, 0,   255, 133, 0,   255, 128,
    0,   255, 122, 0,   255, 116, 0,   255, 110, 0,   255, 104, 0,   255, 98,
    0,   255, 92,  0,   255, 86,  0,   255, 80,  0,   255, 74,  0,   255, 68,
    0,   255, 62,  0,   255, 56,  0,   255, 50,  0,   255, 44,  0,   255, 38,
    0,   255, 32,  0,   255, 26,  0,   255, 20,  0,   255, 14,  0,   255, 8,
    0,   255, 2,   4,   255, 0,   10,  255, 0,   16,  255, 0,   22,  255, 0,
    28,  255, 0,   34,  255, 0,   40,  255, 0,   46,  255, 0,   52,  255, 0,
    58,  255, 0,   64,  255, 0,   70,  255, 0,   76,  255, 0,   82,  255, 0,
    88,  255, 0,   94,  255, 0,   100, 255, 0,   106, 255, 0,   112, 255, 0,
    118, 255, 0,   124, 255, 0,   129, 255, 0,   135, 255, 0,   141, 255, 0,
    147, 255, 0,   153, 255, 0,   159, 255, 0,   165, 255, 0,   171, 255, 0,
    177, 255, 0,   183, 255, 0,   189, 255, 0,   195, 255, 0,   201, 255, 0,
    207, 255, 0,   213, 255, 0,   219, 255, 0,   225, 255, 0,   231, 255, 0,
    237, 255, 0,   243, 255, 0,   249, 255, 0,   255, 255, 0,   255, 249, 0,
    255, 243, 0,   255, 237, 0,   255, 231, 0,   255, 225, 0,   255, 219, 0,
    255, 213, 0,   255, 207, 0,   255, 201, 0,   255, 195, 0,   255, 189, 0,
    255, 183, 0,   255, 177, 0,   255, 171, 0,   255, 165, 0,   255, 159, 0,
    255, 153, 0,   255, 147, 0,   255, 141, 0,   255, 135, 0,   255, 129, 0,
    255, 124, 0,   255, 118, 0,   255, 112, 0,   255, 106, 0,   255, 100, 0,
    255, 94,  0,   255, 88,  0,   255, 82,  0,   255, 76,  0,   255, 70,  0,
    255, 64,  0,   255, 58,  0,   255, 52,  0,   255, 46,  0,   255, 40,  0,
    255, 34,  0,   255, 28,  0,   255, 22,  0,   255, 16,  0,   255, 10,  0,
    255, 4,   0,   255, 0,   2,   255, 0,   8,   255, 0,   14,  255, 0,   20,
    255, 0,   26,  255, 0,   32,  255, 0,   38,  255, 0,   44,  255, 0,   50,
    255, 0,   56,  255, 0,   62,  255, 0,   68,  255, 0,   74,  255, 0,   80,
    255, 0,   86,  255, 0,   92,  255, 0,   98,  255, 0,   104, 255, 0,   110,
    255, 0,   116, 255, 0,   122, 255, 0,   128, 255, 0,   133, 255, 0,   139,
    255, 0,   145, 255, 0,   151, 255, 0,   157, 255, 0,   163, 255, 0,   169,
    255, 0,   175, 255, 0,   181, 255, 0,   187, 255, 0,   193, 255, 0,   199,
    255, 0,   205, 255, 0,   211, 255, 0,   217, 255, 0,   223, 255, 0,   229,
    255, 0,   235, 255, 0,   241, 255, 0,   247, 255, 0,   253, 251, 0,   255,
    245, 0,   255, 239, 0,   255, 233, 0,   255, 227, 0,   255, 221, 0,   255,
    215, 0,   255, 209, 0,   255, 203, 0,   255, 197, 0,   255, 191, 0,   255,
    185, 0,   255, 179, 0,   255, 173, 0,   255, 167, 0,   255, 161, 0,   255,
    155, 0,   255, 149, 0,   255, 143, 0,   255, 137, 0,   255, 131, 0,   255,
    126, 0,   255, 120, 0,   255, 114, 0,   255, 108, 0,   255, 102, 0,   255,
    96,  0,   255, 90,  0,   255, 84,  0,   255, 78,  0,   255, 72,  0,   255,
    66,  0,   255, 60,  0,   255, 54,  0,   255, 48,  0,   255, 42,  0,   255,
    36,  0,   255, 30,  0,   255, 24,  0,   255, 18,  0,   255, 12,  0,   255,
    6,   0,   255,
};

constexpr unsigned char kJetColormap[] = {
    131, 0,   0,   135, 0,   0,   139, 0,   0,   143, 0,   0,   147, 0,   0,
    151, 0,   0,   155, 0,   0,   159, 0,   0,   163, 0,   0,   167, 0,   0,
    171, 0,   0,   175, 0,   0,   179, 0,   0,   183, 0,   0,   187, 0,   0,
    191, 0,   0,   195, 0,   0,   199, 0,   0,   203, 0,   0,   207, 0,   0,
    211, 0,   0,   215, 0,   0,   219, 0,   0,   223, 0,   0,   227, 0,   0,
    231, 0,   0,   235, 0,   0,   239, 0,   0,   243, 0,   0,   247, 0,   0,
    251, 0,   0,   255, 0,   0,   255, 4,   0,   255, 8,   0,   255, 12,  0,
    255, 16,  0,   255, 20,  0,   255, 24,  0,   255, 28,  0,   255, 32,  0,
    255, 36,  0,   255, 40,  0,   255, 44,  0,   255, 48,  0,   255, 52,  0,
    255, 56,  0,   255, 60,  0,   255, 64,  0,   255, 68,  0,   255, 72,  0,
    255, 76,  0,   255, 80,  0,   255, 84,  0,   255, 88,  0,   255, 92,  0,
    255, 96,  0,   255, 100, 0,   255, 104, 0,   255, 108, 0,   255, 112, 0,
    255, 116, 0,   255, 120, 0,   255, 124, 0,   255, 128, 0,   255, 131, 0,
    255, 135, 0,   255, 139, 0,   255, 143, 0,   255, 147, 0,   255, 151, 0,
    255, 155, 0,   255, 159, 0,   255, 163, 0,   255, 167, 0,   255, 171, 0,
    255, 175, 0,   255, 179, 0,   255, 183, 0,   255, 187, 0,   255, 191, 0,
    255, 195, 0,   255, 199, 0,   255, 203, 0,   255, 207, 0,   255, 211, 0,
    255, 215, 0,   255, 219, 0,   255, 223, 0,   255, 227, 0,   255, 231, 0,
    255, 235, 0,   255, 239, 0,   255, 243, 0,   255, 247, 0,   255, 251, 0,
    255, 255, 0,   251, 255, 4,   247, 255, 8,   243, 255, 12,  239, 255, 16,
    235, 255, 20,  231, 255, 24,  227, 255, 28,  223, 255, 32,  219, 255, 36,
    215, 255, 40,  211, 255, 44,  207, 255, 48,  203, 255, 52,  199, 255, 56,
    195, 255, 60,  191, 255, 64,  187, 255, 68,  183, 255, 72,  179, 255, 76,
    175, 255, 80,  171, 255, 84,  167, 255, 88,  163, 255, 92,  159, 255, 96,
    155, 255, 100, 151, 255, 104, 147, 255, 108, 143, 255, 112, 139, 255, 116,
    135, 255, 120, 131, 255, 124, 128, 255, 128, 124, 255, 131, 120, 255, 135,
    116, 255, 139, 112, 255, 143, 108, 255, 147, 104, 255, 151, 100, 255, 155,
    96,  255, 159, 92,  255, 163, 88,  255, 167, 84,  255, 171, 80,  255, 175,
    76,  255, 179, 72,  255, 183, 68,  255, 187, 64,  255, 191, 60,  255, 195,
    56,  255, 199, 52,  255, 203, 48,  255, 207, 44,  255, 211, 40,  255, 215,
    36,  255, 219, 32,  255, 223, 28,  255, 227, 24,  255, 231, 20,  255, 235,
    16,  255, 239, 12,  255, 243, 8,   255, 247, 4,   255, 251, 0,   255, 255,
    0,   251, 255, 0,   247, 255, 0,   243, 255, 0,   239, 255, 0,   235, 255,
    0,   231, 255, 0,   227, 255, 0,   223, 255, 0,   219, 255, 0,   215, 255,
    0,   211, 255, 0,   207, 255, 0,   203, 255, 0,   199, 255, 0,   195, 255,
    0,   191, 255, 0,   187, 255, 0,   183, 255, 0,   179, 255, 0,   175, 255,
    0,   171, 255, 0,   167, 255, 0,   163, 255, 0,   159, 255, 0,   155, 255,
    0,   151, 255, 0,   147, 255, 0,   143, 255, 0,   139, 255, 0,   135, 255,
    0,   131, 255, 0,   128, 255, 0,   124, 255, 0,   120, 255, 0,   116, 255,
    0,   112, 255, 0,   108, 255, 0,   104, 255, 0,   100, 255, 0,   96,  255,
    0,   92,  255, 0,   88,  255, 0,   84,  255, 0,   80,  255, 0,   76,  255,
    0,   72,  255, 0,   68,  255, 0,   64,  255, 0,   60,  255, 0,   56,  255,
    0,   52,  255, 0,   48,  255, 0,   44,  255, 0,   40,  255, 0,   36,  255,
    0,   32,  255, 0,   28,  255, 0,   24,  255, 0,   20,  255, 0,   16,  255,
    0,   12,  255, 0,   8,   255, 0,   4,   255, 0,   0,   255, 0,   0,   251,
    0,   0,   247, 0,   0,   243, 0,   0,   239, 0,   0,   235, 0,   0,   231,
    0,   0,   227, 0,   0,   223, 0,   0,   219, 0,   0,   215, 0,   0,   211,
    0,   0,   207, 0,   0,   203, 0,   0,   199, 0,   0,   195, 0,   0,   191,
    0,   0,   187, 0,   0,   183, 0,   0,   179, 0,   0,   175, 0,   0,   171,
    0,   0,   167, 0,   0,   163, 0,   0,   159, 0,   0,   155, 0,   0,   151,
    0,   0,   147, 0,   0,   143, 0,   0,   139, 0,   0,   135, 0,   0,   131,
    0,   0,   128,
};

constexpr double kMRColorMap[] = {
    0.000, 0.447, 0.741, 0.850, 0.325, 0.098, 0.929, 0.694, 0.125, 0.494, 0.184,
    0.556, 0.466, 0.674, 0.188, 0.301, 0.745, 0.933, 0.635, 0.078, 0.184, 0.300,
    0.300, 0.300, 0.600, 0.600, 0.600, 1.000, 0.000, 0.000, 1.000, 0.500, 0.000,
    0.749, 0.749, 0.000, 0.000, 1.000, 0.000, 0.000, 0.000, 1.000, 0.667, 0.000,
    1.000, 0.333, 0.333, 0.000, 0.333, 0.667, 0.000, 0.333, 1.000, 0.000, 0.667,
    0.333, 0.000, 0.667, 0.667, 0.000, 0.667, 1.000, 0.000, 1.000, 0.333, 0.000,
    1.000, 0.667, 0.000, 1.000, 1.000, 0.000, 0.000, 0.333, 0.500, 0.000, 0.667,
    0.500, 0.000, 1.000, 0.500, 0.333, 0.000, 0.500, 0.333, 0.333, 0.500, 0.333,
    0.667, 0.500, 0.333, 1.000, 0.500, 0.667, 0.000, 0.500, 0.667, 0.333, 0.500,
    0.667, 0.667, 0.500, 0.667, 1.000, 0.500, 1.000, 0.000, 0.500, 1.000, 0.333,
    0.500, 1.000, 0.667, 0.500, 1.000, 1.000, 0.500, 0.000, 0.333, 1.000, 0.000,
    0.667, 1.000, 0.000, 1.000, 1.000, 0.333, 0.000, 1.000, 0.333, 0.333, 1.000,
    0.333, 0.667, 1.000, 0.333, 1.000, 1.000, 0.667, 0.000, 1.000, 0.667, 0.333,
    1.000, 0.667, 0.667, 1.000, 0.667, 1.000, 1.000, 1.000, 0.000, 1.000, 1.000,
    0.333, 1.000, 1.000, 0.667, 1.000, 0.167, 0.000, 0.000, 0.333, 0.000, 0.000,
    0.500, 0.000, 0.000, 0.667, 0.000, 0.000, 0.833, 0.000, 0.000, 1.000, 0.000,
    0.000, 0.000, 0.167, 0.000, 0.000, 0.333, 0.000, 0.000, 0.500, 0.000, 0.000,
    0.667, 0.000, 0.000, 0.833, 0.000, 0.000, 1.000, 0.000, 0.000, 0.000, 0.167,
    0.000, 0.000, 0.333, 0.000, 0.000, 0.500, 0.000, 0.000, 0.667, 0.000, 0.000,
    0.833, 0.000, 0.000, 1.000, 0.000, 0.000, 0.000, 0.143, 0.143, 0.143, 0.286,
    0.286, 0.286, 0.429, 0.429, 0.429, 0.571, 0.571, 0.571, 0.714, 0.714, 0.714,
    0.857, 0.857, 0.857, 1.000, 1.000, 1.000};

constexpr unsigned char kOceanColormap[] = {
    0,   0,   0,   1,   0,   0,   2,   0,   0,   3,   0,   0,   4,   0,   0,
    5,   0,   0,   6,   0,   0,   7,   0,   0,   8,   0,   0,   9,   0,   0,
    10,  0,   0,   11,  0,   0,   12,  0,   0,   13,  0,   0,   14,  0,   0,
    15,  0,   0,   16,  0,   0,   17,  0,   0,   18,  0,   0,   19,  0,   0,
    20,  0,   0,   21,  0,   0,   22,  0,   0,   23,  0,   0,   24,  0,   0,
    25,  0,   0,   26,  0,   0,   27,  0,   0,   28,  0,   0,   29,  0,   0,
    30,  0,   0,   31,  0,   0,   32,  0,   0,   33,  0,   0,   34,  0,   0,
    35,  0,   0,   36,  0,   0,   37,  0,   0,   38,  0,   0,   39,  0,   0,
    40,  0,   0,   41,  0,   0,   42,  0,   0,   43,  0,   0,   44,  0,   0,
    45,  0,   0,   46,  0,   0,   47,  0,   0,   48,  0,   0,   49,  0,   0,
    50,  0,   0,   51,  0,   0,   52,  0,   0,   53,  0,   0,   54,  0,   0,
    55,  0,   0,   56,  0,   0,   57,  0,   0,   58,  0,   0,   59,  0,   0,
    60,  0,   0,   61,  0,   0,   62,  0,   0,   63,  0,   0,   64,  0,   0,
    65,  0,   0,   66,  0,   0,   67,  0,   0,   68,  0,   0,   69,  0,   0,
    70,  0,   0,   71,  0,   0,   72,  0,   0,   73,  0,   0,   74,  0,   0,
    75,  0,   0,   76,  0,   0,   77,  0,   0,   78,  0,   0,   79,  0,   0,
    80,  0,   0,   81,  0,   0,   82,  0,   0,   83,  0,   0,   84,  0,   0,
    85,  0,   0,   86,  2,   0,   87,  3,   0,   88,  5,   0,   89,  6,   0,
    90,  8,   0,   91,  9,   0,   92,  11,  0,   93,  12,  0,   94,  14,  0,
    95,  15,  0,   96,  17,  0,   97,  18,  0,   98,  20,  0,   99,  21,  0,
    100, 23,  0,   101, 24,  0,   102, 26,  0,   103, 27,  0,   104, 29,  0,
    105, 30,  0,   106, 32,  0,   107, 33,  0,   108, 35,  0,   109, 36,  0,
    110, 38,  0,   111, 39,  0,   112, 41,  0,   113, 42,  0,   114, 44,  0,
    115, 45,  0,   116, 47,  0,   117, 48,  0,   118, 50,  0,   119, 51,  0,
    120, 53,  0,   121, 54,  0,   122, 56,  0,   123, 57,  0,   124, 59,  0,
    125, 60,  0,   126, 62,  0,   127, 63,  0,   128, 65,  0,   129, 66,  0,
    130, 68,  0,   131, 69,  0,   132, 71,  0,   133, 72,  0,   134, 74,  0,
    135, 75,  0,   136, 77,  0,   137, 78,  0,   138, 80,  0,   139, 81,  0,
    140, 83,  0,   141, 84,  0,   142, 86,  0,   143, 87,  0,   144, 89,  0,
    145, 90,  0,   146, 92,  0,   147, 93,  0,   148, 95,  0,   149, 96,  0,
    150, 98,  0,   151, 99,  0,   152, 101, 0,   153, 102, 0,   154, 104, 0,
    155, 105, 0,   156, 107, 0,   157, 108, 0,   158, 110, 0,   159, 111, 0,
    160, 113, 0,   161, 114, 0,   162, 116, 0,   163, 117, 0,   164, 119, 0,
    165, 120, 0,   166, 122, 0,   167, 123, 0,   168, 125, 0,   169, 126, 0,
    170, 128, 0,   171, 129, 3,   172, 131, 6,   173, 132, 9,   174, 134, 12,
    175, 135, 15,  176, 137, 18,  177, 138, 21,  178, 140, 24,  179, 141, 27,
    180, 143, 30,  181, 144, 33,  182, 146, 36,  183, 147, 39,  184, 149, 42,
    185, 150, 45,  186, 152, 48,  187, 153, 51,  188, 155, 54,  189, 156, 57,
    190, 158, 60,  191, 159, 63,  192, 161, 66,  193, 162, 69,  194, 164, 72,
    195, 165, 75,  196, 167, 78,  197, 168, 81,  198, 170, 84,  199, 171, 87,
    200, 173, 90,  201, 174, 93,  202, 176, 96,  203, 177, 99,  204, 179, 102,
    205, 180, 105, 206, 182, 108, 207, 183, 111, 208, 185, 114, 209, 186, 117,
    210, 188, 120, 211, 189, 123, 212, 191, 126, 213, 192, 129, 214, 194, 132,
    215, 195, 135, 216, 197, 138, 217, 198, 141, 218, 200, 144, 219, 201, 147,
    220, 203, 150, 221, 204, 153, 222, 206, 156, 223, 207, 159, 224, 209, 162,
    225, 210, 165, 226, 212, 168, 227, 213, 171, 228, 215, 174, 229, 216, 177,
    230, 218, 180, 231, 219, 183, 232, 221, 186, 233, 222, 189, 234, 224, 192,
    235, 225, 195, 236, 227, 198, 237, 228, 201, 238, 230, 204, 239, 231, 207,
    240, 233, 210, 241, 234, 213, 242, 236, 216, 243, 237, 219, 244, 239, 222,
    245, 240, 225, 246, 242, 228, 247, 243, 231, 248, 245, 234, 249, 246, 237,
    250, 248, 240, 251, 249, 243, 252, 251, 246, 253, 252, 249, 254, 254, 252,
    255, 255, 255,
};

constexpr unsigned char kPinkColormap[] = {
    0,   0,   15,  13,  13,  25,  18,  18,  32,  23,  23,  38,  26,  26,  43,
    29,  29,  47,  32,  32,  51,  34,  34,  55,  37,  37,  58,  39,  39,  62,
    41,  41,  65,  43,  43,  68,  45,  45,  71,  47,  47,  73,  49,  49,  76,
    50,  50,  79,  52,  52,  81,  54,  54,  83,  55,  55,  86,  57,  57,  88,
    58,  58,  90,  60,  60,  92,  61,  61,  95,  63,  63,  97,  64,  64,  99,
    65,  65,  101, 66,  66,  103, 68,  68,  104, 69,  69,  106, 70,  70,  108,
    71,  71,  110, 73,  73,  112, 74,  74,  114, 75,  75,  115, 76,  76,  117,
    77,  77,  119, 78,  78,  120, 79,  79,  122, 80,  80,  124, 81,  81,  125,
    82,  82,  127, 83,  83,  128, 84,  84,  130, 85,  85,  131, 86,  86,  133,
    87,  87,  134, 88,  88,  136, 89,  89,  137, 90,  90,  139, 91,  91,  140,
    92,  92,  141, 93,  93,  143, 94,  94,  144, 95,  95,  146, 96,  96,  147,
    97,  97,  148, 98,  98,  150, 98,  98,  151, 99,  99,  152, 100, 100, 154,
    101, 101, 155, 102, 102, 156, 103, 103, 157, 103, 103, 159, 104, 104, 160,
    105, 105, 161, 106, 106, 162, 107, 107, 164, 108, 108, 165, 108, 108, 166,
    109, 109, 167, 110, 110, 168, 111, 111, 169, 111, 111, 171, 112, 112, 172,
    113, 113, 173, 114, 114, 174, 114, 114, 175, 115, 115, 176, 116, 116, 177,
    117, 117, 179, 117, 117, 180, 118, 118, 181, 119, 119, 182, 119, 119, 183,
    120, 120, 184, 121, 121, 185, 122, 122, 186, 122, 122, 187, 123, 123, 188,
    124, 124, 189, 124, 124, 190, 125, 125, 191, 126, 126, 192, 126, 126, 193,
    127, 127, 194, 128, 129, 195, 128, 130, 195, 129, 132, 196, 130, 133, 196,
    130, 135, 197, 131, 136, 197, 132, 138, 198, 132, 139, 198, 133, 140, 198,
    134, 142, 199, 134, 143, 199, 135, 145, 200, 135, 146, 200, 136, 147, 201,
    137, 149, 201, 137, 150, 201, 138, 151, 202, 139, 153, 202, 139, 154, 203,
    140, 155, 203, 140, 156, 203, 141, 158, 204, 142, 159, 204, 142, 160, 205,
    143, 161, 205, 143, 163, 206, 144, 164, 206, 145, 165, 206, 145, 166, 207,
    146, 167, 207, 146, 169, 208, 147, 170, 208, 148, 171, 208, 148, 172, 209,
    149, 173, 209, 149, 174, 210, 150, 175, 210, 150, 177, 210, 151, 178, 211,
    151, 179, 211, 152, 180, 212, 153, 181, 212, 153, 182, 212, 154, 183, 213,
    154, 184, 213, 155, 185, 214, 155, 186, 214, 156, 187, 214, 156, 189, 215,
    157, 190, 215, 158, 191, 216, 158, 192, 216, 159, 193, 216, 159, 194, 217,
    160, 195, 217, 160, 196, 218, 161, 197, 218, 161, 198, 218, 162, 199, 219,
    162, 200, 219, 163, 201, 220, 163, 202, 220, 164, 203, 220, 164, 204, 221,
    165, 205, 221, 165, 206, 221, 166, 207, 222, 166, 208, 222, 167, 208, 223,
    167, 209, 223, 168, 210, 223, 168, 211, 224, 169, 212, 224, 169, 213, 225,
    170, 214, 225, 170, 215, 225, 171, 216, 226, 171, 217, 226, 172, 218, 226,
    172, 219, 227, 173, 220, 227, 173, 220, 228, 174, 221, 228, 174, 222, 228,
    175, 223, 229, 175, 224, 229, 176, 225, 229, 176, 226, 230, 177, 227, 230,
    177, 228, 230, 178, 228, 231, 178, 229, 231, 179, 230, 232, 179, 231, 232,
    180, 232, 232, 180, 233, 233, 182, 233, 233, 183, 233, 233, 184, 234, 234,
    186, 234, 234, 187, 235, 235, 188, 235, 235, 190, 235, 235, 191, 236, 236,
    192, 236, 236, 194, 236, 236, 195, 237, 237, 196, 237, 237, 198, 237, 237,
    199, 238, 238, 200, 238, 238, 202, 238, 238, 203, 239, 239, 204, 239, 239,
    205, 240, 240, 207, 240, 240, 208, 240, 240, 209, 241, 241, 210, 241, 241,
    211, 241, 241, 213, 242, 242, 214, 242, 242, 215, 242, 242, 216, 243, 243,
    217, 243, 243, 218, 243, 243, 220, 244, 244, 221, 244, 244, 222, 244, 244,
    223, 245, 245, 224, 245, 245, 225, 245, 245, 226, 246, 246, 228, 246, 246,
    229, 247, 247, 230, 247, 247, 231, 247, 247, 232, 248, 248, 233, 248, 248,
    234, 248, 248, 235, 249, 249, 236, 249, 249, 237, 249, 249, 239, 250, 250,
    240, 250, 250, 241, 250, 250, 242, 251, 251, 243, 251, 251, 244, 251, 251,
    245, 252, 252, 246, 252, 252, 247, 252, 252, 248, 253, 253, 249, 253, 253,
    250, 253, 253, 251, 254, 254, 252, 254, 254, 253, 254, 254, 254, 255, 255,
    255, 255, 255,
};

constexpr unsigned char kRainbowColormap[] = {
    0,   0,   255, 0,   3,   255, 0,   5,   255, 0,   8,   255, 0,   10,  255,
    0,   13,  255, 0,   15,  255, 0,   18,  255, 0,   20,  255, 0,   22,  255,
    0,   25,  255, 0,   28,  255, 0,   30,  255, 0,   33,  255, 0,   35,  255,
    0,   38,  255, 0,   40,  255, 0,   43,  255, 0,   45,  255, 0,   48,  255,
    0,   50,  255, 0,   53,  255, 0,   55,  255, 0,   58,  255, 0,   60,  255,
    0,   63,  255, 0,   65,  255, 0,   68,  255, 0,   70,  255, 0,   73,  255,
    0,   75,  255, 0,   78,  255, 0,   80,  255, 0,   83,  255, 0,   85,  255,
    0,   88,  255, 0,   90,  255, 0,   93,  255, 0,   95,  255, 0,   98,  255,
    0,   100, 255, 0,   103, 255, 0,   105, 255, 0,   108, 255, 0,   110, 255,
    0,   113, 255, 0,   115, 255, 0,   118, 255, 0,   120, 255, 0,   123, 255,
    0,   125, 255, 0,   128, 255, 0,   130, 255, 0,   133, 255, 0,   135, 255,
    0,   138, 255, 0,   140, 255, 0,   143, 255, 0,   145, 255, 0,   148, 255,
    0,   150, 255, 0,   153, 255, 0,   155, 255, 0,   158, 255, 0,   160, 255,
    0,   163, 255, 0,   165, 255, 0,   168, 255, 0,   170, 255, 0,   172, 255,
    0,   175, 255, 0,   178, 255, 0,   180, 255, 0,   182, 255, 0,   185, 255,
    0,   188, 255, 0,   190, 255, 0,   193, 255, 0,   195, 255, 0,   198, 255,
    0,   200, 255, 0,   203, 255, 0,   205, 255, 0,   208, 255, 0,   210, 255,
    0,   212, 255, 0,   215, 255, 0,   218, 255, 0,   220, 255, 0,   223, 255,
    0,   225, 255, 0,   228, 255, 0,   230, 255, 0,   233, 255, 0,   235, 255,
    0,   238, 255, 0,   240, 255, 0,   243, 255, 0,   245, 255, 0,   248, 255,
    0,   250, 255, 0,   252, 255, 0,   255, 255, 0,   255, 250, 0,   255, 245,
    0,   255, 240, 0,   255, 235, 0,   255, 230, 0,   255, 225, 0,   255, 220,
    0,   255, 215, 0,   255, 210, 0,   255, 205, 0,   255, 200, 0,   255, 195,
    0,   255, 190, 0,   255, 185, 0,   255, 180, 0,   255, 175, 0,   255, 170,
    0,   255, 165, 0,   255, 160, 0,   255, 155, 0,   255, 150, 0,   255, 145,
    0,   255, 140, 0,   255, 135, 0,   255, 130, 0,   255, 125, 0,   255, 120,
    0,   255, 115, 0,   255, 110, 0,   255, 105, 0,   255, 100, 0,   255, 95,
    0,   255, 90,  0,   255, 85,  0,   255, 80,  0,   255, 75,  0,   255, 70,
    0,   255, 65,  0,   255, 60,  0,   255, 55,  0,   255, 50,  0,   255, 45,
    0,   255, 40,  0,   255, 35,  0,   255, 30,  0,   255, 25,  0,   255, 20,
    0,   255, 15,  0,   255, 10,  0,   255, 5,   0,   255, 0,   5,   250, 0,
    10,  245, 0,   15,  240, 0,   20,  235, 0,   25,  230, 0,   30,  225, 0,
    35,  220, 0,   40,  215, 0,   45,  210, 0,   50,  205, 0,   55,  200, 0,
    60,  195, 0,   65,  190, 0,   70,  185, 0,   75,  180, 0,   80,  175, 0,
    85,  170, 0,   90,  165, 0,   95,  160, 0,   100, 155, 0,   105, 150, 0,
    110, 145, 0,   115, 140, 0,   120, 135, 0,   125, 130, 0,   130, 125, 0,
    135, 120, 0,   140, 115, 0,   145, 110, 0,   150, 105, 0,   155, 100, 0,
    160, 95,  0,   165, 90,  0,   170, 85,  0,   175, 80,  0,   180, 75,  0,
    185, 70,  0,   190, 65,  0,   195, 60,  0,   200, 55,  0,   205, 50,  0,
    210, 45,  0,   215, 40,  0,   220, 35,  0,   225, 30,  0,   230, 25,  0,
    235, 20,  0,   240, 15,  0,   245, 10,  0,   250, 5,   0,   255, 0,   0,
    255, 0,   3,   255, 0,   7,   255, 0,   10,  255, 0,   13,  255, 0,   17,
    255, 0,   20,  255, 0,   23,  255, 0,   27,  255, 0,   30,  255, 0,   33,
    255, 0,   37,  255, 0,   40,  255, 0,   43,  255, 0,   47,  255, 0,   50,
    255, 0,   53,  255, 0,   57,  255, 0,   60,  255, 0,   63,  255, 0,   67,
    255, 0,   70,  255, 0,   73,  255, 0,   77,  255, 0,   80,  255, 0,   83,
    255, 0,   87,  255, 0,   90,  255, 0,   93,  255, 0,   97,  255, 0,   100,
    255, 0,   103, 255, 0,   107, 255, 0,   110, 255, 0,   113, 255, 0,   117,
    255, 0,   120, 255, 0,   123, 255, 0,   127, 255, 0,   130, 255, 0,   133,
    255, 0,   137, 255, 0,   140, 255, 0,   143, 255, 0,   147, 255, 0,   150,
    255, 0,   153, 255, 0,   157, 255, 0,   160, 255, 0,   163, 255, 0,   167,
    255, 0,   170,
};

constexpr unsigned char kSpringColormap[] = {
    255, 0,   255, 254, 1,   255, 253, 2,   255, 252, 3,   255, 251, 4,   255,
    250, 5,   255, 249, 6,   255, 248, 7,   255, 247, 8,   255, 246, 9,   255,
    245, 10,  255, 244, 11,  255, 243, 12,  255, 242, 13,  255, 241, 14,  255,
    240, 15,  255, 239, 16,  255, 238, 17,  255, 237, 18,  255, 236, 19,  255,
    235, 20,  255, 234, 21,  255, 233, 22,  255, 232, 23,  255, 231, 24,  255,
    230, 25,  255, 229, 26,  255, 228, 27,  255, 227, 28,  255, 226, 29,  255,
    225, 30,  255, 224, 31,  255, 223, 32,  255, 222, 33,  255, 221, 34,  255,
    220, 35,  255, 219, 36,  255, 218, 37,  255, 217, 38,  255, 216, 39,  255,
    215, 40,  255, 214, 41,  255, 213, 42,  255, 212, 43,  255, 211, 44,  255,
    210, 45,  255, 209, 46,  255, 208, 47,  255, 207, 48,  255, 206, 49,  255,
    205, 50,  255, 204, 51,  255, 203, 52,  255, 202, 53,  255, 201, 54,  255,
    200, 55,  255, 199, 56,  255, 198, 57,  255, 197, 58,  255, 196, 59,  255,
    195, 60,  255, 194, 61,  255, 193, 62,  255, 192, 63,  255, 191, 64,  255,
    190, 65,  255, 189, 66,  255, 188, 67,  255, 187, 68,  255, 186, 69,  255,
    185, 70,  255, 184, 71,  255, 183, 72,  255, 182, 73,  255, 181, 74,  255,
    180, 75,  255, 179, 76,  255, 178, 77,  255, 177, 78,  255, 176, 79,  255,
    175, 80,  255, 174, 81,  255, 173, 82,  255, 172, 83,  255, 171, 84,  255,
    170, 85,  255, 169, 86,  255, 168, 87,  255, 167, 88,  255, 166, 89,  255,
    165, 90,  255, 164, 91,  255, 163, 92,  255, 162, 93,  255, 161, 94,  255,
    160, 95,  255, 159, 96,  255, 158, 97,  255, 157, 98,  255, 156, 99,  255,
    155, 100, 255, 154, 101, 255, 153, 102, 255, 152, 103, 255, 151, 104, 255,
    150, 105, 255, 149, 106, 255, 148, 107, 255, 147, 108, 255, 146, 109, 255,
    145, 110, 255, 144, 111, 255, 143, 112, 255, 142, 113, 255, 141, 114, 255,
    140, 115, 255, 139, 116, 255, 138, 117, 255, 137, 118, 255, 136, 119, 255,
    135, 120, 255, 134, 121, 255, 133, 122, 255, 132, 123, 255, 131, 124, 255,
    130, 125, 255, 129, 126, 255, 128, 127, 255, 127, 128, 255, 126, 129, 255,
    125, 130, 255, 124, 131, 255, 123, 132, 255, 122, 133, 255, 121, 134, 255,
    120, 135, 255, 119, 136, 255, 118, 137, 255, 117, 138, 255, 116, 139, 255,
    115, 140, 255, 114, 141, 255, 113, 142, 255, 112, 143, 255, 111, 144, 255,
    110, 145, 255, 109, 146, 255, 108, 147, 255, 107, 148, 255, 106, 149, 255,
    105, 150, 255, 104, 151, 255, 103, 152, 255, 102, 153, 255, 101, 154, 255,
    100, 155, 255, 99,  156, 255, 98,  157, 255, 97,  158, 255, 96,  159, 255,
    95,  160, 255, 94,  161, 255, 93,  162, 255, 92,  163, 255, 91,  164, 255,
    90,  165, 255, 89,  166, 255, 88,  167, 255, 87,  168, 255, 86,  169, 255,
    85,  170, 255, 84,  171, 255, 83,  172, 255, 82,  173, 255, 81,  174, 255,
    80,  175, 255, 79,  176, 255, 78,  177, 255, 77,  178, 255, 76,  179, 255,
    75,  180, 255, 74,  181, 255, 73,  182, 255, 72,  183, 255, 71,  184, 255,
    70,  185, 255, 69,  186, 255, 68,  187, 255, 67,  188, 255, 66,  189, 255,
    65,  190, 255, 64,  191, 255, 63,  192, 255, 62,  193, 255, 61,  194, 255,
    60,  195, 255, 59,  196, 255, 58,  197, 255, 57,  198, 255, 56,  199, 255,
    55,  200, 255, 54,  201, 255, 53,  202, 255, 52,  203, 255, 51,  204, 255,
    50,  205, 255, 49,  206, 255, 48,  207, 255, 47,  208, 255, 46,  209, 255,
    45,  210, 255, 44,  211, 255, 43,  212, 255, 42,  213, 255, 41,  214, 255,
    40,  215, 255, 39,  216, 255, 38,  217, 255, 37,  218, 255, 36,  219, 255,
    35,  220, 255, 34,  221, 255, 33,  222, 255, 32,  223, 255, 31,  224, 255,
    30,  225, 255, 29,  226, 255, 28,  227, 255, 27,  228, 255, 26,  229, 255,
    25,  230, 255, 24,  231, 255, 23,  232, 255, 22,  233, 255, 21,  234, 255,
    20,  235, 255, 19,  236, 255, 18,  237, 255, 17,  238, 255, 16,  239, 255,
    15,  240, 255, 14,  241, 255, 13,  242, 255, 12,  243, 255, 11,  244, 255,
    10,  245, 255, 9,   246, 255, 8,   247, 255, 7,   248, 255, 6,   249, 255,
    5,   250, 255, 4,   251, 255, 3,   252, 255, 2,   253, 255, 1,   254, 255,
    0,   255, 255,
};

constexpr unsigned char kSummerColormap[] = {
    102, 128, 0,   102, 128, 1,   102, 129, 2,   102, 129, 3,   102, 130, 4,
    102, 130, 5,   102, 131, 6,   102, 131, 7,   102, 132, 8,   102, 132, 9,
    102, 133, 10,  102, 133, 11,  102, 134, 12,  102, 134, 13,  102, 135, 14,
    102, 135, 15,  102, 136, 16,  102, 136, 17,  102, 137, 18,  102, 137, 19,
    102, 138, 20,  102, 138, 21,  102, 139, 22,  102, 139, 23,  102, 140, 24,
    102, 140, 25,  102, 141, 26,  102, 141, 27,  102, 142, 28,  102, 142, 29,
    102, 143, 30,  102, 143, 31,  102, 144, 32,  102, 144, 33,  102, 145, 34,
    102, 145, 35,  102, 146, 36,  102, 146, 37,  102, 147, 38,  102, 147, 39,
    102, 147, 40,  102, 148, 41,  102, 149, 42,  102, 149, 43,  102, 150, 44,
    102, 150, 45,  102, 151, 46,  102, 151, 47,  102, 152, 48,  102, 152, 49,
    102, 153, 50,  102, 153, 51,  102, 154, 52,  102, 154, 53,  102, 155, 54,
    102, 155, 55,  102, 156, 56,  102, 156, 57,  102, 157, 58,  102, 157, 59,
    102, 158, 60,  102, 158, 61,  102, 159, 62,  102, 159, 63,  102, 160, 64,
    102, 160, 65,  102, 161, 66,  102, 161, 67,  102, 162, 68,  102, 162, 69,
    102, 163, 70,  102, 163, 71,  102, 163, 72,  102, 164, 73,  102, 165, 74,
    102, 165, 75,  102, 166, 76,  102, 166, 77,  102, 167, 78,  102, 167, 79,
    102, 168, 80,  102, 168, 81,  102, 169, 82,  102, 169, 83,  102, 170, 84,
    102, 170, 85,  102, 171, 86,  102, 171, 87,  102, 172, 88,  102, 172, 89,
    102, 173, 90,  102, 173, 91,  102, 174, 92,  102, 174, 93,  102, 175, 94,
    102, 175, 95,  102, 176, 96,  102, 176, 97,  102, 177, 98,  102, 177, 99,
    102, 178, 100, 102, 178, 101, 102, 179, 102, 102, 179, 103, 102, 179, 104,
    102, 180, 105, 102, 181, 106, 102, 181, 107, 102, 182, 108, 102, 182, 109,
    102, 183, 110, 102, 183, 111, 102, 184, 112, 102, 184, 113, 102, 185, 114,
    102, 185, 115, 102, 186, 116, 102, 186, 117, 102, 187, 118, 102, 187, 119,
    102, 188, 120, 102, 188, 121, 102, 189, 122, 102, 189, 123, 102, 190, 124,
    102, 190, 125, 102, 191, 126, 102, 191, 127, 102, 192, 128, 102, 192, 129,
    102, 193, 130, 102, 193, 131, 102, 194, 132, 102, 194, 133, 102, 195, 134,
    102, 195, 135, 102, 195, 136, 102, 196, 137, 102, 196, 138, 102, 197, 139,
    102, 198, 140, 102, 198, 141, 102, 199, 142, 102, 199, 143, 102, 200, 144,
    102, 200, 145, 102, 201, 146, 102, 201, 147, 102, 202, 148, 102, 202, 149,
    102, 203, 150, 102, 203, 151, 102, 204, 152, 102, 204, 153, 102, 205, 154,
    102, 205, 155, 102, 206, 156, 102, 206, 157, 102, 207, 158, 102, 207, 159,
    102, 208, 160, 102, 208, 161, 102, 209, 162, 102, 209, 163, 102, 210, 164,
    102, 210, 165, 102, 211, 166, 102, 211, 167, 102, 211, 168, 102, 212, 169,
    102, 212, 170, 102, 213, 171, 102, 214, 172, 102, 214, 173, 102, 215, 174,
    102, 215, 175, 102, 216, 176, 102, 216, 177, 102, 217, 178, 102, 217, 179,
    102, 218, 180, 102, 218, 181, 102, 219, 182, 102, 219, 183, 102, 220, 184,
    102, 220, 185, 102, 221, 186, 102, 221, 187, 102, 222, 188, 102, 222, 189,
    102, 223, 190, 102, 223, 191, 102, 224, 192, 102, 224, 193, 102, 225, 194,
    102, 225, 195, 102, 226, 196, 102, 226, 197, 102, 227, 198, 102, 227, 199,
    102, 227, 200, 102, 228, 201, 102, 228, 202, 102, 229, 203, 102, 230, 204,
    102, 230, 205, 102, 231, 206, 102, 231, 207, 102, 232, 208, 102, 232, 209,
    102, 233, 210, 102, 233, 211, 102, 234, 212, 102, 234, 213, 102, 235, 214,
    102, 235, 215, 102, 236, 216, 102, 236, 217, 102, 237, 218, 102, 237, 219,
    102, 238, 220, 102, 238, 221, 102, 239, 222, 102, 239, 223, 102, 240, 224,
    102, 240, 225, 102, 241, 226, 102, 241, 227, 102, 242, 228, 102, 242, 229,
    102, 243, 230, 102, 243, 231, 102, 243, 232, 102, 244, 233, 102, 244, 234,
    102, 245, 235, 102, 246, 236, 102, 246, 237, 102, 247, 238, 102, 247, 239,
    102, 248, 240, 102, 248, 241, 102, 249, 242, 102, 249, 243, 102, 250, 244,
    102, 250, 245, 102, 251, 246, 102, 251, 247, 102, 252, 248, 102, 252, 249,
    102, 253, 250, 102, 253, 251, 102, 254, 252, 102, 254, 253, 102, 255, 254,
    102, 255, 255,
};

constexpr unsigned char kWinterColormap[] = {
    255, 0,   0, 255, 1,   0, 254, 2,   0, 254, 3,   0, 253, 4,   0,
    253, 5,   0, 252, 6,   0, 252, 7,   0, 251, 8,   0, 251, 9,   0,
    250, 10,  0, 250, 11,  0, 249, 12,  0, 249, 13,  0, 248, 14,  0,
    248, 15,  0, 247, 16,  0, 247, 17,  0, 246, 18,  0, 246, 19,  0,
    245, 20,  0, 245, 21,  0, 244, 22,  0, 244, 23,  0, 243, 24,  0,
    243, 25,  0, 242, 26,  0, 242, 27,  0, 241, 28,  0, 241, 29,  0,
    240, 30,  0, 240, 31,  0, 239, 32,  0, 239, 33,  0, 238, 34,  0,
    238, 35,  0, 237, 36,  0, 237, 37,  0, 236, 38,  0, 236, 39,  0,
    235, 40,  0, 235, 41,  0, 234, 42,  0, 234, 43,  0, 233, 44,  0,
    233, 45,  0, 232, 46,  0, 232, 47,  0, 231, 48,  0, 231, 49,  0,
    230, 50,  0, 230, 51,  0, 229, 52,  0, 229, 53,  0, 228, 54,  0,
    227, 55,  0, 227, 56,  0, 227, 57,  0, 226, 58,  0, 226, 59,  0,
    225, 60,  0, 225, 61,  0, 224, 62,  0, 224, 63,  0, 223, 64,  0,
    223, 65,  0, 222, 66,  0, 222, 67,  0, 221, 68,  0, 221, 69,  0,
    220, 70,  0, 220, 71,  0, 219, 72,  0, 219, 73,  0, 218, 74,  0,
    218, 75,  0, 217, 76,  0, 217, 77,  0, 216, 78,  0, 216, 79,  0,
    215, 80,  0, 215, 81,  0, 214, 82,  0, 214, 83,  0, 213, 84,  0,
    213, 85,  0, 212, 86,  0, 211, 87,  0, 211, 88,  0, 211, 89,  0,
    210, 90,  0, 210, 91,  0, 209, 92,  0, 209, 93,  0, 208, 94,  0,
    208, 95,  0, 207, 96,  0, 207, 97,  0, 206, 98,  0, 206, 99,  0,
    205, 100, 0, 205, 101, 0, 204, 102, 0, 204, 103, 0, 203, 104, 0,
    203, 105, 0, 202, 106, 0, 202, 107, 0, 201, 108, 0, 201, 109, 0,
    200, 110, 0, 200, 111, 0, 199, 112, 0, 199, 113, 0, 198, 114, 0,
    198, 115, 0, 197, 116, 0, 197, 117, 0, 196, 118, 0, 195, 119, 0,
    195, 120, 0, 195, 121, 0, 194, 122, 0, 194, 123, 0, 193, 124, 0,
    193, 125, 0, 192, 126, 0, 192, 127, 0, 191, 128, 0, 191, 129, 0,
    190, 130, 0, 190, 131, 0, 189, 132, 0, 189, 133, 0, 188, 134, 0,
    188, 135, 0, 187, 136, 0, 187, 137, 0, 186, 138, 0, 186, 139, 0,
    185, 140, 0, 185, 141, 0, 184, 142, 0, 184, 143, 0, 183, 144, 0,
    183, 145, 0, 182, 146, 0, 182, 147, 0, 181, 148, 0, 180, 149, 0,
    180, 150, 0, 179, 151, 0, 179, 152, 0, 179, 153, 0, 178, 154, 0,
    178, 155, 0, 177, 156, 0, 177, 157, 0, 176, 158, 0, 176, 159, 0,
    175, 160, 0, 175, 161, 0, 174, 162, 0, 174, 163, 0, 173, 164, 0,
    173, 165, 0, 172, 166, 0, 172, 167, 0, 171, 168, 0, 171, 169, 0,
    170, 170, 0, 170, 171, 0, 169, 172, 0, 169, 173, 0, 168, 174, 0,
    168, 175, 0, 167, 176, 0, 167, 177, 0, 166, 178, 0, 166, 179, 0,
    165, 180, 0, 164, 181, 0, 164, 182, 0, 163, 183, 0, 163, 184, 0,
    163, 185, 0, 162, 186, 0, 162, 187, 0, 161, 188, 0, 161, 189, 0,
    160, 190, 0, 160, 191, 0, 159, 192, 0, 159, 193, 0, 158, 194, 0,
    158, 195, 0, 157, 196, 0, 157, 197, 0, 156, 198, 0, 156, 199, 0,
    155, 200, 0, 155, 201, 0, 154, 202, 0, 154, 203, 0, 153, 204, 0,
    153, 205, 0, 152, 206, 0, 152, 207, 0, 151, 208, 0, 151, 209, 0,
    150, 210, 0, 150, 211, 0, 149, 212, 0, 148, 213, 0, 148, 214, 0,
    147, 215, 0, 147, 216, 0, 147, 217, 0, 146, 218, 0, 146, 219, 0,
    145, 220, 0, 145, 221, 0, 144, 222, 0, 144, 223, 0, 143, 224, 0,
    143, 225, 0, 142, 226, 0, 142, 227, 0, 141, 228, 0, 141, 229, 0,
    140, 230, 0, 140, 231, 0, 139, 232, 0, 139, 233, 0, 138, 234, 0,
    138, 235, 0, 137, 236, 0, 137, 237, 0, 136, 238, 0, 136, 239, 0,
    135, 240, 0, 135, 241, 0, 134, 242, 0, 134, 243, 0, 133, 244, 0,
    132, 245, 0, 132, 246, 0, 132, 247, 0, 131, 248, 0, 131, 249, 0,
    130, 250, 0, 130, 251, 0, 129, 252, 0, 129, 253, 0, 128, 254, 0,
    128, 255, 0,
};

constexpr unsigned char kParulaColormap[] = {
    169, 39,  62,  172, 39,  63,  175, 40,  63,  178, 41,  64,  181, 42,  64,
    184, 43,  65,  187, 44,  65,  189, 45,  65,  192, 46,  66,  195, 47,  66,
    198, 48,  67,  200, 49,  67,  203, 50,  68,  206, 51,  68,  209, 53,  69,
    211, 54,  69,  214, 55,  69,  216, 56,  70,  218, 57,  70,  220, 58,  70,
    223, 60,  70,  224, 61,  71,  226, 62,  71,  228, 64,  71,  230, 65,  71,
    231, 67,  71,  233, 68,  72,  234, 69,  72,  236, 71,  72,  237, 72,  72,
    238, 74,  72,  239, 75,  72,  241, 76,  72,  242, 78,  72,  243, 79,  72,
    244, 81,  72,  245, 82,  72,  246, 83,  72,  247, 85,  72,  248, 86,  72,
    248, 88,  72,  249, 89,  71,  250, 90,  71,  251, 92,  71,  251, 93,  71,
    252, 95,  71,  252, 96,  70,  253, 97,  70,  253, 99,  69,  254, 100, 69,
    254, 102, 68,  254, 103, 68,  255, 105, 67,  255, 106, 66,  255, 108, 65,
    255, 109, 64,  255, 111, 63,  255, 112, 62,  255, 114, 61,  255, 115, 59,
    255, 117, 58,  255, 118, 56,  255, 120, 54,  254, 121, 53,  254, 123, 51,
    253, 124, 50,  253, 126, 49,  252, 127, 48,  251, 129, 47,  251, 130, 47,
    250, 132, 47,  249, 133, 46,  249, 134, 46,  248, 136, 46,  247, 137, 46,
    246, 138, 45,  245, 140, 45,  244, 141, 45,  243, 142, 45,  241, 143, 45,
    240, 145, 44,  240, 146, 43,  239, 147, 42,  238, 149, 41,  237, 150, 40,
    236, 151, 39,  235, 152, 39,  234, 154, 38,  233, 155, 38,  232, 156, 37,
    232, 157, 37,  231, 158, 37,  230, 159, 36,  230, 161, 35,  229, 162, 35,
    229, 163, 34,  228, 164, 33,  228, 165, 32,  227, 166, 31,  226, 167, 30,
    225, 169, 29,  225, 170, 29,  224, 171, 28,  223, 172, 27,  222, 173, 26,
    220, 174, 25,  219, 175, 23,  218, 176, 22,  217, 177, 20,  215, 178, 18,
    214, 178, 16,  212, 179, 14,  211, 180, 11,  209, 181, 8,   208, 182, 6,
    206, 183, 4,   205, 183, 2,   203, 184, 1,   202, 185, 0,   200, 186, 0,
    198, 186, 0,   197, 187, 1,   195, 188, 2,   193, 188, 4,   192, 189, 6,
    190, 189, 9,   188, 190, 13,  187, 191, 16,  185, 191, 20,  183, 192, 23,
    181, 192, 26,  180, 193, 29,  178, 193, 32,  176, 194, 35,  174, 194, 37,
    173, 195, 39,  171, 195, 41,  169, 196, 43,  167, 197, 45,  165, 197, 46,
    163, 198, 47,  161, 198, 49,  159, 199, 50,  157, 199, 51,  155, 200, 53,
    153, 200, 54,  151, 201, 56,  149, 201, 57,  147, 202, 59,  144, 202, 61,
    142, 203, 64,  140, 203, 66,  137, 204, 69,  135, 204, 72,  132, 204, 75,
    130, 204, 78,  127, 205, 81,  125, 205, 85,  122, 205, 88,  120, 205, 91,
    117, 205, 94,  114, 205, 97,  111, 206, 100, 108, 206, 104, 106, 206, 107,
    103, 206, 111, 100, 205, 114, 97,  205, 118, 94,  205, 122, 92,  205, 125,
    89,  205, 129, 86,  204, 133, 84,  204, 136, 81,  204, 140, 78,  203, 144,
    75,  203, 147, 73,  203, 151, 70,  202, 154, 67,  202, 158, 65,  201, 161,
    62,  201, 165, 60,  200, 168, 57,  200, 172, 55,  199, 175, 53,  198, 178,
    51,  198, 182, 49,  197, 185, 47,  197, 188, 46,  196, 191, 44,  195, 194,
    42,  195, 197, 41,  194, 200, 40,  193, 203, 40,  193, 206, 39,  192, 209,
    39,  192, 212, 40,  191, 215, 40,  190, 217, 41,  190, 220, 41,  189, 223,
    42,  189, 225, 43,  188, 228, 45,  188, 230, 46,  187, 233, 48,  187, 235,
    51,  187, 237, 53,  187, 240, 55,  186, 242, 57,  186, 244, 59,  186, 246,
    61,  187, 248, 62,  187, 250, 63,  188, 252, 62,  189, 253, 61,  190, 255,
    60,  191, 255, 59,  192, 255, 58,  194, 255, 57,  195, 255, 56,  197, 255,
    55,  198, 255, 54,  200, 255, 53,  201, 255, 52,  203, 255, 51,  204, 254,
    50,  206, 254, 49,  207, 254, 48,  209, 253, 47,  210, 252, 47,  212, 252,
    46,  214, 251, 45,  215, 250, 44,  217, 250, 43,  218, 249, 43,  220, 248,
    42,  222, 248, 41,  223, 247, 40,  225, 247, 40,  226, 246, 39,  228, 246,
    39,  229, 246, 38,  231, 246, 37,  233, 246, 36,  234, 246, 35,  236, 246,
    34,  237, 246, 33,  239, 246, 32,  240, 247, 31,  242, 247, 30,  243, 247,
    29,  245, 248, 27,  246, 248, 26,  247, 249, 24,  249, 249, 22,  250, 250,
    21,  252, 250};
}  // namespace color

// define some color plan
const int kColorPlanOldCaffe[] = {
    0,   113, 188, 216, 82,  24,  236, 176, 31,  125, 46,  141, 118, 171, 47,
    76,  189, 237, 161, 19,  46,  76,  76,  76,  153, 153, 153, 255, 0,   0,
    255, 127, 0,   190, 190, 0,   0,   255, 0,   0,   0,   255, 170, 0,   255,
    84,  84,  0,   84,  170, 0,   84,  255, 0,   170, 84,  0,   170, 170, 0,
    170, 255, 0,   255, 84,  0,   255, 170, 0,   255, 255, 0,   0,   84,  127,
    0,   170, 127, 0,   255, 127, 84,  0,   127, 84,  84,  127, 84,  170, 127,
    84,  255, 127, 170, 0,   127, 170, 84,  127, 170, 170, 127, 170, 255, 127,
    255, 0,   127, 255, 84,  127, 255, 170, 127, 255, 255, 127, 0,   84,  255,
    0,   170, 255, 0,   255, 255, 84,  0,   255, 84,  84,  255, 84,  170, 255,
    84,  255, 255, 170, 0,   255, 170, 84,  255, 170, 170, 255, 170, 255, 255,
    255, 0,   255, 255, 84,  255, 255, 170, 255, 42,  0,   0,   84,  0,   0,
    127, 0,   0,   170, 0,   0,   212, 0,   0,   255, 0,   0,   0,   42,  0,
    0,   84,  0,   0,   127, 0,   0,   170, 0,   0,   212, 0,   0,   255, 0,
    0,   0,   42,  0,   0,   84,  0,   0,   127, 0,   0,   170, 0,   0,   212,
    0,   0,   255, 0,   0,   0,   36,  36,  36,  72,  72,  72,  109, 109, 109,
    145, 145, 145, 182, 182, 182, 218, 218, 218, 255, 255, 255,
};

}  // namespace vis
}  // namespace thor
