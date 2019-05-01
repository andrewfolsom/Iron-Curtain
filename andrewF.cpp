#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "fonts.h"
#include "andrewF.h"
#include "core.h"
#include "chadM.h"

#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];\
(c)[2]=(a)[2]-(b)[2]

const int MAX_BULLETS = 1000;
const int MAX_MISSILES = 1;
extern float convertToRads(float angle);
extern double getTimeSlice(Ship*, timespec*);
extern void timeCopy(struct timespec *dest, struct timespec *source);
extern Game g;
extern Image img;
extern EnemyShip* headShip;
extern unsigned char *buildAlphaData(Image *img);

/**
 * Image classes containing image files for the HUD
 */
Image hudLife[4] = {
	"./img/life-0.png",
	"./img/life-1.png",
	"./img/life-2.png",
	"./img/life-3.png"
};

Image hudWeapon[3] = {
    "./img/basic.png",
    "./img/rapid.png",
    "./img/scatter.png"
};

Image dig[10] = {
	"./img/zero.png",
	"./img/one.png",
	"./img/two.png",
	"./img/three.png",
	"./img/four.png",
	"./img/five.png",
	"./img/six.png",
	"./img/seven.png",
	"./img/eight.png",
	"./img/nine.png"
};

Image hudSecond("./img/missile.png");

Image hudScore("./img/scoreboard.png");

Image hudDisplay("./img/weaponDisplay.png");

Image upgradePod("./img/upgrade.png");

/**
 * Displays my picture and name
 * @param float x           Desired x position of image
 * @param float y           Desired y position of image
 * @param GLuint texture    Texture mapped image of my face
 */
void displayAndrew(float x, float y, GLuint texture)
{
    float res = 100.0f;
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(-res,-res);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(-res, res);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(res, res);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(res, -res);
    glEnd();
    glPopMatrix();

    unsigned int color = 0x00bfb418;
    Rect r;
    r.width = 250.0;
    r.height = 100.0;
    r.center = x;
    r.left = x;
    r.bot = y - 125;

    ggprint16(&r, 16, color, "Andrew Folsom");
}

//===========================================================
//                GENERAL UTILITY FUNCTIONS
//===========================================================

/**
 * Adjusts an objects velocity according to the angle at which
 * it should travel.
 * @param vel   Array containing the objects X/Y velocities
 * @param angle The desired angle of fire
 */
void angularAdjustment(float *vel, float angle)
{
    vel[0] = vel[1] * cos(convertToRads(angle));
    vel[1] = vel[1] * sin(convertToRads(angle));
}

int generateUpgrade()
{
    int value = rand() % 101;
    value = value % 10;
    if (value == 0)
        return 1;
    else
        return 0;
}

//===========================================================
//    DEFINITION OF WEAPON CLASS AND IT'S DERIVED CLASSES
//===========================================================

/**
 * Weapon class constructor
 */
Weapon::Weapon() { }

/**
 * Weapon class destructor
 */
Weapon::~Weapon() { }

/**
 * Sets the initial position of generated Bullet objects.
 * @param float *s  position array of firing ship
 * @param float *b  position array of fired bullet
 */
void Weapon::setPosition(float *s, float *b)
{
    b[0] = s[0];
    b[1] = s[1];
    b[2] = s[2];
}

/**
 * Sets the velocity of the generated Bullet object. This implementation does
 * not account for bullet spread.
 * @param float *vel    velocity array of fired bullet
 */
void Weapon::setVelocity(float *vel)
{
    vel[0] = 0.0;
    vel[1] = bulletSpeed;
}

/**
 * Sets the color of the generated Bullet object.
 * @param float *c  color array of the fired bullet
 */
void Weapon::setColor(float *c)
{
    c[0] = color[0];
    c[1] = color[1];
    c[2] = color[2];
}

/**
 * Basic weapon class constructor
 */
Basic::Basic()
{
    fireRate = 0.25;
    bulletSpeed = 15.0;
    color[0] = 1.0;
    color[1] = 0.0;
    color[2] = 0.0;
}

/**
 * Implementation of the fire function for the Basic weapon
 */
void Basic::fire()
{
    struct timespec bt;
    if (getTimeSlice(&g.ship, &bt) > fireRate) {
        timeCopy(&g.ship.bulletTimer, &bt);
        if (g.nPlayerBullets < MAX_BULLETS) {
            Bullet *b = &g.playerBarr[g.nPlayerBullets];
            timeCopy(&b->time, &bt);
            setPosition(g.ship.pos, b->pos);
            setVelocity(b->vel);
            setColor(b->color);
            g.nPlayerBullets++;
        }
    }
}

/**
 * Rapid weapon class constructor
 */
Rapid::Rapid()
{
    fireRate = 0.125;
}

