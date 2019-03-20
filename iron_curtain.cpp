/* Source:	iron-curtain.cpp                                                                          
 * Origin:	asteroids.cpp                                                     
 * Author:  Gordon Griesel                                                    
 * Date:    2014 - 2018                                                       
 * Mods:
 * 	Spring 2015:
 * 		G. Greisel - added constructors                                        
 * 	Spring 2019:
 * 		B. Garza   - 
 * 		C. Manning - 
 *		S. Austin  -
 *		N. Jackson - 
 */            

#include <iostream>                                                          
#include <cstdlib>                                                           
#include <cstring>                                                           
#include <unistd.h>                                                          
#include <ctime>                                                             
#include <cmath>                                                             
#include <X11/Xlib.h>                                                        
#include <X11/keysym.h>                                                      
#include <GL/glx.h>                                                          
#include "fonts.h"
#include <stdio.h>
#include "core.h"
#include "andrewF.h"
#include "chadM.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 1000;
const int MAX_MISSILES = 1;
const float MAX_VELOCITY = 15;
float test[3] = {450.0, 900.0, 0.0};
float radius = 8.0;
float step = 0.0;
float d0, d1, dist;

//-------------------------------------------------------------------------- :                                                                         
//Setup timers                                                               
const double physicsRate = 1.0 / 60.0;                                       
const double oobillion = 1.0 / 1e9;                                          
extern struct timespec timeStart, timeCurrent;                               
extern struct timespec timePause;                                            
extern double physicsCountdown;                                              
extern double timeSpan;                                                      
extern double timeDiff(struct timespec *start, struct timespec *end);        
extern void timeCopy(struct timespec *dest, struct timespec *source);
//
extern void displayNick(float x, float y, GLuint texture);
//Externs -- Chad
extern void renderShip(Ship ship);
extern void displayChad(float x, float y, GLuint texture);

//Externs -- Andrew
extern void displayAndrew(float x, float y, GLuint texture);

extern void displaySpencer(float x, float y, GLuint texture);
extern void displayStartScreen();
extern void scrollingBackground();
//Externs -- Benjamin
extern void displayBenjamin(float x, float y);
extern void displayStartScreen2();
extern void displayMenu();
extern void displayLoadingScreen();
extern void displayPauseMenu();
extern void displayHiddenWorld();
extern void displayErrorScreen();

//Externs -- Jackson
extern void displayNick(float x, float y, GLuint texture);
extern void spawnOpFor(int x, int y, int movType);
extern void renderOpFor();
extern void updatePosition();
extern void configOpFor(int ID, int destOffSet);
//-------------------------------------------------------------------------- 

Image img[7] = {
    "./img/NICKJA.jpg",
    "./img/andrewimg.png",
    "./img/spencerA.jpg",
    "./img/chad-egg.jpg",
    "./img/BGarza.jpg",
    "./img/ironImage.jpg",
    "./img/verticalBackground.jpg"
};

Global& gl = Global::getInstance();

Game g;

X11_wrapper x11;

//Weapon *wpn = new Basic;

//Weapon *scnd = new Secondary;

EnemyShip *headShip = NULL;
EnemyShip *tailShip = NULL;
EnemyShip *eShip = NULL;
EnemyShip *e = NULL;
enum MOVETYPE { RUSH, STRAFE, CIRCLING, BANK, DIAG_RUSH};



//Function Prototypes
double getTimeSlice(timespec* bt);
float convertToRads(float angle);
void init_opengl(void);
int check_keys(XEvent *e);
void physics();
void render();

int main()
{
	init_opengl();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done = 0;

	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			done = check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics();
			updatePosition();
			physicsCountdown -= physicsRate;
		}
		render();
		x11.swapBuffers();
		x11.clearWindow();
	}
	cleanup_fonts();
	return 0;
}

