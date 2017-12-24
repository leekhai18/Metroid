#include "OptionScence.h"



void OptionScence::init()
{
	textureM = new TextureManager();
	if(!textureM->initialize(graphics, STARTGAME_IMAGE))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load image start game");
	}
	//data = new SpriteData();
	data.origin = VECTOR2(0, 0);
	data.scale = VECTOR2(1.0f, 1.0f);
	data.width = textureM->getWidth();
	data.height = textureM->getHeight();
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right= textureM->getWidth();
	rect.bottom = textureM->getHeight();
	data.rect = rect;
	data.position = VECTOR2(0, 0);
	data.rotate = 0;
	data.flipVertical = false;
	data.flipHorizontal = false;
}

void OptionScence::update()
{
}

void OptionScence::draw()
{
	graphics->spriteBegin();

	
	data.texture = textureM->getTexture();

	graphics->drawSprite(false,data, GraphicsNS::WHITE);

	graphics->spriteEnd();
}

OptionScence::OptionScence()
{
}

OptionScence::OptionScence(Graphics * graphics, Input * input)
{
	this->graphics = graphics;
	this->input = input;
}


OptionScence::~OptionScence()
{
	textureM->onLostDevice();
	delete textureM;
	//delete data;
}
