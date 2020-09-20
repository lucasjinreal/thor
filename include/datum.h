#ifndef THOR_DATUM_H_
#define THOR_DATUM_H_

/**
 *
 *
 *
 *
 * We need define some datum inside our usage
 * so that we can using it without other dependencies
 * define a Vec2d is sometimes useful
 *
 * */


#ifdef USE_OPENCV
#ifdef USE_EIGEN


#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cmath>

#include "./math.h"
#include "./str_util.h"
#include "./generic.h"


namespace thor {
namespace datum {

using namespace thor::generic;
using namespace thor::log;


class LineSegment2d {
 public:
  LineSegment2d();
  LineSegment2d(const Vector2d &start, const Vector2d &end);

  const Vector2d &start() const { return start_; }
  const Vector2d &end() const { return end_; }
  const Vector2d &unit_direction() const { return unit_direction_; }
  Vector2d center() const { return (start_ + end_) / 2.0; }
  double heading() const { return heading_; }
  double cos_heading() const { return unit_direction_.x(); }
  double sin_heading() const { return unit_direction_.y(); }


  double length() const;
  double length_sqr() const;
  double DistanceTo(const Vector2d &point) const;
  double DistanceTo(const Vector2d &point, Vector2d *const nearest_pt) const;
  double DistanceSquareTo(const Vector2d &point) const;
  double DistanceSquareTo(const Vector2d &point, Vector2d *const nearest_pt) const;
  bool IsPointIn(const Vector2d &point) const;
  bool HasIntersect(const LineSegment2d &other_segment) const;
  bool GetIntersect(const LineSegment2d &other_segment,
                    Vector2d *const point) const;
  double ProjectOntoUnit(const Vector2d &point) const;
  double ProductOntoUnit(const Vector2d &point) const;
  double GetPerpendicularFoot(const Vector2d &point,
                              Vector2d *const foot_point) const;
  std::string DebugString() const;

 private:
  Vector2d start_;
  Vector2d end_;
  Vector2d unit_direction_;
  double heading_ = 0.0;
  double length_ = 0.0;
};


/**
 *
 * Simply axis-aligned box2d
 */
class AABox2d {
 public:
  AABox2d() = default;
  AABox2d(const Vector2d &center, const double length, const double width);
  AABox2d(const Vector2d &one_corner, const Vector2d &opposite_corner);

  explicit AABox2d(const std::vector<Vector2d> &points);

  const Vector2d &center() const { return center_; }
  double center_x() const { return center_.x(); }
  double center_y() const { return center_.y(); }
  double length() const { return length_; }
  double width() const { return width_; }
  double half_length() const { return half_length_; }
  double half_width() const { return half_width_; }
  double area() const { return length_ * width_; }
  double min_x() const { return center_.x() - half_length_; }
  double max_x() const { return center_.x() + half_length_; }
  double min_y() const { return center_.y() - half_width_; }
  double max_y() const { return center_.y() + half_width_; }

  void GetAllCorners(std::vector<Vector2d> *const corners) const;
  bool IsPointIn(const Vector2d &point) const;
  bool IsPointOnBoundary(const Vector2d &point) const;
  double DistanceTo(const Vector2d &point) const;
  double DistanceTo(const AABox2d &box) const;
  bool HasOverlap(const AABox2d &box) const;
  void Shift(const Vector2d &shift_vec);
  void MergeFrom(const AABox2d &other_box);
  void MergeFrom(const Vector2d &other_point);
  std::string DebugString() const;

 private:
  Vector2d center_;
  double length_ = 0.0;
  double width_ = 0.0;
  double half_length_ = 0.0;
  double half_width_ = 0.0;
};



/***
 *
 * this is a box2d definition with rotation (not single box)
 */
class Box2d {
 public:
  Box2d() = default;
  Box2d(const Vector2d &center, const double heading, const double length,
        const double width);
  Box2d(const LineSegment2d &axis, const double width);
  explicit Box2d(const AABox2d &aabox);
  static Box2d CreateAABox(const Vector2d &one_corner,
                           const Vector2d &opposite_corner);

