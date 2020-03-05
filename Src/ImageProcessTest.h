// Copyright (c) 2014-2020, QQ<654393155@qq.com>, all rights reserved.

// 测试

#ifndef __IMAGEPROCESS_TEST_H__
#define __IMAGEPROCESS_TEST_H__

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
	void LBP_Uniform_SVM(); // LBP等价模式
	void LBP_Rotation_Uniform_SVM(); // LBP旋转不变，等价模式
	void TestLBP(); // 测试LBP特征

};

#endif



