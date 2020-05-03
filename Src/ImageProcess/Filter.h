//////////////////////////////////////////////////////////////////////////
// �˲�
// 2014-12-8,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __FILTER_H__
#define __FILTER_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"
using namespace cv;

namespace QQ
{

/////////////////////////////////��ֵ�˲�/////////////////////////////////////////
//V1.0
DLL_EXPORTS void Blur_1(const Mat &srcImage, Mat &dstImage, Size size_Aperture);

//V2.0 ��Ч��ֵ�˲�(ͼ��ĸ�Ч���Ҫ��֮��),û�д���߽�
DLL_EXPORTS void Blur_2(const Mat &srcImage, Mat &dstImage, Size size_Aperture);

//V3.0 ��Ч��ֵ�˲�������Ա߽�Ĵ���û��ʹ��copyMakeBorder()����ԭͼ
DLL_EXPORTS void Blur_3(const Mat &srcImage, Mat &dstImage, Size size_Aperture);

//V4.0 ��Ч��ֵ�˲�������Ա߽�Ĵ���,ʹ��copyMakeBorder()����ԭͼ
DLL_EXPORTS void Blur(const Mat &srcImage, Mat &dstImage, Size size_Aperture);


/////////////////////////////////��ֵ�˲�/////////////////////////////////////////
//V2.0 ��ͨ��ֵ�˲�
//�߽�û�д���
//3.0�Ѳ�֧�ֺ����еĻ���������ȡ��ʽ���ú�����������
DLL_EXPORTS void MedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture);//��ֵ�˲�

//V3.0 ��Ч��ֵ�˲�(ͼ��ĸ�Ч���Ҫ��֮��)
//�߽�û�д���
DLL_EXPORTS void FastMedianBlur_1(const Mat &srcImage, Mat &dstImage, int width_Aperture);//��ֵ�˲�

//V4.0 ��Ч��ֵ�˲�(ͼ��ĸ�Ч���Ҫ��֮��)
//�Ľ��ĸ�Ч��ֵ�˲���������OpenCV��ֱ��ͼ
DLL_EXPORTS void FastMedianBlur(const Mat &srcImage, Mat &dstImage, int width_Aperture);



/////////////////////////////////��˹�˲�/////////////////////////////////////////
DLL_EXPORTS void GaussianBlur(const Mat &srcImage, Mat &dstImage, double sigma);


//////////////////////////////�������ھ����������ַ�ʽ///////////////////////////////////////////////////////////
// ������㷽ʽ1��ʹ��caffe�еľ�����㷽ʽ
DLL_EXPORTS void Convolution1(const Mat &srcImage,const Mat &kernel, Mat &dstImage);
DLL_EXPORTS void Convolution12(const Mat &srcImage, const Mat &kernel, Mat &dstImage);// Convolution1���Ż��汾1 

//  ������㷽ʽ2����ÿ���������ڲ���,�÷�������������pooling,ֻ֧�ֵ�ͨ��
DLL_EXPORTS void Convolution2(const Mat &srcImage,const Mat &kernel, Mat &dstImage);
DLL_EXPORTS void Convolution22(const Mat &srcImage, const Mat &kernel, Mat &dstImage); // ͨ�����ұ��Ż�Convolution2



}//namespace QQ

#endif
