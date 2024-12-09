// Bibliothèques / Classes à inclure
#include "game.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "opencv2/dnn.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QMovie>

// Namespaces
using namespace cv;
using namespace cv::dnn;
using namespace std;


MainWindow::MainWindow(CommandLineParser *parser, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Constructeur de la MainWindow
{
    /*QPixMap bkgnd("path");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window,bkgnd);
    this->setPalette(palette);
    */
    this->handDetected = false; // On initialise la détection de la main à faux
    this->fingersRaised = {false,false,false,false,false}; // On initialise la détection de chaque doigt à faux : [0,1,2,3,4] = [pouce,index,majeur,annulaire,auriculaire]
    this->p = parser;
    this->cap.open(0); // On ouvre le flux vidéo de la webcam par défaut
    //this->cap.set(CAP_PROP_FRAME_WIDTH,10000);
    //this->cap.set(CAP_PROP_FRAME_HEIGHT,10000);
    this->cap.set(CAP_PROP_FRAME_WIDTH,640);
    this->cap.set(CAP_PROP_FRAME_HEIGHT,480);
    QTimer* timer = new QTimer(this); // On démarre un QTimer
    ui->setupUi(this);
    //connect(&this->watcher,SIGNAL(progressValueChanged(int)),this,SLOT(updateAll()));
    //connect(timer,SIGNAL(timeout()),this,SLOT(updateCapture()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateAll()));
    //connect(timer, SIGNAL(timeout()),this, SLOT(updateAll())); // On update la vidéo
    timer->start(16); // 60 fps
}

const int POSE_PAIRS[20][2] =
{   // Main
    {0,1}, {1,2}, {2,3}, {3,4},         // Pouce
    {0,5}, {5,6}, {6,7}, {7,8},         // Index
    {0,9}, {9,10}, {10,11}, {11,12},    // Majeur
    {0,13}, {13,14}, {14,15}, {15,16},  // Annulaire
    {0,17}, {17,18}, {18,19}, {19,20}   // Auriculaire
};

void MainWindow::checkHands(Mat img){
    /*
     * Entrée : Mat img = Image prise en capture d'écran, sur laquelle on va détecter les doigts
     * But : On utilise le CommandLineParser ppur détecter les doigts de la matrice img
     *
    */
    // Informations tirées du CommandLineParser
    String modelTxt = samples::findFile(this->p->get<String>("proto"));
    String modelBin = samples::findFile(this->p->get<String>("model"));
    String dataset = this->p->get<String>("dataset");
    int W_in = this->p->get<int>("width");
    int H_in = this->p->get<int>("height");
    float thresh = this->p->get<float>("threshold");
    float scale  = this->p->get<float>("scale");

    int npairs, nparts; // npairs = nombre de parties reconnues sur la main ; nparts = nombre de liens entre chaque partie de la main
    if (!dataset.compare("HAND")) {
        npairs = 20;
        nparts = 22;
    }
    else
    {
        std::cerr << "Can't interpret dataset parameter: " << dataset << std::endl;
        exit(-1);
    }
    // Lecture du modèle network
    Net net = readNet(modelBin, modelTxt);
    //Flip de l'image en miroir
    //Mat img_inv;
    //flip(img,img_inv,1);
    //img = img_inv;
    // On envoie l'image dans le network
    Mat inputBlob = blobFromImage(img, scale, Size(W_in, H_in), Scalar(0, 0, 0), false, false);
    net.setInput(inputBlob);
    Mat result = net.forward();
    // Le resultat est un array de "Heatmaps", avec la probabilité que le membre du corps soit localisé à la position (x,y)
    int H = result.size[2];
    int W = result.size[3];
    // On trouve la position de chaque partie du corps
    vector<Point> points(22);
    for (int n=0; n<nparts; n++)
    {
        // On coupe la heatmap de chaque partie du corps
        Mat heatMap(H, W, CV_32F, result.ptr(0,n));
        // 1 max par heatmap
        Point p(-1,-1),pm;
        double conf;
        minMaxLoc(heatMap, 0, &conf, 0, &pm);
        if (conf > thresh)
            p = pm;
        points[n] = p;
    }
    // On connecte les parties du corps et on les dessine
    float SX = float(img.cols) / W;
    float SY = float(img.rows) / H;
    vector<Point2f> handPoints(npairs);
    for (int n=0; n<npairs; n++)
    {
        // On regarde
        Point2f a = points[POSE_PAIRS[n][0]];
        Point2f b = points[POSE_PAIRS[n][1]];
        // we did not find enough confidence before
        if (a.x<=0 || a.y<=0 || b.x<=0 || b.y<=0){
            handPoints[n]=Point2f(0.0,0.0);
        }
        else{
            // scale to image size
            a.x*=SX; a.y*=SY;
            b.x*=SX; b.y*=SY;
            handPoints[n]=a;
        }
        /*
        cout << "==="<<endl;
        cout << "Point a : "<<a.x << ";"<<a.y<<endl;
        cout << "Point b : "<<b.x << ";"<<b.y<<endl;*/
        line(img, a, b, Scalar(0,200,0), 2);
        circle(img, a, 3, Scalar(0,0,200), -1);
        circle(img, b, 3, Scalar(0,0,200), -1);
    }
    // Tests pour savoir quel doigt est baissé ou levé
    this->handDetected = true;
    for (int i=0;i<npairs;i++){
        if((handPoints[i].x==0)&&(handPoints[i].y==0)){
            this->handDetected=false;
        }
    }
    if(this->handDetected){
        cout<<"Main détecté"<<endl;
        // Pouce
        if(handPoints[4].x>=handPoints[2].x){
            cout << "Pouce baissé"<< endl;
            this->fingersRaised[0]=false;
        }
        else{
            cout <<"Pouce levé"<<endl;
            this->fingersRaised[0]=true;
        }
        // Index
        if(handPoints[8].y<=handPoints[6].y){
            cout << "Index baissé"<< endl;
            this->fingersRaised[1]=false;
        }
        else{
            cout <<"Index levé"<<endl;
            this->fingersRaised[1]=true;
        }
        // Majeur
        if(handPoints[12].y<=handPoints[10].y){
            cout << "Majeur baissé"<< endl;
            this->fingersRaised[2]=false;
        }
        else{
            cout <<"Majeur levé"<<endl;
            this->fingersRaised[2]=true;
        }
        // Annulaire
        if(handPoints[16].y<=handPoints[14].y){
            cout << "Annulaire baissé"<< endl;
            this->fingersRaised[3]=false;
        }
        else{
            cout <<"Annulaire levé"<<endl;
            this->fingersRaised[3]=true;
        }        // Auriculaire
        if(handPoints[20].y<=handPoints[18].y){
            cout << "Auriculaire baissé"<< endl;
            this->fingersRaised[4]=false;
        }
        else{
            cout <<"Auriculaire levé"<<endl;
            this->fingersRaised[4]=true;
        }
    }
    else{
        cout << "Main non détectée" << endl;
    }
    this->handPointsPosition = handPoints;
    updateHand3D();
}

void MainWindow::paintHand(){
    this->scene_ = new QGraphicsScene(ui->HandGraphicsView);
    this->scene_->setSceneRect(0,0,ui->HandGraphicsView->width(),ui->HandGraphicsView->height());
    ui->HandGraphicsView->setScene(this->scene_);
    ui->HandGraphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    if(this->handDetected){
            QPen penBase(Qt::blue);
            QGraphicsRectItem *baseHand = new QGraphicsRectItem(50,125,75,45);
            baseHand->setPen(penBase);
            baseHand->setBrush(Qt::blue);
            this->scene_->addItem(baseHand);
            if(this->handPointsPosition[2].x>this->handPointsPosition[17].x){
                if(this->fingersRaised[0]){
                    QGraphicsLineItem *thumb = new QGraphicsLineItem(125.0,145.0,145.0,125.0);
                    QPen penThumb(Qt::red);
                    penThumb.setWidth(15);
                    thumb->setPen(penThumb);
                    this->scene_->addItem(thumb);
                }
                else{
                    QGraphicsLineItem *thumb = new QGraphicsLineItem(125.0,145.0,135.0,120.0);
                    thumb->setPos(this->handPointsPosition[2].x,this->handPointsPosition[2].y);
                    QPen penThumb(Qt::darkRed);
                    penThumb.setWidth(15);
                    thumb->setPen(penThumb);
                    this->scene_->addItem(thumb);
                }
                if(this->fingersRaised[1]){
                    QGraphicsLineItem *pinkie = new QGraphicsLineItem(115.0,125.0,115.0,70.0);
                    QPen penPinkie(Qt::cyan);
                    penPinkie.setWidth(15);
                    pinkie->setPen(penPinkie);
                    this->scene_->addItem(pinkie);
                }
                else{
                    QGraphicsLineItem *pinkie = new QGraphicsLineItem(115.0,125.0,115.0,90.0);
                    QPen penPinkie(Qt::darkCyan);
                    penPinkie.setWidth(15);
                    pinkie->setPen(penPinkie);
                    this->scene_->addItem(pinkie);
                }
                if(this->fingersRaised[2]){
                    QGraphicsLineItem *middle = new QGraphicsLineItem(95.0,125.0,95.0,60.0);
                    QPen penMiddle(Qt::green);
                    penMiddle.setWidth(15);
                    middle->setPen(penMiddle);
                    this->scene_->addItem(middle);
                }
                else{
                    QGraphicsLineItem *middle = new QGraphicsLineItem(95.0,125.0,95.0,95.0);
                    QPen penMiddle(Qt::darkGreen);
                    penMiddle.setWidth(15);
                    middle->setPen(penMiddle);
                    this->scene_->addItem(middle);
                }

                if(this->fingersRaised[3]){
                    QGraphicsLineItem *ring = new QGraphicsLineItem(75.0,125.0,75.0,65.0);
                    QPen penRing(Qt::magenta);
                    penRing.setWidth(15);
                    ring->setPen(penRing);
                    this->scene_->addItem(ring);
                }
                else{
                    QGraphicsLineItem *ring = new QGraphicsLineItem(75.0,125.0,75.0,100.0);
                    QPen penRing(Qt::darkMagenta);
                    penRing.setWidth(15);
                    ring->setPen(penRing);
                    this->scene_->addItem(ring);
                }
                if(this->fingersRaised[4]){
                    QGraphicsLineItem *small = new QGraphicsLineItem(55.0,125.0,55.0,75.0);
                    QPen penSmall(Qt::yellow);
                    penSmall.setWidth(15);
                    small->setPen(penSmall);
                    this->scene_->addItem(small);
                }
                else{
                    QGraphicsLineItem *small = new QGraphicsLineItem(55.0,125.0,55.0,115.0);
                    QPen penSmall(Qt::darkYellow);
                    penSmall.setWidth(15);
                    small->setPen(penSmall);
                    this->scene_->addItem(small);
                }
            }
            else{
                if(this->fingersRaised[0]){
                    QGraphicsLineItem *thumb = new QGraphicsLineItem(50.0,145.0,30.0,125.0);
                    QPen penThumb(Qt::red);
                    penThumb.setWidth(15);
                    thumb->setPen(penThumb);
                    this->scene_->addItem(thumb);
                }
                else{
                    QGraphicsLineItem *thumb = new QGraphicsLineItem(50.0,145.0,30.0,120.0);
                    thumb->setPos(this->handPointsPosition[2].x,this->handPointsPosition[2].y);
                    QPen penThumb(Qt::darkRed);
                    penThumb.setWidth(15);
                    thumb->setPen(penThumb);
                    this->scene_->addItem(thumb);
                }
                if(this->fingersRaised[1]){
                    QGraphicsLineItem *pinkie = new QGraphicsLineItem(55.0,125.0,55.0,70.0);
                    QPen penPinkie(Qt::cyan);
                    penPinkie.setWidth(15);
                    pinkie->setPen(penPinkie);
                    this->scene_->addItem(pinkie);
                }
                else{
                    QGraphicsLineItem *pinkie = new QGraphicsLineItem(55.0,125.0,55.0,90.0);
                    QPen penPinkie(Qt::darkCyan);
                    penPinkie.setWidth(15);
                    pinkie->setPen(penPinkie);
                    this->scene_->addItem(pinkie);
                }
                if(this->fingersRaised[2]){
                    QGraphicsLineItem *middle = new QGraphicsLineItem(75.0,125.0,75.0,60.0);
                    QPen penMiddle(Qt::green);
                    penMiddle.setWidth(15);
                    middle->setPen(penMiddle);
                    this->scene_->addItem(middle);
                }
                else{
                    QGraphicsLineItem *middle = new QGraphicsLineItem(75.0,125.0,75.0,95.0);
                    QPen penMiddle(Qt::darkGreen);
                    penMiddle.setWidth(15);
                    middle->setPen(penMiddle);
                    this->scene_->addItem(middle);
                }

                if(this->fingersRaised[3]){
                    QGraphicsLineItem *ring = new QGraphicsLineItem(95.0,125.0,95.0,65.0);
                    QPen penRing(Qt::magenta);
                    penRing.setWidth(15);
                    ring->setPen(penRing);
                    this->scene_->addItem(ring);
                }
                else{
                    QGraphicsLineItem *ring = new QGraphicsLineItem(95.0,125.0,95.0,100.0);
                    QPen penRing(Qt::darkMagenta);
                    penRing.setWidth(15);
                    ring->setPen(penRing);
                    this->scene_->addItem(ring);
                }
                if(this->fingersRaised[4]){
                    QGraphicsLineItem *small = new QGraphicsLineItem(115.0,125.0,115.0,75.0);
                    QPen penSmall(Qt::yellow);
                    penSmall.setWidth(15);
                    small->setPen(penSmall);
                    this->scene_->addItem(small);
                }
                else{
                    QGraphicsLineItem *small = new QGraphicsLineItem(115.0,125.0,115.0,115.0);
                    QPen penSmall(Qt::darkYellow);
                    penSmall.setWidth(15);
                    small->setPen(penSmall);
                    this->scene_->addItem(small);
                }
            }
        }
}

void MainWindow::updateHand3D(){
    // But : Savoir la position de la main parmis 5 positions et adapter la main 3D
    // Positions disponibles : Index levé / Index & Majeur levés / Indes, Majeur & Annulaire levés / Index, Majeur, Annulaire & Auriculaire levés / Tous les doigts levés
    if(this->fingersRaised[1]){ // Index levé
        if(this->fingersRaised[2]){ // Majeur levé
            if(this->fingersRaised[3]){ // Annulaire levé
                if (this->fingersRaised[4]){ // Auriculaire levé
                    if(this->fingersRaised[0]){ // Pouce levé
                        this->ui->GameGLWidget->fingers = 5;
                    }
                    else{
                        this->ui->GameGLWidget->fingers = 4;
                    }
                }
                else{
                    this->ui->GameGLWidget->fingers = 3;
                }
            }
            else{
                this->ui->GameGLWidget->fingers = 2;
            }
        }
        else{
            this->ui->GameGLWidget->fingers = 1;
        }
    }
}

void MainWindow::updateCapture(){
    ui->cameraLabel->resize(this->cap.get(CAP_PROP_FRAME_WIDTH),this->cap.get(CAP_PROP_FRAME_HEIGHT));
    Mat image;
    this->cap >> image;
    Mat frame_inv;
    flip(image,frame_inv,1);
    //image = frame_inv;
    //conversion from Mat to QImage
    Mat dest;
    cv::cvtColor(image, dest,cv::COLOR_BGR2RGB);
    QImage screen = QImage(dest.data, dest.cols, dest.rows,QImage::Format_RGB888);
    //ui->cameraLabel->setPixmap(QPixmap::fromImage(screen));
}

void MainWindow::screenshoot(){
    cout<<"Clicked"<<endl;
    this->cap>>this->screen;
    /*
    QThreadPool p1;
    QFuture<void> t1 = QtConcurrent::run(&p1,checkHands,this->screen);
    t1.waitForFinished();*/
    checkHands(this->screen);
    //namedWindow("Screen",1);
    //imshow("Screen",this->screen);
}

void MainWindow::screenClicked(){
    screenshoot();
    drawHand();
}

void MainWindow::drawHand(){
    paintHand();
}

void MainWindow::updateAll(){
    //ui->GameGLWidget->update();
    QFuture<void> future_scene = QtConcurrent::run([this](){
        updateCapture();
    });
    this->ui->lcdNumber->display(this->ui->GameGLWidget->points);
    //this->watcher.setFuture(future_scene);
    //future_scene.result();
    //future_scene.waitForFinished();
}

MainWindow::~MainWindow()
{
    delete ui;
}
