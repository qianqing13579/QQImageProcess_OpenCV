// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// 灰度变换

#ifndef __GRAY_TRANSFORMATION_H__
#define __GRAY_TRANSFORMATION_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"

using namespace cv;

namespace QQ
{
DLL_EXPORTS void RGB2Gray(const Mat &srcImage, Mat &dstImage);
DLL_EXPORTS void GammaCorrection(const Mat &srcImage, Mat &dstImage, float gamma);

// 直方图均衡化
DLL_EXPORTS void EqualizeHistogram(const Mat &srcImage, Mat &dstImage);


}//end of namespace QQ
#endif
