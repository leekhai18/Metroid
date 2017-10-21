#include "Sprite.h"

Sprite::Sprite()
{
	initialized = false;            // set true when successfully initialized
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;       // used to select one frame from multi-frame image
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;      // the sprite texture (picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	textureManager = NULL;
	spriteManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;               // default to 1 second per frame of animation
	animTimer = 0.0;
	visible = true;                 // the image is visible
	loop = true;                    // loop frames
	animComplete = false;
	graphics = NULL;                // link to graphics system
	colorFilter = GraphicsNS::WHITE; // WHITE for no change
}


Sprite::~Sprite()
{
}
//= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
// Initialize the Image.
// Post: returns true if successful, false if failed
// pointer to Graphics
// width of Image in pixels  (0 = use full texture width)
// height of Image in pixels (0 = use full texture height)
// number of columns in texture (1 to n) (0 same as 1)
// pointer to TextureManager
//=============================================================================
bool Sprite::initialize(Graphics *g, TextureManager *textureM, SpriteManager *spriteM)
{
	try {
		graphics = g;                               // the graphics object
		textureManager = textureM;                  // pointer to texture object
		spriteManager = spriteM;

		spriteData.texture = textureManager->getTexture();
		setRect();
	}
	catch (...) { return false; }
	initialized = true;                                // successfully initialized
	return true;
}


//=============================================================================
// Draw the image using color as filter
// The color parameter is optional, WHITE is assigned as default in image.h
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//=============================================================================
void Sprite::draw(COLOR_ARGB color)
{

	if (!visible || graphics == NULL)
		return;
	// get fresh texture incase onReset() was called
	spriteData.texture = textureManager->getTexture();
	if (color == GraphicsNS::FILTER)                     // if draw with filter
		graphics->drawSprite(spriteData, colorFilter);  // use colorFilter
	else
		graphics->drawSprite(spriteData, color);        // use color as filter
}

//=============================================================================
// Draw this image using the specified SpriteData.
//   The current SpriteData.rect is used to select the texture.
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//=============================================================================
void Sprite::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	sd.rect = spriteData.rect;                  // use this Images rect to select texture
	sd.texture = textureManager->getTexture();  // get fresh texture incase onReset() was called

	if (color == GraphicsNS::FILTER)             // if draw with filter
		graphics->drawSprite(sd, colorFilter);  // use colorFilter
	else
		graphics->drawSprite(sd, color);        // use color as filter
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Sprite::update(float frameTime)
{
	if (endFrame - startFrame > 0)          // if animated sprite
	{
		animTimer += frameTime;             // total elapsed time
		if (animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame < startFrame || currentFrame > endFrame)
			{
				if (loop == true)            // if looping animation
					currentFrame = startFrame;
				else                        // not looping animation
				{
					currentFrame = endFrame;
					animComplete = true;    // animation complete
				}
			}
			setRect();                      // set spriteData.rect
		}
	}
}

//=============================================================================
// Set the current frame of the image
//=============================================================================
void Sprite::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = c;
		animComplete = false;
		setRect();                          // set spriteData.rect
	}
}

//=============================================================================
//  Set spriteData.rect to draw currentFrame
//=============================================================================
inline void Sprite::setRect()
{
	//spriteData.rect = spriteManager->getSpritesData()[MOVE_RIGHT[currentFrame]].rect;
	//spriteData.width = spriteManager->getSpritesData()[MOVE_RIGHT[currentFrame]].width;
	//spriteData.height = spriteManager->getSpritesData()[MOVE_RIGHT[currentFrame]].height;
	//spriteData.angle = spriteManager->getSpritesData()[MOVE_RIGHT[currentFrame]].angle;
}