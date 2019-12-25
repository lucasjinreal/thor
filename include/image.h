//
// Created by fagangjin on 11/12/2019.
//

#ifndef THOR_INCLUDE_IMAGE_H_
#define THOR_INCLUDE_IMAGE_H_


/**
 *
 * We are going to add image
 * related code
 */

#include <opencv2/core/mat.hpp>



namespace thor{
namespace image {

typedef struct {
  int w;
  int h;
  int c;
  float *data;
} Image;

float* Normalize(cv::Mat img);

/**
 * this function using for convert cv::Mat default HWC data to CHW while doing
 *
 *    cv::Mat resizedImage = cv::Mat::zeros(INPUT_H, INPUT_W, CV_32FC3);
      cv::resize(frame, resizedImage, cv::Size(INPUT_W, INPUT_H));
      data = HWC2CHW(resizedImage, kMeans);
 *
 * @param img
 * @param kMeans
 * @param kStds
 * @return
 */
float* HWC2CHW(cv::Mat img, const float kMeans[3], const float kStds[3]);


/**
 * Some resize function will be added,
 * such as resizeAlongShortest etc.
 *
 */

}
}


#endif //THOR_INCLUDE_IMAGE_H_
