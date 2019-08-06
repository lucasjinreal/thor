//
// Created by jintian on 19-4-30.
//

#ifndef THOR_INCLUDE_GEOMETRY_H_
#define THOR_INCLUDE_GEOMETRY_H_


#ifdef USE_EIGEN
#include "eigen3/Eigen/Core"
#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"

namespace thor {

namespace geometry {

Eigen::Matrix3d Quaternion2RotationMatrix(const double x, const double y,
                                          const double z, const double w);

Eigen::Quaterniond EulerAngle2Quaternion(const double yaw, const double pitch,
                                         const double roll);

}
}
#endif


#endif //THOR_INCLUDE_GEOMETRY_H_
