//////////////////////////////////////////////////////////////////////////
// 灰度变换 GrayTransformation.h
// 2016-7-9，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////


#ifndef __QQ_GRAY_TRANSFORMATION_H__
#define __QQ_GRAY_TRANSFORMATION_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
#include"Histogram.h"
using namespace cv;

namespace QQ
{
DLL_EXPORTS void RGB2Gray(const Mat &srcImage, Mat &dstImage);
DLL_EXPORTS void GammaCorrection(const Mat &srcImage, Mat &dstImage, float gamma);

// 直方图均衡化
DLL_EXPORTS void EqualizeHistogram(const Mat &srcImage, Mat &dstImage);


}//end of namespace QQ
#endif