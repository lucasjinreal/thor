//
// Created by jintian on 2020/4/29.
//

#ifndef THOR_UTILS_H
#define THOR_UTILS_H


#include "google/protobuf/stubs/common.h"

// Add some common used utils here
namespace thor{
    namespace utils {
        void ShutdownProtobuf() {
            google::protobuf::ShutdownProtobufLibrary();
        }
    };
}

#endif //THOR_UTILS_H
