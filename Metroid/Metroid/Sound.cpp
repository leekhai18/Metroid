#include "Sound.h"
#include "Metroid.h"
#include "GameLog.h"
#include "ScenceManager.h"

Sound* Sound::instance = nullptr;

Sound* Sound:: getInstance()
{
	if (instance == nullptr)
		instance = new Sound(ScenceManager::getInstance()->getHWnd());

	return instance;
}

Sound::Sound(HWND Hwnd)
{
	this->hwnd = Hwnd;
	this->dsound = NULL;
	Init_DirectSound();
}

Sound::~Sound()
{

}

bool Sound::Init_DirectSound()
{
	HRESULT result;

	//Tạo 1 quản lý sound
	dsound = new GSoundManager();

	//Khởi tạo 1 dsound
	result = dsound->Initialize(hwnd, DSSCL_PRIORITY);

	if (result != DS_OK)
	{
		return false;
	}

	//Tạo định dạng đệm mặc định
	result = dsound->SetPrimaryBufferFormat(2, 22050, 16);

	if (result != DS_OK)
	{
		return false;
	}

	//Hoàn thành
	return true;
}

GSound* Sound::LoadSound(LPTSTR filename)
{
	HRESULT result;

	//Tạo dữ liệu nhạc
	GSound *wave;

	//Load 1 tệp nhạc lên
	result = dsound->Create(&wave, filename);

	if (result != DS_OK)
	{
		return NULL;
	}

	//Đã load đc nhạc
	return wave;
}

void Sound::loadSound(LPTSTR fileName, std::string name)
{
	GSound* sound = LoadSound(fileName);
	soundBufferMap[name] = sound;
}

//Chạy nhạc
void Sound::PlaySound(GSound *sound)
{
	sound->Play();
}

//Lặp lại nhạc
void Sound::LoopSound(GSound *sound)
{
	sound->Play(0, DSBPLAY_LOOPING);
}

void Sound::play(std::string name, bool isLoop)
{
	// Find it in map
	std::map< std::string, GSound*> ::iterator it;
	it = soundBufferMap.find(name);

	if (it == soundBufferMap.end())
		return;

	if (isLoop)
		LoopSound(it->second);
	else
		PlaySound(it->second);
}

//Dừng nhạc
void Sound::StopSound(GSound *sound)
{
	sound->Stop();
}

void Sound::stop(std::string name)
{
	if (name == "")
	{
		for (std::map< std::string, GSound*> ::iterator it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
		{
			StopSound(it->second);
		}
	}
	else
	{
		std::map< std::string, GSound*> ::iterator it;
		it = soundBufferMap.find(name);

		if (it == soundBufferMap.end())
			return;
		else 
			StopSound(it->second);
	}
}

void Sound::loadAllSound()
{
	loadSound("Resources/Sounds/TitleScene.wav", SOUND_TITLE);
	loadSound("Resources/Sounds/Background.wav", SOUND_BACKGROUND);
	loadSound("Resources/Sounds/BoomPut.wav", SOUND_BOMB_PUT);
	loadSound("Resources/Sounds/BoomBurst.wav", SOUND_BOMB_BURST);
	loadSound("Resources/Sounds/Die.wav", SOUND_SAMUS_DEATH);
	loadSound("Resources/Sounds/Dying.wav", SOUND_SAMUS_NEAR_DEATH);
	loadSound("Resources/Sounds/EnemyCollision.wav", SOUND_ENEMIES_COLLISION);
	loadSound("Resources/Sounds/Fire.wav", SOUND_BULLET_NORMAL);
	loadSound("Resources/Sounds/IceFire.wav", SOUND_BULLET_ICE);
	loadSound("Resources/Sounds/RocketFire.wav", SOUND_ROCKET);
	loadSound("Resources/Sounds/BonusEN.wav", SOUND_BONUS_EN);
	loadSound("Resources/Sounds/BonusRocket.wav", SOUND_BONUS_ROCKET);
	loadSound("Resources/Sounds/Jump.wav", SOUND_SAMUS_JUMP);
	loadSound("Resources/Sounds/Run.wav", SOUND_SAMUS_RUN);
	loadSound("Resources/Sounds/PortCollison.wav", SOUND_PORT);
	loadSound("Resources/Sounds/RipperCollision.wav", SOUND_RIPPER);
	loadSound("Resources/Sounds/CollectionItem.wav", SOUND_COLLECTION_ITEMS);
}

void Sound::cleanUp()
{
	for (auto i = soundBufferMap.begin(); i != soundBufferMap.end(); i++)
	{
		delete (*i).second;
	}

	soundBufferMap.erase(soundBufferMap.begin(), soundBufferMap.end());
	soundBufferMap.clear();
	delete instance;
}
