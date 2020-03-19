#define DLLAPI_EXPORTS
#include "SimpleLog.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <Windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/time.h>
#endif
#include "Filesystem.h"

using namespace std;

namespace QQ
{

LogManager::LogManager()
{

}

LogManager::~LogManager()
{

}

void LogManager::Initialize(const string &parentPath,const string &_logName)
{
    // print to stdout
    if(_logName.size()==0)
        return;

    if(!Exists(parentPath))
    {
        CreateDirectories(parentPath);
    }

    // check whether exist
    std::map<string, FILE*>::const_iterator iter = logMap.find(_logName);
    if (iter == logMap.end())
    {
        string logName = parentPath+ _logName + ".log";
        FILE *logFile = fopen(logName.c_str(), "a"); // w,a 根据实际情况，决定使用w还是a
        if(logFile!=NULL)
        {
            logMap.insert(std::make_pair(_logName, logFile));
        }
    }

}

FILE* LogManager::GetLogFile(const string &logName)
{
    std::map<string, FILE*>::const_iterator iter=logMap.find(logName);
    if(iter==logMap.end())
    {
        return NULL;
    }

    return (*iter).second;
}
void LogManager::Close(const string &logName)
{
    std::map<string, FILE*>::const_iterator iter=logMap.find(logName);
    if(iter==logMap.end())
    {
        return;
    }

    // close and delete
    fclose((*iter).second);
    logMap.erase(iter);
}
std::mutex &LogManager::GetLogMutex()
{
    return logMutex;
}
LogTime LogManager::GetTime()
{
    LogTime currentTime;

#if (defined WIN32 || defined _WIN32)
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);

    char temp[8] = { 0 };
    sprintf(temp, "%04d", systemTime.wYear);
    currentTime.year=string(temp);
    sprintf(temp, "%02d", systemTime.wMonth);
    currentTime.month=string(temp);
    sprintf(temp, "%02d", systemTime.wDay);
    currentTime.day=string(temp);
    sprintf(temp, "%02d", systemTime.wHour);
    currentTime.hour=string(temp);
    sprintf(temp, "%02d", systemTime.wMinute);
    currentTime.minute=string(temp);
    sprintf(temp, "%02d", systemTime.wSecond);
    currentTime.second=string(temp);
    sprintf(temp, "%03d", systemTime.wMilliseconds);
    currentTime.millisecond=string(temp);
    sprintf(temp, "%d", systemTime.wDayOfWeek);
    currentTime.weekDay=string(temp);
#else
    struct timeval    tv;
    struct tm         *p;
    gettimeofday(&tv, NULL);
    p = localtime(&tv.tv_sec);

    char temp[8]={0};
    sprintf(temp,"%04d",1900+p->tm_year);
    currentTime.year=string(temp);
    sprintf(temp,"%02d",1+p->tm_mon);
    currentTime.month=string(temp);
    sprintf(temp,"%02d",p->tm_mday);
    currentTime.day=string(temp);
    sprintf(temp,"%02d",p->tm_hour);
    currentTime.hour=string(temp);
    sprintf(temp,"%02d",p->tm_min);
    currentTime.minute=string(temp);
    sprintf(temp,"%02d",p->tm_sec);
    currentTime.second=string(temp);
    sprintf(temp,"%03d",tv.tv_usec/1000);
    currentTime.millisecond = string(temp);
    sprintf(temp, "%03d", tv.tv_usec % 1000);
    currentTime.microsecond = string(temp);
    sprintf(temp, "%d", p->tm_wday);
    currentTime.weekDay = string(temp);
#endif
    return currentTime;
}

LogManager* LogManager::GetInstance()
{
    static LogManager logManager;
    return &logManager;
}




}
