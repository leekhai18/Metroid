#include "Map.h"



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

void Map::setCamera(Camera * camera)
{
	this->camera = camera;
}

void Map::draw()
{
	VECTOR2 trans = VECTOR2(camera->getWidth()*0.5f - camera->getPosition().x, camera->getHeight()*0.5f - camera->getPosition().y);

	int height = this->info->getMapHeight();
	int width = this->info->getMapWidth();
	int tileH = this->info->getTileHeight();
	int tileW = this->info->getTileWidth();

	SpriteData spriteData;
	RECT rectForCheck;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{

			MapCell** mapCell = this->info->getMap();
			if (mapCell[i][j].visible == false || mapCell[i][j].rect == NULL)
			{
				continue;
			}

			spriteData.rotate = 0;
			spriteData.texture = this->texture->getTexture();
			spriteData.position.x = (float)j*tileW;
			spriteData.position.y = (float)i*tileH;
			spriteData.scale = VECTOR2(1.0f, 1.0f);
			spriteData.origin = VECTOR2(0, 0);
			spriteData.rect = *(mapCell[i][j].rect);
			spriteData.width = tileW;
			spriteData.height = tileH;
			spriteData.flipVertical = false;
			spriteData.flipHorizontal = false;
			spriteData.transformCamera = trans;

			if (camera != NULL)
			{
				rectForCheck.left = (LONG) spriteData.position.x;
				rectForCheck.top = (LONG) spriteData.position.y;
				rectForCheck.right = rectForCheck.left + tileW;
				rectForCheck.bottom = rectForCheck.top + tileH;

				//neu nam ngoai camera thi khong Draw
				if (isContain(rectForCheck, camera->getBound()) == false)
					continue;
			}

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
