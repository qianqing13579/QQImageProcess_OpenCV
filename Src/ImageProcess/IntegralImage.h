//////////////////////////////////////////////////////////////////////////
// ����ͼ
// 2014-12-1,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __INTEGRALIMAGE_H__
#define __INTEGRALIMAGE_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"

using namespace cv;

namespace QQ
{

/////////////////////////////����ͼ/////////////////////////////////////////////
/*V3.0

����ͼ��������ô���㣺
1. ����ͼ������һ�У�ȫ����ֵΪ0��������ͳһ����������ͼ��ǰ����ֵ=��ǰ���ۼӺ�+��һ�ж�Ӧ���ص�ֵ��
2. ����ͼ�������߶���һ��(OpenCV������)����������Ҳͳһ������ͼ��ǰ���ص�ֵ=ǰ��һ�����ص�ֵ+�����ص�ֵ+�������ص�ֵ��

*/
DLL_EXPORTS void CalculateIntegralImage_Old(const Mat &srcImage, Mat &image_Integral);//ԭʼ����
DLL_EXPORTS void CalculateIntegralImage(const Mat &srcImage,Mat &image_Integral);//�Ľ��ķ���

//�������ַ�ʽ�ǳ���Ч
//V2.0
void CalculateIntegralImage_2(const Mat &srcImage,Mat &image_Integral);//�������е�ַ+[]�������������ͼ
//V1.0
void CalculateIntegralImage_1(const Mat &srcImage,Mat &image_Integral);//����at�����������ͼ



}//namespace QQ

#endif
