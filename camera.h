#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <QKeyEvent>
#include <QLabel>

using namespace cv;
using namespace std;


class Camera
{
    cv::VideoCapture cap; // Video Capture
    Mat img; // Retour Caméra
    Mat screen; //Screenshoot de la caméra
    int posHand; //Int pour savoir la position de la main
public:
    Camera();
    void loop(int argc, char* argv[],QKeyEvent * event,QLabel l); //Boucle pour faire tourner la caméra et le programme entier
    void setImg(QLabel l); //Méthode pour actualiser le Retour Caméra
    Mat getImg(); //Méthode pour obtenir l'image enregistrée pour le retour caméra
    void setScreen(); //Méthode pour enregistrer le screenshoot et définir le type de main
    int getPosHand(); //Méthode pour obtenir quelle position de la main est montrée devant la caméra
};

#endif // CAMERA_H
