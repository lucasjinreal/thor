//
// Created by jintian on 19-5-23.
//


#include "include/generic.h"


namespace thor{
namespace generic{


////////////////// Vec2d ////////////////////////
Vector2d Vector2d::CreateUnitVec2d(const double angle) {
  return Vector2d(cos(angle), sin(angle));
}

double Vector2d::Length() const { return std::hypot(x_, y_); }
double Vector2d::LengthSquare() const { return x_ * x_ + y_ * y_; }
double Vector2d::Angle() const { return std::atan2(y_, x_); }
void Vector2d::Normalize() {
  const double l = Length();
  if (l > kMathEpsilon) {
    x_ /= l;
    y_ /= l;
  }
}

double Vector2d::DistanceTo(const Vector2d &other) const {
  return std::hypot(x_ - other.x_, y_ - other.y_);
}

double Vector2d::DistanceSquareTo(const Vector2d &other) const {
  const double dx = x_ - other.x_;
  const double dy = y_ - other.y_;
  return dx * dx + dy * dy;
}

double Vector2d::CrossProd(const Vector2d &other) const {
  return x_ * other.y() - y_ * other.x();
}

double Vector2d::InnerProd(const Vector2d &other) const {
  return x_ * other.x() + y_ * other.y();
}

Vector2d Vector2d::rotate(const double angle) const {
  return Vector2d(x_ * cos(angle) - y_ * sin(angle),
               x_ * sin(angle) + y_ * cos(angle));
}

Vector2d Vector2d::operator+(const Vector2d &other) const {
  return Vector2d(x_ + other.x(), y_ + other.y());
}

Vector2d Vector2d::operator-(const Vector2d &other) const {
  return Vector2d(x_ - other.x(), y_ - other.y());
}

Vector2d Vector2d::operator*(const double ratio) const {
  return Vector2d(x_ * ratio, y_ * ratio);
}

Vector2d Vector2d::operator/(const double ratio) const {
  return Vector2d(x_ / ratio, y_ / ratio);
}

Vector2d &Vector2d::operator+=(const Vector2d &other) {
  x_ += other.x();
  y_ += other.y();
  return *this;
}

Vector2d &Vector2d::operator-=(const Vector2d &other) {
  x_ -= other.x();
  y_ -= other.y();
  return *this;
}

Vector2d &Vector2d::operator*=(const double ratio) {
  x_ *= ratio;
  y_ *= ratio;
  return *this;
}

Vector2d &Vector2d::operator/=(const double ratio) {
  x_ /= ratio;
  y_ /= ratio;
  return *this;
}

bool Vector2d::operator==(const Vector2d &other) const {
  return (std::abs(x_ - other.x()) < kMathEpsilon &&
      std::abs(y_ - other.y()) < kMathEpsilon);
}

Vector2d operator*(const double ratio, const Vector2d &vec) { return vec * ratio; }

std::string Vector2d::DebugString() const {
  return "vec2d ( x = " + to_string(x_) + "  y = " + to_string(y_) + " )";
}

}
}
