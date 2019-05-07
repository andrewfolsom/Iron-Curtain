//Created By: Nicholas Jackson
//May 3rd, 2019




//Sprite Class Declarations

class SpriteList {
	public:
		SpriteList();
		GLuint phantom;
		GLuint mig;


		void drawPhantom(float x, float y);
		void drawMig(float x, float y, float angle);
};

//Enemy Tank Class Def.

class EnemyTank : public Tank {
	public:
		EnemyTank(float x, float y, int faction);
		EnemyTank *nextTank;
		EnemyTank *prevTank;

};
