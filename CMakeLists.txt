cmake_minimum_required(VERSION 2.9)

# 项目名
project(QQImageProcess_OpenCV)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 设置编译器路径，如果不知道执行cmake命名的时候系统会提示
SET(CMAKE_C_COMPILER "/usr/bin/cc")
SET(CMAKE_CXX_COMPILER "/usr/bin/c++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE release)

add_definitions(-Wall -Wextra -Wno-unused-function)
add_definitions(-fPIC)

# 添加预定义的宏
#ADD_DEFINITIONS(-DDEBUG_INFO -DRK3399_OPENCV)

# 设置包含目录
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src/ ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/ ${CMAKE_CURRENT_SOURCE_DIR}/Src/Utility/  ${CMAKE_CURRENT_SOURCE_DIR}/Resource/3rdParty/OpenCV_3.4.2/include/)
include_directories(${INC_DIR})


# 设置依赖库so路径
set(LINK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resource/3rdParty/OpenCV_3.4.2/Ubuntu16.04/lib/)
link_directories(${LINK_DIR})

# 设置依赖库
link_libraries(pthread  opencv_core opencv_highgui  opencv_imgproc opencv_objdetect opencv_ml opencv_videoio opencv_imgcodecs opencv_dnn )

# 需要编译的源文件
set(SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/Edge.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/Filter.cpp
				  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/GeometryTransformation.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/GradientHistogram.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/GrayTransformation.cpp
				  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/Histogram.cpp
				  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/IntegralImage.cpp
				  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/LBP.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/ImageProcess/ProjectXY.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Utility/CommonUtility.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Utility/SimpleLog.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/Src/Utility/Filesystem.cpp)

# so库文件名
add_library(LibQQImageProcess_OpenCV SHARED ${SOURCE_FILES})

# 可执行文件名
add_executable(QQImageProcess_OpenCV ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp)
