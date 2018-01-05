#include "Scence.h"



Scence::Scence()
{
}

Scence::Scence(Graphics * graphics, Input * input)
{
	this->graphics = graphics;
	this->input = input;
}

void Scence::init()
{
}

void Scence::update()
{
}

void Scence::draw()
{
}

void Scence::run()
{
	
}
void Scence::release()
{
}
void Scence::handleLostGraphicsDevice()
{
	// test for and handle lost device
	HRESULT hr = graphics->getDeviceState();
	if (FAILED(hr))                  // if graphics device is not in a valid state
	{
		// if the device is lost and not available for reset
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);             // yield cpu time (100 mili-seconds)
			return;
		}
		// the device was lost but is now available for reset
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			//del();
			hr = graphics->reset(); // attempt to reset graphics device
			if (FAILED(hr))          // if reset failed
				return;
			//resetAll();
		}
		else
			return;                 // other device error
	}
}


Scence::~Scence()
{
}
