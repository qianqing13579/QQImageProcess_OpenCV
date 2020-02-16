//////////////////////////////////////////////////////////////////////////
// 测试类 ImageProcessTest.h 

//////////////////////////////////////////////////////////////////////////

#ifndef __QQ_IMAGEPROCESS_TEST_H__
#define __QQ_IMAGEPROCESS_TEST_H__

#include "opencv2/opencv.hpp"
#include"All.h"
#include<stdlib.h>
#include <fstream> 
#include <string> 
using namespace cv;
using namespace std;


////////////////////////////图像路径//////////////////////////////////////////////
#ifdef _WIN32
#define LENA_GRAY		"../../../../Resource/Image/Gray/Lena512.bmp"
#define LENA_COLOR		"../../../../Resource/Image/Color/Lena800.bmp"
#define BEAUTY_COLOR	"../../../../Resource/Image/Color/Beauty.bmp"
#define BEAUTY_GRAY		"../../../../Resource/Image/Gray/Beauty.bmp"
#define CAMERA_GRAY		"../../../../Resource/Image/Gray/Camera256.bmp"
#define CAMERA_COLOR	"../../../../Resource/Image/Color/Camera256.bmp"
#else
#define LENA_GRAY		"../../../Resource/Image/Gray/Lena512.bmp"
#define LENA_COLOR		"../../../Resource/Image/Color/Lena800.bmp"
#define BEAUTY_COLOR	"../../../Resource/Image/Color/Beauty.bmp"
#define BEAUTY_GRAY		"../../../Resource/Image/Gray/Beauty.bmp"
#define CAMERA_GRAY		"../../../Resource/Image/Gray/Camera256.bmp"
#define CAMERA_COLOR	"../../../Resource/Image/Color/Camera256.bmp"
#endif

//源图像路径
#define GRAY	"D:/Image/Gray/"
#define COLOR	"D:/Image/Color/"

//目标图像路径
#define RESULT			"D:/Image/Temp/"

//角度转换
#define DEGREE2RADIAN(x) (x*CV_PI/180)//角度转弧度
#define RADIAN2DEGREE(x) (x*180/CV_PI)//弧度转角度

using namespace cv;

class ImageProcessTest
{
public:
	// 简单的测试算法的平均时间
	void TestMeanTime();


	void TestAccessImage();//测试访问图像的各种方法
	void TestConnectImage();//测试拼接图像
	void TestProject();//测试投影
	void TestMat_IplImage4ALigned();//测试Mat和IplImage是否4字节对齐
	void TestDrawHist();//测试画直方图
	void TestImageMedianMinMax();//测试图像的亮度中值，最大值，最小值
	void TestSmooth();//测试自己的滤波器
	void TestConvolution();// 测试卷积
	void TestEdge();//测试边缘检测
	void TestCanny();
	void TestGeometryTransformation();//测试几何变换
	void TestRotate();
	void TestCvtColor(); // 测试灰度变换
	void TestEqualiz();
	void TestThin();
	void TestGaussianBlur();
	void TestGradientHist();

	// LBP的测试
	void LBP_SVM();
	void LBP_SVM_Rotation();
	void TestLBP(); // 测试LBP特征

};

#endif



