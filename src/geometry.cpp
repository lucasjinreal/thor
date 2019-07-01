//
// Created by jintian on 19-4-30.
//

#include "include/geometry.h"


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