/**
 * Scatter weapon class constructor
 */
Scatter::Scatter()
{
    fireRate = 0.5;
    bulletSpeed = 15;
    color[0] = 0.5;
    color[1] = 1.0;
    color[2] = 0.5;
    shotsFired = 4;
    spread = 30.0;
    start = 90 - (spread / 2);
    increment = spread / (shotsFired - 1);
    temp = 0.0;
}

/**
 * Generate bullets that will move within a specified spread
 * @param int shots     Number of bullets within the spread
 * @param float spread  The degree of spread desired
 * @param float *bull   Array of bullet objects to be adjusted
 */
void Scatter::bulletSpread(float *vel)
{
    vel[0] = vel[1] * cos(convertToRads(temp));
    vel[1] = vel[1] * sin(convertToRads(temp));
}

/**
 * Initializes multiple instances of Bullet objects for
 * a burst like fire mode.
 */
void Scatter::fire()
{
    temp = start;
    struct timespec bt;
    if (getTimeSlice(&g.ship, &bt) > fireRate) {
        timeCopy(&g.ship.bulletTimer, &bt);
        for (int i = 0; i < shotsFired; i++) {
            if(g.nPlayerBullets < MAX_BULLETS) {
                Bullet *b = &g.playerBarr[g.nPlayerBullets];
                timeCopy(&b->time, &bt);
                setPosition(g.ship.pos, b->pos);
                setVelocity(b->vel);
                bulletSpread(b->vel);
                setColor(b->color);
                temp += increment;
                g.nPlayerBullets++;
            }
        }
    }
}

/**
 * Ring weapon class constructor
 */
Ring::Ring()
{
    fireRate = 1.0;
    bulletSpeed = 2.0;
    color[0] = 0.5;
    color[1] = 1.0;
    color[2] = 0.5;
    shotsFired = 45;
    spread = 360.0;
    start = 90 - (spread / 2);
    increment = spread / (shotsFired - 1);
    temp = 0.0;
};

/**
 * Pinwheel weapon class constructor
 */
Pinwheel::Pinwheel()
{
    fireRate = 0.6;
    bulletSpeed = 1.0;
    color[0] = 0.5;
    color[1] = 1.0;
    color[2] = 0.5;
    shotsFired = 4;
    spread = 0.0;
    increment = 2.0;
}

/**
 * Generates multiple Bullet objects while constantly
 * incrementing their angle of fire to create a pinwheel
 * like effect.
 */
void Pinwheel::fire()
{
    struct timespec bt;
    if (getTimeSlice(&g.ship, &bt) > fireRate) {
        timeCopy(&g.ship.bulletTimer, &bt);
        for (int i = 0; i < shotsFired; i++) {
            if(g.nPlayerBullets < MAX_BULLETS) {
                Bullet *b = &g.playerBarr[g.nPlayerBullets];
                timeCopy(&b->time, &bt);
                setPosition(g.ship.pos, b->pos);
                b->vel[0] = b->vel[1] = bulletSpeed;
                b->vel[0] *= cos(convertToRads(spread));
                b->vel[1] *= sin(convertToRads(spread));
                setColor(b->color);
                spread += 90.0;
                g.nPlayerBullets++;
            }
        }
    }
    spread = 0.0 + increment;
    increment += 2.0;
    if (increment > 90)
        increment = 0.0;
}

/**
 * Reticle seek function
 */
void Reticle::seek()
{
    if (e->nextShip != NULL)
        e = e->nextShip;
}

/**
 * Function to update position of reticle
 */
void Reticle::update()
{
    pos[0] = e->pos[0];
    pos[1] = e->pos[1];
    pos[2] = e->pos[2];
}

/**
 * Reticle rendering function
 */
