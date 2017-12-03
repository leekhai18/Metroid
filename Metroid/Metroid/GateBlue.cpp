#include "GateBlue.h"
#include "Camera.h"

#define SCALE_PER_FRAME 0.1f
#define TIME_RE_SPAWN 3
#define TIME_O_C_GATE 0.2f

GateBlue::GateBlue()
{
}

GateBlue::GateBlue(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::GATEBLUE)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite GateBlue");
	}

	this->sprite->setData(IndexManager::getInstance()->gateBlueR);

	isHit = false;
	timer = 0;
}


GateBlue::~GateBlue()
{
	delete this->sprite;
}

void GateBlue::update(float dt)
{
	if (isHit)
	{
		timer += dt;

		if (timer > TIME_RE_SPAWN)
			effectRespawn();
		else
			effectDisappear();
	}


	if (isCollideSamusInPort && !Camera::getInstance()->moveWhenSamusOnPort())
	{
		timer += dt;

		if (timer > TIME_O_C_GATE)
			effectRespawn();
		else
			effectDisappear();
	}

	if (Camera::getInstance()->moveWhenSamusOnPort() && isHit)
	{
		effectRespawn();
	}
}

void GateBlue::draw()
{
	this->sprite->draw();
}

void GateBlue::effectDisappear()
{
	float scaleX = this->getScale().x;

	if (scaleX >= 0)
		this->setScaleX(scaleX - SCALE_PER_FRAME);
	else
		this->boundCollision = MetroidRect(0, 0, 0, 0);
}

void GateBlue::effectRespawn()
{
	float scaleX = this->getScale().x;

	if (scaleX < 1)
		this->setScaleX(scaleX + SCALE_PER_FRAME*1.5f);
	else
	{
		timer = 0;
		isHit = false;
		isCollideSamusInPort = false;
		this->boundCollision = this->baseBound;
	}
}

void GateBlue::setHit(bool flag)
{
	isHit = flag;
}

void GateBlue::setIsCollideSamusInPort(bool flag)
{
	this->isCollideSamusInPort = flag;
}

void GateBlue::setBoundCollision(MetroidRect rect)
{
	BaseObject::setBoundCollision(rect);
	this->baseBound = rect;
}
