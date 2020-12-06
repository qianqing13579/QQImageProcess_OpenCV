//////////////////////////////////////////////////////////////////////////
// 投影图
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


///////////////////////////投影///////////////////////////////////////////////
void ProjectX(const Mat &srcImage,int height_ProjectXImage);//垂直投影(向X轴方向投影)
void ProjectY(const Mat &srcImage,int width_ProjectYImage);//水平投影(向Y轴方向投影)


/////////////////////////画投影图/////////////////////////////////////////////////
//画垂直投影图(向X轴方向投影)
DLL_EXPORTS void DrawProjectXImage(Mat &image_ProjectX,int height_ProjectXImage,int *projectX,int projectXSize);
//画水平投影(向Y轴方向投影)
DLL_EXPORTS void DrawProjectYImage(Mat &image_ProjectY,int width_ProjectYImage,int *projectY,int projectYSize);


/////////////////////////////拼接图像/////////////////////////////////////////////
DLL_EXPORTS void ConnectTwoImage_UpDown(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst);//纵向拼接两幅图像
DLL_EXPORTS void ConnectTwoImage_LeftRight(const Mat &srcImage1,const Mat &srcImage2,Mat &image_Dst);//横向拼接两幅图像


}//end of namespace QQ


#endif