void Reticle::drawReticle(bool lock)
{
    int x, y, x1, y1;
    if (!lock) {
        glPushMatrix();
	    glTranslatef(pos[0], pos[1], pos[2]);
	    glColor3f(seekColor[0],seekColor[1],seekColor[2]);
	    glLineWidth(2.0f);
	    glBegin(GL_LINE_LOOP);
	    for (int i = 0; i < 120; i++) {
		    x = 40 * cos(convertToRads(angle + (i*3)));
		    y = 40 * sin(convertToRads(angle + (i*3)));
		    glVertex3f(x, y, 0.0);
	    }
	    glEnd();
        glBegin(GL_LINES);
        for (int i = 0; i < 4; i++) {
            x = 30 * cos(convertToRads(angle + (i*90)));
            y = 30 * sin(convertToRads(angle + (i*90)));
            x1 = 50 * cos(convertToRads(angle + (i*90)));
            y1 = 50 * sin(convertToRads(angle + (i*90)));
            glVertex3f(x, y, 0.0);
            glVertex3f(x1, y1, 0.0);
        }
        glEnd();
	    glPopMatrix();

	    if (angle < 90) {
		    angle += 5.0;
	    } else {
		    angle = 0.0;
	    }
    } else {
        glPushMatrix();
	    glTranslatef(pos[0], pos[1], pos[2]);
	    glColor3f(lockColor[0],lockColor[1],lockColor[2]);
	    glLineWidth(2.0f);
	    glBegin(GL_LINE_LOOP);
        angle = 0.0;
	    for (int i = 0; i < 120; i++) {
		    x = 40 * cos(convertToRads(angle + (i*3)));
		    y = 40 * sin(convertToRads(angle + (i*3)));
		    glVertex3f(x, y, 0.0);
	    }
	    glEnd();
        glBegin(GL_LINES);
        for (int i = 0; i < 4; i++) {
            x = 30 * cos(convertToRads(angle + (i*90)));
            y = 30 * sin(convertToRads(angle + (i*90)));
            x1 = 50 * cos(convertToRads(angle + (i*90)));
            y1 = 50 * sin(convertToRads(angle + (i*90)));
            glVertex3f(x, y, 0.0);
            glVertex3f(x1, y1, 0.0);
        }
        glEnd();
	
	    glPopMatrix();
    }
}

/**
 * Secondary weapon class constructor
 */
Secondary::Secondary()
{
    fireRate = 1.0;
    bulletSpeed = 5.0;
    color[0] = 1.5;
    color[1] = 0.5;
    color[2] = 1.0;
	reticle.seekColor[0] = 0.5;
	reticle.seekColor[1] = 0.5;
	reticle.seekColor[2] = 1.0;
	reticle.lockColor[0] = 1.0;
	reticle.lockColor[1] = 0.0;
	reticle.lockColor[2] = 0.0;
	reticle.angle = 0.0;
	armed = false;
    locked = false;
}

/**
 * Secondary weapon specific implementation of setVelocity.
 * Sets both X and Y velocity to bulletSpeed so that they
 * can be adjusted with trig functions.
 * @param vel Array containing the objects X/Y velocity
 */
void Secondary::setVelocity(float *vel)
{
    vel[0] = bulletSpeed;
    vel[1] = bulletSpeed;
}

/**
 * Generates a Missile object. Special implementation required
 * to account for tracking starting and ending position.
 */
void Secondary::fire()
{
    struct timespec bt;
    if (getTimeSlice(&g.ship, &bt) > fireRate) {
        timeCopy(&g.missileTimer, &bt);
        if (g.nmissiles < MAX_MISSILES) {
            Missile *m = &g.marr[g.nmissiles];
            timeCopy(&m->time, &bt);
            setPosition(g.ship.pos, m->pos);
            setPosition(g.ship.pos, m->start);
            setColor(m->color);
            g.nmissiles++;
        }
    }
}

/**
 * Missile object constructor
 */
Missile::Missile() { }

/**
 * Updates objects position so that it continually moves towards
 * the target.
 * @param m      Missile object to be moved
 * @param target Array containing targets X/Y coordinates
 * @param t      Time value for the Bezier curve
 */
void Missile::tracking(float *target, float t)
{
    float mid[3] = {0.0, 0.0, 0.0};
    if (start[0] < 500.0)
        mid[1] = start[1];

    if (start[0] > 500.0) {
        mid[0] = 900.0;
        mid[1] = start[1];
    }

    pos[0] = (pow(1 - t, 2.0) * start[0]) + (2 * (1 - t) * t * mid[0]
        ) + (pow(t, 2.0) * target[0]);
    pos[1] = (pow(1 - t, 2.0) * start[1]) + (2 * (1 - t) * t * mid[1]
        ) + (pow(t, 2.0) * target[1]);
}

/**
 * Standard Enemy Weapon constructor
 */
EnemyStd::EnemyStd()
{
    bulletSpeed = -5.0;
    fireRate = 2.0;
}

/**
 * Overload fire function for EnemyStd.
 * Accepts an angle as an argument and uses it to keep
 * the weapon firing in the correct direction during ship
 * movement.
 * @param float angle   Angle determining weapon's firing direction
 */
void EnemyStd::fire(EnemyShip *ship, float angle)
{
    struct timespec bt;
    double time = getTimeSlice(ship, &bt);
    if (time > fireRate) {
        timeCopy(&ship->bulletTimer, &bt);
        if (g.nEnemyBullets < MAX_BULLETS) {
            Bullet *b = &g.enemyBarr[g.nEnemyBullets];
            timeCopy(&b->time, &bt);
            setPosition(ship->pos, b->pos);
            setVelocity(b->vel);
            // b->vel[0] *= cos(convertToRads(angle));
            // b->vel[1] *= sin(convertToRads(angle));
            setColor(b->color);
            g.nEnemyBullets++;
        }
    }
}

