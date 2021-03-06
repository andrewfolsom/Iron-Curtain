/*
 * Core Class Declerations for Iron Curtain
 */

#ifndef CORE
#define CORE

#include "andrewF.h"
//#include "nickJA.h"
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
	bool chosen;
	GLuint nickImage;
	GLuint andrewImage;
	GLuint spencerImage;
	GLuint chadImage;
	GLuint benImg;
	GLuint ironImage;
        GLuint verticalBackground;
	GLuint tankBackground;
	GLuint clouds;
	GLuint pauseScreen;
	GLuint hiddenWorld;
	GLuint errorPage;
	GLuint startScreen;
	GLuint loadingScreen;
    GLuint gameControls;
	GLuint theDuck;
	GLuint mainMenu;
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
        int health;
		struct timespec bulletTimer;
        wpnType equiped;
        scndType altEquip;
		Weapon *wpn;
		Secondary *scnd;
		Shield *shield;
		Ship();
        ~Ship();
};

/*
 *Tank Class
 *	Jackson
 
 class Tank
 {
	public:
		bool moving;
		float pos[3];
		float tPos[3];
		float color[3] = {0.33, 0.42, 0.18};
		float angle;
		float tAngle;
		float tgtAngle;
		float tColor[3] = {0.33, 0.5, 0.18};
		float maxVel = 1.0;
		Vec vel = {0, 0, 0};
		Vec tgt;
		Weapon *prm;
		Tank();
		~Tank();

		void renderTurret(SpriteList SPR);
		void renderTank(SpriteList SPR);
		void updateTarget(int x, int y);
		void moveTank();
 };*/

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
	Bullet *playerBarr;
	Bullet *enemyBarr;
	Missile *marr;
	Particle *parr;
	int nPlayerBullets;
	int nEnemyBullets;
	int nmissiles;
	int nParticles;
	int tier = 1;
	Flt radius;
	timespec missileTimer;
	timespec thrustTimer;
	bool thrustOn;
	int playerScore;
	//-----------------------------
	//Jackson
	//Tank playerTank;
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
