#pragma once
#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>

/////////////////
// LINKING lib //
/////////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#define SOUND_BULLET "bullet.wav"
#define SOUND_EXPLOSION "explosion.wav"
#define SOUND_JUMP "jump.wav"
#define SOUND_ROCKET "missile.wav"
#define SOUND_BOSSMOTHER_HIT "bossMotherHit.wav"
#define SOUND_COLLECTION_BONUS "collectionBonus.wav"
#define SOUND_COLLECTION_ITEMS "collectionItem.wav"
#define SOUND_COLLIDE_ENEMIES "collideEnemies.wav"
#define SOUND_DEATH "die.wav"
#define SOUND_NEAR_DEATH "nearDie.wav"
#define SOUND_PUT_BOMB "putBomb.wav"


// Sử dụng DirectSound để đọc file .wav (only wav support). Current failure: Không thể sử dụng lại file mp3 convert sang wav

class Sound
{
public:

	//When loading in .wav files I first read in the header to determine the required information for loading in the .wav audio data
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	void static create(HWND hWnd); // create sound instance in current hWnd game
	void setVolume(float percentage, std::string name = ""); // setup volume
	bool loadSound(char* fileName, std::string name); // 
	void play(std:: string name , bool infiniteLoop); // play sound by name, or 1 or loop
	void stop(std::string name = ""); // stop by name sound
	float getVolume(); // get volume
	~Sound();
	static Sound* getInstance();  // using singleton
	void mute(); // mute
	void unMute(); // un mute
	void cleanUp();  // delete this instance

	void loadAllSound();

private:
	Sound(HWND hWnd); // constructor to init driectSound on hWnd
	static Sound * instance;
	IDirectSound8* pDevice;
	IDirectSoundBuffer* primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*> soundBufferMap; // using map to save by name sound
	bool isMute;
	float volume;
};

