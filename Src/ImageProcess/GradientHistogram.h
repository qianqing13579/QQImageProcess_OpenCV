//////////////////////////////////////////////////////////////////////////
// 梯度直方图 GradientHistogram.h
// 2017-8-3，by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:qianqing13579@163.com
//		Blog: http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __QQ_GRADIENT_HISTOGRAM_H__
#define __QQ_GRADIENT_HISTOGRAM_H__

#include <fstream> 
#include"CommonDefinition.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

namespace QQ
{

/*计算梯度图和梯度方向图(梯度大小不在梯度方向做插值)
 如果是彩色图，则取梯度大小最大的那个通道
 gradient:梯度图(强度图)
 gradientAngle:梯度方向图
 numberOfBins:bin的个数
 signedGradient:是否采用有符号梯度方向
 
 为什么ComputeGradient与ComputeGradient_HOG在计算3通道图像的时候，个别像素会有差异？
 因为在计算哪个通达梯度大小最大的时候，ComputeGradient是从第1到第3比较，而ComputeGradient_HOG是从第3到第1比较，这是不稳定的比较方式，如果第1个和
 第3个梯度大小相同，那么ComputeGradient就会取第1个通道，而ComputeGradient_HOG就会取第3个通道，而这两个通道只能保证fx0*fx0 + fy0*fy0相同，并不能保证
 fx0与fy0是相同的，所以会略有差别，但是并不影响使用
*/
DLL_EXPORTS	void ComputeGradient(const Mat &srcImage, Mat &gradient, Mat &gradientAngle, const int numberOfBins, bool signedGradient=false);


/* 计算梯度图和梯度方向图(梯度大小在梯度方向做了插值)
gradient:梯度图，双通道(分别存放每个像素点对应的两个bin的权值)
gradientAngle：梯度方向图，双通道(分别存放每个像素点对应的两个bin)

经过测试：ComputeGradient2与ComputeGradient_HOG结果一致
*/
DLL_EXPORTS	void ComputeGradient2(const Mat &srcImage, Mat &gradient, Mat &gradientAngle, const int numberOfBins, bool signedGradient = false);


/*HOG中计算梯度方向直方图函数
* 其中梯度大小在梯度方向上做了插值
* paddingTL,paddingBR默认为空(用作扩充图像)
*/
DLL_EXPORTS void ComputeGradient_HOG(const Mat& img, Mat& grad, Mat& qangle, int nbins, bool signedGradient=false,bool gammaCorrection = false, Size paddingTL=Size(), Size paddingBR=Size());





}// namesapce QQ


#endif