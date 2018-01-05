#include "PlayScence.h"



void PlayScence::init()
{
}

void PlayScence::update()
{
}

void PlayScence::draw()
{
}

void PlayScence::run()
{
	metroidGame->run();
}

void PlayScence::release()
{
	metroidGame->releaseAll();
}

PlayScence::PlayScence(Graphics * graphics, Input * input)
{
	this->metroidGame = new  Metroid();
	metroidGame->initialize(graphics, input);
}

PlayScence::PlayScence()
{
}


PlayScence::~PlayScence()
{
}
