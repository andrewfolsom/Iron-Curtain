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
    ring,
    pinwheel,
    missile,
    enemyStd,
    upgrade
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
        void fire(float angle);
};

// Upgrade Declarations

class Upgrade : public Basic {
    public:
        Upgrade();
};

class Shield
{
	public:
		float angle;
		float increment;
		float radius;
		float x, y;
		float color[3];
		bool status;
	public:
		Shield();
		void drawShield(float* pos);
};
#endif
