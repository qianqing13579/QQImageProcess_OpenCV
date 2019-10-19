//////////////////////////////////////////////////////////////////////////
// 积分图 IntegralImage.h
// 2014-12-1，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Telephone:17761745857
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __INTEGRALIMAGE_H__
#define __INTEGRALIMAGE_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
using namespace cv;
namespace QQ
{

/////////////////////////////积分图/////////////////////////////////////////////
/*V3.0

积分图还可以这么计算：
1. 积分图上面留一行，全部赋值为0，这样就统一操作：积分图当前像素值=当前行累加和+上一行对应像素的值。
2. 积分图上面和左边都留一行(OpenCV的做法)，这样操作也统一：积分图当前像素的值=前面一个像素的值+该像素的值+上面像素的值。

*/
DLL_EXPORTS void CalculateIntegralImage(const Mat &srcImage,Mat &image_Integral);//改进的方法
DLL_EXPORTS void CalculateIntegralImage_Old(const Mat &srcImage,Mat &image_Integral);//原始方法


//下面两种方式非常低效
//V2.0
void CalculateIntegralImage_2(const Mat &srcImage,Mat &image_Integral);//采用行列地址+[]操作符构造积分图
//V1.0
void CalculateIntegralImage_1(const Mat &srcImage,Mat &image_Integral);//采用at方法构造积分图



}//namespace QQ

#endif