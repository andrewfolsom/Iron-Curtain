//Created By: Nicholas Jackson
//May 3rd, 2019




//Sprite Class Declarations

class SpriteList {
	public:
		SpriteList();
		GLuint phantom;
		GLuint mig;
		GLuint M60Hull;
		GLuint M60Trt;


		void drawPhantom(float x, float y, float angle);
		void drawMig(float x, float y, float angle);
		void drawM60Turret(float x, float y, float angle);
		void drawM60Hull(float x, float y, float angle);
};

//Tank Class Declarations.
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

};

//Enemy Tank Class Declarations.
class EnemyTank : public Tank {
	public:
		EnemyTank(float x, float y, int faction);
		~EnemyTank();
		EnemyTank *nextTank;
		EnemyTank *prevTank;

		int factFlag;
		float aggression;
		float movTgtAngle;
		Vec movTgt;
		Vec potentialMov[3];

		void generatePositions();
		void pickMovTgt();
		void moveEnemyTank();

};
