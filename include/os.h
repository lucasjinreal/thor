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

#ifndef _T_OS_H
#define _T_OS_H

#include <iostream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <cassert>

using namespace std;


namespace thor{
    namespace os{
//    using namespace ;

    bool exists(string path);
        bool isdir(string path);
        bool isfile(string path);

        vector<string> list_files(string path, bool full_path);
        vector<string> list_files_recurse(string path, bool full_path);
        vector<string> list_dirs(string path, bool full_path);
        vector<string> list_all(string path, bool full_path);

        string join(string path, string filename);

        string parent_path(string path);
        string filename(string path);

        int do_mkdir(string path, mode_t mode);
        int makedirs(string path, mode_t mode);

        string abs_path(const string &path);

        string suffix(string path);

        ///////////////// add some new APIs /////////////////////////
        std::string GetAbsolutePath(const std::string &prefix,
                                    const std::string &relative_path);
        inline std::string GetFileName(const std::string &path) {
          std::string filename;
          std::string::size_type loc = path.rfind('/');
          if (loc == std::string::npos) {
            filename = path;
          } else {
            filename = path.substr(loc + 1);
          }
          return filename;
        }



    }
}

// Define some micros to check the method params is null or not.
#define CHECK_NULL(param) (if (param == NULL) cout << "got null.\n";)

#endif //_OS_H
