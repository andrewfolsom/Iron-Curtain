//Author: Nick Jackson
//Created: Feb 13 2019
//
//Contains:
//Personal Display Function
//MOVEMENT
//

#include <GL/glx.h>
#include <unistd.h>
#include "fonts.h"
#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "chadM.h"
#include "nickJA.h"
#include "andrewF.h"
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif

extern Game g;
extern Global gl;
extern EnemyTank *headTank;
extern EnemyTank *tailTank;
extern EnemyTank *eTank;
extern void angularAdjustment(float *vel, float angle);
extern void timeCopy(struct timespec *dest, struct timespec *source);
extern void cannonFire();
extern double timeDiff(struct timespec *start, struct timespec *end);
extern double getTimeSlice(Tank*, timespec*);
extern unsigned char *buildAlphaData(Image *img);
//const int RUSH = 0;
//const int STRAFE = 1;
//const int CIRCLING = 2;
//const int BANK = 3;

const Flt PI = 3.141592653589793;
const int MAX_BULLETS = 1000;

enum MOVETYPE { RUSH, STRAFE, CIRCLING, BANK, DIAG_RUSH};

Image Sprites[6]{
"./img/Phantom_1a.png",
"./img/MiG-21_a.png",
"./img/M60Hull.png",
"./img/M60_Turret_2.png",
"./img/T-62_Base.png",
"./img/T-62_Turret.png"
};

//Set of spawnpoints for spawnTank function.
Vec spawnPoints[5] = {{-100,1200,0},
					  {450, 1200, 0},
					  {1000, 1200, 0},
					  {-100, 800, 0},
					  {1000, 800, 0}};

