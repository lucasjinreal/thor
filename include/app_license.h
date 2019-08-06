// ************************************
// Copyrights by Jin Fagang
// 2/15/19-15-17
// app_license
// jinfagang19@gmail.com
// CTI Robotics
// ************************************

//
// Created by jintain on 2/15/19.
//

#ifndef THOR_APP_LICENSE_H
#define THOR_APP_LICENSE_H

/**
 * this is using for license check, we need to check license when
 * executing init() function in our applications
 */

#ifdef USE_CURL
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
#endif

#endif //THOR_APP_LICENSE_H
