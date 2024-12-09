#include "cameracapture.h"

#include "opencv2/opencv.hpp"
#include <iostream>
#include <QTimer>


using namespace cv;
using namespace std;

CameraCapture::CameraCapture(QWidget *parent){
    this->cap.open(0); // open the default camera
    this->cap.set(CAP_PROP_FRAME_WIDTH,10000);
    this->cap.set(CAP_PROP_FRAME_HEIGHT,10000);
    cout<<"max width: "<<this->cap.get(CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"max height: "<<this->cap.get(CAP_PROP_FRAME_HEIGHT)<<endl;
    this->cap.set(CAP_PROP_FRAME_WIDTH,640);
    this->cap.set(CAP_PROP_FRAME_HEIGHT,480);
    cout<<"width: "<<this->cap.get(CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"height: "<<this->cap.get(CAP_PROP_FRAME_HEIGHT)<<endl;
}

void CameraCapture::set(){
    Mat image;
    this->cap >> image;
    //conversion from Mat to QImage
    Mat dest;
    cvtColor(image, dest,cv::COLOR_BGR2RGB);
    this->screen = QImage(dest.data, dest.cols, dest.rows,QImage::Format_RGB888);
}

QImage CameraCapture::getScreen(){
    return this->screen;
}

CameraCapture::~CameraCapture(){
}
