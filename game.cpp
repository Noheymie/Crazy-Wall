#define _USE_MATH_DEFINES

#include <QLabel>
#include <QImage>
#include <QTimer>
#include "game.h"
#include <stdlib.h>
#include <GL/glu.h>
#include <QApplication>
#include <iostream>
using namespace std;

// Declarations des constantes
const unsigned int WIN_WIDTH_HEIGHT = 650;

// Constructeur
Game::Game(QWidget * parent) : QOpenGLWidget(parent)
{
   setFixedSize(WIN_WIDTH_HEIGHT, WIN_WIDTH_HEIGHT);

   // Connexion du timer
   connect(&m_AnimationTimer,  &QTimer::timeout, [&] {
       m_TimeElapsed += 1.0f / 12.0f;
       update();
   });

   m_AnimationTimer.setInterval(1000);
   m_AnimationTimer.start();
}

// Fonction d'initialisation
void Game::initializeGL()
{
    // Reglage de la couleur de fond
    glClearColor(0.0f, 0.0f, 0.f, 0.0f);
}

// Fonction de redimensionnement
void Game::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);
}

// Fonction d'affichage
void Game::paintGL()
{
    // Définition de la matrice de projection pour définir la perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, (double)width()/height(), 2.0f, 80.0f);

    // Définition de la matrice de modélisation-visualisation pour définir la camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(25.0f, 10.0f, 25.0f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

    if (buffer) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }


    // Définition et placement d'une source de lumière
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_tab[]={0.0,10.0,10.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light_tab);

    // Gestion des textures
    glEnable(GL_TEXTURE_2D);

    QImage texture1=QImage(":/images/res/texture6.jpg");
    QImage tex_image1 = texture1.convertToFormat(QImage::Format_RGBA8888);

    QImage texture2=QImage(":/images/res/texture.jpg");
    QImage tex_image2 = texture2.convertToFormat(QImage::Format_RGBA8888);

    QImage texture3=QImage(":/images/res/textel.jpg");
    QImage tex_image3 = texture3.convertToFormat(QImage::Format_RGBA8888);

    QImage texture_win=QImage(":/images/res/win.png");
    QImage tex_win = texture_win.convertToFormat(QImage::Format_RGBA8888);

    QImage texture_lose=QImage(":/images/res/lose.png");
    QImage tex_lose = texture_lose.convertToFormat(QImage::Format_RGBA8888);

    glGenTextures(1, &tex1);
    glGenTextures(1, &tex2);
    glGenTextures(1, &tex3);
    glGenTextures(1, &text_win);
    glGenTextures(1, &text_lose);

    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D,0,4,tex_image1.width(),tex_image1.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex_image1.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexImage2D(GL_TEXTURE_2D,0,4,tex_image2.width(),tex_image2.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex_image2.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex3);
    glTexImage2D(GL_TEXTURE_2D,0,4,tex_image3.width(),tex_image3.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex_image3.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, text_win);
    glTexImage2D(GL_TEXTURE_2D,0,4,tex_win.width(),tex_win.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex_win.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, text_lose);
    glTexImage2D(GL_TEXTURE_2D,0,4,tex_lose.width(),tex_lose.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,tex_lose.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // Dessin de la scène qui s'actualise avec le timer(avancement du mur et changement de position de la main
    glPushMatrix();
    display(m_TimeElapsed);
    if (zmur==-1.0f){
        if (fingers==hole){
            win(true);
            points = points + 1;
            zmur=-20.0f;
            hole=(rand()%5)+1;
        }
        else {
            win(false);
        }
    }
    glPopMatrix();

};

