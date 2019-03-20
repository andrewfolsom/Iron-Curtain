/**
 * Chad Manning's file for Iron Curtain
 * @author: Chad Manning
 * Course: CMPS 3350
 * Instructor: Gordon Griesel 
*/
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include "core.h"
#include "chadM.h"
#include "fonts.h"

extern Global& gl;
extern EnemyShip *headShip;
extern EnemyShip *tailShip;

/*
    Displays Chads picture and name on the title screen
*/
void displayChad (float x , float y, GLuint textid) {
    float wid = 100.0;

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glPopMatrix();

    unsigned int color = 0xffff0000;
    Rect r;
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x - 10;
    r.bot = y - 125;

    ggprint16(&r, 16, color, "Chad Manning");
}

/*
    Constructor for EnemyShip class
    If you want to make the ship behave differently call the config class
        from nickJA.cpp to modify path.

    Ex. 
        eShip = new EnemyShip(x, y, movType);
        tailShip->config$NAME_OF_PATTERN$;
*/
EnemyShip::EnemyShip(int x, int y, int movType)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = 1;
	color[0] = color[1] = color[2] = 0.35;
    health = 100;
    movPattern = movType;
    if (headShip == NULL) {
        headShip = tailShip = this;
        prevShip = nextShip = NULL;
    } else {
        prevShip = tailShip;
        prevShip->nextShip = tailShip = this;
    }
    nextShip = NULL;
}

/*
    Destructor for EnemyShip class
    Readjusts linked list depending on which ship was destroyed.
*/
EnemyShip::~EnemyShip()
{
    if (prevShip != NULL) {
        prevShip->nextShip = nextShip;
    } else {
        headShip = nextShip;
    }
    if (nextShip != NULL) {
        nextShip->prevShip = prevShip;
    } else {
        tailShip = prevShip;
    }
}

/*
    Returns radius of bounding circle for the ship
*/
float EnemyShip::getRadius() {
    return detRadius;
}

/*
    Destroys the ship when its health reaches zero. KABOOM.
    "But Chad all this does is call the destructor." Yeah but destroyShip
        makes more sense than delete.
*/
void EnemyShip::destroyShip() {
    delete this;
}

/*
    Takes a Ship class as input and renders it to screen.
        used for both player ship and all enemy ships.
*/
void renderShip(Ship ship)
{
	glColor3fv(ship.color);
	glPushMatrix();
	glTranslatef(ship.pos[0], ship.pos[1], ship.pos[2]);
	glBegin(GL_QUADS);
	glVertex2f(-20.0f, -20.0f);
	glVertex2f(-20.0f, 20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(20.0, -20.0);
	glEnd();
	glPopMatrix();
}
