QT += core
QT -= gui

TARGET = QQImageProcess_OpenCV
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES +=\
    Src/ImageProcess/Edge.cpp \
    Src/ImageProcess/Filter.cpp \
    Src/ImageProcess/GeometryTransformation.cpp \
    Src/ImageProcess/GradientHistogram.cpp \
    Src/ImageProcess/GrayTransformation.cpp \
    Src/ImageProcess/Histogram.cpp \
    Src/ImageProcess/IntegralImage.cpp \
    Src/ImageProcess/LBP.cpp \
    Src/ImageProcess/ProjectXY.cpp \
    Src/Utility/CommonUtility.cpp \
    Src/Utility/Filesystem.cpp \
    Src/Utility/SimpleLog.cpp \
    Src/ImageProcessTest.cpp \
    Src/main.cpp

HEADERS += \
    Src/ImageProcess/Edge.h \
    Src/ImageProcess/Filter.h \
    Src/ImageProcess/GeometryTransformation.h \
    Src/ImageProcess/GradientHistogram.h \
    Src/ImageProcess/GrayTransformation.h \
    Src/ImageProcess/Histogram.h \
    Src/ImageProcess/IntegralImage.h \
    Src/ImageProcess/LBP.h \
    Src/ImageProcess/ProjectXY.h \
    Src/Utility/All.h \
    Src/Utility/CommonDefinition.h \
    Src/Utility/CommonUtility.h \
    Src/Utility/Filesystem.h \
    Src/Utility/SimpleLog.h \
    Src/ImageProcessTest.h

INCLUDEPATH +=./Resource/3rdParty/OpenCV_3.4.2/include/ \
            ./Src/ \
            ./Src/ImageProcess/  \
            ./Src/Utility/

# QT的配置文件中，LIBS路径相对于可执行文件路径，其他相对于项目当前路径
# windows
win32-msvc*{
CONFIG(debug,debug|release) {
LIBS += -L../../../../Resource/3rdParty/OpenCV_3.4.2/x86/vc12/lib/ \
    -lopencv_calib3d342d \
    -lopencv_core342d \
    -lopencv_dnn342d \
    -lopencv_dpm342d \
    -lopencv_features2d342d \
    -lopencv_flann342d \
    -lopencv_highgui342d \
    -lopencv_imgcodecs342d \
    -lopencv_imgproc342d \
    -lopencv_ml342d \
    -lopencv_objdetect342d \
    -lopencv_photo342d \
    -lopencv_shape342d \
    -lopencv_superres342d \
    -lopencv_videoio342d \
    -lopencv_video342d
} else {
LIBS += -L../../../../Resource/3rdParty/OpenCV_3.4.2/x86/vc12/lib/ \
    -lopencv_calib3d342 \
    -lopencv_core342 \
    -lopencv_dnn342 \
    -lopencv_dpm342 \
    -lopencv_features2d342 \
    -lopencv_flann342 \
    -lopencv_highgui342 \
    -lopencv_imgcodecs342 \
    -lopencv_imgproc342 \
    -lopencv_ml342 \
    -lopencv_objdetect342 \
    -lopencv_photo342 \
    -lopencv_shape342 \
    -lopencv_superres342 \
    -lopencv_videoio342 \
    -lopencv_video342
}

}

# linux
linux-g++*{
LIBS += -L../../../Resource/3rdParty/OpenCV_3.4.2/Ubuntu16.04/lib/ \
    -lopencv_calib3d \
    -lopencv_core \
    -lopencv_dnn \
    -lopencv_dpm \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_imgproc \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_photo \
    -lopencv_shape \
    -lopencv_superres \
    -lopencv_videoio \
    -lopencv_video
}
