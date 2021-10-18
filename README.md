# QQImageProcess_OpenCV
## 简介
平时写的一些小程序，包括了博客中的LBP的实现，HOG测试程序等相关内容,代码未经过严格测试，项目中请慎用。


## 目录结构
```

├── Resource:资源文件
│   └── Image:示例图像
│   └── InstallOpenCV:安装OpenCV需要的文件
│   └── 3rdParty.tar.gz:第三方依赖库，比如OpenCV等
├── Src:源码文件
│   └── ImageProcess:图像处理模块
│        └── Edge.h:边缘检测算法，主要包括Sobel算法、经典的Canny算法等(Canny的解读见博客:https://blog.csdn.net/qianqing13579/article/details/51708493)
│        └── Filter.h:滤波算法，主要包括均值滤波、中值滤波、高斯滤波等
│        └── GeometryTransformation.h:几何变换，主要包括仿射变换(原理的解读见博客:https://blog.csdn.net/qianqing13579/article/details/52504089)
│        └── GrayTransformation.h:灰度变换，主要包括图像灰度化算法和直方图均衡化算法(直方图均衡化算法见博客:https://blog.csdn.net/qianqing13579/article/details/52422364)
│        └── Histogram.h:直方图算法，主要包括计算图像的直方图(只支持灰度图)
│        └── IntegralImage.h:积分图
│        └── LBP.h:计算LBP特征(相关博客参考:https://blog.csdn.net/qianqing13579/article/details/49406563)
│        └── ProjectXY.h:投影
│   └── Utility:包括通用定义，常用工具
│   └── ImageProcessTest.h:示例程序，包含了各种算法的示例程序，比如边缘检测算法的示例TestEdge()
│   └── main.cpp:main函数
```

## 使用说明

本工程提供了多种编译方式, 包括了Windows下的QT和VS,Linux下的QT和cmake等方式,你可以选择一种最适合你的方式。

注：
1. 下文中的当前目录为工程根目录

### 安装OpenCV
Resource/3rdParty.tar.gz提供了编译好的OpenCV库，目前只支持VS2013和Ubuntu16.04，如果需要其他版本，需要重新编译OpenCV。Resource/InstallOpenCV/目录下提供了安装OpenCV需要的文件。

#### 如何使用 Resource/3rdParty.tar.gz

1. 对于Linux系统需要先安装OpenCV依赖：sh ./Resource/InstallOpenCV/InstallOpenCVDependences.sh，该脚本同时提供了Ubuntu和CentOS两种系统下的安装方式
2. 解压Resource/3rdParty.tar.gz，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
3. 对于Windows系统可以直接将需要的.dll文件拷贝到可执行文件目录，对于Linux系统需要将OpenCV加入~/.bashrc环境变量

### Windows
#### VS
1. 使用VS2013打开Windows目录下的VS3013工程
2. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在“Windows/VS2013/QQImageProcess_OpenCV/QQImageProcess_OpenCV/”目录中看到结果图像LBPImage.jpg

#### QT
1. 使用QTCreator打开QQImageProcess_OpenCV.pro
2. 创建如下路径"Build/Windows/QT/"，并该将目录设置为QT的构建目录(注意，不能随便改动构建目录，否则工程无法正常运行)，比如使用QT5.5的32位版本,则Debug版本的构建目录为"Build/Windows/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/"
3. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在QT的构建目录(Build/Windows/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/)中看到结果图像LBPImage.jpg

### Linux
#### QT
1. 使用QTCreator打开QQImageProcess_OpenCV.pro
2. 创建如下路径"Build/Linux/QT/"，并该将目录设置为QT的构建目录(注意，不能随便改动构建目录，否则工程无法正常运行)，比如使用QT5.10的64位版本,则Debug版本的构建目录为"Build/Linux/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_10_0_GCC_64bit-Debug/"
3. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在QT的构建目录(Build/Linux/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_10_0_GCC_64bit-Debug/)中看到结果图像LBPImage.jpg

#### cmake
```
mkdir -p ./Build/Linux/cmake/Release
cd ./Build/Linux/cmake/Release
cmake ../../../../
make -j8
./QQImageProcess_OpenCV
```