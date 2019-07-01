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
  double r, g, b;
};

struct RGBA {
  int r, g, b, a;
};

// adding default values for better calling
thor::vis::RGBA gen_unique_color(int idx, bool is_track=false, double hue_step = 0.0125,
                                 float alpha = 0.7);

// tracking color is slightly different, they are all close
cv::Scalar gen_unique_color_cv(int idx, bool is_track=false, double hue_step = 0.0125,
                               float alpha = 0.7);

void hsv2rgb(float &r, float &g, float &b, int h, float s, double v);

cv::Mat createAlpha(cv::Mat &src);
int addAlpha(cv::Mat &src, cv::Mat &dst, cv::Mat &alpha);


// adding render HumanPose on image
void renderHumanPose(std::vector<HumanPose>& poses, cv::Mat& image);
void renderHumanPoseSimple(std::vector<HumanPose>& poses, cv::Mat& image);

}  // namespace vis
}  // namespace thor

#endif  // CAO_VIS_H
