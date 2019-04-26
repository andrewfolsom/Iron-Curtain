//Author: Nick Jackson
//Created: Feb 13 2019
//
//Contains:
//Personal Display Function
//MOVEMENT
//

#include <GL/glx.h>
#include "fonts.h"
#include "core.h"
#include <stdio.h>
#include <math.h>
#include "chadM.h"
extern Game g;
extern Global gl;
extern EnemyShip *headShip;
//const int RUSH = 0;
//const int STRAFE = 1;
//const int CIRCLING = 2;
//const int BANK = 3;

const Flt PI = 3.141592653589793;

enum MOVETYPE { RUSH, STRAFE, CIRCLING, BANK, DIAG_RUSH};

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
//MOVEMENT TYPES
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
//Enemy will follow a diagonal line from its spawn to the bottom of the screen.
//EnemyShip Compatible
void EnemyShip::updateDiagRush()
{
	pos[1] += (speedMul * sin((3*PI/2) - angle));
	pos[0] += (speedMul * cos((3*PI/2) - angle));

	printf("Angle is %f, x is %f, y is %f\n", angle, pos[0], pos[1]);
}


void EnemyShip::configDiagRush(float x, float y, float speed)
{
	movPattern = DIAG_RUSH;

	spawnPos[0] = pos[0];
	spawnPos[1] = pos[1];
	angle = atan((x-spawnPos[0]) / (y-spawnPos[1]));
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


//============================================
//				TANK MOVEMENT
//============================================

void Tank::renderTurret()
{
	Vec pt[50];
	float circAngle = 0;
	float angleInc = (2*PI)/50;

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
		}
		else { 
			tAngle += 0.25;
		}
		//printf("Called %i times", calledCount);
	}
	
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
	glPopMatrix();
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

void Tank::renderTank() 
{

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

	renderTurret();

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
	/*
	if (vel[1] > 0)
		vel[1] -= decelVal;
	if (vel[1] < 0)
		vel[1] += decelVal;
	*/
	tPos[0] = pos[0];
	tPos[1] = pos[1];
	updateTarget(tgt[0], tgt[1]);
}
