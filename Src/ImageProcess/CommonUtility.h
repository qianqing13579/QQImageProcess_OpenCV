/** @file    CommonUtility.h
 *  @note   
 *  @brief  通用工具
 *
 *  @author	QQ
 *  @date	2016-5-9
 */

#ifndef __COMMON_UTILITY_H__
#define __COMMON_UTILITY_H__


#include<mutex>
#include<string>
#include<vector>

#include"CommonDefinition.h"

using namespace std;

namespace QQ
{

// time
typedef struct DLL_EXPORTS __Time
{
	string year;
	string month;
	string day;
	string hour;
	string minute;
	string second;
	string millisecond; // ms
	string microsecond; // us
	string weekDay;
}_Time;

// use C API which can not get millisecond,
DLL_EXPORTS _Time GetCurrentTime2();// 修改命名为GetCurrentTime2,GetCurrentTime与windows中函数名同名
// use OS API which can get millisecond,Linux can get microsecond
DLL_EXPORTS _Time GetCurrentTime3();

DLL_EXPORTS string GetCurrentTimeString();
DLL_EXPORTS string GetCurrentTimeString2();// 以2019-12-21格式来显示


// string->float/int/double
DLL_EXPORTS float ToFloat(string str);
DLL_EXPORTS double ToDouble(string str);
DLL_EXPORTS int ToInt(string str);
DLL_EXPORTS unsigned char ToUchar(string str);

DLL_EXPORTS void Sleep2(int time_ms);// 与windows同名函数Sleep区分开

DLL_EXPORTS std::vector<string> SplitString(string str,std::string separator);

// release memory of Heap
template<typename T>
class DLL_EXPORTS ReleasingHeapMemory
{
public:
    ReleasingHeapMemory(T *_heapMemory=NULL):heapMemory(_heapMemory){}
    virtual ~ReleasingHeapMemory()
    {
        delete heapMemory;
    }

private:
    T *heapMemory;
};

// release array memory of Heap
template<typename T>
class DLL_EXPORTS ReleasingHeapArrayMemory
{
public:
    ReleasingHeapArrayMemory(T *_heapArrayMemory=NULL):heapArrayMemory(_heapArrayMemory){}
    virtual ~ReleasingHeapArrayMemory()
    {
        delete[] heapArrayMemory;
    }

private:
    T *heapArrayMemory;
};

// support the combination Chinese and English character
DLL_EXPORTS std::wstring string2wstring(const string &_str);

class MutexManager
{
public:
    MutexManager(std::mutex *_mutex)
    {
        mtx=_mutex;
        mtx->lock();
    }
    ~MutexManager()
    {
        mtx->unlock();
    }
private:
    std::mutex *mtx;
};


}

#endif //
