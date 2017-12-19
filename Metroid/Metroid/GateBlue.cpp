#include "GateBlue.h"
#include "Camera.h"

#define TIME_RE_OPEN 3

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

	timer = 0;
	isHit = false;

	closeAnim = new Animation(this->sprite, IndexManager::getInstance()->gateBlueRClose, NUM_FRAMES_GATE_ANIM, 0.1f, false);
	openAnim = new Animation(this->sprite, IndexManager::getInstance()->gateBlueROpen, NUM_FRAMES_GATE_ANIM, 0.1f, false);

	this->isActivity = true;
}


GateBlue::~GateBlue()
{
	delete this->sprite;
	delete this->closeAnim;
	delete this->openAnim;
}

void GateBlue::update(float dt)
{
	if (isHit)
	{
		effectOpen();

		timer += dt;
		if (timer > TIME_RE_OPEN)
		{
			timer = 0;
			effectClose();
		}
	}

	if (isCollideSamusInPort && !Camera::getInstance()->moveWhenSamusOnPort())
	{
		effectOpen();

		if (openAnim->isFinished())
		{
			effectClose();
		}
	}

	if (Camera::getInstance()->moveWhenSamusOnPort() && isHit)
	{
		effectClose();
	}

	openAnim->update(dt);
	closeAnim->update(dt);
}

void GateBlue::draw()
{
	this->sprite->draw();
}

void GateBlue::effectOpen()
{
	if (!openAnim->isFinished())
	{
		openAnim->start();
		closeAnim->reInit();
		this->isActivity = false;
	}
}

void GateBlue::effectClose()
{
	closeAnim->start();
	openAnim->reInit();
	isHit = false;
	isCollideSamusInPort = false;
	this->isActivity = true;
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
}
