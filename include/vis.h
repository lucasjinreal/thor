//
// Created by jintian on 18-1-12.
//

#ifndef CAO_VIS_H
#define CAO_VIS_H

/**
 *  Vision module in cao
 *  the module provide some utility methods to draw bboxes
 */
#include <iostream>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "cmath"
#include "structures.h"


using namespace cv;

namespace thor {
namespace vis {

struct RGB {
  float r, g, b;
};

struct RGBA {
  float r, g, b, a;

  inline void print() {
    std::cout << "r:" << r << " g:" << g << " b:" << b << " a:" << a << endl;
  }
};

// adding default values for better calling
thor::vis::RGBA gen_unique_color(int idx, bool is_track=false, double hue_step = 0.41,
                                 float alpha = 0.7);

// tracking color is slightly different, they are all close
cv::Scalar gen_unique_color_cv(int idx, bool is_track=false, double hue_step = 0.41,
                               float alpha = 0.7);

void hsv2rgb(float &r, float &g, float &b, float h, float s, float v);
void hsv2rgb(thor::vis::RGBA &rgba, float h, float s, float v);
cv::Mat createAlpha(cv::Mat &src);
int addAlpha(cv::Mat &src, cv::Mat &dst, cv::Mat &alpha);


// adding render HumanPose on image
void renderHumanPose(std::vector<HumanPose>& poses, cv::Mat& image);
void renderHumanPoseSimple(std::vector<HumanPose>& poses, cv::Mat& image);

}  // namespace vis
}  // namespace thor

#endif  // CAO_VIS_H
