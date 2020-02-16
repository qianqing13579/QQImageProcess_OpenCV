//////////////////////////////////////////////////////////////////////////
// 几何变换 GeometryTransformation.h
// 2015-3-5，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __QQ_GEOMETRY_TRANSFORMATION_H__
#define __QQ_GEOMETRY_TRANSFORMATION_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
using namespace cv;

#define  SHIFT  10
#define  DESCALE(x,n)  (((x)+(1 << ((n)-1))) >> (n)) // OpenCV 采用的形式
// #define  DESCALE(x,n)  ((x) >> (n)) // 与OpenCV效果等同

namespace QQ
{

// 最近邻插值，缩放
DLL_EXPORTS void NearestInterpolation(const Mat &image_Src,Mat &image_Dst,Size size_Dst);

// 双线性插值，缩放
DLL_EXPORTS void BilinearInterpolation(const Mat &image_Src,Mat &image_Dst,Size size_Dst);//双线性插值(灰度图)

//垂直翻转
DLL_EXPORTS void VerticalFlip(const Mat &srcImage, Mat &dstImage);

// 水平翻转
DLL_EXPORTS void HorizontalFlip(const Mat &srcImage, Mat &dstImage);

// 仿射变换：srcImage绕点center旋转theta角度（顺时针为正）,缩放scale,平移到dstSize中
// dstSize:目标图像大小(将原图中心移动到目标图像中心)
// theta:单位角度
// scale:缩放比例
// 当图像宽高为偶数时，对于部分旋转角度，边界稍有误差
/* 测试环境 Intel core i5-6200U,12G,VS2015 使用BEAUTY_GRAY
Rotate_Nearest(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2000, 2000), 30.0, 1); // 46.905
Rotate_Bilinear(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2000, 2000),30.0, 1); // 97.618
Rotate_Bilinear1(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2000, 2000), 30.0, 1); // 80.591
Rotate_Bilinear2(srcImage, dstImage, Point(srcImage.cols / 2, srcImage.rows / 2), Size(2000, 2000), 30.0, 1); // 52.797
Rotate_Bilinear2和Rotate_Bilinear3的差别并不大，有时Rotate_Bilinear3比Rotate_Bilinear2还慢
优先使用Rotate_Bilinear2
*/
DLL_EXPORTS void Rotate_Nearest(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage); // 最近邻
DLL_EXPORTS void Rotate_Bilinear(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage); // 双线性
DLL_EXPORTS void Rotate_Bilinear1(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage); // 双线性,优化1
DLL_EXPORTS void Rotate_Bilinear2(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage);	// 双线性，优化2
DLL_EXPORTS void Rotate_Bilinear3(const Mat &srcImage, Point center, double theta, double scale, Size dstSize, Mat &dstImage);

}//namespace QQ


#endif