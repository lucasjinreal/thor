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
#pragma once

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/text_format.h"

using std::ostringstream;
using std::string;
using std::vector;

namespace thor {
namespace io {
inline bool ReadTxtLines(const string txt_file, vector<string>* res) {
  std::ifstream in(txt_file.c_str());
  // Check if object is valid
  if (!in) {
    return false;
  }
  std::string str;
  while (std::getline(in, str)) {
    if (str.size() > 0) res->push_back(str);
  }
  in.close();
  return true;
}

// save ProtoBuff messages to binary
template <typename T>
void savePbToBinary(T t, string f_str) {
  std::ofstream fw;
  // not implemented
  //            fw.open(f_str, std::ios::out | std::ios::binary);
  //            google::protobuf::io::CodedOutputStream *output = new
  //            google::protobuf::io::CodedOutputStream(&fw);
  //            google::protobuf::TextFormat::Print(t, output);
  //            delete output;
  //            fw.close();
}

// these are need by parsing tar files
template <typename... T>
inline std::string make_string(T&&... args) {
  std::ostringstream os;
  os << std::boolalpha;
  (os << ... << static_cast<T&&>(args));
  return os.str();
}

/// \brief Mask all non pritable characters with escape sequences
inline std::string mask_non_print(std::string const& str) {
  std::ostringstream result;
  for (auto c : str) {
    // Mask any UTF-8 characters
    auto const d = static_cast<std::uint8_t>(c);
    if (d > 127) {
      result << "\\x";
      auto nipple_to_hex = [](std::uint8_t e) {
        return static_cast<char>(e < 10 ? e + '0' : e - 10 + 'A');
      };
      result << nipple_to_hex(d >> 4);
      result << nipple_to_hex(d & 0x0F);
      continue;
    }

    switch (c) {
      case 0:
        result << "\\0";
        break;
      case 1:
        result << "\\x01";
        break;
      case 2:
        result << "\\x02";
        break;
      case 3:
        result << "\\x03";
        break;
      case 4:
        result << "\\x04";
        break;
      case 5:
        result << "\\x05";
        break;
      case 6:
        result << "\\x06";
        break;
      case 7:
        result << "\\a";
        break;
      case 8:
        result << "\\b";
        break;
      case 9:
        result << "\\t";
        break;
      case 10:
        result << "\\n";
        break;
      case 11:
        result << "\\v";
        break;
      case 12:
        result << "\\f";
        break;
      case 13:
        result << "\\r";
        break;
      case 14:
        result << "\\x0E";
        break;
      case 15:
        result << "\\x0F";
        break;
      case 16:
        result << "\\x10";
        break;
      case 17:
        result << "\\x11";
        break;
      case 18:
        result << "\\x12";
        break;
      case 19:
        result << "\\x13";
        break;
      case 20:
        result << "\\x14";
        break;
      case 21:
        result << "\\x15";
        break;
      case 22:
        result << "\\x16";
        break;
      case 23:
        result << "\\x17";
        break;
      case 24:
        result << "\\x18";
        break;
      case 25:
        result << "\\x19";
        break;
      case 26:
        result << "\\x1A";
        break;
      case 27:
        result << "\\x1B";
        break;
      case 28:
        result << "\\x1C";
        break;
      case 29:
        result << "\\x1D";
        break;
      case 30:
        result << "\\x1E";
        break;
      case 31:
        result << "\\x1F";
        break;
      case 127:
        result << "\\x7F";
        break;
      case '\\':
        result << "\\\\";
        break;
      default:
        result << c;
    }
  }
  return result.str();
}

class substreambuf : public std::streambuf {
 public:
  substreambuf(std::streambuf* buffer, std::streampos start,
               std::streamsize size)
      : buffer_(buffer), start_(start), size_(size) {
    setbuf(nullptr, 0);
  }

  substreambuf(substreambuf const&) = delete;
  substreambuf(substreambuf&&) = default;

 protected:
  void imbue(std::locale const& loc) override { buffer_->pubimbue(loc); }

  std::streambuf* setbuf(char_type* s, std::streamsize n) override {
    return buffer_->pubsetbuf(s, n);
  }

  std::streampos seekoff(std::streamoff off, std::ios_base::seekdir dir,
                         std::ios_base::openmode which =
                             std::ios_base::in | std::ios_base::out) override {
    std::streampos new_pos;

    if (dir == std::ios_base::beg) {
      new_pos = buffer_->pubseekpos(start_ + off, which);
    } else if (dir == std::ios_base::cur) {
      new_pos = buffer_->pubseekoff(off, dir, which);
    } else {
      new_pos =
          buffer_->pubseekpos(start_ + std::streampos(size_) + off, which);
    }

    if (new_pos < start_ || new_pos >= start_ + std::streampos(size_)) {
      return -1;
    }

    return new_pos;
  }

  std::streampos seekpos(std::streampos pos,
                         std::ios_base::openmode which =
                             std::ios_base::in | std::ios_base::out) override {
    auto new_pos = buffer_->pubseekpos(start_ + pos, which);

    if (new_pos < start_ || new_pos >= start_ + std::streampos(size_)) {
      return -1;
    }

    return new_pos;
  }

  int sync() override { return buffer_->pubsync(); }

  int_type underflow() override { return buffer_->sgetc(); }

  int_type uflow() override { return buffer_->sbumpc(); }

  std::streamsize xsgetn(char_type* s, std::streamsize count) override {
    return buffer_->sgetn(s, count);
  }

  std::streamsize xsputn(char_type const* s, std::streamsize count) override {
    return buffer_->sputn(s, count);
  }

 private:
  std::streambuf* buffer_;
  std::streampos start_;
  std::streamsize size_;
};

class isubstream : public std::istream {
 public:
  isubstream(std::streambuf* buffer, std::streampos start, std::streamsize size)
      : std::istream(&buffer_), buffer_(buffer, start, size) {}

  isubstream(isubstream const&) = delete;
  isubstream(isubstream&&) = default;

 private:
  substreambuf buffer_;
};

}  // namespace io
}  // namespace thor
