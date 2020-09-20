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
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/text_format.h"

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


// save ProtoBuff messages to binary
        template<typename T>
        void savePbToBinary(T t, string f_str) {
            std::ofstream fw;
            // not implemented
//            fw.open(f_str, std::ios::out | std::ios::binary);
//            google::protobuf::io::CodedOutputStream *output = new google::protobuf::io::CodedOutputStream(&fw);
//            google::protobuf::TextFormat::Print(t, output);
//            delete output;
//            fw.close();
        }

    }  // namespace io
}  // namespace thor

#endif