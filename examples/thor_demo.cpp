//
// Created by JinTian on 02/01/2018.
//

/**
 * A simple example of using Thor library
 */

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "thor/app_license.h"
#include "thor/colors.h"
#include "thor/cv_ex.h"
#include "thor/datum.h"
#include "thor/logging.h"
#include "thor/math.h"
#include "thor/os.h"
#include "thor/slam.h"
#include "thor/str_util.h"
#include "thor/vis.h"

using namespace std;
using namespace thor;

using namespace thor::str_util;
using namespace thor::datum;
using namespace thor::generic;

static int ToWchar(char *&src, wchar_t *&dest,
                   const char *locale = "zh_CN.utf8") {
  if (src == NULL) {
    dest = NULL;
    return 0;
  }
  setlocale(LC_CTYPE, locale);
  int w_size = mbstowcs(NULL, src, 0) + 1;
  if (w_size == 0) {
    dest = NULL;
    return -1;
  }

  dest = new wchar_t[w_size];
  if (!dest) {
    return -1;
  }
  int ret = mbstowcs(dest, src, strlen(src) + 1);
  if (ret <= 0) {
    return -1;
  }
  return 0;
}

int main(int argc, char **argv) {
  cout << "Welcome!\n";
  cout << colors::bold << colors::yellow << "thor lib\n" << colors::reset;

  if (thor::os::isdir("../")) {
    cout << colors::green << "and this is a " << colors::bold << colors::yellow
         << "dir.\n";
    vector<string> allFiles;
    allFiles = thor::os::list_files("../", true);
    cout << "list all files under it:\n";
    for (auto f : allFiles) {
      cout << f << endl;
    }
  }
  cout << colors::reset;

  string p = "/media/netac/a.mp4";
  string suffix = thor::os::suffix(p);
  cout << "suffix: " << suffix << endl;

  thor::Log(kInfo, "this a log test.");

  vector<float> a = {4.5, 6.5, 6, 7, 9};
  vector<float> b = {4.1, 3.5, 4, 7, 9};

  float res = thor::math::euclidean(a, b);
  cout << "euclidean res: " << to_string(res) << endl;

  cv::Mat img = cv::imread("../1.png");
  if (!img.data || img.channels() != 3) {
    fprintf(stderr, "read image fail\n");
    return -1;
  }

  thor::CvxText cvText("../FZSSJW.TTF");  //指定字体
  cv::Scalar size1{38, 0.5, 0.1, 0};  // (字体大小, 无效的, 字符间距, 无效的 }
  cvText.setFont(nullptr, &size1, nullptr, 0);
  char *str = (char *)"你好，世界!";
  wchar_t *w_str;
  ToWchar(str, w_str);
  cvText.putText(img, w_str, cv::Point(50, 100), cv::Scalar(255, 0, 255));

  // add some str util
  string s = "straight docker";
  if (thor::str_util::startswith(s, "st")) {
    cout << s << " start with "
         << "st" << endl;
  }

  if (thor::str_util::endswith(s, "ker")) {
    cout << s << " end with "
         << "ker" << endl;
  }

  vector<int> v = {1, 4, 5, 6, 7};
  PrintVec(v);
  vector<double> vv = {2.36878, 4.5, 6.7};
  PrintVec(vv, ", ");

  ///////////// datum usage /////////////////
  AABox2d abox1(Vector2d(2, 4), 224, 345);
  cout << abox1.DebugString() << endl;
  cout << "area: " << abox1.area() << endl;
  cout << "max_x: " << abox1.max_x() << endl;
  cout << "min_x: " << abox1.min_x() << endl;

  LineSegment2d l1(Vector2d(34, 34), Vector2d(67, 67));
  Vector2d p1(23, 23);
  if (!l1.IsPointIn(p1)) {
    cout << p1.DebugString() << " not on line. " << l1.DebugString() << endl;
  } else {
    cout << p1.DebugString() << " on line: " << l1.DebugString() << endl;
  }
  Vector2d p2(43, 43);
  if (!l1.IsPointIn(p2)) {
    cout << p2.DebugString() << " not on line. " << l1.DebugString() << endl;
  } else {
    cout << p2.DebugString() << " on line: " << l1.DebugString() << endl;
  }

  // test visualization
  // show rectangle
  cv::Scalar unique_color = thor::vis::gen_unique_color_cv(1);
  cv::rectangle(img, cv::Point(155, 155), cv::Point(230, 360), unique_color,
                cv::FILLED, 0);
  // should be: 0, 255, 117
  cout << "class: " << 23 << " color: " << unique_color << endl;

  unique_color = thor::vis::gen_unique_color_cv(3);
  cv::rectangle(img, cv::Point(12, 155), cv::Point(40, 360), unique_color,
                cv::FILLED, 0);
  // should: 126, 204, 0
  cout << "class: " << 3 << " color: " << unique_color << endl;

  unique_color = thor::vis::gen_unique_color_cv(43);
  cv::rectangle(img, cv::Point(250, 155), cv::Point(360, 360), unique_color,
                cv::FILLED, 0);


  cv::imshow("demo", img);
  cv::waitKey(0);
  return 0;
}
