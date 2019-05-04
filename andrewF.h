// Iron Curtain Weapon System - Declarations
// Author: Andrew Folsom
// Date: 02/20/2019

#ifndef WEAPONS_H
#define WEAPONS_H

//===========================================================
//              DEFINITION OF WEAPON TYPE ENUM
//===========================================================
enum wpnType {
    basic = 0,
    rapid,
    scatter,
    shield,
    ring,
    pinwheel,
    miss,
    enemyStd,
    upgrade
};

enum scndType {
    missile = 0
};

// Player Weapon Declarations

class Weapon
{
public:
	float fireRate;
	float bulletSpeed;
	float color[3];
public:
	Weapon();
	virtual ~Weapon();
	void setPosition(float*, float*);
	virtual void setVelocity(float*);
	void setColor(float*);
	virtual void fire() =0;
};

class Basic : public Weapon
{
public:
	Basic();
	void fire();
	void fire(float angle);
};

class Rapid : public Basic
{
public:
	Rapid();
};

class Scatter : public Weapon
{
public:
	int shotsFired;
	float spread;
	float start;
	float increment;
	float temp;
public:
	Scatter();
	void bulletSpread(float*);
	void fire();
};

class Ring : public Scatter
{
public:
	Ring();
};

class Pinwheel : public Scatter
{
public:
	Pinwheel();
	void fire();
};

class EnemyShip;

struct Reticle
{
	float pos[3];
	float seekColor[3];
	float lockColor[3];
	float angle;
    EnemyShip* e;
    void seek();
    void update();
    void drawReticle(bool);
};

// Secondary Weapon Declarations

class Secondary : public Basic
{
public:
	Reticle reticle;
    bool armed;
    bool locked;
public:
	Secondary();
	void setVelocity(float*);
	void fire();
};

class Missile : public Secondary
{
public:
	float start[3];
	float pos[3];
	timespec time;
public:
	Missile();
	void tracking(float *, float);
};

//Added Lab 8 03/14/2019
// Enemy Weapon Declarations
class EnemyStd : public Basic
{
    public:
        EnemyStd();
        void fire(EnemyShip *ship, float angle);
};

// Upgrade Declarations

class Upgrade {
	public:
		GLuint pod;
		float dropSpeed;
		float w, h;
		float pos[3];
		int payload;
    public:
        Upgrade(float x, float y);
		void drawUpgrade();
        int detectCollision(float x, float y);
};

class Shield
{
	public:
		float angle;
		float increment;
		float radius;
		float x, y;
		float color[3];
        float time;
		bool status;
        struct timespec shieldTimer;
	public:
		Shield();
		void drawShield(float* pos);
        void checkTime();
        bool detectCollision(float dist);
};

class Digit
{
    public:
        GLuint digits[10];
        float resX, resY;
        float posX, posY;
    public:
        Digit();
        ~Digit();
        void displayDigit(char ch, float x, float y);
};

class Hud
{
	public:
		GLuint life[4];
        GLuint weapon[3];
        GLuint display;
		GLuint score;
        GLuint secondary;
        Digit nums[10];
	public:
		Hud();
		void genTextures();
		void drawHud(int l, int w, int s);
};

class Particle
{
	public:
		struct timespec pTimer;
		float color[3];
		float pos[3];
		float vel[3];
	public:
		Particle();
};

#endif
