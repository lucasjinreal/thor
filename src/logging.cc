
#include "../include/logging.h"


namespace thor{

namespace log{

void gen_log(std::ostream& log_stream_, const char* file, const char* func, int lineno, const char* level, const int kMaxLen) {
  const int len = strlen(file);
  std::string time_str;
  struct tm tm_time;  // Time of creation of LogMessageThor
  time_t timestamp = time(NULL);
  localtime_r(&timestamp, &tm_time);
  struct timeval tv;
  gettimeofday(&tv, NULL);

  // print date / time
  log_stream_ << level << ' ' << std::setw(1) << 1 + tm_time.tm_mon
              << '/' << tm_time.tm_mday << ' '
              << tm_time.tm_hour << ':' << std::setw(2) << tm_time.tm_min
              << ':' << std::setw(2) << tm_time.tm_sec << '.' << std::setw(3)
              << tv.tv_usec / 1000 << " ";

  if (len > kMaxLen) {
    log_stream_ << "..." << file + len - kMaxLen << " " << func << ":"
                << lineno << "] ";
  } else {
    log_stream_ << file << " " << func << ":" << lineno << "] ";
  }
}


}
}