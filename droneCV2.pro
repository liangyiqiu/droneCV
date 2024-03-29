QT += core
QT -= gui

TARGET = droneCV
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2\
                /home/pi/droneCV


LIBS += -lwiringPi
LIBS += -lncurses
LIBS += /usr/local/lib/libopencv_calib3d.so \
/usr/local/lib/libopencv_core.so \
/usr/local/lib/libopencv_features2d.so \
/usr/local/lib/libopencv_flann.so \
/usr/local/lib/libopencv_highgui.so \
/usr/local/lib/libopencv_imgcodecs.so \
/usr/local/lib/libopencv_imgproc.so \
/usr/local/lib/libopencv_ml.so \
/usr/local/lib/libopencv_objdetect.so \
/usr/local/lib/libopencv_photo.so \
/usr/local/lib/libopencv_shape.so \
/usr/local/lib/libopencv_stitching.so \
/usr/local/lib/libopencv_superres.so \
/usr/local/lib/libopencv_videoio.so \
/usr/local/lib/libopencv_video.so \
/usr/local/lib/libopencv_videostab.so


TEMPLATE = app

SOURCES += main.cpp \
    sys.cpp \
    gpio.cpp \
    network.cpp \
    cv.cpp \
    timer.cpp \
    pilot.cpp

HEADERS += \
    sys.hpp \
    gpio.hpp \
    network.hpp \
    cv.hpp \
    timer.hpp \
    pilot.hpp
