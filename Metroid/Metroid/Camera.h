#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Constants.h"

//start position
//#define CAM_POS_X 640
//#define CAM_POS_Y 1320

//boss position
#define CAM_POS_X 896
#define CAM_POS_Y 4200

//zeb
//#define CAM_POS_X 2432
//#define CAM_POS_Y 4200

//Rio
//#define CAM_POS_X 2432.00000
//#define CAM_POS_Y 1320.00000

//MotherBrain
//boss
//#define CAM_POS_X 912
//#define CAM_POS_Y 4208

//zeb position
//#define CAM_POS_X 3810
//#define CAM_POS_Y 2040

//waver postision
//#define CAM_POS_X 3911
//#define CAM_POS_Y 3000
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

