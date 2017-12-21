#include "BossKraid.h"
#include "Collision.h"
#include "Camera.h"
#define BOSSKRAID_VELOCITY_Y 100
#define RATE_BEZIER 1.0f
#define BOSSKRAID_JUMP_HEIGHT 100
#define BOSSKRAID_OFFSET 0.1f
#define WIDTH_ACTIVE 144.0f
BossKraid::BossKraid()
{
}

BossKraid::BossKraid(TextureManager * textureM, Graphics * graphics,Samus* samus) : BaseObject(eID::BOSSKRAID)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite BossKraid");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->bossKraidR, NUM_FRAMES_BOSS_KRAID, 0.3f);
	this->anim->start();
	this->listWallCanCollide = new list<BaseObject*>();
	this->listCollide = new list<CollisionReturn>();

	time = 0;
	this->samus = samus;
	isDown = false;
	isActivity = false;

	this->getSprite()->setOrigin(VECTOR2(0.5, 0.5));
	setBoundCollision();

	direction = eDirection::right;

	heath = 100;
}




BossKraid::~BossKraid()
{
	delete this->anim;
	delete this->sprite;
}

list<BaseObject*>* BossKraid::getListWallCanCollide()
{
	return this->listWallCanCollide;
}

list<CollisionReturn>* BossKraid::getListCollide()
{
	return this->listCollide;
}

void BossKraid::setStartPosition(VECTOR2 position)
{
	this->startPosition = position;
}

void BossKraid::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f + BOSSKRAID_OFFSET;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f -BOSSKRAID_OFFSET;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f - BOSSKRAID_OFFSET;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f + BOSSKRAID_OFFSET;

	this->boundCollision = rect;
}

void BossKraid::reInit()
{
	floor = this->getPosition().y - this->getSprite()->getHeight() *0.5f;
	if (samus->getPosition().x - this->getPosition().x <= WIDTH_ACTIVE&&( this->samus->isInStatus(eStatus::STANDING)
		|| this->samus->isInStatus(eStatus::RUNNING) || this->samus->isInStatus(eStatus::ROLLING)))
	{
		this->samus->getPosition().x > this->getPosition().x ? this->sprite->setFlipX(false) : this->sprite->setFlipX(true);
		isActivity = true;
		P1 = VECTOR2(this->getPosition().x, floor);

		P3 = VECTOR2(samus->getPosition().x, floor);

		float center = P1.x > P3.x ? (P1.x - P3.x)*0.5f : (P3.x - P1.x)*0.5f;
		this->P2 = VECTOR2(P1.x + center, P1.y + BOSSKRAID_JUMP_HEIGHT);
		isDown = false;
		time = 0;
	}
}

void BossKraid::calculateBezier()
{

}
void BossKraid::handleVelocity(float dt)
{
	
	if (isActivity)
	{
		
		if (!isDown)
		{
			this->beforePosition = this->getPosition();
			if(time >1)
			{
				int test =0;
			}
			time += dt * RATE_BEZIER;
			if (time >= 1.0f)
			{
				time = 1.0f;
			}
			
			this->afterPosition = ((1 - time)*(1 - time)*P1 + 2 * (1 - time)*time*P2 + time*time*P3);

			//if(time !=0)
			if(direction==eDirection::right)
			{
				this->velocity = (this->afterPosition - this->beforePosition)/dt ;
			}
			else
			{
				this->velocity = (this->beforePosition - this->afterPosition) / dt;
			}
		
		}
		else
		{
			
			reInit();
		}
	}
	else
	{
		reInit();
	}
}

void BossKraid::onCollision(float dt)
{
	for (auto i = this->listWallCanCollide->begin(); i != this->listWallCanCollide->end(); i++)
	{
		Collision::getInstance()->checkCollision(this, *i, dt);
	}

	for (auto x = this->listCollide->begin(); x != this->listCollide->end(); x++)
	{
		switch (x->direction)
		{
		case CollideDirection::LEFT:
			//this->velocity.y
			this->velocity.x = 0;
			//isDown = true;
			break;
		case CollideDirection::RIGHT:
			this->velocity.x = 0;
			//isDown = true;
			break;
		case CollideDirection::TOP:
			this->velocity.y = 0;
			
			break;
		case CollideDirection::BOTTOM:
			this->velocity.y = 0;
			break;
		}
	}

	this->listCollide->clear();
}


void BossKraid::update(float dt)
{
	this->anim->update(dt);
	this->setPosition(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt);
	setBoundCollision();
	if(time == 1.0f)
	{
		isDown = true;
		this->velocity.x = 0;
	}
	if (this->getPosition().x - this->getSprite()->getWidth() *0.5f  < Camera::getInstance()->getBound().left ||
		this->getPosition().x + this->getSprite()->getWidth() *0.5f> Camera::getInstance()->getBound().right)
	{
		
		reInit();
	}
}


void BossKraid::draw()
{
	this->sprite->draw();
}
