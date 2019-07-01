//
// Created by jintian on 18-1-12.
//

#ifndef CAO_CV_H
#define CAO_CV_H

/**
 *  Several Compute Vision Algorithm
 *
 *  we using cao::cv:: do many things such as:
 *  nms
 *  crf
 */

#include "structures.h"
#include "vector"
#include "iostream"

using namespace std;


namespace thor{

    namespace dl{

        class BaseDetector{
        public:
            BaseDetector();
            void preProcess(cv::Mat &img);
            void detect();
        };
    }
}

#endif //CAO_CV_H
