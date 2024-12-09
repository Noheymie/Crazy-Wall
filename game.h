#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QColor>
#include <QTimer>

// Classe dediee a l'affichage de la fenêtre de jeu 3D
class Game : public QOpenGLWidget
{
    Q_OBJECT
    bool buffer=true;
    bool wall=false;

public:

    // Constructeur
    Game(QWidget * parent = nullptr);

    // Construction de la scène
    void display(const float time);

    // Fonction pour dessiner les cubes
    void draw_cube(float x,float y,float z);

    // Nombres de doigts levés pour le dessin de la main
    int fingers=5;

    // Taille du trou dans le mur (position de main demandée)
    int hole=(rand()%5)+1;

    // Position du mur
    float zmur=-20.0f;

    // Ecran final (true = win, false = lose)
    void win(bool b);
    bool lost = false;

    // Nombre de points
    int points = 0;

    // Timer d'animation
    float m_TimeElapsed { 0.0f };
    QTimer m_AnimationTimer;

    // Stockage des textures
    GLuint tex1;
    GLuint tex2;
    GLuint tex3;
    GLuint text_win;
    GLuint text_lose;


protected:

    // Fonction d'initialisation
    virtual void initializeGL() override;

    // Fonction de redimensionnement
    virtual void resizeGL(int width, int height) override;

    // Fonction d'affichage
    virtual void paintGL() override;

};


#endif // GAME_H
