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
