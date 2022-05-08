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
// Created by jintian on 19-4-17.
//

#ifndef THOR_SRC_CV_EX_H_
#define THOR_SRC_CV_EX_H_


#ifdef USE_CV_EX
#include <freetype2/freetype/config/ftheader.h>
#include FT_FREETYPE_H
#include <opencv2/opencv.hpp>
#include <wchar.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
#include <cmath>




namespace thor{

class CvxText
{
 public:
  CvxText(const char *freeType);
  virtual ~CvxText();
  void getFont(int *type, cv::Scalar *size = nullptr, bool *underline = nullptr, float *diaphaneity = nullptr);
  void setFont(int *type, cv::Scalar *size = nullptr, bool *underline = nullptr, float *diaphaneity = nullptr);

  void restoreFont();

  int putText(cv::Mat &img, char *text, cv::Point pos);
  int putText(cv::Mat &img, const wchar_t *text, cv::Point pos);
  int putText(cv::Mat &img, const char *text, cv::Point pos, cv::Scalar color);
  int putText(cv::Mat &img, const wchar_t *text, cv::Point pos, cv::Scalar color);

  inline static int ToWchar(char *&src, wchar_t *&dest, const char *locale = "zh_CN.utf8")
  {
    if (src == NULL)
    {
      dest = NULL;
      return 0;
    }
    setlocale(LC_CTYPE, locale);
    int w_size = mbstowcs(NULL, src, 0) + 1;
    if (w_size == 0)
    {
      dest = NULL;
      return -1;
    }
    dest = new wchar_t[w_size];
    if (!dest)
    {
      return -1;
    }
    int ret = mbstowcs(dest, src, strlen(src) + 1);
    if (ret <= 0)
    {
      return -1;
    }
    return 0;
  }

 private:
  CvxText &operator=(const CvxText &);
  void putWChar(cv::Mat &img, wchar_t wc, cv::Point &pos, cv::Scalar color);

  FT_Library m_library; // 字库
  FT_Face m_face;       // 字体
  int m_fontType;
  cv::Scalar m_fontSize;
  bool m_fontUnderline;
  float m_fontDiaphaneity;
};
}
#endif


#endif //THOR_SRC_CV_EX_H_