void init_opengl(void)
{

    glViewport(0, 0, gl.xres, gl.yres);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    glGenTextures(1, &gl.nickImage);
    glGenTextures(1, &gl.andrewImage);
    glGenTextures(1, &gl.spencerImage);
    glGenTextures(1, &gl.chadImage);
    glGenTextures(1, &gl.benImg);

    glBindTexture(GL_TEXTURE_2D, gl.ironImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,img[5].width,img[5].height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
    
    //Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    ///scrolling background
	gl.tex.xc[0] = 0.0;
	gl.tex.xc[1] = 0.25;
	gl.tex.yc[0] = 0.0;
	gl.tex.yc[1] = 1.0;
    
}


void normalize2d(Vec v)
{
    Flt len = v[0]*v[0] + v[1]*v[1];
    if (len == 0.0f) {
        v[0] = 1.0;
        v[1] = 0.0;
        return;
    }
    len = 1.0f / sqrt(len);
    v[0] *= len;
    v[1] *= len;
}

int check_keys(XEvent *e)
{
    Ship *s = &g.ship;
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        switch (key) {
            //Game States
	    case XK_y:
		gl.gameState = 1;
		break;
	    case XK_l:
		gl.gameState = 2;
	    	break;
	    case XK_g:
                gl.gameState = 3;
                break;
            case XK_p: //Pause
                gl.gameState = 4;
                break;
            case XK_c : //Credits
                gl.gameState = 5;
                break;
	    case XK_j:
                gl.gameState = 6;
                break;
            
            //Movements and Gameplay
            case XK_a:
                //moveLeft();
                gl.keys[XK_a] = 1;
                break;
            case XK_d:
                //moveRight();
                gl.keys[XK_d] = 1;
                break;
            case XK_w:
                gl.keys[XK_w] = 1;
                break;
            case XK_s:
                gl.keys[XK_s] = 1;
                break;

            case XK_space:
                gl.keys[XK_space] = 1;
                break;
            case XK_Escape:
                return 1;

            case XK_m:
                s->scnd->locked = true;
                gl.keys[XK_m] = 1;
                break;
            case XK_1:
                delete s->wpn;
                s->wpn = new Basic;
                break;
            case XK_2:
                delete s->wpn;
                s->wpn = new Rapid;
                break;
            case XK_3:
                delete s->wpn;
                s->wpn = new Scatter;
                break;
            case XK_4:
                delete s->wpn;
                s->wpn = new Ring;
                break;
            case XK_5:
                delete s->wpn;
                s->wpn = new Pinwheel;
                break;
            case XK_6:
                delete s->wpn;
                s->wpn = new EnemyStd;
                break;
	    	case XK_b:
				s->shield->status = !s->shield->status;
				break;
			case XK_Shift_R:
                if (headShip != NULL) {
			        s->scnd->armed = true;
                    s->scnd->reticle.e = headShip;
                    s->scnd->reticle.update();
                }
				break;
            case XK_period:
                if (s->scnd->reticle.e->nextShip != NULL)
                    s->scnd->reticle.e = s->scnd->reticle.e->nextShip;
                break;
            case XK_comma:
                if (s->scnd->reticle.e->prevShip != NULL)
                    s->scnd->reticle.e = s->scnd->reticle.e->prevShip;
                break;
            case XK_t:
                eShip = new EnemyShip(gl.xres / 3, 900, RUSH);
                eShip = new EnemyShip(gl.xres / 2, 900, STRAFE);
                tailShip->configStrafe(10, 90, 5, 2, -1);
                eShip = new EnemyShip(200, 900, CIRCLING);
                tailShip->configCircle(30, 90, 3, 2, -1);
                break;

        }
    }

    if (e->type == KeyRelease) {
        switch (key) {
            case XK_a:
                //s->vel[0] = 10.0;
                gl.keys[XK_a] = 0;
                break;
            case XK_d:
                //s->vel[0] = 10.0;
                gl.keys[XK_d] = 0;
                break;
            case XK_w:
                gl.keys[XK_w] = 0;
                break;
            case XK_s:
                gl.keys[XK_s] = 0;
                break;
            case XK_space:
                gl.keys[XK_space] = 0;
                break;
            case XK_m:
                gl.keys[XK_m] = 0;
                break;
        }
    }
    return 0;
}

