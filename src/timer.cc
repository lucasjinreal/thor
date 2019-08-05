#include "include/timer.h"




namespace thor{


Timer::Timer(const float& hz)
    : hz_(hz),
      start_time_(),
      target_duration_(1.0 / hz),
      busy_(false) {}


Timer::Timer(): hz_(20),
                start_time_(),
                target_duration_(1.0/20),
                busy_(false){
}


void Timer::on() {
  start_time_ = std::chrono::system_clock::now();
  last_update_time_ = start_time_;
  busy_ = true;
}

double Timer::off() {
  std::chrono::duration<double> time_cost = std::chrono::system_clock::now() - start_time_;
  if (time_cost < target_duration_) {
    std::this_thread::sleep_for(target_duration_ - time_cost);
  }
  std::chrono::duration<double> total_time = std::chrono::system_clock::now() - start_time_;
  busy_ = false;
  return total_time.count();
}

//void Timer::press() {
//  last_update_time_ = std::chrono::system_clock::now();
//}

double Timer::lap() {
  // return as seconds
  // we only need seconds as readable
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::duration<double > duration = now - last_update_time_;
  last_update_time_ = now;
//  return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
// this should return as seconds
  return duration.count();
}


}