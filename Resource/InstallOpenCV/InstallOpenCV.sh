#! /bin/sh

# 该脚本在directoryOfOpenCV的同级目录执行
# 首先解压好OpenCV
# 打印日志  sh InstallOpenCV.sh 2>&1 | tee InstallOpenCV.log
directoryOfOpenCV="opencv-4.0.0"
directoryOfBuild="Build_NoCUDA" # 注意区分不同编译版本
pathOfInstall="/home/qq/usr/local/OpenCV/OpenCV_4.0.0_NoCUDA/so/" # 安装路径
pathOfExtraModel="" # opencv_contrib 模块路径(opencv_contrib/modules)，如果没有设置为空

# 创建目录
if [ ! -d ${pathOfInstall} ]; then
  mkdir -p ${pathOfInstall}
fi
cd ${directoryOfOpenCV}
mkdir ${directoryOfBuild} 
cd ${directoryOfBuild}

# 如果不指定CMAKE_INSTALL_PREFIX，默认安装到系统/usr/local/中，为了避免OpenCV版本冲突，建议将OpenCV安装到其他目录中。
cmake \
	-D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=${pathOfInstall}  \
	-D OPENCV_EXTRA_MODULES_PATH=${pathOfExtraModel} \
	-D WITH_TBB=OFF \
	-D WITH_CUDA=OFF \
	-D WITH_OPENGL=OFF \
	-D BUILD_SHARED_LIBS=ON ..

make -j8
make install

################### OpenCV4.2.0 CUDA版的安装 ################### 
#! /bin/sh
# directoryOfOpenCV="opencv-4.2.0"
# directoryOfBuild="Build_CUDA" # 注意区分不同编译版本
# pathOfInstall="/root/usr/local/OpenCV/OpenCV_4.2.0_CUDA/so/" # 安装路径
# pathOfExtraModel="/root/Software/OpenCV/opencv-4.2.0/opencv_contrib/modules/" 

# if [ ! -d ${pathOfInstall} ]; then
#   mkdir -p ${pathOfInstall}
# fi
# cd ${directoryOfOpenCV}
# mkdir ${directoryOfBuild} 
# cd ${directoryOfBuild}

# cmake \
# 	-D CMAKE_BUILD_TYPE=RELEASE \
# 	-D CMAKE_INSTALL_PREFIX=${pathOfInstall}  \
# 	-D OPENCV_EXTRA_MODULES_PATH=${pathOfExtraModel} \
# 	-D WITH_TBB=OFF \
# 	-D WITH_CUDA=ON \
# 	-D WITH_OPENGL=OFF \
# 	-D BUILD_opencv_xfeatures2d=OFF \
# 	-D BUILD_SHARED_LIBS=ON ..
# make -j48
# make install

###################  ###################

