// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// 类型定义

#ifndef _COMMON_DEFINITION_H__
#define _COMMON_DEFINITION_H__

#include"SimpleLog.h"

#ifdef DLLAPI_EXPORTS
#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
#  define DLL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define DLL_EXPORTS __attribute__((visibility ("default")))
#endif
#endif

#ifndef DLL_EXPORTS
#define DLL_EXPORTS
#endif

typedef unsigned char BYTE;

//角度转换
#define DEGREE2RADIAN(x) (x*CV_PI/180)//角度转弧度
#define RADIAN2DEGREE(x) (x*180/CV_PI)//弧度转角度

/* 该形式中x为整型
printf("%d\n", SATURATE(1098)); // 255
printf("%d\n", SATURATE(255)); // 255
printf("%d\n", SATURATE(-89)); // 0
*/
#define  SATURATE(x)   (uchar)(((x) & ~255) == 0 ? (x) : ~((x)>>31)) // min(max(x,0),255)，适用于x为任意类型


#endif
