#include "Camera.h"
#include "Constants.h"
#include <cmath>

#define ACTIVE_AREA_WIDTH 32
#define ACTIVE_AREA_HEIGHT 40

Camera* Camera::instance = nullptr;
Camera * Camera::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
		instance->width = VIEWPORT_WIDTH;
		instance->height = VIEWPORT_HEIGHT;
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
	this->canFollowToUp = false;
	this->canFollowToDown = false;
	this->canFollowOnLeft = true;
	this->canFollowOnRight = true;
	this->isOnPort = false;
	this->distanceMoveWhenThoughtPort = 0;
	this->numPort = 0;

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
    bound.top =  (LONG) (position.y + height * 0.5f);
    bound.bottom=(LONG) (bound.top - height);

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

MetroidRect Camera::getActiveArea()
{
	MetroidRect area;
	area.left = (this->position.x - ACTIVE_AREA_WIDTH*0.5f);
	area.right =(area.left + ACTIVE_AREA_WIDTH);
	area.top =  (this->position.y + ACTIVE_AREA_HEIGHT*0.5f);
	area.bottom= (area.top - ACTIVE_AREA_HEIGHT);

	return area;
}

void Camera::update(float dt)
{
	this->position = D3DXVECTOR3(this->position.x + this->velocity.x * dt, this->position.y + this->velocity.y * dt, 0);

	if (this->isOnPort)
	{
		this->distanceMoveWhenThoughtPort += this->velocity.x * dt;

		if (std::abs(this->distanceMoveWhenThoughtPort) >= VIEWPORT_WIDTH)
		{
			this->velocity.x = 0;
			this->distanceMoveWhenThoughtPort = 0;
			this->isOnPort = false;
		}
	}
}

void Camera::setCanFollowToUp(bool flag)
{
	this->canFollowToUp = flag;
}

bool Camera::canFolowUp()
{
	return this->canFollowToUp;
}

void Camera::setCanFollowToDown(bool flag)
{
	this->canFollowToDown = flag;
}

bool Camera::canFolowDown()
{
	return this->canFollowToDown;
}

void Camera::setCanFollowOnRight(bool flag)
{
	this->canFollowOnRight = flag;
}

bool Camera::canFolowOnRight()
{
	return this->canFollowOnRight;
}

void Camera::setCanFollowOnLeft(bool flag)
{
	this->canFollowOnLeft = flag;
}

bool Camera::canFolowOnLeft()
{
	return this->canFollowOnLeft;
}

void Camera::setOnPort(bool flag)
{
	this->isOnPort = flag;
}

bool Camera::moveWhenSamusOnPort()
{
	return this->isOnPort;
}

void Camera::setNumPort(int num)
{
	this->numPort = num;
}

int Camera::getNumPort()
{
	return this->numPort;
}

void Camera::release()
{
	delete instance;
}

int Camera::getWidth()
{
    return this->width;
}

int Camera::getHeight()
{
    return this->height;
}