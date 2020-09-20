/*
 * Copyright (c) 2020 Fagang Jin.
 *
 * This file is part of thor
 * (see manaai.cn).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
//
// Created by jintian on 7/14/17.
//

#ifndef _T_STRING_H
#define _T_STRING_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <vector>
#include <sstream>

// remove this, protobuf 2.6 does not comptable
//#include "google/protobuf/stubs/strutil.h"

using namespace std;

namespace thor {
namespace str_util {

void SplitString(const std::string &s, std::vector<std::string> &v,
                 const std::string &c);
string join_str(const std::string &c, vector<string> s_v);

void StripString(vector<string> s_v, const string &c);

bool Replace(std::string &str, const std::string &from, const std::string &to);
void ReplaceAll(std::string &str, const std::string &from,
                const std::string &to);



//////////////  new APIs of strings /////////////////////////////
inline bool endswith(const std::string &ori, const std::string &pat) {
  return ori.size() >= pat.size() && 0 == ori.compare(ori.size() - pat.size(), pat.size(), pat);
}
inline bool startswith(const std::string &ori, const std::string &pat) {
  return ori.size() >= pat.size() && 0 == ori.compare(0, pat.size(), pat);
}

int split(const std::string &str, char ch, std::vector<std::string> *result);
void ltrim(std::string *str);
inline std::string ltrim(std::string str) {
  ltrim(&str);
  return str;
}
void rtrim(std::string *str);
inline std::string rtrim(std::string str) {
  rtrim(&str);
  return str;
}
void trim(std::string *str);
inline std::string trim(std::string str) {
  trim(&str);
  return str;
}



//////////////////////// For vector debugging usage ///////////////////////////////////
template <typename Iter>
void JoinIter(Iter begin, Iter end, const std::string &separator, std::string &res)
{
  std::ostringstream result;
  if (begin != end)
    result << *begin++;
  while (begin != end)
    result << separator << *begin++;
  res = result.str();
}

template<typename T>
std::string Print(const T &val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

template<typename Iter>
std::string PrintIter(const Iter &begin, const Iter &end,
                      const std::string &delimiter = " ") {
  std::string result;
  JoinIter(begin, end, delimiter, result);
  return result;
}

template<typename T>
void PrintVec(const vector<T> v, const std::string &delimiter = " ") {

  // TODO: T maybe nested vector, nested vector not supported
  std::cout << PrintIter(v.begin(), v.end(), delimiter) << "\n";

}


template<typename Iter>
std::string PrintIter(const Iter &begin, const Iter &end,
                      const std::function<std::string(Iter)> transformer,
                      const std::string &delimiter = " ") {
  std::string result;
  if (transformer) {
    for (auto iter = begin; iter != end; ++iter) {
      if (iter == begin) {
//        StrAppend(&result, );
        result += transformer(*iter);
      } else {
//        StrAppend(&result, delimiter, transformer(*iter));
        result += delimiter + transformer(*iter);
      }
    }
  } else {
    PrintIter(begin, end, delimiter);
  }
  return result;
}

template<typename Container, typename Iter>
std::string PrintIter(const Container &container,
                      const std::function<std::string(Iter)> transformer,
                      const std::string &delimiter = " ") {
  return PrintIter(container.begin(), container.end(), transformer, delimiter);
}

template<typename Container>
std::string PrintIter(const Container &container,
                      const std::string &delimiter = " ") {
  return PrintIter(container.begin(), container.end(), delimiter);
}
//
template<typename T, int length>
std::string PrintIter(T (&array)[length], T *end,
                      const std::string &delimiter = " ") {
  std::string result;
  Join(array, end, delimiter.c_str(), &result);
  return result;
}

template<typename T, int length>
std::string PrintIter(T (&array)[length], const std::string &delimiter = " ") {
  return PrintIter(array, array + length, delimiter);
}


template<typename Iter>
std::string PrintDebugStringIter(const Iter &begin, const Iter &end,
                                 const std::string &delimiter = " ") {
  std::string result;
  for (auto iter = begin; iter != end; ++iter) {
    if (iter == begin) {
//      StrAppend(&result, iter->DebugString());
      result += iter->DebugString();
    } else {
//      StrAppend(&result, delimiter, iter->DebugString());
      result += delimiter + iter->DebugString();
    }
  }
  return result;
}

template<typename Container>
std::string PrintDebugStringIter(const Container &container,
                                 const std::string &delimiter = " ") {
  return PrintDebugStringIter(container.begin(), container.end(), delimiter);
}


std::string DecodeBase64(const std::string &base64_str);
std::string EncodeBase64(const std::string &in);


}  // namespace str_util
}  // namespace thor

#endif  //_T_STRING_H
