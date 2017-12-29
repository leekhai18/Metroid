#include "Sound.h"
#include "Metroid.h"
#include "GameLog.h"

Sound* Sound::instance = nullptr;

Sound* Sound:: getInstance()
{
	if (instance == nullptr)
		instance = new Sound(Metroid::getInstance()->getCurrentHWND());

	return instance;
}

Sound::Sound(HWND hWnd)
{
	primaryBuffer = 0;
	HRESULT result;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc; //describe the buffer

	result = DirectSoundCreate8(NULL, &pDevice, NULL); // creates and initializes an object that supports the IDirectSound8 interface

	if(FAILED(result))
	{
		GAMELOG("Can not create device");
	}

	result = pDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY); // Set the cooperative level to priority so the format of the primary sound buffer can be modified.

	if (FAILED(result))
	{
		GAMELOG("Can not set cooperative level");
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME; // The dwFlags are the important part of this structure...  
																	//In this case we just want to setup a primary buffer description with the capability of adjusting its volume.
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;


	result = pDevice -> CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL); // Get control of the primary sound buffer on the default sound device.

	if(FAILED(result))
	{
		GAMELOG("Can not create primaryBuffer");
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100; // hz
	waveFormat.wBitsPerSample = 16; // bit
	waveFormat.nChannels = 2; // stereo
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		GAMELOG("Can not setup primary buffer to be the wave format specified");
	}

	volume = 100.0f;
	isMute = false;
}

Sound::~Sound()
{
	for (auto it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
	{
		if(it -> second)
		{
			it -> second -> Release();
			it -> second = NULL;
		}
		
	}
	if(primaryBuffer)
		primaryBuffer ->Release();
	if(pDevice)
		pDevice -> Release();
	pDevice = NULL;
	
	primaryBuffer = NULL;
}

void Sound :: create(HWND hWnd)
{
	if(instance == nullptr)
	{
		instance = new Sound(hWnd);
	}
}

float Sound :: getVolume()
{
	return volume;
}


//The function is what handles loading in a .wav audio file and then copies the data onto a new secondary buffer
bool Sound :: loadSound(char* fileName, std:: string name)
{
	if(soundBufferMap.find(name) != soundBufferMap.end())
		return true;

	FILE* filePtr;
	size_t count;
	WaveHeaderStruct waveHeaderStruct;
	IDirectSoundBuffer* tempBuffer;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
	unsigned char* wavData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	int error = fopen_s(&filePtr, fileName, "rb");
	if(error != 0)
	{
		GAMELOG(" Can not load: %s", fileName);
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveHeaderStruct, sizeof(WaveHeaderStruct), 1, filePtr); 
	if (count != 1)
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if((waveHeaderStruct.format[0] != 'W') || (waveHeaderStruct.format[1] != 'A') ||
	   (waveHeaderStruct.format[2] != 'V') || (waveHeaderStruct.format[3] != 'E'))
	{
		GAMELOG(" file format does not support: %s", fileName);
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveHeaderStruct.audioFormat != WAVE_FORMAT_PCM)
	{
		GAMELOG("Audio format is not WAVE_FORMAT_PCM");
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveHeaderStruct.numChannels != 2)
	{
		GAMELOG("Wave file was not recoreded in stereo format");
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveHeaderStruct.sampleRate != 44100)
	{
		GAMELOG("Wave file was not recorded at a sample rate of 44.1 KHz");
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveHeaderStruct.bitsPerSample != 16)
	{
		GAMELOG("Wave file was not recorded in 16 bit format");
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
	bufferDesc.dwBufferBytes = waveHeaderStruct.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	pDevice->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL); // Create a temporary sound buffer with the specific buffer settings.


	IDirectSoundBuffer8* secondaryBuffer = 0;
	IDirectSoundBuffer8** pSecondaryBuffer = &secondaryBuffer;

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	long result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&(*pSecondaryBuffer)); 
	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;
	if (FAILED(result))
	{
		GAMELOG("Can not create secondaryBuffer");
		return false;
	}

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderStruct), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	wavData = new unsigned char[waveHeaderStruct.dataSize];
	if (!wavData)
	{
		return false;
	}
	
	// Read in the wave file data into the newly created buffer.
	count = fread(wavData, 1, waveHeaderStruct.dataSize, filePtr);
	if (count != waveHeaderStruct.dataSize)
	{
		return false;
	}


	// Close the file once done reading.
	error = fclose(filePtr);
	if(error != 0)
	{
		GAMELOG(" Can not close file ");
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*pSecondaryBuffer)->Lock(0, waveHeaderStruct.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);

	if(FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, wavData, waveHeaderStruct.dataSize);
	
	// Unlock the secondary buffer after the data has been written to it.
	(*pSecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);

	// Release the wave data since it was copied into the secondary buffer.
	if (wavData != NULL)
	{
		delete wavData;
		wavData = 0;
	}
	
	// set volume
	long tempVolume = (long) ((1.0f - volume*0.01f) * DSBVOLUME_MIN);
	(*pSecondaryBuffer) -> SetVolume(tempVolume);

	soundBufferMap[name] = secondaryBuffer;

	return true;
}

