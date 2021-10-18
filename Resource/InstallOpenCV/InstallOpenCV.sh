#! /bin/sh

pathOfOpenCV="./Resource/OpenCV_3.4.2/" # OpenCV路径
directoryOfBuild="Build" # 构建目录
pathOfInstall="~/Software/OpenCV_3.4.2/" # 安装路径，如果为空，则默认安装到系统/usr/local/中
pathOfExtraModel="" # 扩展模块路径(opencv_contrib/modules)，如果没有设置为空

# 创建目录
if [ ! -d ${pathOfInstall} ]; then
  mkdir -p ${pathOfInstall}
fi
cd ${pathOfOpenCV}
mkdir ${directoryOfBuild} 
cd ${directoryOfBuild}

cmake \
	-D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=${pathOfInstall}  \
	-D OPENCV_EXTRA_MODULES_PATH=${pathOfExtraModel} \
	-D WITH_TBB=OFF \
	-D WITH_CUDA=OFF \
	-D WITH_OPENGL=OFF \
	-D BUILD_SHARED_LIBS=ON ..

make -j48
make install