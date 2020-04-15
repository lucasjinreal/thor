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
        void renderHumanPose(std::vector<HumanPose> &poses, cv::Mat &image) {
            // drawing HumanPoses on image
            CV_Assert(image.type() == CV_8UC3);
            const std::vector<cv::Scalar> colors = {
                    cv::Scalar(255, 0, 0), cv::Scalar(255, 85, 0), cv::Scalar(255, 170, 0),
                    cv::Scalar(255, 255, 0), cv::Scalar(170, 255, 0), cv::Scalar(85, 255, 0),
                    cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 85), cv::Scalar(0, 255, 170),
                    cv::Scalar(0, 255, 255), cv::Scalar(0, 170, 255), cv::Scalar(0, 85, 255),
                    cv::Scalar(0, 0, 255), cv::Scalar(85, 0, 255), cv::Scalar(170, 0, 255),
                    cv::Scalar(255, 0, 255), cv::Scalar(255, 0, 170), cv::Scalar(255, 0, 85)};
            const std::vector<std::pair<int, int>> limbKeypointsIds = {
                    {1,  2},
                    {1,  5},
                    {2,  3},
                    {3,  4},
                    {5,  6},
                    {6,  7},
                    {1,  8},
                    {8,  9},
                    {9,  10},
                    {1,  11},
                    {11, 12},
                    {12, 13},
                    {1,  0},
                    {0,  14},
                    {14, 16},
                    {0,  15},
                    {15, 17}};

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
                    {1,  2},
                    {1,  5},
                    {2,  3},
                    {3,  4},
                    {5,  6},
                    {6,  7},
                    {1,  8},
                    {8,  9},
                    {9,  10},
                    {1,  11},
                    {11, 12},
                    {12, 13},
                    {1,  0},
                    {0,  14},
                    {14, 16},
                    {0,  15},
                    {15, 17}};
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
        cv::Mat VisualizeDetection(cv::Mat &img, vector<vector<float>> detections, vector<string> classes_names,
                                   bool enable_mask, float confidence_threshold, bool normalized) {
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
                    std::string label_text = classes_names[d[offset]] + " " + string(score_str);
                    int base_line = 0;
                    cv::Point text_origin = cv::Point(pt1.x - 2, pt1.y - 3);
                    cv::Size text_size = cv::getTextSize(label_text, font, font_scale, font_thickness, &base_line);
                    cv::rectangle(img, cv::Point(text_origin.x, text_origin.y + 5),
                                  cv::Point(text_origin.x + text_size.width, text_origin.y - text_size.height - 5),
                                  u_c, -1, 0);
                    cv::putText(img, label_text, text_origin, font, font_scale, cv::Scalar(0, 0, 0), font_thickness);
                }
            }
            cv::Mat combined;
            cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
            // maybe combine a mask img back later
            return combined;
        }

        cv::Mat
        VisualizeDetection(cv::Mat &img, vector<thor::Box> detections, vector<string> classes_names, bool enable_mask,
                           float confidence_threshold, bool normalized) {
            // for visualize
            const int font = cv::FONT_HERSHEY_TRIPLEX;
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
                    cv::Size text_size = cv::getTextSize(label_text, font, font_scale, font_thickness, &base_line);
                    cv::rectangle(img, cv::Point(text_origin.x, text_origin.y + 5),
                                  cv::Point(text_origin.x + text_size.width, text_origin.y - text_size.height - 5),
                                  u_c, -1, 0);
                    cv::putText(img, label_text, text_origin, font, font_scale, cv::Scalar(0, 0, 0), font_thickness);
                }
            }
            cv::Mat combined;
            cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
            // maybe combine a mask img back later
            return combined;
        }

        cv::Mat
        VisualizeDetectionStyleDetectron2(cv::Mat &img, vector<thor::Box> detections, vector<string> classes_names,
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
                    cv::Size text_size = cv::getTextSize(label_text, font, font_scale, font_thickness, &base_line);
                    cv::rectangle(img, cv::Point(text_origin.x, text_origin.y),
                                  cv::Point(text_origin.x + text_size.width, text_origin.y - text_size.height),
                                  cv::Scalar(0, 0, 0), -1, 0);
                    cv::putText(img, label_text, text_origin, font, font_scale, cv::Scalar(255, 255, 255),
                                font_thickness);
                }
            }
            cv::Mat combined;
            cv::addWeighted(img, 0.8, mask, 0.6, 0.6, combined);
            // maybe combine a mask img back later
            return combined;
        };

        cv::Mat
        VisualizeDetections(cv::Mat &img, vector<thor::Detection> detections, const vector<string> classes_names,
                            const vector<cv::Scalar> *colors, const float line_thickness, const float font_scale,
                            const bool fancy,
                            const float confidence_threshold, const bool enable_mask, const bool normalized) {
            const int font = cv::FONT_HERSHEY_DUPLEX;
            const int font_thickness = 1;
            cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
            for (int i = 0; i < detections.size(); ++i) {
                thor::Detection det = detections[i];
                const float score = det.prob;
                if (score >= confidence_threshold) {
                    cv::Point pt1, pt2;
                    pt1.x = det.bbox.x1;
                    pt1.y = det.bbox.y1;
                    pt2.x = det.bbox.x2;
                    pt2.y = det.bbox.y2;

                    cv::Scalar u_c;
                    if (colors != NULL) {
                        u_c = (*colors)[det.classId];
                    } else {
                        u_c = thor::vis::gen_unique_color_cv(det.classId);
                    }

//                    if (enable_mask) {
//                        cv::rectangle(mask, pt1, pt2, u_c, cv::FILLED, 0);
//                    }

                    if (fancy) {
                        cv::rectangle(mask, pt1, pt2, u_c, line_thickness, cv::LINE_AA, 0);
                                                cv::rectangle(img, pt1, pt2, u_c, line_thickness, cv::LINE_AA, 0);
                    } else {
                        cv::rectangle(img, pt1, pt2, u_c, line_thickness, cv::LINE_AA, 0);
                    }

                    char score_str[256];
                    sprintf(score_str, "%.1f", score);
                    std::string label_text = classes_names[det.classId] + ":" + string(score_str);
                    int base_line = 0;
                    cv::Point text_origin = cv::Point(pt1.x, pt1.y - 2);
                    cv::Size text_size = cv::getTextSize(label_text, font, font_scale, font_thickness, &base_line);
                    if (fancy) {
                        cv::rectangle(mask, cv::Point(text_origin.x - 2, text_origin.y - text_size.height - 2),
                                      cv::Point(text_origin.x + text_size.width + 2, text_origin.y + 2),
                                      u_c, cv::FILLED, 0);
                        cv::rectangle(img, cv::Point(text_origin.x - 2, text_origin.y - text_size.height - 2),
                                      cv::Point(text_origin.x + text_size.width + 2, text_origin.y + 2),
                                      u_c, 1, cv::LINE_AA, 0);
                    } else {
                        cv::rectangle(img, cv::Point(text_origin.x - 2, text_origin.y - text_size.height - 2),
                                      cv::Point(text_origin.x + text_size.width + 2, text_origin.y + 2),
                                      cv::Scalar(0, 0, 0), -1, 0);
                    }
                    cv::putText(img, label_text, text_origin, font, font_scale, cv::Scalar(255, 255, 255),
                                font_thickness);
                }
            }
            cv::Mat combined;
            cv::addWeighted(img, 0.7, mask, 0.5, 0.6, combined);
            return combined;
        }

#endif

    } // namespace vis
} // namespace thor
