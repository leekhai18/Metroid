#include "BossKraid.h"
#include "Collision.h"
#include "Camera.h"
#define BOSSKRAID_VELOCITY_Y 100
#define RATE_BEZIER 1.0f
#define BOSSKRAID_JUMP_HEIGHT 100
#define BOSSKRAID_OFFSET 0.1f
#define WIDTH_ACTIVE 144.0f

#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 1.0f
#define TIME_TO_FIRE 1.0f
#define TIME_TO_JUMP 3.0f
#define TIME_TO_RUN 5.0f
#define BOSS_VELOCITY_X 20
BossKraid::BossKraid()
{
}

BossKraid::BossKraid(TextureManager * textureM, Graphics * graphics, Samus* samus) : BaseObject(eID::BOSSKRAID)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite BossKraid");
	}

	initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->bossKraidR, NUM_FRAMES_BOSS_KRAID, 0.3f);
	this->anim->start();
	this->listWallCanCollide = new map<int,BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
	

	rocketPool = new BossRocketPool(graphics, textureM, samus, this);
	timeToFire = 0;
	this->samus = samus;
	
	isActivity = true;

	this->getSprite()->setOrigin(VECTOR2(0.5, 0.5));
	

	direction = eDirection::right;

	health = 20;

	rocketBoss = new BossKraidRocket(graphics, textureM, samus, this);
	
	this->rocket1 = new BossKraidRocket(graphics, textureM, samus, this);

	timeJump = 0;
}




BossKraid::~BossKraid()
{
	delete this->anim;
	delete this->sprite;
	delete this->listCollide;
	delete this->listWallCanCollide;
	delete this->rocketBoss;
	delete this->rocket1;
	BossRocketPool::getInstance()->release();
}

map<int,BaseObject*>* BossKraid::getListWallCanCollide()
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
	rect.right = position.x + this->getSprite()->getWidth() *0.5f - BOSSKRAID_OFFSET;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f - BOSSKRAID_OFFSET;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f + BOSSKRAID_OFFSET;

	this->boundCollision = rect;
	
}

void BossKraid::reInit()
{
	//floor = this->getPosition().y - this->getSprite()->getHeight() *0.5f;
	//if (samus->getPosition().x - this->getPosition().x <= WIDTH_ACTIVE && (this->samus->isInStatus(eStatus::STANDING)
	//	|| this->samus->isInStatus(eStatus::RUNNING) || this->samus->isInStatus(eStatus::ROLLING)))
	//{
	//	this->samus->getPosition().x > this->getPosition().x ? this->sprite->setFlipX(false) : this->sprite->setFlipX(true);
	//	//isActivity = true;
	//	P1 = VECTOR2(this->getPosition().x, floor);

	//	P3 = VECTOR2(samus->getPosition().x, floor);

	//	float center = P1.x > P3.x ? (P1.x - P3.x)*0.5f : (P3.x - P1.x)*0.5f;
	//	this->P2 = VECTOR2(P1.x + center, P1.y + BOSSKRAID_JUMP_HEIGHT);
	//	isDown = false;
	//	time = 0;
	//}
	rocket1->returnPool();
}

void BossKraid::calculateBezier()
{

}
void BossKraid::runToSamus()
{

}
void BossKraid::lauchRocket()
{
	BossKraidRocket* rocket = BossRocketPool::getInstance()->getRocket();
	setBoundCollision();
	rocket->setListWallCanCollide(this->listWallCanCollide);
	rocket->init();
}

void BossKraid::handleVelocity(float dt)
{
	
	if (isActivity)
	{
		if(samus->getPosition().x >= this->getPosition().x)
		{
			direction = eDirection::right;
			this->sprite->setFlipX(false);
			this->velocity.x = BOSS_VELOCITY_X;
		}
		else
		{
			direction = eDirection::left;
			this->sprite->setFlipX(true);
			this->velocity.x = -BOSS_VELOCITY_X;
		}
		this->timeToFire += dt;
		this->timeJump += dt;
		this->timeRun += dt;
		if(this->timeToFire >=TIME_TO_FIRE)
		{
			this->timeToFire = 0;
			lauchRocket();
		}
		
		if(this->timeRun >= TIME_TO_RUN)
		{
			stopRun = false;
			
		}

		for (int i = 0; i < BossRocketPool::getInstance()->getListRocketUsing().size(); i++)
		{
			BossRocketPool::getInstance()->getListRocketUsing()[i]->handleVelocity(dt);
		}
	}
}

void BossKraid::onCollisionSamus(float dt)
{
	if (isActivity)
	{
		for (int i = 0; i < BossRocketPool::getInstance()->getListRocketUsing().size(); i++)
		{
			BossRocketPool::getInstance()->getListRocketUsing()[i]->onCollisionSamus(dt);
		}
	}
}

void BossKraid::onCollision(float dt)
{
	if (isActivity)
	{
		for (auto i = this->listWallCanCollide->begin(); i != this->listWallCanCollide->end(); i++)
		{
			BaseObject* x = (*i).second;
			Collision::getInstance()->checkCollision(this, x, dt);
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

		for (int i = 0; i < BossRocketPool::getInstance()->getListRocketUsing().size(); i++)
		{
			BossRocketPool::getInstance()->getListRocketUsing()[i]->onCollision(dt);
		}
	}
}



void BossKraid::update(float dt)
{
	if (isActivity)
	{
		this->anim->update(dt);
		if (beHit)
		{

			timerHit = 0;
			beHit = false;
			this->anim->setPause(false);

			if (this->health <= 0)
			{

				IExplosible::start();
				this->setVelocity(VECTOR2(0, 0));
				this->isActivity = false;
				samus->setCanMoveElevator(true);
			}

		}
	

		this->setPosition(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt);
		setBoundCollision();

		if(direction==eDirection::right)
		{
			rocketBoss->getSprite()->setRotate(270);
			rocketBoss->setPosition(VECTOR2(boundCollision.left, boundCollision.bottom + this->getSprite()->getHeight() *0.5f));
		}
		else
		{
			rocketBoss->getSprite()->setRotate(270);
			rocketBoss->setPosition(VECTOR2(boundCollision.right, boundCollision.bottom + this->getSprite()->getHeight() *0.5f));
		}
		

		for (int i = 0; i < BossRocketPool::getInstance()->getListRocketUsing().size(); i++)
		{
			BossRocketPool::getInstance()->getListRocketUsing()[i]->update(dt);
		}
	
	}
	IExplosible::update(dt);
}


void BossKraid::draw()
{
	if (!isExplose)
	{
		this->sprite->draw();
		this->rocketBoss->draw();
		
		for (int i = 0; i < BossRocketPool::getInstance()->getListRocketUsing().size(); i++)
		{
			BossRocketPool::getInstance()->getListRocketUsing()[i]->draw();
		}
	}
}
void BossKraid::setBeHit(bool hit)
{
	this->beHit = hit;
}
void BossKraid::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}