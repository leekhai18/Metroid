#include "BossKraid.h"

BossKraid::BossKraid()
{
}

BossKraid::BossKraid(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BOSSKRAID)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite BossKraid");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->bossKraidR, NUM_FRAMES_BOSS_KRAID, 0.3f);
	this->anim->start();
}


BossKraid::~BossKraid()
{
	delete this->anim;
	delete this->sprite;
}

void BossKraid::update(float dt)
{
	this->anim->update(dt);
}

void BossKraid::draw()
{
	this->sprite->draw();
}
