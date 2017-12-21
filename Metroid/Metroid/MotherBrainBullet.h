#pragma once
#include "BaseObject.h"
#include "Samus.h"
class MotherBrainBullet :public BaseObject 
{
private:
	float time;

	float maxTime;

	VECTOR2 P1;

	VECTOR2 P2;

	VECTOR2 P3;

	VECTOR2 beforePosition;

	VECTOR2 afterPosition;

	bool stop;

	bool visible;

	bool isCreate;
public:

	MotherBrainBullet(TextureManager * textureM, Graphics * graphics);

	void setP1(VECTOR2 p1);

	void setP2(VECTOR2 p2);

	void setP3(VECTOR2 p3);

	bool getCreate();

	bool getStop();

	void setStop(bool stop);

	void setCreate(bool isCreate);

	void handleVelocity(float dt);

	void update(float dt);

	void draw();

	void onCollision(Samus* samus,float dt);

	void setBoundCollision();

	float getTime();

	void setTime(float time);

	float getMaxTime();

	void setMaxTime(float maxTime);

	bool getVisible();

	void setVisible(bool visible);
	//void init(VECTOR2 stPosition);

	MotherBrainBullet();
	~MotherBrainBullet();
};

