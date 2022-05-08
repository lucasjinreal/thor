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
// Created by jintian on 19-4-30.
//

#ifdef USE_EIGEN
#include "thor/geometry.h"


Eigen::Matrix3d thor::geometry::Quaternion2RotationMatrix(const double x,
                                                          const double y,
                                                          const double z,
                                                          const double w) {
  Eigen::Quaterniond q;
  q.x() = x;
  q.y() = y;
  q.z() = z;
  q.w() = w;
  Eigen::Matrix3d R = q.normalized().toRotationMatrix();
  return R;
}


Eigen::Quaterniond thor::geometry::EulerAngle2Quaternion(const double yaw, const double pitch, const double roll) {
  // convert yaw, pitch, roll to quaternion
  Eigen::Vector3d ea0(yaw, pitch, roll);
  Eigen::Matrix3d R;
  R = ::Eigen::AngleAxisd(ea0[0], ::Eigen::Vector3d::UnitZ()) *
      ::Eigen::AngleAxisd(ea0[1], ::Eigen::Vector3d::UnitY()) *
      ::Eigen::AngleAxisd(ea0[2], ::Eigen::Vector3d::UnitX());
  // RotationMatrix to Quaterniond
  Eigen::Quaterniond q;
  q = R;
  return q;
}
#endif