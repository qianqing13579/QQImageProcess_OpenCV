# QQImageProcess_OpenCV
## 简介
平时写的一些小程序，包括了博客中的LBP的实现，HOG测试程序等相关内容

## 使用说明
### Windows
#### VS
1. 解压Resource/3rdParty.tar.gz到当前目录，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
2. 使用VS2013直接打开Windows目录下的VS3013工程
3. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在“Windows/VS2013/QQImageProcess_OpenCV/QQImageProcess_OpenCV/”目录中看到结果图像LBPImage.jpg

注：原始工程使用的是VS2013，OpenCV是使用VS2013编译的，如果你使用的是其他版本的VS，需要修改相关的配置。
#### QT
1. 解压Resource/3rdParty.tar.gz到当前目录，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
2. 使用QT打开QQImageProcess_OpenCV.pro
3. 在工程的根目录中依次创建如下路径"Build/Windows/QT/"，并该将目录设置为QT的构建目录(注意，不能随便改动构建目录，否则工程无法正常运行)，比如使用QT5.5的32位版本,则Debug版本的构建目录为"Build/Windows/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/"
4. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在QT的构建目录中看到结果图像LBPImage.jpg

### Linux
#### QT
1. 解压Resource/3rdParty.tar.gz到当前目录，解压完路径为"Resource/3rdParty/OpenCV_3.4.2/"
2. 使用QT打开QQImageProcess_OpenCV.pro
3. 在工程的根目录中依次创建如下路径"Build/Linux/QT/"，并该将目录设置为QT的构建目录(注意，不能随便改动构建目录，否则工程无法正常运行)，比如使用QT5.10的64位版本,则Debug版本的构建目录为"Build/Linux/QT/build-QQImageProcess_OpenCV-Desktop_Qt_5_10_0_GCC_64bit-Debug/"
4. 在main函数中选择ImageProcessTest类中的一个测试用例，比如TestLBP(),运行程序,可以在QT的构建目录中看到结果图像LBPImage.jpg

注：由于当前OpenCV只支持Ubuntu16.04，如果换成其他版本，需要重新编译OpenCV
#### CMake
如果你不喜欢使用IDE或者当前无法使用IDE,你也可以在终端直接使用cmake来编译该工程，该工程提供了一个CMakeLists.txt示例,你可以根据自己的需要修改相应的配置。