void Sound :: play(std:: string name, bool infiniteLoop)
{
	if(isMute)
	{
		return;
	}

	// Find it in map
	std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
	it = soundBufferMap.find(name);
	if(it == soundBufferMap.end())
		return;

	if(infiniteLoop)
	{
		it -> second -> Play(0, 0 , DSBPLAY_LOOPING);
	}
	else
	{
		it->second->Stop();
		it->second->SetCurrentPosition(0); // sets the position of the play cursor, which is the point at which the next byte of data is read from the buffer
		it->second->Play(0, 0, 0);
	}
}

void Sound :: stop(std::string name)
{
	if(name == "")
	{
		for ( std::map< std::string, IDirectSoundBuffer8*> ::iterator it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
		{
			it -> second ->Stop();
			it -> second ->SetCurrentPosition(0);
		}
	}
	else
	{
		std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
		it = soundBufferMap.find(name);
		if(it == soundBufferMap.end())
			return;
		else it -> second ->Stop();
	}
}

void Sound :: setVolume(float percentage, std:: string name)
{
	this->volume = percentage;
	long volumne = (long) ( (1.0f - volume*0.01f) * (-DSBVOLUME_MIN));

	if(name == "")
	{
		for ( std::map< std::string, IDirectSoundBuffer8*> ::iterator it = soundBufferMap.begin(); it != soundBufferMap.end(); it++)
		{
			it -> second ->SetVolume(volumne);
		}
	}
	else
	{
		std::map< std::string, IDirectSoundBuffer8*> ::iterator it;
		it = soundBufferMap.find(name);
		if(it == soundBufferMap.end())
			return;

		it -> second ->SetVolume(volumne);
	}
}

void Sound :: mute()
{
	isMute = true;
	Sound::getInstance() -> stop();
}

void Sound :: unMute()
{
	isMute = false;
}

void Sound :: cleanUp()
{
	delete this->instance;
}

void Sound::loadAllSound()
{
	loadSound("Resources/Sounds/bullet.wav", SOUND_BULLET);
	loadSound("Resources/Sounds/explosion.wav", SOUND_EXPLOSION);
	loadSound("Resources/Sounds/jump.wav", SOUND_JUMP);
	loadSound("Resources/Sounds/missile.wav", SOUND_ROCKET);
	loadSound("Resources/Sounds/bossMotherHit.wav", SOUND_BOSSMOTHER_HIT);
	loadSound("Resources/Sounds/collectionBonus.wav", SOUND_COLLECTION_BONUS);
	loadSound("Resources/Sounds/collectionItem.wav", SOUND_COLLECTION_ITEMS);
	loadSound("Resources/Sounds/collideEnemies.wav", SOUND_COLLIDE_ENEMIES);
	loadSound("Resources/Sounds/die.wav", SOUND_DEATH);
	loadSound("Resources/Sounds/nearDie.wav", SOUND_NEAR_DEATH);
	loadSound("Resources/Sounds/putBomb.wav", SOUND_PUT_BOMB);
}
