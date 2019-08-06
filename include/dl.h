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

namespace thor {
namespace dl {

// add several CLASSES here
vector<string> VOC_CLASSES = {"__background__",
							  "aeroplane", "bicycle", "bird", "boat",
							  "bottle", "bus", "car", "cat", "chair",
							  "cow", "diningtable", "dog", "horse",
							  "motorbike", "person", "pottedplant",
							  "sheep", "sofa", "train", "tvmonitor"};
vector<string> COCO_CLASSES = {
	"__background__", "person", "bicycle", "car", "motorcycle", "airplane",
	"bus", "train", "truck", "boat", "traffic light", "fire hydrant",
	"stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse",
	"sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
	"umbrella", "handbag", "tie", "suitcase", "frisbee", "skis",
	"snowboard", "sports ball", "kite", "baseball bat", "baseball glove",
	"skateboard", "surfboard", "tennis racket", "bottle", "wine glass",
	"cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich",
	"orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
	"chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv",
	"laptop", "mouse", "remote", "keyboard", "cell phone", "microwave",
	"oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
	"scissors", "teddy bear", "hair drier", "toothbrush"
};



}
}

#endif //CAO_CV_H
