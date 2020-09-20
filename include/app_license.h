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

#include <iostream>
#include "curl/curl.h"
#include "json.h"
#include "./logging.h"

using namespace std;
using json = thor::json;
using namespace thor::log;

namespace thor{
    namespace security{


        struct CheckResult{
            bool ok;
            string msg;
        };


        class LicenseChecker{
        public:
            LicenseChecker();
            CheckResult checkLicense(string license);
            static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

        private:
            // add ?license=dferfgye5894569 do get request to get license result
            // if success
            string _url;

        };
    }
}

