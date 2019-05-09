//Created By: Nicholas Jackson
//May 3rd, 2019


class TankWeapon;

//Sprite Class Declarations

class SpriteList {
	public:
		SpriteList();
		GLuint phantom;
		GLuint mig;
		GLuint M60Hull;
		GLuint M60Trt;
		GLuint T62Hull;
		GLuint T62Trt;


		void drawPhantom(float x, float y, float angle);
		void drawMig(float x, float y, float angle);
		void drawM60Turret(float x, float y, float angle);
		void drawM60Hull(float x, float y, float angle);
		void drawT62Turret(float x, float y, float angle);
		void drawT62Hull(float x, float y, float angle);
};

//Tank Class Declarations.
class Tank
{
	public:
		bool moving;
		int factFlag;
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
		Vec gunPos;
		TankWeapon *prm;
		Tank();
		virtual ~Tank();
		struct timespec bulletTimer;

		void renderTurret(SpriteList SPR);
		void renderTank(SpriteList SPR);
		void updateTarget(int x, int y);
		void moveTank();

};

//Enemy Tank Class Declarations.
class EnemyTank : public Tank {
	public:
		EnemyTank(float x, float y, int faction);
		~EnemyTank();
		EnemyTank *nextTank;
		EnemyTank *prevTank;

		int enterFlag;
		int needNewDirection = 1;
		float aggression;
		float movTgtAngle;
		float radius = 50;
		Vec movTgt;
		Vec potentialMov[3];

		void generatePositions();
		void pickMovTgt();
		void moveEnemyTank();
		void updateAngle();

};

class TankWeapon
{
	public:
		float fireRate;
		float bulletSpeed;
		float color[3];

		TankWeapon();
		~TankWeapon();

		void setPosition(float*, float*);
		void setVelocity(float*);
		void setColor(float*);
		void fire(Tank *tank);
		void enemyFire(Tank *tank);
};
