#include "mainwindow.h"
#include <ctime>
#include <QApplication>

/// Classe Main
/// Elle permet le lancement de l'application
int main(int argc, char *argv[])
{
    // Initialisation du generateur de nombres aleatoires
    srand(time(nullptr));

    // Le CommandLineParser permet d'obtenir un modèle de détection de la main
    cv::CommandLineParser parser(argc, argv,
        "{ h help           | false     | print this help message }"
        "{ p proto          | pose_deploy.prototxt.txt          | (required) model configuration, e.g. hand/pose.prototxt }"
        "{ m model          | pose_iter_102000.caffemodel         | (required) model weights, e.g. hand/pose_iter_102000.caffemodel }"
        "{ d dataset        | HAND          | specify what kind of model was trained. It could be (COCO, MPI, HAND) depends on dataset. }"
        "{ width            |  368      | Preprocess input image by resizing to a specific width. }"
        "{ height           |  368      | Preprocess input image by resizing to a specific height. }"
        "{ t threshold      |  0.1      | threshold or confidence value for the heatmap }"
        "{ s scale          |  0.003922 | scale for blob }"
    );

    // Creation de l'application QT
    QApplication a(argc, argv);

    // Creation de la Window OpenGL
    MainWindow w(&parser);
    w.show();

    return a.exec();
}
