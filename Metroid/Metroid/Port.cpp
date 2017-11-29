#include "Port.h"
#include "Camera.h"

Port::Port() : BaseObject(eID::PORT)
{
}

Port::~Port()
{
}

void Port::update(float dt)
{
}

void Port::draw()
{
	if (this->boundCollision.left + (this->boundCollision.right - this->boundCollision.left)*0.5 <= Camera::getInstance()->getBound().right)
	{
		Camera::getInstance()->setCanFollowOnRight(false);

		Camera::getInstance()->setVelocity(VECTOR2(0, 0));
	}
	else
	{
		Camera::getInstance()->setCanFollowOnRight(true);
	}
}
