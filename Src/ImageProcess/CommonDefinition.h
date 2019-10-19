//////////////////////////////////////////////////////////////////////////
// 类型定义 TypeDefinition.h
// 2014-11-28，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Telephone:17761745857
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////


#ifndef __TYPE_DEFINITION_H__
#define __TYPE_DEFINITION_H__

//////////////////////////////////////////////////////////////////////////
//						Common macros
//////////////////////////////////////////////////////////////////////////

//WIN32,_WIN32：平台判断
//DLLAPI_EXPORTS：条件宏
//DLL_EXPORTS:DLL导出符号
#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined DLLAPI_EXPORTS
#  define DLL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#  define DLL_EXPORTS __attribute__((visibility ("default")))
#else
#  define DLL_EXPORTS
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


//////////////////////////////////////////////////////////////////////////
//						Common type definitions
//////////////////////////////////////////////////////////////////////////

#endif