void physics()
{
    //float spdLeft, spdRight, spdUp, spdDown;
    Ship *s = &g.ship;
    if (s->pos[0] < 20.0) {
        s->pos[0] = 20.0;
    } else if (s->pos[0] > gl.xres - 20.0) {
        s->pos[0] = gl.xres - 20;
    } else if (s->pos[1] < 20.0) {
        s->pos[1] = 20.0;
    } else if (s->pos[1] > gl.yres - 20.0) {
        s->pos[1] = gl.yres - 20.0;
    }

    e = headShip;
    //Update positions of all enemy ships
    while(e != NULL){
        e->updatePosition();
        e = e->nextShip;
    }

    if (headShip != NULL && headShip->pos[1] < -20) {
        e = headShip;
        headShip = headShip->nextShip;        
        delete e;
    }

    // Update position of reticle
    if (s->scnd->armed) {
        s->scnd->reticle.update();
    }
    
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i = 0;
    while (i < g.nbullets) {
        Bullet *b = &g.barr[i];
        if (b->pos[1] > gl.yres + 10 || b->pos[1] < -10.0 ||
                b->pos[0] > gl.xres + 10 || b->pos[0] < -10.0) {
            memcpy(&g.barr[i], &g.barr[g.nbullets - 1], sizeof(Bullet));
            g.nbullets--;
            continue;
        }
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];
        i++;
    }

    i = 0;
    e = headShip;
    if (e != NULL) {
        while (i < g.nmissiles) {
            Missile *m = &g.marr[i];
            d0 = s->scnd->reticle.e->pos[0] - m->pos[0];
            d1 = s->scnd->reticle.e->pos[1] - m->pos[1];
            dist = (d0*d0 + d1*d1);
            if (dist < (radius * radius)) {
                memcpy(&g.marr[i], &g.marr[g.nmissiles - 1], sizeof(Missile));
                g.nmissiles--;
                continue;
            }

            step += 0.01;
            m->tracking(s->scnd->reticle.e->pos, step);

            if (step > 1.0)
                step = 0.0;
            i++;
        }
    }
    //Collision with bullets?
    //If collision detected:
    //     1. delete the bullet
    //     2. delete the ship 
    e = headShip;
    while (e != NULL) {
        //is there a bullet within its radius?
        int i=0;
        while (i < g.nbullets) {
            Bullet *b = &g.barr[i];
            Flt d0 = b->pos[0] - e->pos[0];
            Flt d1 = b->pos[1] - e->pos[1];
            Flt dist = (d0*d0 + d1*d1);
            if (dist < (e->getRadius()*e->getRadius())) {
                //delete the ship
                e->destroyShip();
                //delete the bullet
                memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
                g.nbullets--;
            }

            i++;
            if (headShip == NULL) {
                break;
            }
        }
        e = e->nextShip;
    }

    s = &g.ship;

    if (gl.keys[XK_a]) {
        s->pos[0] -= s->vel[0];
        s->vel[0] += s->speed;
        if (s->vel[0] > MAX_VELOCITY)
            s->vel[0] = MAX_VELOCITY;
    } else {
        if(s->vel[0] > 0.0) {
            s->pos[0] -= s->vel[0];
            s->vel[0] -= s->speed;
        } else {
            s->vel[0] = 0.0;
        }
    }


    if (gl.keys[XK_d]) {
        s->pos[0] += s->vel[1];
        s->vel[1] += s->speed;
        if (s->vel[1] > MAX_VELOCITY)
            s->vel[1] = MAX_VELOCITY;
    } else {
        if(s->vel[1] > 0.0) {
            s->pos[0] += s->vel[1];
            s->vel[1] -= s->speed;
        } else {
            s->vel[1] = 0.0;
        }
    }

    if (gl.keys[XK_w]) {
        s->pos[1] += s->vel[2];
        s->vel[2] += s->speed;
        if (s->vel[2] > MAX_VELOCITY)
            s->vel[2] = MAX_VELOCITY;
    } else {
        if(s->vel[2] > 0.0) {
            s->pos[1] += s->vel[2];
            s->vel[2] -= s->speed;
        } else {
            s->vel[2] = 0.0;
        }
    }

    if (gl.keys[XK_s]) {
        s->pos[1] -= s->vel[3];
        s->vel[3] += s->speed;
        if (s->vel[3] > MAX_VELOCITY)
            s->vel[3] = MAX_VELOCITY;
    } else {
        if(s->vel[3] > 0.0) {
            s->pos[1] -= s->vel[3];
            s->vel[3] -= s->speed;
        } else {
            s->vel[3] = 0.0;
        }
    }

    if (gl.keys[XK_space])
        s->wpn->fire();

    if (gl.keys[XK_m])
        s->scnd->fire();

    if (g.thrustOn) {
        struct timespec mtt;
        clock_gettime(CLOCK_REALTIME, &mtt);
        double tdif = timeDiff(&mtt, &g.thrustTimer);
        if (tdif < -0.3)
            g.thrustOn = false;
    }
    //scrolling physics
    gl.tex.xc[0] -=0.0006;
    gl.tex.xc[1] -=0.0006;
    return;
}

