#include "CanonBullet.h"
#include "MachineCanon.h"
#include "Collision.h"
#include "SamusStateManager.h"
#define CANON_BULLET_VELOCITY_X 150
#define CANON_BULLET_VELOCITY_Y 150
#define TIME_HIT 0.3f
CanonBullet::CanonBullet()
{
}

CanonBullet::CanonBullet(Graphics * graphics, TextureManager * textureM, CanonType type, MachineCanon* canon)
	:BaseObject(eID::CANON_BULLET)
{
	this->sprite = new Sprite();
	this->canon = canon;
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}
	this->explose = new Animation(sprite, IndexManager::getInstance()->canonBulletExplose, 2, TIME_HIT,false);
	this->type = type;
	switch (type)
	{
	case CanonType::CANON_LEFT:
		this->sprite->setData(IndexManager::getInstance()->canonBulletRightDown);
		resetFrame = IndexManager::getInstance()->canonBulletRightDown;
		break;
	case CanonType::CANON_RIGHT:
		this->sprite->setData(IndexManager::getInstance()->canonBulletLeftDown);
		resetFrame = IndexManager::getInstance()->canonBulletLeftDown;
		break;
	case CanonType::CANON_TOP:
		this->sprite->setData(IndexManager::getInstance()->canonBulletDown);
		resetFrame = IndexManager::getInstance()->canonBulletDown;
		break;
	default:
		break;
	}
	isActivity = false;
	this->sprite->setOrigin(VECTOR2(0.5, 0.5));
}

void CanonBullet::setListWallCanCollide(map<int, BaseObject*>* list)
{
	this->listWallCanCollide = list;
}

void CanonBullet::init()
{
	switch (type)
	{
	case CanonType::CANON_LEFT:

		P1 = VECTOR2(canon->getBoundCollision().right + 1, canon->getBoundCollision().bottom - 1) 
			-VECTOR2(canon->getBoundCollision().right, canon->getBoundCollision().bottom) ;
		P2 = VECTOR2(canon->getBoundCollision().right, canon->getBoundCollision().bottom - 1)
			- VECTOR2(canon->getBoundCollision().right, canon->getBoundCollision().bottom);

		//angle = acos(D3DXVec2Dot(&P1, &P2) / (D3DXVec2Length(&P1)*D3DXVec2Length(&P2)));
		D3DXVec2Normalize(&normalize, &P1);
		this->setPosition(VECTOR2(canon->getBoundCollision().right, canon->getBoundCollision().bottom));
		break;
	case CanonType::CANON_RIGHT:
		P1 = VECTOR2(canon->getBoundCollision().left - 1, canon->getBoundCollision().bottom -1)
			- VECTOR2(canon->getBoundCollision().left , canon->getBoundCollision().bottom);
		P2 = VECTOR2(canon->getBoundCollision().left , canon->getBoundCollision().bottom - 1)
			- VECTOR2(canon->getBoundCollision().left , canon->getBoundCollision().bottom);

		D3DXVec2Normalize(&normalize, &P1);
		//angle = acos(D3DXVec2Dot(&P1, &P2) / (D3DXVec2Length(&P1)*D3DXVec2Length(&P2)));
		this->setPosition(VECTOR2(canon->getBoundCollision().left , canon->getBoundCollision().bottom ));
		break;
	case CanonType::CANON_TOP:
		P1 = VECTOR2(canon->getBoundCollision().left - 1, canon->getBoundCollision().bottom - 1)
			- VECTOR2(canon->getBoundCollision().left, canon->getBoundCollision().bottom);
		P2 = VECTOR2(canon->getBoundCollision().left, canon->getBoundCollision().bottom - 1)
			- VECTOR2(canon->getBoundCollision().left, canon->getBoundCollision().bottom);

		D3DXVec2Normalize(&normalize, &P1);
		this->setPosition(VECTOR2((canon->getBoundCollision().right - canon->getBoundCollision().left)*0.5f, canon->getBoundCollision().bottom));
		//angle = acos(D3DXVec2Dot(&P1, &P2) / (D3DXVec2Length(&P1)*D3DXVec2Length(&P2)));
		break;
	default:
		break;
	}
	this->velocity = VECTOR2(CANON_BULLET_VELOCITY_X*normalize.x, CANON_BULLET_VELOCITY_Y*normalize.y);
	this->isActivity = true;
	isCollided = false;
	listCollide = new list<CollisionReturn>();
	this->sprite->setData(this->resetFrame);
	this->explose->setPause(false);
	setBoundCollision();

	dame = 3;
}

void CanonBullet::end()
{
	this->isActivity = false;
}

bool CanonBullet::getCollided()
{
	return isCollided;
}

void CanonBullet::setCollided(bool collide)
{
	isCollided = collide;
}



list<CollisionReturn>* CanonBullet::getListCollide()
{
	return this->listCollide;
}


void CanonBullet::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->sprite->getWidth() * 0.5f;
	rect.right = position.x + this->sprite->getWidth() * 0.5f;
	rect.top = position.y + this->sprite->getHeight() * 0.5f;
	rect.bottom = position.y - this->sprite->getHeight() * 0.5f;

	this->boundCollision = rect;
}

void CanonBullet::handleVelocity(float dt)
{
	
}
void CanonBullet::onCollisionSamus(Samus* samus, float dt)
{
	if (Collision::getInstance()->checkCollision(this, samus, dt))
	{
		if(!isCollided&&this->isActivity)
		{
			isCollided = true;
			this->velocity = VECTOR2ZERO;
			if (!samus->isInStatus(eStatus::INJURING))
			{
				SamusStateManager::getInstance()->setOldStatus(samus->getStatus());
				samus->setStatus(eStatus::INJURING);
				samus->setHealth(samus->getHealth() - dame);
				SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getCurrentState());
			}
		}
	}
}
void CanonBullet::onCollision(float dt)
{
	if(isActivity&&!isCollided)
	{
		for (auto i = listWallCanCollide->begin(); i != listWallCanCollide->end(); ++i)
		{
			Collision::getInstance()->checkCollision(this, i->second, dt);
		}
		for (auto i = listCollide->begin(); i != listCollide->end(); ++i)
		{
			isCollided = true;
			this->velocity = VECTOR2ZERO;
			this->setPosition(VECTOR2(this->getPosition().x + 2,i->object->getBoundCollision().top-  2));
		}
		if(listCollide->size() > 0)
		{
			
			//this->isActivity = false;
			
		}
		listCollide->clear();
	}
	
}

void CanonBullet::update(float dt)
{
	
	if (isActivity)
	{
		if (isCollided)
		{
			explose->start();
			explose->update(dt);
			if (this->explose->isFinished())
			{
				this->explose->setPause(true);
				isCollided = false;
				this->isActivity = false;
			}
		}
		this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));
		setBoundCollision();
	}
}

void CanonBullet::draw()
{
	if (isActivity)
	{
		sprite->draw();
	}
	
}


CanonBullet::~CanonBullet()
{
	delete listCollide;
	delete sprite;
	delete this->explose;
}
