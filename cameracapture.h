#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H
#include "QLabel"
#include <QTimer>
#include "opencv2/opencv.hpp"
#include <iostream>

class CameraCapture
{
    QImage screen;
    cv::VideoCapture cap;
public:
    CameraCapture(QWidget *parent = nullptr);
    ~CameraCapture();
    void set();
    QImage getScreen();
};

#endif // CAMERACAPTURE_H
