#define DLLAPI_EXPORTS
#include "CommonUtility.h"
#include<assert.h>
#include<ctype.h>
#include<time.h>
#include<stdlib.h>
#include<algorithm>
#include<sstream>
#include<vector>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include<Windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#endif

namespace QQ
{



float ToFloat(string str)
{
    std::stringstream s;
    s << str;
    float d;
    s >> d;
    return d;
}

// string->double
double ToDouble(string str)
{
    // 转为double
    std::stringstream s;
    s << str;
    double d;
    s >> d;
    return d;
}

// string->int
int ToInt(string str)
{
    std::stringstream s;
    s << str;
    int d;
    s >> d;
    return d;
}

// string->uchar
unsigned char ToUchar(string str)
{
    std::stringstream s;
    s << str;
    unsigned char  d;
    s >> d;
    return d;
}

void Sleep2(int time_ms)
{
#ifdef _WIN32
    Sleep(time_ms);
#else
    sleep(time_ms/1000);
#endif
}

vector<string> SplitString(string str, std::string separator)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=separator;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(separator,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+separator.size()-1;
        }
    }
    return result;
}

_Time GetCurrentTime2()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p =localtime(&timep);  // 已经进行过时区转化为本地时间

    _Time currentTime;
    currentTime.year=to_string(1900+p->tm_year);

    char temp[8]={0};
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

    currentTime.weekDay=to_string(p->tm_wday);

    return currentTime;
}

_Time GetCurrentTime3()
{
    _Time currentTime;

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

string GetCurrentTimeString()
{

    char timeString[256]={0};
    _Time currentTime=GetCurrentTime3();
    sprintf(timeString,"%s%s%s%s%s%s%s",currentTime.year.c_str(),currentTime.month.c_str(),
            currentTime.day.c_str(),currentTime.hour.c_str(),
            currentTime.minute.c_str(),currentTime.second.c_str(),currentTime.millisecond.c_str());

    return timeString;

}

string GetCurrentTimeString2()
{

    char timeString[256]={0};
    _Time currentTime=GetCurrentTime3();
    sprintf(timeString,"%s-%s-%s_%s-%s-%s-%s",currentTime.year.c_str(),currentTime.month.c_str(),
            currentTime.day.c_str(),currentTime.hour.c_str(),
            currentTime.minute.c_str(),currentTime.second.c_str(),currentTime.millisecond.c_str());

    return timeString;

}

// set numberOfBytesPerWChar=2 ,if wchar_t  is made up of 2 bytes
std::wstring string2wstring(const string &_str)
{
    const char *str=_str.c_str();

#ifdef _WIN32  // for Windows
    // in my ubuntu14.04, wchar_t  is made up of 3 bytes
    // in windows,wchar_t is made up of 2 bytes,so the value should be set to 2
    int numberOfBytesPerWChar=2;
#else // for Linux
    int numberOfBytesPerWChar=3;
#endif

    // compute numberOf wchar_t
    int i=0;
    int numberOfWChar=0;
    while(str[i]!='\0')
    {
        wchar_t temp=str[i];
        if(isascii(temp))
        {
           ++numberOfWChar;
           ++i;
        }
        else
        {
            ++numberOfWChar;
            i+=numberOfBytesPerWChar;
        }
    }

    // memory allocation
    wchar_t *wc=new wchar_t[numberOfWChar+1];

    // convert to wchar_t
    i=0;
    int j=0;
    while(str[i]!='\0')
    {
        wc[j] = str[i];

        // convert to wchar_t
        if(isascii(wc[j]))
        {
            mbtowc((wc+j), &str[i], 1);
            ++j;
            ++i;
        }
        else
        {
            mbtowc((wc+j), &str[i], numberOfBytesPerWChar);
            ++j;
            i+=numberOfBytesPerWChar;
        }

    }

    wc[j]='\0';

    // release memory
    ReleasingHeapArrayMemory<wchar_t> releasingHeapArrayMemory(wc);

    return std::wstring(wc);
}

}
