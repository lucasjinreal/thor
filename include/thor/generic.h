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
// Created by jintian on 19-5-23.
//

#ifndef THOR_INCLUDE_GENERIC_H_
#define THOR_INCLUDE_GENERIC_H_
#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "assert.h"

/**
 *
 *
 * Contains simple enough data structures
 * the most basic and atomic structure
 *
 */

using std::vector;
using std::to_string;

namespace thor {
namespace generic {

template <typename IntType>
class ForwardIndexValue {
 public:
  explicit ForwardIndexValue(IntType i) : value_(i) {}

  IntType operator*() const { return value_; }

  bool operator!=(ForwardIndexValue rhs) { return value_ != rhs.value_; }

  ForwardIndexValue &operator++() {
    ++value_;
    return *this;
  }

 protected:
  IntType value_;
};

template <typename IntType, template <typename> class IndexValueType>
class IndexSequence {
 public:
  IndexSequence(IntType b, IntType e) : begin_(b), end_(e) {}

  IndexValueType<IntType> begin() const { return begin_; }
  IndexValueType<IntType> end() const { return end_; }

 protected:
  IndexValueType<IntType> begin_;
  IndexValueType<IntType> end_;
};

template <typename Container>
auto IndexesOf(
    Container &&container) {
  using IntType = decltype(container.size());
  return IndexSequence<IntType, ForwardIndexValue>(0, container.size());
}

constexpr double kMathEpsilon = 1e-10;

class Vector2d {
 public:
  //! Constructor which takes x- and y-coordinates.
  constexpr Vector2d(const double x, const double y) noexcept : x_(x), y_(y) {}
  //! Constructor returning the zero vector.
  constexpr Vector2d() noexcept : Vector2d(0, 0) {}
  //! Creates a unit-vector with a given angle to the positive x semi-axis
  static Vector2d CreateUnitVec2d(const double angle);

  double x() const { return x_; }
  double y() const { return y_; }

  void set_x(const double x) { x_ = x; }
  void set_y(const double y) { y_ = y; }

  double Length() const;
  double LengthSquare() const;
  double Angle() const;
  void Normalize();
  double DistanceTo(const Vector2d &other) const;
  double DistanceSquareTo(const Vector2d &other) const;
  //! Returns the "cross" product between these two Vec2d (non-standard).
  double CrossProd(const Vector2d &other) const;
  //! Returns the inner product between these two Vec2d.
  double InnerProd(const Vector2d &other) const;
  //! rotate the vector by angle.
  Vector2d rotate(const double angle) const;
  //! Sums two Vec2d
  Vector2d operator+(const Vector2d &other) const;
  //! Subtracts two Vec2d
  Vector2d operator-(const Vector2d &other) const;
  //! Multiplies Vec2d by a scalar
  Vector2d operator*(const double ratio) const;
  //! Divides Vec2d by a scalar
  Vector2d operator/(const double ratio) const;
  //! Sums another Vec2d to the current one
  Vector2d &operator+=(const Vector2d &other);
  //! Subtracts another Vec2d to the current one
  Vector2d &operator-=(const Vector2d &other);
  //! Multiplies this Vec2d by a scalar
  Vector2d &operator*=(const double ratio);
  //! Divides this Vec2d by a scalar
  Vector2d &operator/=(const double ratio);
  //! Compares two Vec2d
  bool operator==(const Vector2d &other) const;
  //! Returns a human-readable string representing this object
  std::string DebugString() const;

 protected:
  double x_ = 0.0;
  double y_ = 0.0;
};

//! Multiplies the given Vec2d by a given scalar
Vector2d operator*(const double ratio, const Vector2d &vec);

}  // namespace generic
}  // namespace thor

#endif  // THOR_INCLUDE_GENERIC_H_
