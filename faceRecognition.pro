#-------------------------------------------------
#
# Project created by QtCreator 2016-05-19T11:25:41
#
#-------------------------------------------------

QT       += core gui

TARGET = faceRecognition
TEMPLATE = app


#INCLUDEPATH += /usr/include/gstreamer-0.10/
#INCLUDEPATH += /usr/include/glib-2.0/
#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/
#INCLUDEPATH += /usr/include/gstreamer-0.10 /usr/include/glib-2.0 /usr/lib/glib-2.0/include /usr/include/libxml2
#CONFIG += link_pkgconfig

#INCLUDEPATH += /usr/include/gstreamer-0.10/
#INCLUDEPATH += /usr/include/glib-2.0/
#PKGCONFIG += gstreamer-0.10 gstreamer-app-0.10

#PKGCONFIG += gstreamer-0.10 gstreamer-app-0.10 glib-2.0

INCLUDEPATH += /home/standby/opencv/opencv-2.4.10/build/lib


LIBS += -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_video -lopencv_highgui -lopencv_objdetect -lopencv_contrib
#LIBS += -L/usr/lib -lgstreamer-0.10 -lgstapp-0.10 -lgstbase-0.10 -lglib-2.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lz -lxml2

SOURCES += main.cpp\
    recognition.cpp \
    objectDetection.cpp

HEADERS  += \
    recognition.h \
    objectDetection.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../../opencv/opencv-2.4.10/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml \
    ../../opencv/opencv-2.4.10/data/haarcascades/haarcascade_eye.xml \
    ../../opencv/opencv-2.4.10/data/lbpcascades/lbpcascade_frontalface.xml \
