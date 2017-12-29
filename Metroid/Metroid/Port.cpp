#include "Port.h"
#include "Camera.h"

#define PORT_WIDTH_HALF 16
#define OFFSET_Y_CAM 40
#define OFFSET_X_CAM 3

Port::Port() : BaseObject(eID::PORT)
{
}

Port::~Port()
{
}

bool Port::Win()
{
	return isWin;
}

void Port::setWin(bool win)
{
	this->isWin = win;
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
		if (isReset)
		{
			if (this->boundCollision.left + PORT_WIDTH_HALF - OFFSET_X_CAM < Camera::getInstance()->getBound().right &&
				this->boundCollision.left + PORT_WIDTH_HALF + OFFSET_X_CAM > Camera::getInstance()->getBound().right)
			{
				Camera::getInstance()->setPosition(VECTOR2(this->boundCollision.left + PORT_WIDTH_HALF - VIEWPORT_WIDTH*0.5f,
					Camera::getInstance()->getPosition().y));
				isReset = false;
			}
			if (this->boundCollision.right - PORT_WIDTH_HALF + OFFSET_X_CAM > Camera::getInstance()->getBound().left &&
				this->boundCollision.right - PORT_WIDTH_HALF - OFFSET_X_CAM < Camera::getInstance()->getBound().left)
			{
				Camera::getInstance()->setPosition(VECTOR2(this->boundCollision.right - PORT_WIDTH_HALF + VIEWPORT_WIDTH*0.5f,
					Camera::getInstance()->getPosition().y));
				isReset = false;
			}
		
		}

	}
	else
	{
		if (!Camera::getInstance()->moveWhenSamusOnPort())
		{
			if (Camera::getInstance()->getPosition().x < this->boundCollision.left) // port ben phai
			{
				if (this->boundCollision.left + PORT_WIDTH_HALF - OFFSET_X_CAM < Camera::getInstance()->getBound().right &&
					this->boundCollision.left + PORT_WIDTH_HALF + OFFSET_X_CAM > Camera::getInstance()->getBound().right)
				{
					Camera::getInstance()->setCanFollowOnRight(false);
					Camera::getInstance()->setCanFollowOnLeft(true);

					if (!isSetUpPosition)
					{
						Camera::getInstance()->setPosition(VECTOR2(this->boundCollision.left + PORT_WIDTH_HALF - VIEWPORT_WIDTH*0.5f,
							Camera::getInstance()->getPosition().y));
						isSetUpPosition = true;
					}
				}
				else
				{
					Camera::getInstance()->setCanFollowOnRight(true);
					//Camera::getInstance()->setCanFollowOnLeft(false);
					isSetUpPosition = false;
				}
			}
			else // port ben trai
			{
				if (this->boundCollision.right - PORT_WIDTH_HALF + OFFSET_X_CAM > Camera::getInstance()->getBound().left &&
					this->boundCollision.right - PORT_WIDTH_HALF - OFFSET_X_CAM < Camera::getInstance()->getBound().left)
				{
					Camera::getInstance()->setCanFollowOnLeft(false);
					Camera::getInstance()->setCanFollowOnRight(true);

					if (!isSetUpPosition)
					{
						Camera::getInstance()->setPosition(VECTOR2(this->boundCollision.right - PORT_WIDTH_HALF + VIEWPORT_WIDTH*0.5f,
							Camera::getInstance()->getPosition().y));
						isSetUpPosition = true;
					}
				}
				else
				{
					Camera::getInstance()->setCanFollowOnLeft(true);
					//Camera::getInstance()->setCanFollowOnRight(false);
					isSetUpPosition = false;
				}

			}
		}
	}

	if (Camera::getInstance()->moveWhenSamusOnPort())
	{
		Camera::getInstance()->setPosition(VECTOR2(Camera::getInstance()->getPosition().x, this->boundCollision.top - OFFSET_Y_CAM));
		isSetUpPosition = false;
		//Camera::getInstance()->setCanFollowOnLeft(true);
		//Camera::getInstance()->setCanFollowOnRight(true);
		isReset = true;
	}
#pragma endregion
}
