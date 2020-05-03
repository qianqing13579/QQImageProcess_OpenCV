//////////////////////////////////////////////////////////////////////////
// ͶӰͼ
// 2014-11-28,by QQ
//
// Please contact me if you find any bugs, or have any suggestions.
// Contact:
//		Email:654393155@qq.com
//		Blog:http://blog.csdn.net/qianqing13579
//////////////////////////////////////////////////////////////////////////

#ifndef __PROJECTXY_H__
#define __PROJECTXY_H__

#include "CommonDefinition.h"
#include "opencv2/opencv.hpp"

using namespace cv;

namespace QQ
{


///////////////////////////ͶӰ///////////////////////////////////////////////
void ProjectX(const Mat &srcImage,int height_ProjectXImage);//��ֱͶӰ(��X�᷽��ͶӰ)
void ProjectY(const Mat &srcImage,int width_ProjectYImage);//ˮƽͶӰ(��Y�᷽��ͶӰ)


/////////////////////////��ͶӰͼ/////////////////////////////////////////////////
//����ֱͶӰͼ(��X�᷽��ͶӰ)
DLL_EXPORTS void DrawProjectXImage(Mat &image_ProjectX,int height_ProjectXImage,int *projectX,int projectXSize);
//��ˮƽͶӰ(��Y�᷽��ͶӰ)
DLL_EXPORTS void DrawProjectYImage(Mat &image_ProjectY,int width_ProjectYImage,int *projectY,int projectYSize);


/////////////////////////////ƴ��ͼ��/////////////////////////////////////////////
DLL_EXPORTS void ConnectTwoImage_UpDown(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst);//����ƴ������ͼ��
DLL_EXPORTS void ConnectTwoImage_LeftRight(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst);//����ƴ������ͼ��


}//end of namespace QQ


#endif
