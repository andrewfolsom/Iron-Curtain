/**
 * Header for all of Chad's functions. Implementation is in chadM.cpp
 * The functions under By Jackson are located in nickJA.cpp since he wrote
 * 	them all and deserves credit
*/
#ifndef CHADM_H
#define CHADM_H
class EnemyShip : public Ship {
	public :
		EnemyShip(int x, int y, int movType);
		~EnemyShip();
		double getTimeSlice(timespec *bt);
		float getRadius();
		int getDeathScore();
		void destroyShip();
		//By JACKSON
		void updateStrafe();
		void configStrafe(float radius, float angleSet, float angleSpeed, float speed, int dir);
		void updateRush();
		void configRush(float speed, int dir);
		void updateCircle();
		void configCircle(float radius, float angleSet, float angleSpeed, float speed, int dir);
		void updateBank();
		void configBank(float destX, float destY, float speed);
		void updateDiagRush();
		void configDiagRush(float x, float y, float speed);
		void updatePosition();
		//
		EnemyShip *nextShip;
		EnemyShip *prevShip;
		EnemyStd *eWpn;
	protected :
		int deathScore;
	private :
		int health;
		// By Jackson
		enum MOVETYPE { RUSH, STRAFE, CIRCLING, BANK, DIAG_RUSH };
		Vec spawnPos;
		float angle;
		float angleSpd;
		float t;  //Time for Bezier Curves
		float destX, destY;
		int speedMul = 1;
		int initFlag = 0;
		int moveFlag = -1;
		int movPattern = RUSH;
		float radius = 25.0;
		int destOffset = 700;
		//
};

class Grunt : public EnemyShip {
	public:
		Grunt(int x, int y, int movType);
};
#endif
