//////////////////////////////////////////////////////////////////////////
// �ݶ�ֱ��ͼ
// 2017-8-3,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __GRADIENT_HISTOGRAM_H__
#define __GRADIENT_HISTOGRAM_H__

#include"CommonDefinition.h"
#include"opencv2/opencv.hpp"

using namespace cv;
using namespace std;

namespace QQ
{

/*�����ݶ�ͼ���ݶȷ���ͼ(�ݶȴ�С�����ݶȷ�������ֵ)
 ����ǲ�ɫͼ����ȡ�ݶȴ�С�����Ǹ�ͨ��
 gradient:�ݶ�ͼ(ǿ��ͼ)
 gradientAngle:�ݶȷ���ͼ
 numberOfBins:bin�ĸ���
 signedGradient:�Ƿ�����з����ݶȷ���
 
 ΪʲôComputeGradient��ComputeGradient_HOG�ڼ���3ͨ��ͼ���ʱ�򣬸������ػ��в��죿
 ��Ϊ�ڼ����ĸ�ͨ���ݶȴ�С����ʱ��ComputeGradient�Ǵӵ�1����3�Ƚϣ���ComputeGradient_HOG�Ǵӵ�3����1�Ƚϣ����ǲ��ȶ��ıȽϷ�ʽ�������1����
 ��3���ݶȴ�С��ͬ����ôComputeGradient�ͻ�ȡ��1��ͨ������ComputeGradient_HOG�ͻ�ȡ��3��ͨ������������ͨ��ֻ�ܱ�֤fx0*fx0 + fy0*fy0��ͬ�������ܱ�֤
 fx0��fy0����ͬ�ģ����Ի����в�𣬵��ǲ���Ӱ��ʹ��
*/
DLL_EXPORTS	void ComputeGradient(const Mat &srcImage, Mat &gradient, Mat &gradientAngle, const int numberOfBins, bool signedGradient=false);


/* �����ݶ�ͼ���ݶȷ���ͼ(�ݶȴ�С���ݶȷ������˲�ֵ)
gradient:�ݶ�ͼ��˫ͨ��(�ֱ���ÿ�����ص��Ӧ������bin��Ȩֵ)
gradientAngle���ݶȷ���ͼ��˫ͨ��(�ֱ���ÿ�����ص��Ӧ������bin)

�������ԣ�ComputeGradient2��ComputeGradient_HOG���һ��
*/
DLL_EXPORTS	void ComputeGradient2(const Mat &srcImage, Mat &gradient, Mat &gradientAngle, const int numberOfBins, bool signedGradient = false);


/*HOG�м����ݶȷ���ֱ��ͼ����
* �����ݶȴ�С���ݶȷ��������˲�ֵ
* paddingTL,paddingBRĬ��Ϊ��(��������ͼ��)
*/
DLL_EXPORTS void ComputeGradient_HOG(const Mat& img, Mat& grad, Mat& qangle, int nbins, bool signedGradient=false,bool gammaCorrection = false, Size paddingTL=Size(), Size paddingBR=Size());





}// namesapce QQ


#endif