// Fonction de dessin de la scène appelée chaques secondes
void Game::display(const float time){

    // Tant que le mur est assez loin de la main on le fait avancer, il s'arrête juste devant la main
    if (zmur<-1.0f){
        zmur= zmur+1.0f;
    }

    // Dessin du mur selon le trou
    wall=true;
    switch(hole){

    case 1 :

        for(float x=-15.0f;x<=-4.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }

        for(float y=-2.0f;y<=1.0f;y+=1.0f){
            for(float x=-15.0f;x<=-8.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,3.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,3.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,4.0f,zmur);
        }
        for(float x=4.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,4.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,5.0f,zmur);
        }
        for(float x=1.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,5.0f,zmur);
        }
        for (float y=6.0f;y<=9.0f;y+=1.0f){
            for(float x=-15.0f;x<=-6.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=-1.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }
        for(float x=-2.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }

        for(float x=-15.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=-15.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,12.0f,zmur);
        }

        break;

    case 2 :

        for(float x=-15.0f;x<=-4.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }

        for(float y=-2.0f;y<=1.0f;y+=1.0f){
            for(float x=-15.0f;x<=-8.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,3.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,3.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,4.0f,zmur);
        }
        for(float x=4.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,4.0f,zmur);
        }

        for (float y=5.0f;y<=9.0f;y+=1.0f){
            for(float x=-15.0f;x<=-6.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=2.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }
        for(float x=2.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }

        for(float x=-15.0f;x<=-2.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=1.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=-15.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,12.0f,zmur);
        }

        break;

    case 3 :

        for(float x=-15.0f;x<=-4.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }

        for(float y=-2.0f;y<=1.0f;y+=1.0f){
            for(float x=-15.0f;x<=-8.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,3.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,3.0f,zmur);
        }

        for (float y=4.0f;y<=8.0f;y+=1.0f){
            for(float x=-15.0f;x<=-6.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=5.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,9.0f,zmur);
        }
        for(float x=4.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }
        for(float x=2.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }

        for(float x=-15.0f;x<=-2.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=1.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=-15.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,12.0f,zmur);
        }

        break;

    case 4 :

        for(float x=-15.0f;x<=-4.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-3.0f,zmur);
        }

        for(float y=-2.0f;y<=1.0f;y+=1.0f){
            for(float x=-15.0f;x<=-8.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,2.0f,zmur);
        }

        for (float y=3.0f;y<=6.0f;y+=1.0f){
            for(float x=-15.0f;x<=-6.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,8.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,8.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,9.0f,zmur);
        }
        for(float x=4.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }
        for(float x=2.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }

        for(float x=-15.0f;x<=-2.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=1.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=-15.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,12.0f,zmur);
        }

        break;

    default :

        for(float x=-15.0f;x<=-4.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-8.0f,zmur);
        }
        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-6.0f,zmur);
        }
        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-5.0f,zmur);
        }

        for(float x=-15.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,-4.0f,zmur);
        }

        for (float y=-3.0f;y<=-1.0f;y+=1.0f){
            for(float x=-15.0f;x<=-11.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-10.0f;x+=1.0f){
            draw_cube(x,0.0f,zmur);
        }
        for(float x=7.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,0.0f,zmur);
        }

        for (float y=1.0f;y<=6.0f;y+=1.0f){
            for(float x=-15.0f;x<=-6.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
            for(float x=7.0f;x<=15.0f;x+=1.0f){
                draw_cube(x,y,zmur);
            }
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,7.0f,zmur);
        }
        for(float x=6.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,7.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,8.0f,zmur);
        }
        for(float x=5.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,8.0f,zmur);
        }

        for(float x=-15.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,9.0f,zmur);
        }
        for(float x=4.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,9.0f,zmur);
        }

        for(float x=-15.0f;x<=-5.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }
        for(float x=2.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,10.0f,zmur);
        }

        for(float x=-15.0f;x<=-2.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=1.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,11.0f,zmur);
        }
        for(float x=-15.0f;x<=15.0f;x+=1.0f){
            draw_cube(x,12.0f,zmur);
        }

        break;
    }


    wall=false;


    // Dessin de la main selon le nombre de doigts levés

    switch(fingers){

    case 1 : // x en -7 +7 & y en -9 +10
        for (float x=-3.0f;x<=4.0f;x+=1.0f){
            draw_cube(x,-9.0f,0.0f);
        }
        for (float y=-8.0f;y<=-5.0f;y+=1.0f){
            draw_cube(5.0f,y,0.0f);
        }
        for (float y=-4.0f;y<=2.0f;y+=1.0f){
            draw_cube(6.0f,y,0.0f);
        }
        draw_cube(-4.0f,-8.0f,0.0f);
        draw_cube(-4.0f,-7.0f,0.0f);
        draw_cube(-5.0f,-6.0f,0.0f);
        draw_cube(-5.0f,-5.0f,0.0f);
        draw_cube(-6.0f,-4.0f,0.0f);
        draw_cube(-6.0f,-3.0f,0.0f);
        draw_cube(-6.0f,2.0f,0.0f);

        for (float y=-2.0f;y<=1.0f;y+=1.0f){
            draw_cube(-7.0f,y,0.0f);
        }
        for (float y=0.0f;y<=9.0f;y+=1.0f){
            draw_cube(-5.0f,y,0.0f);
        }
        draw_cube(-4.0f,10.0f,0.0f);
        draw_cube(-3.0f,10.0f,0.0f);

        for (float y=3.0f;y<=9.0f;y+=1.0f){
            draw_cube(-2.0f,y,0.0f);
        }
        draw_cube(-1.0f,5.0f,0.0f);
        draw_cube(0.0f,5.0f,0.0f);
        draw_cube(1.0f,4.0f,0.0f);
        draw_cube(1.0f,3.0f,0.0f);
        draw_cube(2.0f,4.0f,0.0f);
        draw_cube(3.0f,4.0f,0.0f);
        draw_cube(4.0f,3.0f,0.0f);
        draw_cube(5.0f,3.0f,0.0f);

        break;

    case 2:
        for (float x=-3.0f;x<=4.0f;x+=1.0f){
            draw_cube(x,-9.0f,0.0f);
        }
        for (float y=-8.0f;y<=-5.0f;y+=1.0f){
            draw_cube(5.0f,y,0.0f);
        }
        for (float y=-4.0f;y<=2.0f;y+=1.0f){
            draw_cube(6.0f,y,0.0f);
        }
        draw_cube(-4.0f,-8.0f,0.0f);
        draw_cube(-4.0f,-7.0f,0.0f);
        draw_cube(-5.0f,-6.0f,0.0f);
        draw_cube(-5.0f,-5.0f,0.0f);
        draw_cube(-6.0f,-4.0f,0.0f);
        draw_cube(-6.0f,-3.0f,0.0f);
        draw_cube(-6.0f,2.0f,0.0f);

        for (float y=-2.0f;y<=1.0f;y+=1.0f){
            draw_cube(-7.0f,y,0.0f);
        }
        for (float y=0.0f;y<=9.0f;y+=1.0f){
            draw_cube(-5.0f,y,0.0f);
        }
        draw_cube(-4.0f,10.0f,0.0f);
        draw_cube(-3.0f,10.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(-2.0f,y,0.0f);
        }
        draw_cube(-1.0f,11.0f,0.0f);
        draw_cube(0.0f,11.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(1.0f,y,0.0f);
        }

        draw_cube(2.0f,4.0f,0.0f);
        draw_cube(3.0f,4.0f,0.0f);
        draw_cube(4.0f,3.0f,0.0f);
        draw_cube(5.0f,3.0f,0.0f);

        break;

    case 3:
        for (float x=-3.0f;x<=4.0f;x+=1.0f){
            draw_cube(x,-9.0f,0.0f);
        }
        for (float y=-8.0f;y<=-5.0f;y+=1.0f){
            draw_cube(5.0f,y,0.0f);
        }
        for (float y=-4.0f;y<=2.0f;y+=1.0f){
            draw_cube(6.0f,y,0.0f);
        }
        draw_cube(-4.0f,-8.0f,0.0f);
        draw_cube(-4.0f,-7.0f,0.0f);
        draw_cube(-5.0f,-6.0f,0.0f);
        draw_cube(-5.0f,-5.0f,0.0f);
        draw_cube(-6.0f,-4.0f,0.0f);
        draw_cube(-6.0f,-3.0f,0.0f);
        draw_cube(-6.0f,2.0f,0.0f);

        for (float y=-2.0f;y<=1.0f;y+=1.0f){
            draw_cube(-7.0f,y,0.0f);
        }
        for (float y=0.0f;y<=9.0f;y+=1.0f){
            draw_cube(-5.0f,y,0.0f);
        }
        draw_cube(-4.0f,10.0f,0.0f);
        draw_cube(-3.0f,10.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(-2.0f,y,0.0f);
        }
        draw_cube(-1.0f,11.0f,0.0f);
        draw_cube(0.0f,11.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(1.0f,y,0.0f);
        }

        draw_cube(2.0f,9.0f,0.0f);
        draw_cube(3.0f,9.0f,0.0f);

        for (float y=3.0f;y<=8.0f;y+=1.0f){
            draw_cube(4.0f,y,0.0f);
        }

        draw_cube(5.0f,3.0f,0.0f);

        break;

    case 4:
        for (float x=-3.0f;x<=4.0f;x+=1.0f){
            draw_cube(x,-9.0f,0.0f);
        }
        for (float y=-8.0f;y<=-5.0f;y+=1.0f){
            draw_cube(5.0f,y,0.0f);
        }
        for (float y=-4.0f;y<=6.0f;y+=1.0f){
            draw_cube(6.0f,y,0.0f);
        }
        draw_cube(-4.0f,-8.0f,0.0f);
        draw_cube(-4.0f,-7.0f,0.0f);
        draw_cube(-5.0f,-6.0f,0.0f);
        draw_cube(-5.0f,-5.0f,0.0f);
        draw_cube(-6.0f,-4.0f,0.0f);
        draw_cube(-6.0f,-3.0f,0.0f);
        draw_cube(-6.0f,2.0f,0.0f);

        for (float y=-2.0f;y<=1.0f;y+=1.0f){
            draw_cube(-7.0f,y,0.0f);
        }
        for (float y=0.0f;y<=9.0f;y+=1.0f){
            draw_cube(-5.0f,y,0.0f);
        }
        draw_cube(-4.0f,10.0f,0.0f);
        draw_cube(-3.0f,10.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(-2.0f,y,0.0f);
        }
        draw_cube(-1.0f,11.0f,0.0f);
        draw_cube(0.0f,11.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(1.0f,y,0.0f);
        }

        draw_cube(2.0f,9.0f,0.0f);
        draw_cube(3.0f,9.0f,0.0f);

        for (float y=3.0f;y<=8.0f;y+=1.0f){
            draw_cube(4.0f,y,0.0f);
        }

        draw_cube(5.0f,7.0f,0.0f);

        break;

    default:
        for (float x=-3.0f;x<=4.0f;x+=1.0f){
            draw_cube(x,-9.0f,0.0f);
        }
        for (float y=-8.0f;y<=-5.0f;y+=1.0f){
            draw_cube(5.0f,y,0.0f);
        }
        for (float y=-4.0f;y<=6.0f;y+=1.0f){
            draw_cube(6.0f,y,0.0f);
        }
        draw_cube(-4.0f,-8.0f,0.0f);
        draw_cube(-4.0f,-7.0f,0.0f);
        draw_cube(-5.0f,-6.0f,0.0f);
        draw_cube(-5.0f,-5.0f,0.0f);
        draw_cube(-6.0f,-4.0f,0.0f);

        for (float x=-10.0f;x<=-7.0f;x+=1.0f){
            draw_cube(x,-3.0f,0.0f);
        }

        draw_cube(-10.0f,-2.0f,0.0f);
        draw_cube(-10.0f,-1.0f,0.0f);

        for (float x=-9.0f;x<=-6.0f;x+=1.0f){
            draw_cube(x,0.0f,0.0f);
        }

        for (float y=0.0f;y<=9.0f;y+=1.0f){
            draw_cube(-5.0f,y,0.0f);
        }
        draw_cube(-4.0f,10.0f,0.0f);
        draw_cube(-3.0f,10.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(-2.0f,y,0.0f);
        }
        draw_cube(-1.0f,11.0f,0.0f);
        draw_cube(0.0f,11.0f,0.0f);

        for (float y=3.0f;y<=10.0f;y+=1.0f){
            draw_cube(1.0f,y,0.0f);
        }

        draw_cube(2.0f,9.0f,0.0f);
        draw_cube(3.0f,9.0f,0.0f);

        for (float y=3.0f;y<=8.0f;y+=1.0f){
            draw_cube(4.0f,y,0.0f);
        }

        draw_cube(5.0f,7.0f,0.0f);

        break;
    }
}

