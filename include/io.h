//
// Created by JinTian on 02/01/2020.
//

#ifndef _THOR_IO_H
#define _THOR_IO_H

/***
 *
 * All io relate operations goes here
 *
 *
 * */
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace thor {
namespace io {
inline bool ReadTxtLines(string txt_file, vector<string> &res) {
  std::ifstream in(txt_file.c_str());
  // Check if object is valid
  if (!in) {
    return false;
  }
  std::string str;
  while (std::getline(in, str)) {
    if (str.size() > 0) res.push_back(str);
  }
  in.close();
  return true;
}
}  // namespace io
}  // namespace thor

#endif