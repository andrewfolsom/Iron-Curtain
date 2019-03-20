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
//SPAWN
//X & Y are spawn Coordinates
//movType acts as a flag to define movement behavior.
void spawnOpFor(int x, int y, int movType) 
{
	opForShip *newGuy = &g.opFor[g.numOpFor];
	newGuy->pos[0]= x;
	newGuy->pos[1] = y;
	newGuy->spawnPos[0]= x;
	newGuy->spawnPos[1] = y;
	newGuy->movPattern = movType;
	g.numOpFor++;

	printf( "OpFor %i created at (%.2f, %.2f) with movePattern = %i\n",
			g.numOpFor, newGuy->pos[0], newGuy->pos[1], newGuy->movPattern);
}

//RENDER OPFOR
void renderOpFor() 
{

	opForShip *draw;

	for (int i = 0; i < g.numOpFor; i++) {
		draw = &g.opFor[i];
		glColor3fv(draw->color);
		glPushMatrix();
		glTranslatef(draw->pos[0], draw->pos[1], draw->pos[2]);
		glBegin(GL_QUADS);
		glVertex2f(-20.0, -20.0);
		glVertex2f(-20.0,  20.0);
		glVertex2f( 20.0,  20.0);
		glVertex2f( 20.0, -20.0);
		glEnd();
		glPopMatrix();
		}
}

void configOpFor(int ID, int destOffset) 
{
	opForShip *Edit = &g.opFor[ID];

	Edit->destOffset = destOffset;

}

//MOVEMENT TYPES
//Press 't' to test this mid game.
//1 - Rush
//Enemy begins at the top of the screen and rushes straight to the bottom.
//Iteration will be used to identify which entity to update the position of.
void updateRush(int iteration)
{
	opForShip *move = &g.opFor[iteration];
	move->pos[1] -= move->speedMul;
}
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
void updateStrafe(int iteration)
{
	opForShip *move = &g.opFor[iteration];
	move->pos[1] -= move->speedMul;

	move->pos[0] = 10*cos(move->angle);
	move->angle+= 0.1;
}
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
void updateCircle(int iteration) 
{
	opForShip *move = &g.opFor[iteration];

	move->pos[0] += 10*cos(move->angle);
	move->pos[1] += 10*sin(move->angle) - move->speedMul;

	move->angle+= 0.1;
}
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
void updateBank(int iteration)
{
	opForShip *move = &g.opFor[iteration];

	//printf("Opfor %i spawned at (%f, %f)\n", g.numOpFor, move->spawnPos[0], move->spawnPos[1]);
	//printf("Opfor %i Before update at (%f, %f)\n", g.numOpFor, move->pos[0], move->pos[1]);

	float midPoint[2] = {0.0, 0.0};
	
	if (move->destOffset < 450)
		midPoint[0] = 900.0;

	midPoint[1] = move->spawnPos[1];

	move->pos[0] = (pow(1-move->t, 2.0) * move->spawnPos[0]) + (2*(1-move->t)*move->t*midPoint[0]) + 
					(pow(move->t, 2.0) * move->destOffset);
	move->pos[1] = (pow(1-move->t, 2.0) * move->spawnPos[1]) + (2*(1-move->t)*move->t*midPoint[1]) + 
					(pow(move->t, 2.0) * (0));
	move->t += 0.006;

	if (move->t > 1.0)
		move->t = 0;

	//printf("Opfor %i at (%f, %f)\n", g.numOpFor, move->pos[0], move->pos[1]);
}
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
void updateDiagRush(int iteration)
{
	opForShip *move = &g.opFor[iteration];

	move->pos[1] -= move->speedMul;
	move->pos[0] = (move->pos[1]-1000)/-1.13;
}
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
//each entity in the OpFor array.
void updatePosition()
{

	for (int i = 0; i <= g.numOpFor; i++) {
		opForShip *target = &g.opFor[i];
		
		switch (target->movPattern) {

			case (RUSH):
				updateRush(i);
				break;

			case (STRAFE):
				updateStrafe(i);
				break;

			case (CIRCLING):
				updateCircle(i);
				break;

			case (BANK):
				updateBank(i);
				break;

			case (DIAG_RUSH):
				updateDiagRush(i);
				break;
		}
	}

}

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
