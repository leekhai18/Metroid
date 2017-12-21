#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Constants.h"
//
//#define CAM_POS_X 640
//#define CAM_POS_Y 1320

#define CAM_POS_X 640//6144
#define CAM_POS_Y 3656//2992//

//#define CAM_POS_X 912//6144
//#define CAM_POS_Y 4208//2992//
class Camera
{
private:
	static Camera* instance;

	int             width;
	int             height;

	D3DXVECTOR3     position;

	D3DXVECTOR2		velocity;

	bool			canFollowToUp;
	bool			canFollowToDown;
	bool			canFollowOnRight;
	bool			canFollowOnLeft;
	bool			isOnPort;
	float			distanceMoveWhenThoughtPort;
	int				numPort;

public:
	static Camera* getInstance();

	Camera() {};
    Camera(int width, int height);
	~Camera();

    void setPosition(float x, float y);
    void setPosition(D3DXVECTOR3 pos);
	void setPosition(D3DXVECTOR2 pos);

    int getWidth();
    int getHeight();

	D3DXVECTOR3 getPosition();
	MetroidRect getBound();

	D3DXVECTOR2 getVelocity();
	void setVelocity(D3DXVECTOR2 vec);

	MetroidRect getActiveArea();

	void update(float dt);

	void setCanFollowToUp(bool flag);
	bool canFolowUp();

	void setCanFollowToDown(bool flag);
	bool canFolowDown();

	void setCanFollowOnRight(bool flag);
	bool canFolowOnRight();

	void setCanFollowOnLeft(bool flag);
	bool canFolowOnLeft();

	void setOnPort(bool flag);
	bool moveWhenSamusOnPort();

	void setNumPort(int num);
	int getNumPort();

	void release();
};

