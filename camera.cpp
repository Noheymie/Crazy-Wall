#include "camera.h"

Camera::Camera()
{
    this->cap.open(0); //On ouvre la webcam principale
    // Paramètres de la frame pour la caméra
    this->cap.set(CAP_PROP_FRAME_WIDTH,10000);
    this->cap.set(CAP_PROP_FRAME_HEIGHT,10000);
    cout<<"max width: "<<this->cap.get(CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"max height: "<<this->cap.get(CAP_PROP_FRAME_HEIGHT)<<endl;
    this->cap.set(CAP_PROP_FRAME_WIDTH,640);
    this->cap.set(CAP_PROP_FRAME_HEIGHT,480);
    cout<<"width: "<<this->cap.get(CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"height: "<<this->cap.get(CAP_PROP_FRAME_HEIGHT)<<endl;
}

void Camera::setImg(QLabel l){
    this->cap >> this->img;
    Mat frame_inv;
    flip(this->img,frame_inv,1);
    this->img = frame_inv;
    cv::cvtColor(image, dest,cv::COLOR_BGR2RGB);
    QImage screen = QImage(dest.data, dest.cols, dest.rows,QImage::Format_RGB888);
    l->setPixmap(QPixmap::fromImage(screen));
}

Mat Camera::getImg(){
    return this->img;
}

void Camera::setScreen(){
    this->screen = this->img;
}

int Camera::getPosHand(){

}

void Camera::loop(int argc, char *argv[], QKeyEvent * event, QLabel l){
    CommandLineParser parser(argc, argv,
        "{ h help           | false     | print this help message }"
        "{ p proto          | pose.prototxt          | (required) model configuration, e.g. hand/pose.prototxt }"
        "{ m model          | pose_iter_102000.caffemodel         | (required) model weights, e.g. hand/pose_iter_102000.caffemodel }"
        "{ i image          | 5doigts.PNG          | (required) path to image file (containing a single person, or hand) }"
        "{ d dataset        | HAND          | specify what kind of model was trained. It could be (COCO, MPI, HAND) depends on dataset. }"
        "{ width            |  368      | Preprocess input image by resizing to a specific width. }"
        "{ height           |  368      | Preprocess input image by resizing to a specific height. }"
        "{ t threshold      |  0.1      | threshold or confidence value for the heatmap }"
        "{ s scale          |  0.003922 | scale for blob }"
    );
    while(this->cap.isOpened()){
        this->setImg(l);
        if(event->key()==Qt::Key_Escape){
            this->setScreen();
        }
    }
}
