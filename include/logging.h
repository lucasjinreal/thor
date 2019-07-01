//
// Created by jintian on 17-8-5.
//

#ifndef _T_LOG_H
#define _T_LOG_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "colors.h"

using namespace std;

enum LEVEL
{
    kInfo,
    kWarning,
    kError,
};

namespace thor
{
void Log(LEVEL level, const string content);
void Log(LEVEL level, int content);

string getSysTime();
} // namespace thor

#endif //_T_LOG_H
