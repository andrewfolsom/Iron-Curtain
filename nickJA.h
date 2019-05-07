//Created By: Nicholas Jackson
//May 3rd, 2019




//Sprite Class Declarations

class SpriteList {
	public:
		SpriteList();
		GLuint phantom;


		void drawPhantom(float x, float y);
};

//Enemy Tank Class Def.

class EnemyTank : public Tank {
	public:
		EnemyTank(float x, float y, int faction);
		EnemyTank *nextTank;
		EnemyTank *prevTank;

};
