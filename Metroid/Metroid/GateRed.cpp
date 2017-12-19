#include "GateRed.h"
#include "Camera.h"

#define TIME_RE_OPEN 3

GateRed::GateRed()
{
}

GateRed::GateRed(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::GATERED)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite GATERED");
	}

	this->sprite->setData(IndexManager::getInstance()->gateRedR);

	timer = 0;
	isHit = false;

	closeAnim = new Animation(this->sprite, IndexManager::getInstance()->gateRedRClose, NUM_FRAMES_GATE_ANIM, 0.1f, false);
	openAnim = new Animation(this->sprite, IndexManager::getInstance()->gateRedROpen, NUM_FRAMES_GATE_ANIM, 0.1f, false);
}


GateRed::~GateRed()
{
	delete this->sprite;
	delete this->closeAnim;
	delete this->openAnim;
}

void GateRed::update(float dt)
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

void GateRed::draw()
{
	this->sprite->draw();
}

void GateRed::effectOpen()
{
	if (!openAnim->isFinished())
	{
		openAnim->start();
		closeAnim->reInit();
		this->boundCollision = MetroidRect(0, 0, 0, 0);
	}
}

void GateRed::effectClose()
{
	closeAnim->start();
	openAnim->reInit();
	isHit = false;
	isCollideSamusInPort = false;
	this->boundCollision = this->baseBound;
}

void GateRed::setHit(bool flag)
{
	isHit = flag;
}

void GateRed::setIsCollideSamusInPort(bool flag)
{
	this->isCollideSamusInPort = flag;
}

void GateRed::setBoundCollision(MetroidRect rect)
{
	BaseObject::setBoundCollision(rect);
	this->baseBound = rect;
}
