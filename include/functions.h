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
// Created by fagangjin on 27/12/2019.
//

#ifndef THOR_INCLUDE_FUNCTIONS_H_
#define THOR_INCLUDE_FUNCTIONS_H_

// implement functions in thor
// functions include:
// softmax
// ...

#include <cmath>
#include <algorithm>

namespace thor{
namespace functions{


// how to design softmax in C++?

template<typename _Tp>
int softmax_1d(const _Tp* src, _Tp* dst, int length)
{
  const _Tp alpha = *std::max_element(src, src + length);
  _Tp denominator{ 0 };

  for (int i = 0; i < length; ++i) {
    dst[i] = std::exp(src[i] - alpha);
    denominator += dst[i];
  }

  for (int i = 0; i < length; ++i) {
    dst[i] /= denominator;
  }

  return 0;
}



}
}








#endif //THOR_INCLUDE_FUNCTIONS_H_
