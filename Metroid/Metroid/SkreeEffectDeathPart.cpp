#include "SkreeEffectDeathPart.h"

#define WIDTH_HALF 1
#define HEIGHT_HALF 1
#define TIME_RUN 0.25f


SkreeEffectDeathPart::SkreeEffectDeathPart(TextureManager * textureM, Graphics * graphics) : BaseObject()
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite SkreeEffectDeathPart");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->skreeDeathEffect);
	this->setOrigin(VECTOR2(0.5f, 0.5f));
	timer = 0;
}

SkreeEffectDeathPart::SkreeEffectDeathPart()
{
}

SkreeEffectDeathPart::~SkreeEffectDeathPart()
{
	delete this->sprite;
}

void SkreeEffectDeathPart::init(VECTOR2 target, VECTOR2 startPosition)
{
	this->target = target;
	this->startPosition = startPosition;
	this->setPosition(startPosition);
	this->setVelocity(VECTOR2((target.x - startPosition.x)/TIME_RUN, (target.y - startPosition.y)/TIME_RUN));
	this->isFinish = false;
	timer = 0;
}

void SkreeEffectDeathPart::onCollision()
{
}

void SkreeEffectDeathPart::update(float dt)
{
	if (!isFinish)
	{
		timer += dt;
		if (timer < TIME_RUN)
		{
			this->setPosition(this->getPosition().x + this->getVelocity().x * dt, this->getPosition().y + this->getVelocity().y * dt);
			this->setBoundCollision();
		}
		else
		{
			finish();
		}
	}

}

void SkreeEffectDeathPart::draw()
{
	if (!isFinish)
		this->sprite->draw();
}

void SkreeEffectDeathPart::setBoundCollision()
{
	this->boundCollision.top = this->getPosition().y + 1;
	this->boundCollision.bottom = this->getPosition().y - 1;
	this->boundCollision.left = this->getPosition().y - 1;
	this->boundCollision.right = this->getPosition().y + 1;
}

bool SkreeEffectDeathPart::isFinished()
{
	return isFinish;
}

void SkreeEffectDeathPart::finish()
{
	BaseObject::setBoundCollision(MetroidRect(0, 0, 0, 0));
	isFinish = true;
}