void render()
{
    if (gl.gameState == 0){ //Startup
        //init regular background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gl.ironImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,img[5].width,img[5].height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
    
        displayStartScreen();
        glDisable(GL_TEXTURE_2D);
    
    } else if (gl.gameState == 1){ //Menu
        
        //init regular background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gl.ironImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,img[5].width,img[5].height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
    
        displayMenu();
        glDisable(GL_TEXTURE_2D);
   
    } else if (gl.gameState == 2){ //Loading
       
        displayLoadingScreen();
   
    } else if (gl.gameState == 3) { //Gameplay

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        //scrolling background
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gl.verticalBackground);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,img[6].width,img[6].height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[6].data);

        scrollingBackground();
        glDisable(GL_TEXTURE_2D);
        
        //Draw ships
        
        renderShip(g.ship);
		if (g.ship.shield->status)
			g.ship.shield->drawShield(g.ship.pos);

        EnemyShip *e = headShip;
        while(e != NULL){
            renderShip(*e);
            e = e->nextShip;
        }

        if (g.ship.scnd->armed)
            g.ship.scnd->reticle.drawReticle(g.ship.scnd->locked);

        //Draw enemy ships
        renderOpFor();

        for (int i = 0; i < g.nbullets; i++) {
            Bullet *b = &g.barr[i];
            glColor3fv(b->color);
            glPushMatrix();
            glTranslatef(b->pos[0], b->pos[1], b->pos[2]);
            glBegin(GL_QUADS);
            glVertex2f(-5.0, -5.0);
            glVertex2f(-5.0, 5.0);
            glVertex2f(5.0, 5.0);
            glVertex2f(5.0, -5.0);
            glEnd();
            glPopMatrix();
        }

        for (int i = 0; i < g.nmissiles; i++) {
            Missile *m = &g.marr[i];
            glColor3fv(m->color);
            glPushMatrix();
            glTranslatef(m->pos[0], m->pos[1], m->pos[2]);
            glBegin(GL_QUADS);
            glVertex2f(-5.0, -5.0);
            glVertex2f(-5.0, 5.0);
            glVertex2f(5.0, 5.0);
            glVertex2f(5.0, -5.0);
            glEnd();
            glPopMatrix();
        }

        glDisable(GL_DEPTH_TEST);    
    } else if (gl.gameState == 4){ //Pause
        
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gl.ironImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,img[5].width,img[5].height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
    
        
        
        
        displayPauseMenu();
        glDisable(GL_TEXTURE_2D);
    } else if (gl.gameState == 5) { //Credits 
    //If 'c' was pressed then render credit screen
        int w = img[0].width;
        int h = img[0].height;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gl.nickImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[0].data);

        w = img[1].width;
        h = img[1].height;
        glBindTexture(GL_TEXTURE_2D, gl.andrewImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                img[1].data);

        w = img[2].width;
        h = img[2].height;
        glBindTexture(GL_TEXTURE_2D, gl.spencerImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

        w = img[3].width;
        h = img[3].height;
        glBindTexture(GL_TEXTURE_2D, gl.chadImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
        initialize_fonts();

        w = img[4].width;
        h = img[4].height;
        glBindTexture(GL_TEXTURE_2D, gl.benImg);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img[4].data);

        glClear(GL_COLOR_BUFFER_BIT);

        displaySpencer( 700  , 250, gl.spencerImage); 
        displayNick(gl.xres/2, gl.yres/2, gl.nickImage);
        displayAndrew(gl.xres/4, gl.yres/4, gl.andrewImage);
        displayChad( 700, gl.yres/2 + 250, gl.chadImage); 
        displayBenjamin(gl.xres/4, 3*gl.yres/4);

        glDisable(GL_TEXTURE_2D);
    } else if (gl.gameState == 6) //Hidden Levels
        displayHiddenWorld();
    else
        displayErrorScreen(); //Error Screen
}

// Utility Functions

/**
 * Gets the time differential between the last time an event occured and now
 * @param timespec *bt 	Pointer to a timespec struct
 * @return double ts 	Time slice representing the elapsed time
 */
double getTimeSlice(timespec *bt)
{
    clock_gettime(CLOCK_REALTIME, bt);
    double ts = timeDiff(&g.bulletTimer, bt);
    return ts;
}

/**
 * Converts degrees to radians
 * @param float angle 		The angle to be converted to radians
 * @return float radian 	Value representing the inputed angle
 */
float convertToRads(float angle)
{
    float rads = angle * (PI / 180);
    return rads;
}
