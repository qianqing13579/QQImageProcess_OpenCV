//////////////////////////////////////////////////////////////////////////
// ��Ե���
// 2014-12-17,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __EDGE_H__
#define __EDGE_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

namespace QQ
{
typedef unsigned char BYTE;
void SetImageBoundary(Mat &srcImage, int color);//�߽紦��
void Robert(const Mat &srcImage, Mat &image_Robert);//�ݶ��޲���
void Sobel(const Mat &srcImage, Mat &image_Sobel);//�ݶ�����
void ShenJun(const Mat &srcImage, Mat &image_ShenJun);// ������
void RmwShenJunEdge(BYTE *pGryImg, BYTE *pTmpImg, int width, int height, double alpha, BYTE *pResImg);

// 1.0��
DLL_EXPORTS void Canny1(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture = 3, bool L2 = false);
// ʹ��OpenCV�ĽǶ��ж�
DLL_EXPORTS void Canny2(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture = 3, bool L2 = false);
// 1.0�Ľ��棬��Canny1�������ο�OpenCV�ķ���תΪ����ֵ���
// �������ԣ�Canny3��Ч����Canny2Ч������һ�£����þ���ֵ�����ã���ֱ��ʹ��cv::cannyЧ��Ҳ����һ��
DLL_EXPORTS void Canny3(const Mat &srcImage, Mat &dstImage, double lowThreshold, double highThreshold, int sizeOfAperture = 3, bool L2 = false);

// ϸ���㷨
// ��һ��ʵ�ַ�ʽ
DLL_EXPORTS void thin1(const Mat &src, Mat &dst, const int iterations);
// OpenCV/ximgprocģ��ʵ�ַ�ʽ,���Ա�����OpenCV��ʵ�ַ�ʽЧ������
DLL_EXPORTS void thin2( Mat &src, Mat &dst);


}//namespace QQ

#endif//end of EDGE_H
