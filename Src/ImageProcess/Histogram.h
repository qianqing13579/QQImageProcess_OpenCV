// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// 直方图

#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
using namespace cv;

namespace QQ
{
//直方图
DLL_EXPORTS void GetHistogram(const Mat &image, int *histogram);
DLL_EXPORTS void GetMedianValue(const Mat &image, int &medianValue);
DLL_EXPORTS void GetMedianValue_AndPixelCountLowerMedian(const Mat &image, int &medianValue, int &pixelCountLowerMedian);
DLL_EXPORTS void GetMaxMinValue(const Mat &image, int &maxValue, int &minValue);

//画直方图
DLL_EXPORTS void DrawHistogram(const Mat &histogram,Mat &image_Historam);


}//namespace QQ

#endif//end of HISTOGRAM_H
