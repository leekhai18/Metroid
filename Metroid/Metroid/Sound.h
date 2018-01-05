#pragma once
#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>
#include "dsutil.h"

/////////////////
// LINKING lib //
/////////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#define SOUND_TITLE "TitleScene.wav"
#define SOUND_BACKGROUND "Background.wav"
#define SOUND_BOMB_PUT "BoomPut.wav"
#define SOUND_BOMB_BURST "BoomBurst.wav"
#define SOUND_SAMUS_DEATH "Die.wav"
#define SOUND_SAMUS_NEAR_DEATH "Dying.wav"
#define SOUND_ENEMIES_COLLISION "EnemyCollision.wav"
#define SOUND_BULLET_NORMAL "Fire.wav"
#define SOUND_BULLET_ICE "IceFire.wav"
#define SOUND_ROCKET "RocketFire.wav"
#define SOUND_BONUS_EN "BonusEN.wav"
#define SOUND_BONUS_ROCKET "BonusRocket.wav"
#define SOUND_SAMUS_JUMP "Jump.wav"
#define SOUND_SAMUS_RUN "Run.wav"
#define SOUND_PORT "PortCollison.wav"
#define SOUND_RIPPER "RipperCollision.wav"
#define SOUND_COLLECTION_ITEMS "CollectionItem.wav"


class Sound
{
private:
	GSoundManager *dsound;
	HWND hwnd;

	static Sound* instance;

public:
	Sound(HWND);
	~Sound();

	static Sound* getInstance();

	bool Init_DirectSound();
	GSound *LoadSound(LPTSTR);
	void loadSound(LPTSTR fileName, std::string name);

	void PlaySound(GSound *);
	void LoopSound(GSound *);
	void play(std::string name, bool isLoop);

	void StopSound(GSound *);
	void stop(std::string name);

	void loadAllSound();

	std::map<std::string, GSound*> soundBufferMap; // using map to save by name sound

	void cleanUp();  // delete this instance
};

