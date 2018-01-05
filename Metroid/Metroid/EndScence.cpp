#include "EndScence.h"


void EndScence::init()
{
	textureM = new TextureManager();
	if (!textureM->initialize(graphics, ENDGAME_IMAGE))
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
	rect.right = textureM->getWidth();
	rect.bottom = textureM->getHeight();
	data.rect = rect;
	data.position = VECTOR2(0, 0);
	data.rotate = 0;
	data.flipVertical = false;
	data.flipHorizontal = false;
}



void EndScence::update()
{
}

void EndScence::draw()
{
	if (SUCCEEDED(graphics->beginScene()))
	{

		graphics->spriteBegin();


		data.texture = textureM->getTexture();

		graphics->drawSprite(false, data, GraphicsNS::WHITE);

		graphics->spriteEnd();

		graphics->endScene();
	}

	handleLostGraphicsDevice();

	graphics->showBackbuffer();
}

void EndScence::release()
{
	textureM->onLostDevice();
	delete textureM;
}

EndScence::EndScence(Graphics * graphics, Input * input)
{
	this->graphics = graphics;
	this->input = input;
}

void EndScence::run()
{
	update();
	draw();


}

EndScence::EndScence()
{

}


EndScence::~EndScence()
{

}
