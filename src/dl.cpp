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

#include "include/dl.h"

namespace thor {
namespace dl {

bool ClassAgnosticNonMaximumSuppression(const std::vector<Detection2D> &input,
                                        const double nms_threshold,
                                        std::vector<Detection2D> *output) {
  if (output == nullptr || input.size() == 0) {
    return false;
  }

  std::vector<bool> mask;
  if (!ClassAgnosticNonMaximumSuppression(input, nms_threshold, &mask)) {
    return false;
  }

  output->reserve(input.size());
  for (const auto i : thor::generic::IndexesOf(mask)) {
    if (mask[i]) {
      output->push_back(input[i]);
    }
  }

  return true;
}

bool ClassAgnosticNonMaximumSuppression(
    const std::vector<Detection2D> &candidate_detections,
    const double nms_threshold, std::vector<bool> *mask) {
  if (mask == nullptr || candidate_detections.size() == 0) {
    return false;
  }
  mask->resize(candidate_detections.size(), false);

  // sort in descending order
  std::vector<int> indices(candidate_detections.size());
  std::iota(indices.begin(), indices.end(), 0);
  std::sort(indices.begin(), indices.end(),
            [&candidate_detections](size_t index_1, size_t index_2) {
              return candidate_detections[index_1].prob() >
                     candidate_detections[index_2].prob();
            });

  std::vector<Detection2D> valid_detections;

  // start from the highest scoring
  for (const int index : indices) {
    // see if the current bbox overlaps with any previous selected boxes
    const auto &candidate_bbox = candidate_detections[index];

    bool candidate_valid = true;
    for (const auto &selected_bbox : valid_detections) {
      // compute overlap area
      double upper_left_x = std::max(candidate_bbox.x(), selected_bbox.x());
      double upper_left_y = std::max(candidate_bbox.y(), selected_bbox.y());
      double lower_right_x = std::min(candidate_bbox.x() + candidate_bbox.w(),
                                      selected_bbox.x() + selected_bbox.w());
      double lower_right_y = std::min(candidate_bbox.y() + candidate_bbox.h(),
                                      selected_bbox.y() + selected_bbox.h());

      double overlap_width = std::max(0.0, lower_right_x - upper_left_x + 1.0);
      double overlap_height = std::max(0.0, lower_right_y - upper_left_y + 1.0);

      double overlap_ratio = overlap_width * overlap_height /
                             (selected_bbox.w() * selected_bbox.h());

      if (overlap_ratio > nms_threshold) {
        candidate_valid = false;
      }
    }

    if (candidate_valid) {
      mask->at(index) = true;
      valid_detections.push_back(candidate_bbox);
    }
  }

  return true;
}

}  // namespace dl
}  // namespace thor
