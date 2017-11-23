#include "Camera.h"
#define ACTIVE_AREA_WIDTH 32
#define ACTIVE_AREA_HEIGHT 32
#include "Constants.h"


Camera* Camera::instance = nullptr;
Camera * Camera::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
		instance->width = GAME_WIDTH;
		instance->height = GAME_HEIGHT;
		instance->setPosition(D3DXVECTOR2(CAM_POS_X, CAM_POS_Y));
	}

	return instance;
}

Camera::Camera(int width, int height)
{
    this->width = width ;
    this->height = height ;
    this->position = D3DXVECTOR3(0, 0, 0);
	this->velocity = D3DXVECTOR2(0, 0);
	this->canFollowHorizontal = true;

	instance = this;
}


Camera::~Camera()
{
}

void Camera::setPosition(float x, float y)
{
    setPosition(D3DXVECTOR3(x, y, 0));
}

void Camera::setPosition(D3DXVECTOR3 pos)
{
    this->position = pos;
}

void Camera::setPosition(D3DXVECTOR2 pos)
{
	setPosition(pos.x, pos.y);
}

D3DXVECTOR3 Camera::getPosition()
{
    return this->position;
}

RECT Camera::getBound()
{
    RECT bound; 

    bound.left = (LONG) (position.x - width * 0.5f);
    bound.right =(LONG) (bound.left + width);
    bound.top =  (LONG) (position.y - height * 0.5f);
    bound.bottom=(LONG) (bound.top + height);

    return bound;
}

D3DXVECTOR2 Camera::getVelocity()
{
	return this->velocity;
}

void Camera::setVelocity(D3DXVECTOR2 vec)
{
	this->velocity = vec;
}

RECT Camera::getActiveArea()
{
	RECT area;
	area.left = (LONG) (this->position.x - ACTIVE_AREA_WIDTH*0.5f);
	area.right =(LONG) (area.left + ACTIVE_AREA_WIDTH);
	area.top =  (LONG) (this->position.y - ACTIVE_AREA_HEIGHT*0.5f);
	area.bottom=(LONG) (area.top + ACTIVE_AREA_HEIGHT);

	return area;
}

void Camera::update(float dt)
{
	this->position = D3DXVECTOR3(this->position.x + this->velocity.x * dt, this->position.y + this->velocity.y * dt, 0);
}

void Camera::setCanFoLLowHorizontal(bool flag)
{
	this->canFollowHorizontal = flag;
}

bool Camera::canFollowHorizon()
{
	return this->canFollowHorizontal;
}

int Camera::getWidth()
{
    return this->width;
}

int Camera::getHeight()
{
    return this->height;
}