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

	MetroidRect viewport = Camera::getInstance()->getBound();
	int columnBegin = (int) (viewport.left / tileW + 0.5f) - 1;
	int columnEnd = columnBegin + Camera::getInstance()->getWidth() / tileW + 2;
	int rowBegin = (int) ((MAP_HEIGHT - viewport.top) / tileH);
	int rowEnd = rowBegin + Camera::getInstance()->getHeight() / tileH + 1;

#pragma region set direction for following of camera
	if (mapCell[rowBegin-1][columnBegin + 1].rect == NULL && mapCell[rowEnd][columnBegin + 1].rect != NULL)
	{
		Camera::getInstance()->setCanFollowToUp(false);
		Camera::getInstance()->setCanFollowToDown(true);
		Camera::getInstance()->setCanFollowOnLeft(false);
		Camera::getInstance()->setCanFollowOnRight(false);
	}
	else if (mapCell[rowBegin-1][columnBegin + 1].rect == NULL && mapCell[rowEnd][columnBegin + 1].rect != NULL)
	{
		Camera::getInstance()->setCanFollowToUp(true);
		Camera::getInstance()->setCanFollowToDown(false);
		Camera::getInstance()->setCanFollowOnLeft(false);
		Camera::getInstance()->setCanFollowOnRight(false);
	}
	else if (mapCell[rowBegin-1][columnBegin + 1].rect != NULL && mapCell[rowEnd][columnBegin + 1].rect != NULL)
	{
		Camera::getInstance()->setCanFollowToUp(true);
		Camera::getInstance()->setCanFollowToDown(true);
		Camera::getInstance()->setCanFollowOnLeft(false);
		Camera::getInstance()->setCanFollowOnRight(false);
	}
	else if (mapCell[rowBegin-1][columnBegin + 1].rect == NULL && mapCell[rowEnd][columnBegin + 1].rect == NULL)
	{
		Camera::getInstance()->setCanFollowToUp(false);
		Camera::getInstance()->setCanFollowToDown(false);
	}
#pragma endregion

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
