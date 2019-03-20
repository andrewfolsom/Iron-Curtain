/* Source:	BenjaminG.cpp
 * @Author:	Benjamin A Garza III
 * Course:	CMPS 3350: Software Engineering
 * Assignment:	Lab 4 - Group Project Setup
 * Date:	February 14, 2019
 */

#include <iostream>
#include <unistd.h>
#include <GL/glx.h>
#include "fonts.h"
#include "core.h"
extern Global& gl;
using namespace std;

/* A function that will be called from the main
 * function of our game to display my name and a picture. 
 */
void displayBenjamin(float x, float y) {
    Rect r;
    //glClear(GL_COLOR_BUFFER_BIT);
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x - 10;
    r.bot = y - 125;

    float wid = 100.0f;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glBindTexture(GL_TEXTURE_2D, gl.benImg);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();

    ggprint8b(&r, 16, 0x00ffffff, "Benjamin A Garza");
}

void displayStartScreen2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    gl.startScreen = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.startScreen);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," ");

}

void displayMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    glBindTexture(GL_TEXTURE_2D, gl.ironImage);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0; 
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "G - Play Game");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "O - Options");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "C - Credits Screen");

}
void displayLoadingScreen() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    gl.loadingScreen = gl.ironImage;

    glBindTexture(GL_TEXTURE_2D, gl.loadingScreen);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," "); 
}
void displayPauseMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    gl.pauseScreen = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.pauseScreen);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "C - Credits Screen");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "Y - Return to Main Screen");
    ggprint16(&r, 16,c, " ");
    ggprint16(&r, 16,c, "G - Play Game");

}

void displayHiddenWorld() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    gl.hiddenWorld = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.hiddenWorld);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," ");
}

void displayErrorScreen() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    gl.errorPage = gl.ironImage;
    glBindTexture(GL_TEXTURE_2D, gl.errorPage);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,1.0f);glVertex2i(0,0);
    glTexCoord2f(0.0f,0.0f); glVertex2i(0,gl.yres);
    glTexCoord2f(1.0f,0.0f); glVertex2i(gl.xres,gl.yres);
    glTexCoord2f(1.0f,1.0f); glVertex2i(gl.xres,0);
    glEnd();

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint16(&r, 16,c," ");
}

/*
void genTexture(GLuint &imgID, int imgIndex) {
    glBindTexture(GL_TEXTURE_2D, imgID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,img[imgIndex].width,img[imgIndex].height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[imgIndex].data);
}


void genCreditTextures() {
    for (int i = 0; i < 5; i++) {
        int w = img[i].width;
        int h = img[i].height;
        switch (i) {
            case 0:
                glBindTexture(GL_TEXTURE_2D, gl.nickImage);
                break;
            case 1:
                glBindTexture(GL_TEXTURE_2D, gl.andrewImage);
                break;
            case 2:
                glBindTexture(GL_TEXTURE_2D, gl.spencerImage);
                break;
            case 3:
                glBindTexture(GL_TEXTURE_2D, gl.chadImage);
                break;
            case 4:
                glBindTexture(GL_TEXTURE_2D, gl.benImage);
                break;
            default:
                cout << "Error: genTextures\n";
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[i].data);
    }
}
*/
