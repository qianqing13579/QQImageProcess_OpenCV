//////////////////////////////////////////////////////////////////////////
//  LBP.h (2.0)
// 2015-6-30,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////
// Updated 2016-12-12 01:12:55 by QQ, LBP 1.1,GetMinBinary()�����޸�Ϊ���ұ�����˼����ٶ�
// Updated 2016-12-13 14:41:58 by QQ, LBP 2.0,�ȼ�������ͼ���LBP����ͼ��Ȼ�����ÿ��cell��LBPֱ��ͼ
// Updated 2017-7-29 by QQ,add ��ComputeLBPFeatureVector_256��ComputeLBPFeatureVector_Uniform�������Ż�:����˻����������ز��ұ�

#ifndef __LBP_H__
#define __LBP_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"

using namespace cv;

namespace QQ
{

/*
����LBP�����ļ��㲽�����һ�£��ȼ���LBP����ͼ��Ȼ���ÿ��cell������������

*/
class DLL_EXPORTS LBP
{
public:

	//////////////////////////////////// ���������256άLBP�������� ////////////////////////////////////
	void ComputeLBPFeatureVector_256(const Mat &srcImage, Size cellSize,Mat &featureVector);
	void ComputeLBPImage_256(const Mat &srcImage, Mat &LBPImage);// ����256άLBP����ͼ
    void ComputeLBPFeatureVector_256_O(const Mat &srcImage, Size cellSize, Mat &featureVector);// ��ComputeLBPFeatureVector_256�Ż������������������ز��ұ�
    void ComputeLBPFeatureVector_256_O_2(const Mat &srcImage, Size cellSize, Mat &featureVector);// ��ComputeLBPFeatureVector_256_O���Ż�����Ҫ�Ż���ѭ���еĳ˷�
	

	//////////////////////////////////// ����ҶȲ���+�ȼ�ģʽLBP��������(58��ģʽ) ////////////////////////////////////
	void ComputeLBPFeatureVector_Uniform(const Mat &srcImage, Size cellSize, Mat &featureVector);
    void ComputeLBPImage_Uniform(const Mat &srcImage, Mat &LBPImage);// ����ȼ�ģʽLBP����ͼ
    void ComputeLBPFeatureVector_Uniform_O(const Mat &srcImage, Size cellSize, Mat &featureVector);// ��ComputeLBPFeatureVector_Uniform�Ż������������������ز��ұ�


	//////////////////////////////////// ����ҶȲ���+��ת����+�ȼ�ģʽLBP��������(9��ģʽ) ////////////////////////////////////
	void ComputeLBPFeatureVector_Rotation_Uniform(const Mat &srcImage, Size cellSize, Mat &featureVector);
    void ComputeLBPImage_Rotation_Uniform(const Mat &srcImage, Mat &LBPImage); // ����ҶȲ���+��ת����+�ȼ�ģʽLBP����ͼ,ʹ�ò��ұ�

private:
	void BuildUniformPatternTable(int *table); // ����ȼ�ģʽ���ұ�
	int GetHopCount(int i);// ��ȡi��0,1���������
	int ComputeValue9(int value58);// ����9�ֵȼ�ģʽ
	int GetMinBinary(int binary);// ͨ��LUT������С������
	uchar GetMinBinary(uchar *binary); // ����õ���С������

    // Test
    void Test();// ��֤�ҶȲ���+��ת����+�ȼ�ģʽ����
    void TestGetMinBinaryLUT();
	
};

}

#endif
