//////////////////////////////////////////////////////////////////////////
// 边缘检测 Edge.h
// 2014-12-17，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __QQ_EDGE_H__
#define __QQ_EDGE_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
#include<queue>
using namespace cv;
using namespace std;

namespace QQ
{
typedef unsigned char BYTE;
void SetImageBoundary(Mat &srcImage, int color);//边界处理
void Robert(const Mat &srcImage, Mat &image_Robert);//梯度罗伯特
void Sobel(const Mat &srcImage, Mat &image_Sobel);//梯度索伯
void ShenJun(const Mat &srcImage, Mat &image_ShenJun);// 沈俊算子
void RmwShenJunEdge(BYTE *pGryImg, BYTE *pTmpImg, int width, int height, double alpha, BYTE *pResImg);

// 1.0版
void Canny1(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture = 3, bool L2 = false);
// 使用OpenCV的角度判断
void Canny2(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture = 3, bool L2 = false);
// 1.0改进版，对Canny1方法，参考OpenCV的方法转为绝对值求解
// 经过测试，Canny3的效果与Canny2效果基本一致，采用绝对值求解更好，与直接使用cv::canny效果也基本一致
void Canny3(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture = 3, bool L2 = false);

// 细化算法
// 第一种实现方式
void thin1(const Mat &src, Mat &dst, const int iterations); 
// OpenCV/ximgproc模块实现方式,测试表明，OpenCV的实现方式效果更好
void thin2( Mat &src, Mat &dst);


}//namespace QQ

#endif//end of EDGE_H