//
// Created by jintian on 18-1-11.
//
#include "../include/logging.h"


/**
 * I think future I will add file and line number, but currently it is not implement
 * @param level
 * @param content
 */



void thor::Log(LEVEL level, const string content){
    string t = thor::getSysTime();

    switch (level){
        case LEVEL::kInfo:
            cout << colors::bold << colors::white << "INFO: " << colors::reset << t + " " + content << endl;
            //cout << colors::bold << colors::yellow << "INFO: " << colors::reset << t + " [" + __FILE__ + " " + to_string(__LINE__) + "]" + content << endl;
            break;
        case LEVEL::kWarning:
            cout << colors::bold << "WARN: " << colors::reset << t + " " + content << endl;
            break;
        case LEVEL::kError:
            cout << colors::bold << colors::red <<  "ERRO: " << colors::reset << t + " " + content << endl;
            break;
    }
}
void thor::Log(LEVEL level, int content){
    string t = thor::getSysTime();
    switch (level){
        case LEVEL::kInfo:
            cout << colors::bold << colors::white << "INFO: " << colors::reset << t + " " + to_string(content) << endl;
            break;
        case LEVEL::kWarning:
            cout << colors::bold << "WARN: " << colors::reset << t + " " + to_string(content) << endl;
            break;
        case LEVEL::kError:
            cout << colors::bold << colors::red <<  "ERRO: " << colors::reset << t + " " + to_string(content) << endl;
            break;
    }
}

string thor::getSysTime()
{
    time_t tNowTime;
    time(&tNowTime);
    tm* tLocalTime = localtime(&tNowTime);
    char szTime[30] = {'\0'};
    strftime(szTime, 30, "[%Y-%m-%d %H:%M:%S] ", tLocalTime);
    string strTime = szTime;
    return strTime;
}