/**
 * Upgrade class constructor
 */
Upgrade::Upgrade(float x, float y) {
	dropSpeed = 5.0;
	w = 50.0;
	h = 50.0;
    pos[0] = x;
    pos[1] = y;
	glGenTextures(1, &pod);
    payload = rand() % 3 + 1;
}

void Upgrade::drawUpgrade() {
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, pod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *transData = buildAlphaData(&upgradePod);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, upgradePod.width, upgradePod.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(pos[0], pos[1], 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w,-h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
    glEnd();
    glPopMatrix();
    pos[1] -= dropSpeed;
}

int Upgrade::detectCollision(float x, float y)
{
    float distX = x - pos[0];
    float distY = y - pos[1];
    float totalDist = (distX*distX + distY*distY);
    float range = w*w;
    if (totalDist < range)
       return payload; 
    else
       return 0;
}

Shield::Shield()
{
	angle = 0.0;
	increment = 30.0;
	radius = 60.0;
	color[0] = 1.0;
	color[1] = 0.5;
	color[2] = 0.25;
	status = false;
}

// Render function for shield
void Shield::drawShield(float *pos)
{	
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glColor3f(color[0],color[1],color[2]);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 12; i++) {
		x = radius * cos(convertToRads(angle + (i*increment)));
		y = radius * sin(convertToRads(angle + (i*increment)));
		glVertex3f(x, y, 0.0);
	}
	glEnd();
	glPopMatrix();

	if (angle < 90) {
		angle += 2.0;
	} else {
		angle = 0.0;
	}
}

//===========================================================
//				DEFINITION OF THE DIGITS CLASS 
//===========================================================

Digit::Digit()
{
    resX = 10; 
    resY = 10;
    for (int i = 0; i < 10; i++)
        glGenTextures(1, &digits[i]);
}

Digit::~Digit() { }

void Digit::displayDigit(char ch, float x, float y) {
    int value = (int)ch - 48;
    glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, digits[value]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *transData = buildAlphaData(&dig[value]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dig[value].width, dig[value].height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(x, y, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(resX,-resY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(resX, resY);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-resX, resY);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-resX, -resY);
    glEnd();
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//===========================================================
//				DEFINITION OF THE USER INTERFACE 
//===========================================================

// Hud class constructor
Hud::Hud() { }

void Hud::genTextures()
{
	for (int i = 0; i < 4; i++)
		glGenTextures(1, &life[i]);
    for (int i = 0; i < 3; i++)
        glGenTextures(1, &weapon[i]);
    glGenTextures(1, &secondary);
	glGenTextures(1, &score);
    glGenTextures(1, &display);
}

void Hud::drawHud(int l, int w, int s)
{
	float resX = 128.0f;
	float resY = 72.0f;
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, life[l]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *transData = buildAlphaData(&hudLife[l]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hudLife[l].width, hudLife[l].height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(750.0, 928.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(resX,-resY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(resX, resY);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-resX, resY);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-resX, -resY);
    glEnd();
    glPopMatrix();

	resX = 25.0f;
    resY = 25.0f;
    glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, weapon[w]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	transData = buildAlphaData(&hudWeapon[w]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hudWeapon[w].width, hudWeapon[w].height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(75.0f, 75.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(resX,-resY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(resX, resY);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-resX, resY);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-resX, -resY);
    glEnd();
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);

    resX = 10.0f;
    resY = 10.0f;
    glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, secondary);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	transData = buildAlphaData(&hudSecond);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hudSecond.width, hudSecond.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(105.0f, 45.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(resX,-resY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(resX, resY);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-resX, resY);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-resX, -resY);
    glEnd();
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);

    char buffer[32];
    sprintf(buffer, "%d", s);
    int i = 0;
    float posX = 75.0;
    float posY = 925.0;
    while (buffer[i] != '\0') {
        nums[i].displayDigit(buffer[i], posX, posY);
        posX += 20;
        i++;
    }

    resX = 135.0f;
    resY = 45.0f;
    glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, score);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	transData = buildAlphaData(&hudScore);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hudScore.width, hudScore.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(175.0f, 925.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(resX,-resY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(resX, resY);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-resX, resY);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-resX, -resY);
    glEnd();
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);

    resX = 50.0f;
    resY = 50.0f;
    glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0,255.0,255.0,255.0);
	glBindTexture(GL_TEXTURE_2D, display);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	transData = buildAlphaData(&hudDisplay);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hudDisplay.width, hudDisplay.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, transData);
	free(transData);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
    glTranslatef(75.0f, 75.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(resX,-resY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(resX, resY);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-resX, resY);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-resX, -resY);
    glEnd();
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
