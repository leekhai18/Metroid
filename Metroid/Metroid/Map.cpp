#include "Map.h"
#include "Camera.h"


Map::Map()
{
}


Map::~Map()
{
}

bool Map::initialize(Graphics * graphics, TextureManager * texture, MapInfo * info)
{
	if (graphics == nullptr || texture == nullptr || info == nullptr)
		return false;

	this->graphics = graphics;
	this->texture = texture;
	this->info = info;

	return true;
}

void Map::draw()
{
	int tileH = this->info->getTileHeight();
	int tileW = this->info->getTileWidth();
	MapCell** mapCell = this->info->getMap();

	RECT viewport = Camera::getInstance()->getBound();
	int columnBegin = viewport.left / tileW;
	int columnEnd = columnBegin + Camera::getInstance()->getWidth() / tileW + 1;
	int rowBegin = (MAP_HEIGHT - viewport.top) / tileH;
	int rowEnd = rowBegin + Camera::getInstance()->getHeight() / tileH + 1;

	// set Follow direction for camera
	if (mapCell[rowBegin - 1][columnBegin].rect == NULL)
	{
		Camera::getInstance()->setCanFoLLowVertical(false);
	}
	else
	{
		Camera::getInstance()->setCanFoLLowVertical(true);
		Camera::getInstance()->setCanFollowOnLeft(false);
		Camera::getInstance()->setCanFollowOnRight(false);
	}

	SpriteData spriteData;
	for (int col = columnBegin; col < columnEnd; col++)
	{
		for (int row = rowBegin; row < rowEnd; row++) 
		{
			if (mapCell[row][col].visible == false || mapCell[row][col].rect == NULL)
			{
				continue;
			}

			spriteData.rotate = 0;
			spriteData.texture = this->texture->getTexture();
			spriteData.position.x = (float)col*tileW;
			spriteData.position.y = MAP_HEIGHT - (float)row*tileH;
			spriteData.scale = VECTOR2(1.0f, 1.0f);
			spriteData.origin = VECTOR2(0, 0);
			spriteData.rect = *(mapCell[row][col].rect);
			spriteData.width = tileW;
			spriteData.height = tileH;
			spriteData.flipVertical = false;
			spriteData.flipHorizontal = false;

			graphics->drawSprite(spriteData);
		}
	}
}

void Map::update(float dt)
{
}

void Map::collision()
{
}
