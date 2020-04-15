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

// this file enables lots of string extend methods

#include <algorithm>
#include <cstring>
#include "include/str_util.h"

using namespace std;

void thor::str_util::SplitString(const std::string &s,
                                 std::vector<std::string> &v,
                                 const std::string &c) {
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);

  if (pos2 > s.size()) {
    // indicates delimiter not in this string
    // return this string as vector
    v.push_back(s);
  } else {
    pos1 = 0;
    while (std::string::npos != pos2) {
      v.push_back(s.substr(pos1, pos2 - pos1));

      pos1 = pos2 + c.size();
      pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) {
      v.push_back(s.substr(pos1));
    }
  }
}

string thor::str_util::join_str(const std::string &c, vector<string> s_v) {
  string result_s = "";
  for (auto it = s_v.begin(); it < s_v.end() - 1; ++it) {
    result_s += (*it + c);
  }
  result_s += s_v.back();
  return result_s;
}

/**
 * Strip C from string vector and return a new string vector
 * @param s_v
 * @param c
 * @return
 */
void thor::str_util::StripString(vector<string> s_v, const string &c) {
  for (auto it = s_v.begin(); it < s_v.end(); it++) {
    if (*it == c) {
      s_v.erase(it);
    }
  }
}

/***
 * Replace part of string from original string
 * @param str
 * @param from
 * @param to
 * @return
 */
bool thor::str_util::Replace(std::string &str, const std::string &from,
                             const std::string &to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos) return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

/**
 * Replace all part of string
 * @param str
 * @param from
 * @param to
 */
void thor::str_util::ReplaceAll(std::string &str, const std::string &from,
                                const std::string &to) {
  if (from.empty()) return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

///////////////// Implementation of new APIs in str util ////////////////////
namespace thor {
namespace str_util {

static const char kBase64Array[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// A table which maps a char to its value in Base64 mode.
std::vector<int> Base64CodeTable() {
  std::vector<int> table(256, -1);
  const size_t base64_array_length = strlen(kBase64Array);
  for (size_t i = 0; i < base64_array_length; ++i) {
    table[kBase64Array[i]] = i;
  }
  return table;
}

const char *tripletBase64(const int triplet) {
  static char result[4];
  result[0] = kBase64Array[(triplet >> 18) & 0x3f];
  result[1] = kBase64Array[(triplet >> 12) & 0x3f];
  result[2] = kBase64Array[(triplet >> 6) & 0x3f];
  result[3] = kBase64Array[triplet & 0x3f];
  return result;
}

int split(const std::string &str, char ch, std::vector<std::string> *result) {
  std::stringstream ss(str);
  std::string segment;
  int count = 0;
  while (std::getline(ss, segment, ch)) {
    result->push_back(segment);
    ++count;
  }
  return count;
}

void trim(std::string *str) {
  ltrim(str);
  rtrim(str);
}

void ltrim(std::string *str) {
  if (!str) {
    return;
  }
  str->erase(str->begin(), std::find_if(str->begin(), str->end(), [](int ch) {
    return !std::isspace(ch);
  }));
}

void rtrim(std::string *str) {
  if (!str) {
    return;
  }
  str->erase(std::find_if(str->rbegin(), str->rend(),
                          [](int ch) { return !std::isspace(ch); })
                 .base(),
             str->end());
}

std::string DecodeBase64(const std::string &base64_str) {
  static const std::vector<int> kBase64CodeTable = Base64CodeTable();

  std::string bytes;
  // Binary string is generally 3/4 the length of base64 string
  bytes.reserve(base64_str.length() * 3 / 4 + 3);
  unsigned int sum = 0, sum_bits = 0;
  for (const char c : base64_str) {
    if (kBase64CodeTable[c] == -1) {
      break;
    }

    // Convert 6-bits Base64 chars to 8-bits general bytes.
    sum = (sum << 6) + kBase64CodeTable[c];
    sum_bits += 6;
    if (sum_bits >= 8) {
      bytes.push_back((sum >> (sum_bits - 8)) & 0xFF);
      sum_bits -= 8;
    }
  }
  return bytes;
}

std::string EncodeBase64(const std::string &in) {
  std::string out;
  if (in.empty()) {
    return out;
  }

  int in_size = in.size();

  out.reserve(((in_size - 1) / 3 + 1) * 4);

  int i = 2;
  for (; i < in_size; i += 3) {
    out.append(tripletBase64((in[i - 2] << 16) | (in[i - 1] << 8) | in[i]), 4);
  }
  if (i == in_size) {
    out.append(tripletBase64((in[i - 2] << 16) | (in[i - 1] << 8)), 3);
    out.push_back('=');
  } else if (i == in_size + 1) {
    out.append(tripletBase64(in[i - 2] << 16), 2);
    out.append("==");
  }
  return out;
}

}  // namespace str_util
}  // namespace thor
