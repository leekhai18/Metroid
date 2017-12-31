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
		Document document;
		document.ParseStream(isw);
		assert(document.IsObject());
		// read spriteData from jSon
		// Read rect of sprites
		const Value& frames = document["frames"];
		assert(frames.IsArray());
		//convert document to value to loop all item in array
		SizeType size = frames.Size();
		this->spritesData = new SpriteData[size];
		for (SizeType i = 0; i < size; i++)
		{
			SpriteData spriteData;
			const Value& frame = frames[i]["frame"];
			//get spriteData in json
			spriteData.width = frame["w"].GetInt();
			spriteData.height = frame["h"].GetInt();

			RECT rect;
			rect.left = frame["x"].GetInt();
			rect.top = frame["y"].GetInt();
			rect.bottom = rect.top + spriteData.height;
			rect.right = rect.left + spriteData.width;
			spriteData.rect = rect;
			this->spritesData[i] = spriteData;
		}

#pragma region Write Json File
		Document d;
		d.Parse("json");

		ofstream ofs("json\\MetroidTexture1.json");
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);

		writer.StartObject();
		writer.Key("frames");

		writer.StartArray();

		for (SizeType i = 0; i < size; i++)
		{
			writer.StartObject();
			writer.Key("frame");

				writer.StartObject();

				writer.Key("x");
				writer.Int(this->spritesData[i].rect.left);

				writer.Key("y");
				writer.Int(this->spritesData[i].rect.top);

				writer.Key("w");
				writer.Int(this->spritesData[i].width);

				writer.Key("h");
				writer.Int(this->spritesData[i].height);

				writer.EndObject();

			writer.EndObject();
		}

		writer.EndArray();
		writer.EndObject();
#pragma endregion

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