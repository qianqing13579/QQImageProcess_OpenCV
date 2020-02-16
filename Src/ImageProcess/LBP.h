//////////////////////////////////////////////////////////////////////////
//  LBP.h (2.0)
// 2015-6-30，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////
// Updated 2016-12-12 01:12:55 by QQ, LBP 1.1,GetMinBinary()函数修改为查找表，提高了计算速度
// Updated 2016-12-13 14:41:58 by QQ, LBP 2.0,先计算整幅图像的LBP特征图，然后计算每个cell的LBP直方图
// Updated 2017-7-29&30 by QQ,add 对ComputeLBPFeatureVector_256和ComputeLBPFeatureVector_Uniform进行了优化:添加了滑动窗口像素查找表

#ifndef __QQ_LBP_H__
#define __QQ_LBP_H__

#include "opencv2/opencv.hpp"
#include<vector>
using namespace std;
using namespace cv;

namespace QQ
{

/*
三种LBP特征的计算步骤基本一致：先计算LBP特征图，然后对每个cell计算特征向量

*/
class LBP
{
public:

	//////////////////////////////////// 计算基本的256维LBP特征向量 ////////////////////////////////////
	void ComputeLBPFeatureVector_256(const Mat &srcImage, Size cellSize,Mat &featureVector);
	void ComputeLBPImage_256(const Mat &srcImage, Mat &LBPImage);// 计算256维LBP特征图
	// 对ComputeLBPFeatureVector_256优化，构建滑动窗口像素查找表
	void ComputeLBPFeatureVector_256_O(const Mat &srcImage, Size cellSize, Mat &featureVector);
	// 对ComputeLBPFeatureVector_256_O的优化，主要优化掉循环中的乘法
	void ComputeLBPFeatureVector_256_O_2(const Mat &srcImage, Size cellSize, Mat &featureVector);
	

	//////////////////////////////////// 计算灰度不变+等价模式LBP特征向量(58种模式) ////////////////////////////////////
	void ComputeLBPFeatureVector_Uniform(const Mat &srcImage, Size cellSize, Mat &featureVector);
	// 计算等价模式LBP特征图
	void ComputeLBPImage_Uniform(const Mat &srcImage, Mat &LBPImage);
	// 对ComputeLBPFeatureVector_Uniform优化，构建滑动窗口像素查找表
	void ComputeLBPFeatureVector_Uniform_O(const Mat &srcImage, Size cellSize, Mat &featureVector);


	//////////////////////////////////// 计算灰度不变+旋转不变+等价模式LBP特征向量(9种模式) ////////////////////////////////////
	void ComputeLBPFeatureVector_Rotation_Uniform(const Mat &srcImage, Size cellSize, Mat &featureVector);
    void ComputeLBPImage_Rotation_Uniform(const Mat &srcImage, Mat &LBPImage); // 计算灰度不变+旋转不变+等价模式LBP特征图,使用查找表
	
	// Test
	void Test();// 测试灰度不变+旋转不变+等价模式LBP
	void TestGetMinBinaryLUT();

private:
	void BuildUniformPatternTable(int *table); // 计算等价模式查找表
	int GetHopCount(int i);// 获取i中0,1的跳变次数

    void ComputeLBPImage_Rotation_Uniform_2(const Mat &srcImage, Mat &LBPImage);// 计算灰度不变+旋转不变+等价模式LBP特征图,不使用查找表
	int ComputeValue9(int value58);// 计算9种等价模式
	int GetMinBinary(int binary);// 通过LUT计算最小二进制
	uchar GetMinBinary(uchar *binary); // 计算得到最小二进制
	
};

}

#endif
