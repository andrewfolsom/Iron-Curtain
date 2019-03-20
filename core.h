/*
 * Core Class Declerations for Iron Curtain
 */

#ifndef CORE
#define CORE

#include "andrewF.h"

typedef float Flt;
typedef float Vec[3];

/*
 * Image Class
 */

class Image
{
    public:
	int width, height;
	unsigned char *data;
	Image(const char *);
	~Image();
};

/*
 * Global Class
 */
class Texture
{
    public:
    float xc[2];
    float yc[2];
    Texture();
};

class Global
{
    public:
	Texture tex;
    int xres, yres;
	char keys[65536];
	int gameState;
	GLuint nickImage;
	GLuint andrewImage;
	GLuint spencerImage;
	GLuint chadImage;
	GLuint benImg;
	GLuint ironImage;
    GLuint verticalBackground;
	GLuint pauseScreen;
	GLuint hiddenWorld;
	GLuint errorPage;
	GLuint startScreen;
	GLuint loadingScreen;
	static Global& getInstance();
    private:
	Global();
	Global(Global const&);
	Global& operator=(Global const&);
};

/*
 * Ship Class
 */

class Ship
{
	public:
		Vec pos;
		float vel[4];
		float speed;
		float color[3];
		float detRadius;
		Weapon *wpn;
		Secondary *scnd;
		Shield *shield;
		Ship();
};

/*
 *OPFOR Class
 *	Jackson
 */
class opForShip
{
    public:
	Vec pos;
	float vel[4];
	float speed;
	float color[3] = {255.0, 0.0, 0.0};
	int weaponType;
	int movPattern = 1;
	int speedMul = 1;
	//Currently needed for movement
	float angle = 90.0;
	float slope;
	float t = 0.0;
	Vec spawnPos;
	int destOffset = 0.0;
	//
	opForShip();
	opForShip(const opForShip&);
	opForShip& operator= (const opForShip&);
};

/*
 * Bullet Class
 */

class Bullet
{
    public:
	Vec pos;
	Vec vel;
	float color[3];
	timespec time;
    public:
	Bullet();
};

/*
 * Game Class
 */

class Game
{
    public:
	Ship ship;
	Bullet *barr;
	Missile *marr;
	int nbullets;
	int nmissiles;
	Flt radius;
	timespec bulletTimer;
	timespec missileTimer;
	timespec thrustTimer;
	bool thrustOn;
	//-----------------------------
	//Jackson
	opForShip opFor[100];
	int numOpFor;
	//------------------------------
    public:
	Game();
	~Game();
};

/*
 * X11 Wrapper Class
 */

class X11_wrapper
{
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	X11_wrapper();
	~X11_wrapper();
	void set_title();
	void setup_screen_res(const int, const int);
	void swapBuffers();
	void clearWindow();
	bool getXPending();
	XEvent getXNextEvent();
};

#endif
