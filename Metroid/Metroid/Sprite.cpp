#include "Sprite.h"
#include "IndexManager.h"

Sprite::Sprite()
{
	spriteData.width = 0;
	spriteData.height = 0;
	spriteData.position = VECTOR2ZERO;
	spriteData.origin = VECTOR2ZERO;
	spriteData.scale = VECTOR2ONE;
	spriteData.rotate = 0.0;
	spriteData.rect.left = 0;       // used to select one frame from multi-frame image
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;      // the sprite texture (picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	spriteData.transformCamera = VECTOR2ZERO;
	textureManager = NULL;
	spriteManager = NULL;
	graphics = NULL;                // link to graphics system
}


Sprite::~Sprite()
{
}

void Sprite::setData(int index)
{
	if (index <= MAX_INDEX)
	{
		// Set Data for sprite
		const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[index]);

		this->setSpriteDataRect(data->rect);
		this->setSpriteWidth(data->width);
		this->setSpriteHeigth(data->height);
	}
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
	}
	catch (...) { return false; }
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

	if (graphics == NULL)
		return;
	// get fresh texture incase onReset() was called
	spriteData.texture = textureManager->getTexture();
	graphics->drawSprite(spriteData, color);
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Sprite::update(float dt)
{
}