  const Vector2d &center() const { return center_; }
  double center_x() const { return center_.x(); }
  double center_y() const { return center_.y(); }
  double length() const { return length_; }
  double width() const { return width_; }
  double half_length() const { return half_length_; }
  double half_width() const { return half_width_; }
  double heading() const { return heading_; }
  double cos_heading() const { return cos_heading_; }
  double sin_heading() const { return sin_heading_; }
  double area() const { return length_ * width_; }
  double diagonal() const { return std::hypot(length_, width_); }
  void GetAllCorners(std::vector<Vector2d> *const corners) const;
  std::vector<Vector2d> GetAllCorners() const;
  bool IsPointIn(const Vector2d &point) const;
  bool IsPointOnBoundary(const Vector2d &point) const;
  double DistanceTo(const Vector2d &point) const;
  double DistanceTo(const LineSegment2d &line_segment) const;
  double DistanceTo(const Box2d &box) const;
  bool HasOverlap(const LineSegment2d &line_segment) const;
  bool HasOverlap(const Box2d &box) const;
  AABox2d GetAABox() const;
  void RotateFromCenter(const double rotate_angle);
  void Shift(const Vector2d &shift_vec);
  void LongitudinalExtend(const double extension_length);
  void LateralExtend(const double extension_length);

  std::string DebugString() const;
  void InitCorners();

  double max_x() const { return max_x_; }
  double min_x() const { return min_x_; }
  double max_y() const { return max_y_; }
  double min_y() const { return min_y_; }

 private:
  Vector2d center_;
  double length_ = 0.0;
  double width_ = 0.0;
  double half_length_ = 0.0;
  double half_width_ = 0.0;
  double heading_ = 0.0;
  double cos_heading_ = 1.0;
  double sin_heading_ = 0.0;
  std::vector<Vector2d> corners_;
  double max_x_ = std::numeric_limits<double>::min();
  double min_x_ = std::numeric_limits<double>::max();
  double max_y_ = std::numeric_limits<double>::min();
  double min_y_ = std::numeric_limits<double>::max();
};



///////////////////// Polygon2D ///////////////////////////////
class Polygon2d {
 public:
  Polygon2d() = default;
  explicit Polygon2d(const Box2d &box);
  explicit Polygon2d(std::vector<Vector2d> points);
  const std::vector<Vector2d> &points() const { return points_; }
  const std::vector<LineSegment2d> &line_segments() const {
    return line_segments_;
  }

  int num_points() const { return num_points_; }
  bool is_convex() const { return is_convex_; }
  double area() const { return area_; }
  double DistanceToBoundary(const Vector2d &point) const;
  double DistanceTo(const Vector2d &point) const;
  double DistanceTo(const LineSegment2d &line_segment) const;
  double DistanceTo(const Box2d &box) const;
  double DistanceTo(const Polygon2d &polygon) const;
  double DistanceSquareTo(const Vector2d &point) const;
  bool IsPointIn(const Vector2d &point) const;
  bool IsPointOnBoundary(const Vector2d &point) const;
  bool Contains(const LineSegment2d &line_segment) const;
  bool Contains(const Polygon2d &polygon) const;
  static bool ComputeConvexHull(const std::vector<Vector2d> &points,
                                Polygon2d *const polygon);
  bool HasOverlap(const LineSegment2d &line_segment) const;
  bool GetOverlap(const LineSegment2d &line_segment, Vector2d *const first,
                  Vector2d *const last) const;
  void GetAllVertices(std::vector<Vector2d> *const vertices) const;
  std::vector<Vector2d> GetAllVertices() const;
  std::vector<LineSegment2d> GetAllOverlaps(
      const LineSegment2d &line_segment) const;
  bool HasOverlap(const Polygon2d &polygon) const;
  bool ComputeOverlap(const Polygon2d &other_polygon,
                      Polygon2d *const overlap_polygon) const;

  AABox2d AABoundingBox() const;
  Box2d BoundingBoxWithHeading(const double heading) const;
  Box2d MinAreaBoundingBox() const;
  void ExtremePoints(const double heading, Vector2d *const first,
                     Vector2d *const last) const;
  Polygon2d ExpandByDistance(const double distance) const;
  std::string DebugString() const;

  double min_x() const { return min_x_; }
  double max_x() const { return max_x_; }
  double min_y() const { return min_y_; }
  double max_y() const { return max_y_; }

 protected:
  void BuildFromPoints();
  int Next(int at) const;
  int Prev(int at) const;
  static bool ClipConvexHull(const LineSegment2d &line_segment,
                             std::vector<Vector2d> *const points);

  std::vector<Vector2d> points_;
  int num_points_ = 0;
  std::vector<LineSegment2d> line_segments_;
  bool is_convex_ = false;
  double area_ = 0.0;
  double min_x_ = 0.0;
  double max_x_ = 0.0;
  double min_y_ = 0.0;
  double max_y_ = 0.0;
};


}  // namespace datum
}  // namespace thor

#endif
#endif



#endif //THOR_SRC_CV_EX_H_