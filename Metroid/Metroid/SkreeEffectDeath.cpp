#include "SkreeEffectDeath.h"
#define NUM_PART 4
#define DISTANCE 70

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

			if (this->list[i]->isInStatus(eStatus::JUMPING))
			{
				this->list[i]->release();
				this->list[i]->setStatus(eStatus::ENDING);
			}
		}
	}
}

void SkreeEffectDeath::draw()
{
	for (int i = 0; i < NUM_PART; i++)
	{
		if (!this->list[i]->isInStatus(eStatus::ENDING))
			this->list[i]->draw();
	}
}

void SkreeEffectDeath::init(VECTOR2 position)
{
	if (this->position != position)
	{
		this->position = position;

		VECTOR2 target[NUM_PART];
		target[0] = VECTOR2(position.x + DISTANCE, position.y);
		target[1] = VECTOR2((float) ( (target[0].x - position.x)*cos(D3DXToRadian(-60)) - (target[0].y - position.y)*sin(D3DXToRadian(-60)) + position.x  ),
							(float)	( (target[0].x - position.x)*sin(D3DXToRadian(-60)) + (target[0].y - position.y)*cos(D3DXToRadian(-60)) + position.y ));
		target[2] = VECTOR2((float) ( (target[0].x - position.x)*cos(D3DXToRadian(-120)) - (target[0].y - position.y)*sin(D3DXToRadian(-120)) + position.x  ),
							(float) ( (target[0].x - position.x)*sin(D3DXToRadian(-120)) + (target[0].y - position.y)*cos(D3DXToRadian(-120)) + position.y ));
		target[3] = VECTOR2(position.x - DISTANCE, position.y);

		for (int i = 0; i < NUM_PART; i++)
		{
			this->list[i]->init(target[i], position);
		}
	}
}

bool SkreeEffectDeath::isInit()
{
	if (this->position != VECTOR2ZERO)
		return true;

	return false;
}
