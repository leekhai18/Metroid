#include "SpriteManager.h"

SpriteManager* SpriteManager::instance = nullptr;

const SpriteData* SpriteManager::getSpritesData()
{
	return this->spritesData;
}
SpriteManager * SpriteManager::getInstance()
{
	if (instance == nullptr)
		instance = new SpriteManager();

	return instance;
}
bool SpriteManager::initialize(const char* filename)
{
	this->filename = filename;
	try
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);
		assert(jSon.IsObject());
		// read spriteData from jSon
		// Read rect of sprites
		const Value& frames = jSon["frames"];
		assert(frames.IsArray());
		//convert document to value to loop all item in array
		this->spritesData = new SpriteData[frames.Size()];
		for (SizeType i = 0; i < frames.Size(); i++)
		{
			SpriteData spriteData;
			const Value& frame = frames[i]["frame"];
			//get spriteData in json
			spriteData.width = frame["w"].GetInt();
			spriteData.height = frame["h"].GetInt();
			spriteData.rotate = 0;

			RECT rect;
			rect.left = frame["x"].GetInt();
			rect.top = frame["y"].GetInt();
			rect.bottom = rect.top + spriteData.height;
			rect.right = rect.left + spriteData.width;
			spriteData.rect = rect;
			this->spritesData[i] = spriteData;
		}
	}
	catch (const std::exception&)
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not convert to spriteData");
	}
	return true;
}
void SpriteManager::releaseAll()
{
	delete[] spritesData;
}
SpriteManager::SpriteManager(void)
{
}


SpriteManager::~SpriteManager(void)
{
}