#include "Port.h"
#include "Camera.h"

#define PORT_WIDTH_HALF 16

Port::Port() : BaseObject(eID::PORT)
{
}

Port::~Port()
{
}

void Port::update(float dt)
{
	Camera::getInstance()->setNumPort(Camera::getInstance()->getNumPort() + 1);
}

void Port::draw()
{
#pragma region handle camera
	if (Camera::getInstance()->getNumPort() == 2)
	{
		Camera::getInstance()->setCanFollowOnRight(false);
		Camera::getInstance()->setCanFollowOnLeft(false);
	}
	else
	{
		if (!Camera::getInstance()->moveWhenSamusOnPort())
		{
			if (Camera::getInstance()->getPosition().x < this->boundCollision.left) // port ben phai
			{
				if (this->boundCollision.left + PORT_WIDTH_HALF <= Camera::getInstance()->getBound().right)
				{
					Camera::getInstance()->setCanFollowOnRight(false);
					Camera::getInstance()->setCanFollowOnLeft(true);

					Camera::getInstance()->setVelocity(VECTOR2(0, Camera::getInstance()->getVelocity().y));
				}
				else
				{
					Camera::getInstance()->setCanFollowOnRight(true);
				}
			}
			else // port ben trai
			{
				if (this->boundCollision.right - PORT_WIDTH_HALF >= Camera::getInstance()->getBound().left)
				{
					Camera::getInstance()->setCanFollowOnLeft(false);
					Camera::getInstance()->setCanFollowOnRight(true);

					Camera::getInstance()->setVelocity(VECTOR2(0, Camera::getInstance()->getVelocity().y));
				}
				else
				{
					Camera::getInstance()->setCanFollowOnLeft(true);
				}

			}
		}
	}
#pragma endregion
}