//DISPLAY
void displayNick(float x, float y, GLuint texture)
{
	float wid = 100.0f;
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glColor3ub(255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();

	unsigned int color = 0x003366ff;
	Rect r;
	r.centerx = x;
	r.centery = y + 120;
	r.height = 100;
	r.width = 500;
	r.bot = r.centery - 15;
	r.left = r.centerx - 50;
	ggprint16(&r, 16, color, "Nick Jackson");
}

//***********SPRITES DISPLAY*****************
SpriteList::SpriteList()  
{
	glGenTextures(1, &phantom);
	glGenTextures(1, &mig);
	glGenTextures(1, &M60Hull);
	glGenTextures(1, &M60Trt);
}

//Function to render our F-4 Phantom image
//X & Y are position components, angle determines rotation.
void SpriteList::drawPhantom(float x, float y, float angle) 
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0, 255.0, 255.0, 255.0);
	glBindTexture(GL_TEXTURE_2D, phantom);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *alphaData = buildAlphaData(&Sprites[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Sprites[0].width, Sprites[0].height, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, alphaData);
	free(alphaData);

	float h = 93.0;
	float w = 57.0;

	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glTranslatef(x, y, 1.0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Function to render our MiG-21 image
//X & Y are position components, angle determines rotation.
void SpriteList::drawMig(float x, float y, float angle) 
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0, 255.0, 255.0, 255.0);
	glBindTexture(GL_TEXTURE_2D, mig);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *alphaData = buildAlphaData(&Sprites[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Sprites[1].width, Sprites[1].height, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, alphaData);
	free(alphaData);

	float h = 99;
	float w = 48;

	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glTranslatef(x, y, 0.9);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Function to render our M60's turret.
//The turret is placed above and separate from the hull.
//x, y, and angle do what you think they do.
void SpriteList::drawM60Turret(float x, float y, float angle) 
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0, 255.0, 255.0, 255.0);
	glBindTexture(GL_TEXTURE_2D, M60Trt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *alphaData = buildAlphaData(&Sprites[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Sprites[0].width, Sprites[0].height, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, alphaData);
	free(alphaData);

	float h = 130;
	float w = 110;

	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glTranslatef(x, y, 1.0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Function to render our M60's hull.
//The hull is below and separate from the turret.
//x, y, and angle are still working the same old job.
void SpriteList::drawM60Hull(float x, float y, float angle) 
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0, 255.0, 255.0, 255.0);
	glBindTexture(GL_TEXTURE_2D, M60Hull);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *alphaData = buildAlphaData(&Sprites[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Sprites[2].width, Sprites[2].height, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, alphaData);
	free(alphaData);

	float h = 90;
	float w = 68;

	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glTranslatef(x, y, 0.8);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Function to draw T-62 Hull
//Our old friends X, Y, and angle are at it again!
void SpriteList::drawT62Hull(float x, float y, float angle) 
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0, 255.0, 255.0, 255.0);
	glBindTexture(GL_TEXTURE_2D, T62Hull);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *alphaData = buildAlphaData(&Sprites[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Sprites[4].width, Sprites[4].height, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, alphaData);
	free(alphaData);

	float h = 90;
	float w = 68;

	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glTranslatef(x, y, 0.8);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Function to draw T-62 turret
//X, Y, and angle really need to learn a new trick.
void SpriteList::drawT62Turret(float x, float y, float angle) 
{
	glEnable(GL_TEXTURE_2D);
	glColor4ub(255.0, 255.0, 255.0, 255.0);
	glBindTexture(GL_TEXTURE_2D, T62Trt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *alphaData = buildAlphaData(&Sprites[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Sprites[5].width, Sprites[5].height, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, alphaData);
	free(alphaData);

	float h = 120;
	float w = 95;

	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glTranslatef(x, y, 1.0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Function to render tank background image
void tankBackground() 
{
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gl.tankBackground);

	float w = 900;
	float h = 1000;
	glBegin(GL_QUADS);
		glTexCoord2f(2.0f, 2.0f); glVertex2f( w, -h);
		glTexCoord2f(2.0f, 0.0f); glVertex2f( w,  h);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w, h);
		glTexCoord2f(0.0f, 2.0f); glVertex2f( -w, -h);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//********MOVEMENT TYPES***********
//Press 't' to test this mid game.
//1 - Rush
//Enemy begins at the top of the screen and rushes straight to the bottom.
//Iteration will be used to identify which entity to update the position of.
//EnemyShip compatible.
void EnemyShip::updateRush()
{
	pos[1] += (speedMul * moveFlag);
}

//Speed affects movement speed of an object.
//dir = Controls direction of vertical movement.
//		1 = move UP, 0 = NO MOVEMENT, -1 = move DOWN
void EnemyShip::configRush(float speed, int dir)
{
	movPattern = RUSH;

	moveFlag = dir;
	speedMul = speed;
}

//2 - Strafing
//Enemy Oscillates between two points while moving to the bottom of the screen.
//Iteration will be used to identify which entity to update the position of.
//EnemyShip compatible..
void EnemyShip::updateStrafe()
{
	pos[1] += speedMul* moveFlag;

	pos[0] = spawnPos[0] + radius*cos((angle*PI)/180);
	angle  += angleSpd;
}

//rad = outer bound of strafe.
//angleSet = initial angle, affects initial position of object. 
//			 generally 90 will keep object at initial position.
//angleSpeed = The speed of the object's oscillation.
//speed = speedMul for object's vertical movement
//dir = Controls direction of vertical movement.
//		1 = move UP, 0 = NO MOVEMENT, -1 = move DOWN
void EnemyShip::configStrafe(float rad, float angleSet, float angleSpeed, float speed, int dir)
{
	spawnPos[0] = pos[0];
	spawnPos[1] = pos[1];

	movPattern = STRAFE;
	radius = rad;
	angle = angleSet;
	angleSpd = angleSpeed;
	speedMul = speed;
	moveFlag = dir;
}

//3 - Circling
//Enemy follows the path of a circle whose center is consistently moving
//towards the bottom of the screen.
//Iteration will be used to identify which entity to update the position of.
//EnemyShip compatible
void EnemyShip::updateCircle() 
{
	pos[0] = spawnPos[0] + (radius * cos((angle*PI)/180));

		spawnPos[1] += speedMul * moveFlag;

	pos[1] = spawnPos[1] + (radius*sin((angle*PI)/180));

	angle+= angleSpd;
}

//rad = Radius of circle centered at spawn position.
//angleSet = initial angle. 90 will set the object at rad units above starting position.
//angleSpeed = speed at which object circles.
//speed = speedMul for object's vertical movement
//dir = Controls direction of vertical movement.
//		1 = move UP, 0 = NO MOVEMENT, -1 = move DOWN
void EnemyShip::configCircle(float rad, float angleSet, float angleSpeed, float speed, int dir)
{
	spawnPos[0] = pos[0];
	spawnPos[1] = pos[1];

	movPattern = CIRCLING;
	radius = rad;
	angle = angleSet;
	angleSpd = angleSpeed;
	speedMul = speed;
	moveFlag = dir;
}

//4 - Bank
//Enemy follows a curve while moving from the top to bottom of the screen.
//Iteration will be used to identify which entity to update the position of.

//EnemyShip compatible
void EnemyShip::updateBank()
{
	if (initFlag == 0) {
		spawnPos[0] = pos[0];
		spawnPos[1] = pos[1];
		initFlag = 1;
	}

	float midPoint[2] = {0.0, 0.0};

	if (destX < 450)
		midPoint[0] = 900.0;

	midPoint[1] = spawnPos[1];

	pos[0] = (pow(1-t, 2.0) * spawnPos[0]) + (2*(1-t)*t*midPoint[0]) + 
					(pow(t, 2.0) * destX);
	pos[1] = (pow(1-t, 2.0) * spawnPos[1]) + (2*(1-t)*t*midPoint[1]) + 
					(pow(t, 2.0) * destY);
	t += angleSpd;

	if (t > 1.0)
		t = 0;
}

//destX/destY = The target coordinates for object's path.
//speed = speed at which object follows curve.
//SPEED IS ITERATED FROM A RANGE OF 0-1
//SPEED MUST BE A SMALL VALUE, AROUND 0.005 FOR EXAMPLE.
void EnemyShip::configBank(float x, float y, float speed)
{
	spawnPos[0] = pos[0];
	spawnPos[1] = pos[1];
	movPattern = BANK;

	destX = x;
	destY = y;
	angleSpd = speed;
}

//5 - Diagonal Rush
//Enemy will follow a line following the angle from its current point to the specified point.
//EnemyShip Compatible
void EnemyShip::updateDiagRush()
{
	pos[1] += (speedMul * sin(angle));
	pos[0] += (speedMul * cos(angle));
}


void EnemyShip::configDiagRush(float x, float y, float speed)
{
	movPattern = DIAG_RUSH;

	spawnPos[0] = pos[0];
	spawnPos[1] = pos[1];
	angle = atan((y-pos[1]) / (x-pos[0]));
	if (x - pos[0] > 0) 
	angle = angle;
	else
	angle = PI + angle;

	speedMul = speed;
}

//Movement Function
//Function uses a switch case to determine what function should be applied to
//each entity in the EnemyShip List array.


//EnemyShip Compatible
void EnemyShip::updatePosition()
{
		switch (movPattern) {

			case (RUSH):
				updateRush();
				break;

			case (STRAFE):
				updateStrafe();
				break;

			case (CIRCLING):
				updateCircle();
				break;

			case (BANK):
				updateBank();
				break;

			case (DIAG_RUSH):
				updateDiagRush();
				break;
		}
}

//********TANK CLASS********
Tank::Tank() 
{
	pos[0] = 450;
	pos[1] = 250;
	health = 3;
	prm = new TankWeapon;
	factFlag = 0;
}

Tank::~Tank() 
{
	delete prm;
}

//********TANK WEAPON CLASS********
TankWeapon::TankWeapon() 
{
	fireRate = 2.0;
	bulletSpeed = 15.0;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

TankWeapon::~TankWeapon() 
{}

void TankWeapon::setPosition(float *t,float *b) 
{
	b[0] = t[0];
	b[1] = t[1];
	b[2] = 1;
}

void TankWeapon::setVelocity(float *vel) 
{
	vel[0] = 0.0;
	vel[1] = bulletSpeed;
}

void TankWeapon::setColor(float *c) 
{
	c[0] = color[0];
	c[1] = color[1];
	c[2] = color[2];
}

void TankWeapon::fire(Tank *tank) 
{
	struct timespec bt;
	double time = getTimeSlice(tank, &bt);
	if (time > fireRate) {
		tank->fireFlag = 1;
		timeCopy(&tank->bulletTimer, &bt);
		if (g.nPlayerBullets < 1000) {
			Bullet *b = &g.playerBarr[g.nPlayerBullets];
			timeCopy(&b->time, &bt);
			setPosition(tank->gunPos, b->pos);
			setVelocity(b->vel);
			angularAdjustment(b->vel, tank->tAngle+90);
			setColor(b->color);
			g.nPlayerBullets++;
			cannonFire();
			tank->fireFlag = 0;
		}
	}
}

void TankWeapon::enemyFire(Tank *tank) 
{
	struct timespec bt;
	double time = getTimeSlice(tank, &bt);
	if (time > fireRate) {
		timeCopy(&tank->bulletTimer, &bt);
		if (g.nEnemyBullets < 1000) {
			Bullet *b = &g.enemyBarr[g.nEnemyBullets];
			timeCopy(&b->time, &bt);
			setVelocity(b->vel);
			setPosition(tank->tPos, b->pos);
			angularAdjustment(b->vel, tank->tAngle+90);
			//setColor(b->color);
			b->color[0] = 1;
			//b->color[1] = b->color[2] = 0;
			g.nEnemyBullets++;
			cannonFire();
		}
	}
}

//*************TANK MOVEMENT*****************
void Tank::renderTurret(SpriteList SPR)
{
	tPos[0] = pos[0];
	tPos[1] = pos[1];
	//tAngle is the current angle of the turret.
	//tgtAngle is the angle it needs to reach to aim at the target.
	//Increment number controls turret speed. At 0.25, the turret rotates
	//at around 15 degrees per second.
	
	if (tAngle != tgtAngle) {
		if (tAngle > 135 && tgtAngle < 0)
			tAngle -=360;
		if (tAngle < 0 && tgtAngle > 135)
			tAngle +=360;
		if (tAngle > tgtAngle) {
			tAngle -= 0.25;
		}	else { 
			tAngle += 0.25;
		}
	}
	//Swaps Sprite based on player faction
	if (factFlag == 1)
		SPR.drawT62Turret(tPos[0], tPos[1], tAngle);
	else if (factFlag == 0)
		SPR.drawM60Turret(tPos[0], tPos[1], tAngle);

	gunPos[0] = tPos[0];
	gunPos[1] = tPos[1];

/* **OLD CODE** Kept in place to show pre-Sprite method
	Vec pt[50];
	float circAngle = 0;
	float angleInc = (2*PI)/50;
	//Draw Gun
	glColor3f(0.0, 0.0, 0.1);
	glPushMatrix();
	glTranslatef(tPos[0], tPos[1], .5);
	glRotatef(tAngle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2f(-2.5, 0.0);
		glVertex2f( 2.5, 0.0);
		glVertex2f( 2.5, 100);
		glVertex2f(-2.5, 100);
	glEnd();
	glPopMatrix();

	//Create points to draw turret
	//'25' is the radius ot the turret.
	for (int i = 0; i < 50; i++)
	{
		pt[i][0] = tPos[0] + (25 * cos(circAngle));
		pt[i][1] = tPos[1] + (25 * sin(circAngle));
		circAngle += angleInc;
	}

	//Draw Turret
	
	glColor3fv(tColor);
	glPushMatrix();
	//glTranslatef(tPos[0], tPos[1], 1.0);
	//glRotatef(tAngle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(tPos[0], tPos[1], 1.0);
		for (int j = 0; j < 50; j++)
		{
			glVertex3f(pt[j][0], pt[j][1], 1.0);
		}
		glVertex3f(pt[0][0], pt[0][1], 1.0);
	glEnd();
	glPopMatrix();*/
	//printf("tgtAngle is %f, tAngle is %f\n", tgtAngle, tAngle);
}

//Function is fed an X, Y value to target.
//Is suppled with cursor Coord in iron-curtain.cpp
void Tank::updateTarget(int x, int y)
{
	tgt[0] = x;
	tgt[1] = y;
	tgtAngle = atan((tgt[0] - tPos[0])/(tgt[1] - tPos[1]));
	
	if (tgt[1] - tPos[1] > 0)
		tgtAngle = -(tgtAngle * 180) /PI;
	else 
		tgtAngle = 180 - (tgtAngle * 180) /PI;


	//printf("Target Location is (%f, %f)\n", turret.tgt[0], turret.tgt[1]);
}

void Tank::renderTank(SpriteList SPR) 
{
	//Change Sprite based on player faction
	if (factFlag == 1 )
		SPR.drawT62Hull(pos[0], pos[1], angle);
	else if (factFlag == 0 )
		SPR.drawM60Hull(pos[0], pos[1], angle);

	renderTurret(SPR);

/* **OLD CODE** Kept in place to show pre-Sprite method
	glColor3fv(color);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0.1);
	glRotatef(angle, 0.0f, 0.0f, 0.1f);
	glBegin(GL_QUADS);
		glVertex2f(-30, -45);
		glVertex2f( 30, -45);
		glVertex2f( 30,  45);
		glVertex2f(-30,  45);
	glEnd();
	glPopMatrix();
*/
	//printf("Pos is (%f, %f)\n", tank.turret.tPos[0], tank.turret.tPos[1]);
}

void Tank::moveTank()
{
	float decelVal = 0.01;

	float angleRad = ((angle + 90) * 2 * PI)/360;
	float xDir = cos(angleRad);
	float yDir = sin(angleRad);
	//Speed Governor
	if (vel[0] > maxVel)
		vel[0] = maxVel;
	if (vel[0] < -maxVel)
		vel[0] = -maxVel;


	pos[0] += vel[0] * xDir;
	pos[1] += vel[0] * yDir;

	//Deceleration
	if (!moving) {
		if (vel[0] > 0)
			vel[0] -= decelVal;
		if (vel[0] < 0)
			vel[0] += decelVal;
		if (abs(vel[0]) - 0 <= 0.01)
			vel[0]=0;
	}

	//Update turret location and angle
	tPos[0] = pos[0];
	tPos[1] = pos[1];
	updateTarget(tgt[0], tgt[1]);
}

//********ENEMY TANK CLASS********
EnemyTank::EnemyTank(float x, float y, int faction) 
{
	pos[0] = x;
	pos[1] = y;
	factFlag = faction;
	prm = new TankWeapon;
	prm->fireRate = 5.0;
	prm->bulletSpeed = 6.0;
	angle = 180;
	tAngle = 180;
	aggression = rand()%100 /100;
	if (headTank == NULL) {
		headTank = tailTank = this;
		prevTank = nextTank = NULL;
	}
	else {
		prevTank = tailTank;
		prevTank->nextTank = tailTank = this;
	}
	nextTank = NULL;
}

EnemyTank::~EnemyTank() 
{
	if (prevTank != NULL) {
		prevTank->nextTank = nextTank;
	}
	else {
		headTank = nextTank;
	}
	if (nextTank != NULL) {
		nextTank->prevTank = prevTank;
	}
	else {
		tailTank = prevTank;
	}
}

//********ENEMY TANK FUNCTIONS********
//Original spawnTank function. Still used for debugging
//or for when I want to punish my CPU.
void spawnTank() 
{
	int x = rand()%5;

	eTank = new EnemyTank(spawnPoints[x][0], spawnPoints[x][1], 1);
}

//Overloaded spawnTank to spawn a set number of enemy tanks 
//at randomly selected spawns
void spawnTank(int spawnNum)
{
	int x;
	for (int i = 0; i < spawnNum; i++) {
		x = rand()%5;
		eTank = new EnemyTank(spawnPoints[x][0], spawnPoints[x][1], 1);
	}
}

//Overloaded spawnTank for debugging.
void spawnTank(float x, float y) 
{
	eTank = new EnemyTank(x, y, 1);	
	eTank->movFlag = 5;
}

//Generate 3 random positions for pick function to select from
void EnemyTank::generatePositions() 
{
	//for (int i = 0; i < 3; i++) {
	for (int i = 0; i < 3; i++) {
		potentialMov[i][0] = rand()%9 * 100;
		potentialMov[i][1] = rand()%10 * 100;
	}
}

//Will pick a location to move to.
//Location will be decided by the sum of variables
//affected by each tank's 'aggression'
void EnemyTank::pickMovTgt() 
{
	float favor = 0;
	int greatest;
	float xDiff, yDiff;

	//Parameters to determine favor
	float distToPlayer;
	float distFromStart;
	//Loop to evalutate each potential spot.
	for (int i = 0; i < 3; i++) {
		xDiff = tgt[0] - potentialMov[i][0];
		yDiff = tgt[1] - potentialMov[i][1];
		distToPlayer = sqrt((xDiff*xDiff) + (yDiff*yDiff));
		xDiff = pos[0] - potentialMov[i][0];
		yDiff = pos[1] - potentialMov[i][1];
		distFromStart = sqrt((xDiff*xDiff) + (yDiff*yDiff));
		//Evaluate and select best target.
		if ( (distFromStart * aggression) + (distToPlayer/aggression)  > favor) {
			favor = (distFromStart * aggression) + (distToPlayer/aggression);
			greatest = i;
		}
	}
	movTgt[0] = potentialMov[greatest][0];
	movTgt[1] = potentialMov[greatest][1];
	//Set tank to move and reset direction flag.
	needNewDirection = 0;
	moving = 1;
}

void EnemyTank::updateAngle() 
{
	//Find the angle and convert it to degrees.
	movTgtAngle = atan((movTgt[0] - pos[0])/(movTgt[1] - pos[1]));
	movTgtAngle = (movTgtAngle * 180) / PI;
	if (movTgt[1] - pos[1] > 0)
		movTgtAngle = movTgtAngle;
	else
		movTgtAngle = 180 - movTgtAngle;

}

void EnemyTank::moveEnemyTank()
{
	float xdiff = movTgt[0] - pos[0];
	float ydiff = movTgt[1] - pos[1];
	float distance = sqrt((xdiff * xdiff) + (ydiff * ydiff)); 
	//Used for debugging, if a tank has a movFlag of 5
	//it will not be moved.
	if (movFlag == 5)
		return;


	//Increase velocity while tank is not whwere it wants to be.
	if (distance >=100 && moving) {
		vel[0]+= 0.8;
		moving = 1;
	}	else {
		moving = 0;
		needNewDirection = 1;
	}
		//Cleans up runaway tanks
		if (pos[0] < -200 || pos[0] > 1200)
			delete this;
		if (pos[1] > 1400 || pos[1] < -100)
			delete this;

	//printf("Dist = %f\n", distance);
	//Rotate tank to point to destination
	if (angle != movTgtAngle && moving) {
		if (angle > 135 && movTgtAngle < 0)
			angle -=360;
		if (angle < 0 && movTgtAngle > 135)
			angle +=360;
		if (angle > movTgtAngle) {
			angle -= 0.25;
		}	else { 
			angle += 0.25;
		}
	}
	//Causes tanks to rotate in place if the angle to target is to great.
	if (abs(movTgtAngle-angle) > 12)
		vel[0] = vel[1] = 0;

	//Calls Tank::moveTank() to handle actual position updating.
	moveTank();
}
