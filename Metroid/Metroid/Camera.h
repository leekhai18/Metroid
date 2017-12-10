#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Constants.h"
#define CAM_POS_X 640
#define CAM_POS_Y 1320
class Camera
{
private:
	static Camera* instance;

	int             width;
	int             height;

	D3DXVECTOR3     position;

	D3DXVECTOR2		velocity;

	bool			canFollowVetical;
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
    RECT getBound();

	D3DXVECTOR2 getVelocity();
	void setVelocity(D3DXVECTOR2 vec);

	MetroidRect getActiveArea();

	void update(float dt);

	void setCanFoLLowVertical(bool flag);
	bool canFolowVertical();

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

