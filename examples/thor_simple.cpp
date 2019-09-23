//
// Created by JinTian on 02/01/2018.
//

/**
 * A simple example of using Thor library
 */

#include <zconf.h>
#include <iostream>
#include "thor/logging.h"
#include "thor/macro.h"
#include "thor/os.h"
#include "thor/timer.h"

using namespace std;
using namespace thor::log;

int main() {
  PRINT_RED("hello, we are testing some basic function in thor...");

  PRINT_GREEN("1. test a timer.");
  thor::Timer timer(20);
  timer.on();

  LOG(INFO) << " count start..";
  for (int i = 0; i < 3; ++i) {
    sleep(1);
  }
  LOG(INFO) << " cost time: " << timer.lap();
  for (int i = 0; i < 2; ++i) {
    sleep(1);
  }
  LOG(INFO) << "another time: " << timer.lap();

  PRINT_GREEN("2. test datum.");

  if (thor::os::suffix("hhhaaa.mp4") == "mp4") {
    LOG(INFO) << "this is an video file.";
  }

  LOG(INFO) << "creating some folder recursively.";
  thor::os::makedirs("data/pointcloud", 07777);
}