//////////////////////////////////////////////////////////////////////////
// 滤波 Smooth.h
// 2014-12-8，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Telephone:17761745857
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////
// Updated 2017-07-29 by QQ,add Convolution22

#ifndef __FILTER_H__
#define __FILTER_H__
#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
using namespace cv;

namespace QQ
{

/////////////////////////////////均值滤波/////////////////////////////////////////
//V4.0 高效均值滤波，加入对边界的处理,使用copyMakeBorder()扩充原图
DLL_EXPORTS void Blur(const Mat &srcImage, Mat &dstImage, Size size_Aperture);

//V3.0 高效均值滤波，加入对边界的处理，没有使用copyMakeBorder()扩充原图
DLL_EXPORTS void Blur_3(const Mat &srcImage, Mat &dstImage, Size size_Aperture);

//V2.0 高效均值滤波(图像的高效编程要点之五),没有处理边界
DLL_EXPORTS void Blur_2(const Mat &srcImage, Mat &dstImage, Size size_Aperture);

//V1.0
DLL_EXPORTS void Blur_1(const Mat &srcImage, Mat &dstImage, Size size_Aperture);
int CalculateMeanValue(uchar *imageData,int pixelCount);//计算窗口像素平均值


/////////////////////////////////中值滤波/////////////////////////////////////////
//V4.0 高效中值滤波(图像的高效编程要点之四)
//改进的高效中值滤波：不采用OpenCV的直方图
DLL_EXPORTS void FastMedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture);

//V3.0 高效中值滤波(图像的高效编程要点之四)
//边界没有处理
DLL_EXPORTS void FastMedianBlur_1(const Mat &srcImage, Mat &dstImage, int width_Aperture);//中值滤波
void CalculateImage_MedianGray_PixelCount(const Mat &histogram,int pixelCount,int &medianValue,int &pixleCountLowerMedian);//计算亮度中值和灰度<=中值的像素点个数

//V2.0 普通中值滤波
//边界没有处理
//3.0已不支持函数中的滑动窗口提取方式，该函数不能用了
DLL_EXPORTS void MedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture);//中值滤波
uchar GetMedianValue(const Mat& image);//用于比较


/////////////////////////////////高斯滤波/////////////////////////////////////////
DLL_EXPORTS void GaussianBlur(const Mat &srcImage, Mat &dstImage, double sigma);


//////////////////////////////滑动窗口卷积运算的两种方式///////////////////////////////////////////////////////////
// 卷积运算方式1：使用caffe中的卷积运算方式
DLL_EXPORTS void Convolution1(const Mat &srcImage,const Mat &kernel, Mat &dstImage);
DLL_EXPORTS void Convolution12(const Mat &srcImage, const Mat &kernel, Mat &dstImage);// Convolution1的优化版本1 

//  卷积运算方式2：对每个滑动窗口操作,该方法可以用来做pooling
DLL_EXPORTS void Convolution2(const Mat &srcImage,const Mat &kernel, Mat &dstImage);
DLL_EXPORTS void Convolution22(const Mat &srcImage, const Mat &kernel, Mat &dstImage); // 通过查找表优化Convolution2



}//namespace QQ

#endif