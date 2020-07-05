# QQImageProcess_OpenCV
## 简介
平时写的一些小程序，包括了博客中的LBP的实现，HOG测试程序等相关内容


***
## 使用说明
注：
1. 下文中的当前目录为github工程根目录
2. OpenCV只提供了VS2013版本和Ubuntu16.04版本，如果需要其他版本，需要重新编译OpenCV。
### Windows
#### VS
1. 解压Resource/3rdParty.tar.gz到当前目录，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
2. 使用VS2013直接打开Windows目录下的VS3013工程
3. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在“Windows/VS2013/QQImageProcess_OpenCV/QQImageProcess_OpenCV/”目录中看到结果图像LBPImage.jpg

#### QT
1. 解压Resource/3rdParty.tar.gz到当前目录，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
2. 使用QT打开QQImageProcess_OpenCV.pro
3. 创建如下路径"Build/Windows/QT/"，并该将目录设置为QT的构建目录(注意，不能随便改动构建目录，否则工程无法正常运行)，比如使用QT5.5的32位版本,则Debug版本的构建目录为"Build/Windows/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/"
4. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在QT的构建目录(Build/Windows/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/)中看到结果图像LBPImage.jpg

### Linux
#### QT
1. 解压Resource/3rdParty.tar.gz到当前目录，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
2. 使用QT打开QQImageProcess_OpenCV.pro
3. 创建如下路径"Build/Linux/QT/"，并该将目录设置为QT的构建目录(注意，不能随便改动构建目录，否则工程无法正常运行)，比如使用QT5.10的64位版本,则Debug版本的构建目录为"Build/Linux/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_10_0_GCC_64bit-Debug/"
4. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在QT的构建目录(Build/Linux/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_10_0_GCC_64bit-Debug/)中看到结果图像LBPImage.jpg

#### cmake
如果你不喜欢使用IDE或者当前无法使用IDE,你也可以在终端直接使用cmake来编译该工程，该工程提供了一个CMakeLists.txt示例,你可以根据自己的需要修改相应的配置,使用cmake的编译流程与下面的qmake基本类似，可以参考下面的qmake编译流程。

#### qmake
由于QT工程提供了一个.pro文件，所以可以直接使用qmake来编译，具体编译流程如下：
1. 创建构建目录 ./Build/Linux/QT/Release
2. cd ./Build/Linux/QT/Release
3. qmake ../../../../QQImageProcess_OpenCV.pro
4. make -j8
5. 将OpenCV库路径加入环境变量.bashrc中
6. ./QQImageProcess_OpenCV
