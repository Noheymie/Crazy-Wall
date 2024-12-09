#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Bibliothèque à inclure
#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <QPaintEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QList>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow // Classe gérant l'application en général et le retour caméra
{
    Q_OBJECT
    cv::VideoCapture cap; // VideoCapture pour gérer le flux vidéo
    cv::Mat screen; // Matrice pour la capture d'écran
    cv::CommandLineParser *p; // CommandLineParser permettatn la détection de la main sur une photo
    std::vector<cv::Point2f> handPointsPosition; // Vecteur comprenant les points détéctés de la main
    bool handDetected; // Booléen permettant de savoir si la main a été détectée ou non
    std::vector<bool> fingersRaised; // Vecteur de booléens permettant de savoir si chaque doigt de la main a été détectée
    QGraphicsScene *scene_; // QGraphicsScene pour la zone de dessin de la main 2D
    QFutureWatcher<void> watcher;
public:
    MainWindow(cv::CommandLineParser *parser,QWidget *parent = nullptr); // Constructeur
    ~MainWindow(); // Destructeur
    void checkHands(cv::Mat img); // Fonction permettant de réaliser la détection de la main sur une Matrice img
    void paintHand(); // Fonction dessinant la main virtuelle en 2D pour permettre au joeur de savoir quel doigt a été détecté
    void updateHand3D(); // Fonction permettant l'update de la main 3D à partir des doigts détectés
private:
    Ui::MainWindow *ui;
private slots:
    void updateAll(); // Slot permettant l'update de toute la mainWindow & le multi-Threading
    void updateCapture(); // Slot permettant l'affichage du retour caméra
    void screenshoot(); // Slot permettant la capture d'écran
    void screenClicked(); // Slot permettant la capture d'écran
    void drawHand(); // Slot permettant le lien entre le bouton "Capture d'écran" et l'affichage de la main 2D sur la QGraphicsScene
};
#endif // MAINWINDOW_H
