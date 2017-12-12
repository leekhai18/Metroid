#include "SkreeEffectDeath.h"
#define NUM_PART 4
#define DISTANCE 50

SkreeEffectDeath::SkreeEffectDeath(TextureManager * textureM, Graphics * graphics)
{
	this->list = new SkreeEffectDeathPart*[4];
	this->position = VECTOR2ZERO;

	for (int i = 0; i < NUM_PART; i++)
	{
		this->list[i] = new SkreeEffectDeathPart(textureM, graphics);
	}
}

SkreeEffectDeath::SkreeEffectDeath()
{
}

SkreeEffectDeath::~SkreeEffectDeath()
{
	for (int i = 0; i < NUM_PART; i++)
		delete list[i];

	delete[] list;
}

void SkreeEffectDeath::update(float dt)
{
	if (this->position != VECTOR2ZERO)
	{
		for (int i = 0; i < NUM_PART; i++)
		{
			this->list[i]->update(dt);
		}
	}
}

void SkreeEffectDeath::draw()
{
	if (this->position != VECTOR2ZERO)
	{
		for (int i = 0; i < NUM_PART; i++)
		{
			this->list[i]->draw();
		}
	}
}

void SkreeEffectDeath::init(VECTOR2 position)
{
	this->position = position;

	VECTOR2 target[NUM_PART];
	target[0] = VECTOR2(position.x + DISTANCE, position.y);
	target[1] = VECTOR2((float)((target[0].x - position.x)*cos(D3DXToRadian(60)) - (target[0].y - position.y)*sin(D3DXToRadian(60)) + position.x),
		(float)((target[0].x - position.x)*sin(D3DXToRadian(60)) + (target[0].y - position.y)*cos(D3DXToRadian(60)) + position.y));
	target[2] = VECTOR2((float)((target[0].x - position.x)*cos(D3DXToRadian(120)) - (target[0].y - position.y)*sin(D3DXToRadian(120)) + position.x),
		(float)((target[0].x - position.x)*sin(D3DXToRadian(120)) + (target[0].y - position.y)*cos(D3DXToRadian(120)) + position.y));
	target[3] = VECTOR2(position.x - DISTANCE, position.y);

	for (int i = 0; i < NUM_PART; i++)
	{
		this->list[i]->init(target[i], position);
	}
}


void SkreeEffectDeath::setPosition(VECTOR2 pos)
{
	this->position = pos;
}

VECTOR2 SkreeEffectDeath::getPosition()
{
	return this->position;
}

bool SkreeEffectDeath::isFinished()
{
	for (int i = 0; i < NUM_PART; i++)
	{
		if (!this->list[i]->isFinished())
		{
			return false;
		}
	}

	return true;
}
