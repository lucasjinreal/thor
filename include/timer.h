//
// Created by fagangjin on 8/5/19.
//

/**
 *
 *
 * Our timer enables us count any time
 * whenever you want
 *
 *
 */

#ifndef THOR_INCLUDE_TIMER_H_
#define THOR_INCLUDE_TIMER_H_




#include <time.h>
#include <chrono>
#include <thread>

/**********************************
  Timer class
**********************************/


namespace thor {

class Timer {
 public:
  Timer(const float& hz);
  Timer();
  ~Timer(){};
  void on();
  double off();
//  /// Start measuring time
//  void press();
  double lap();

 private:
  float hz_;
  std::chrono::system_clock::time_point start_time_;
  std::chrono::system_clock::time_point last_update_time_;
  std::chrono::duration<double> target_duration_;
  bool busy_;
};

}



#endif //THOR_INCLUDE_TIMER_H_