// Fonction permettant de dessiner les cubes composant la scène
void Game::draw_cube(float x,float y,float z){

    glBindTexture(GL_TEXTURE_2D, tex1); // texture de la main
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    if (wall==true){
        if (x==15.0f && y==12.0f){ // on place le logo de TSE sur le bloc en haut à droite du mur
            glBindTexture(GL_TEXTURE_2D, tex3);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        }
        else { // texture du mur
            glBindTexture(GL_TEXTURE_2D, tex2);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        }
    }

    // Face avant
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(x+0.0f, y+0.0f, z+1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(x+0.0f, y+1.0f, z+1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(x+1.0f, y+1.0f, z+1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(x+1.0f, y+0.0f, z+1.0f);
    glEnd();

    //Face dessus
    glBegin(GL_QUADS);
    if (wall && y<=11.0f){
        glVertex3f(x+0.0f, y+1.0f, z+0.0f);
        glVertex3f(x+0.0f, y+1.0f, z+1.0f);
        glVertex3f(x+1.0f, y+1.0f, z+1.0f);
        glVertex3f(x+1.0f, y+1.0f, z+0.0f);
    } else {
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x+0.0f, y+1.0f, z+0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x+0.0f, y+1.0f, z+1.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x+1.0f, y+1.0f, z+1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x+1.0f, y+1.0f, z+0.0f);
    }
    glEnd();

    //Face droite
    glBegin(GL_QUADS);
    if (wall && x<15.0f){
        glVertex3f(x+1.0f, y+0.0f, z+0.0f);
        glVertex3f(x+1.0f, y+1.0f, z+0.0f);
        glVertex3f(x+1.0f, y+1.0f, z+1.0f);
        glVertex3f(x+1.0f, y+0.0f, z+1.0f);
    } else {
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x+1.0f, y+0.0f, z+0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x+1.0f, y+1.0f, z+0.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(x+1.0f, y+1.0f, z+1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x+1.0f, y+0.0f, z+1.0f);
    }
    glEnd();

    //Face arrière
    glBegin(GL_QUADS);
    if (wall){
        glVertex3f(x+0.0f, y+0.0f, z+0.0f);
        glVertex3f(x+0.0f, y+1.0f, z+0.0f);
        glVertex3f(x+1.0f, y+1.0f, z+0.0f);
        glVertex3f(x+1.0f, y+0.0f, z+0.0f);
    } else {
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x+0.0f, y+0.0f, z+0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x+0.0f, y+1.0f, z+0.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(x+1.0f, y+1.0f, z+0.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x+1.0f, y+0.0f, z+0.0f);
    }
    glEnd();

    // Face dessous
    glBegin(GL_QUADS);
    if (wall){
        glVertex3f(x+0.0f, y+0.0f, z+0.0f);
        glVertex3f(x+0.0f, y+0.0f, z+1.0f);
        glVertex3f(x+1.0f, y+0.0f, z+1.0f);
        glVertex3f(x+1.0f, y+0.0f, z+0.0f);
    } else {
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x+0.0f, y+0.0f, z+0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x+0.0f, y+0.0f, z+1.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(x+1.0f, y+0.0f, z+1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x+1.0f, y+0.0f, z+0.0f);
    }
    glEnd();

    // Face gauche
    glBegin(GL_QUADS);
    if (wall) {
        glVertex3f(x+0.0f, y+0.0f, z+0.0f);
        glVertex3f(x+0.0f, y+1.0f, z+0.0f);
        glVertex3f(x+0.0f, y+1.0f, z+1.0f);
        glVertex3f(x+0.0f, y+0.0f, z+1.0f);
    } else {
        glTexCoord2f(0.0f, 0.0f);glVertex3f(x+0.0f, y+0.0f, z+0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(x+0.0f, y+1.0f, z+0.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(x+0.0f, y+1.0f, z+1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(x+0.0f, y+0.0f, z+1.0f);
    }
    glEnd();
}

void Game::win(bool b){

    glBindTexture(GL_TEXTURE_2D, text_lose);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    if (b){
        glBindTexture(GL_TEXTURE_2D, text_win);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-5.0f, 13.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-5.0f, 20.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(5.0f, 20.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(5.0f, 13.0f, -1.0f);
        glEnd();
    }
    else {
        lost = true;
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-5.0f, 13.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-5.0f, 20.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(5.0f, 20.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(5.0f, 13.0f, -1.0f);
        glEnd();
    }
    Sleep(2000);
}
