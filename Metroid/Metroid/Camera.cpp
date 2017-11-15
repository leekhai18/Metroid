#include "Camera.h"


Camera::Camera(int width, int height)
{
    this->width = width ;
    this->height = height ;
    this->position = D3DXVECTOR3(0, 0, 0);
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

int Camera::getWidth()
{
    return this->width;
}

int Camera::getHeight()
{
    return this->height;
}