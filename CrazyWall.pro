QT       += core gui
QT       += concurrent

equals(QT_MAJOR_VERSION, 5) {
        QT += opengl widgets
} else:equals(QT_MAJOR_VERSION, 6) {
        QT += openglwidgets
}

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# ajout des libs au linker
win32 {
    win32-msvc* {
        LIBS     += opengl32.lib glu32.lib
    } else {
        LIBS     += -lopengl32 -lglu32
    }
}
else {
        LIBS     += -lGL -lGLU
}


SOURCES += \
    game.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    game.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core455 \
    -lopencv_highgui455 \
    -lopencv_imgproc455 \
    -lopencv_imgcodecs455 \
    -lopencv_videoio455 \
    -lopencv_features2d455 \
    -lopencv_calib3d455 \
    -lopencv_dnn455 \
    -lpthread

RESOURCES += \
    texture.qrc
