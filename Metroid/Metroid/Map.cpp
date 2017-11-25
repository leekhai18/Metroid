#include "Map.h"
#include "Camera.h"


bool Map::isContain(RECT rect1, RECT rect2)
{
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}

	return true;
}

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
	int rowBegin = viewport.top / tileH;
	int rowEnd = rowBegin + Camera::getInstance()->getHeight() / tileH + 1;

	// set Follow direction for camera
	if (mapCell[rowBegin - 1][columnBegin].rect == NULL)
		Camera::getInstance()->setCanFoLLowHorizontal(true);
	else
		Camera::getInstance()->setCanFoLLowHorizontal(false);

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
			spriteData.position.y = (float)row*tileH;
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
