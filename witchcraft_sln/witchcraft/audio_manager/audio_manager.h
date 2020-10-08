#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "SDL.h"
#include "SDL_mixer.h"

#include "../message_bus/message_bus.h"
#include "../resource_manager/engine_resource_base.h"

#include "../string_constants.h"
#include "../utility/utility.h"

class AudioManager : public EngineObjectBase
{
private:
protected:
	std::map<char const *, Mix_Music *> tracks;

	int const CD_SAMPLE_RATE   = 44100;
	int const GAME_SAMPLE_RATE = 22050;
	int const OUTPUT_CHANNELS = 2;
	int const CHUNK_SIZE = 1024;



	MessageBus * message_bus = nullptr;

	void handle_message(Message m)
	{
		message_bus->log_message(m);

		if (m.type == MessageType::REQUEST__AUDIO_STOP_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
		else if (m.type == MessageType::REQUEST__AUDIO_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();

			Mix_PlayMusic(tracks[path], 0);
		}
		else if (m.type == MessageType::REQUEST__AUDIO_PLAY_LOOP)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
		else if (m.type == MessageType::REQUEST__AUDIO_LOAD)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
			auto t = Mix_LoadMUS(path);
			if (!t)
			{
				PLOGE << "Error! Could not load music file!"
					<< "\n\ttrack {"
					<< "\npath: " << path
					<< "\nerror: " << Mix_GetError()
					<< "\n};"
					;
			}
			tracks[path] = t;
		}
		else if (m.type == MessageType::REQUEST__AUDIO_LOAD_AND_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
	}

	unsigned int audio_channel_id = 0;
	unsigned int engine_channel_id = 0;
	unsigned int resource_channel_id = 0;

	void init_mix_audio() 
	{
		auto err = Mix_OpenAudio(
			  GAME_SAMPLE_RATE
			, AUDIO_S16SYS
			, OUTPUT_CHANNELS
			, CHUNK_SIZE
		);
		if (err != 0)
		{
			PLOGE << "Error! Could not initialize Mixer API!\n"
				  << Mix_GetError();
		}

		err = Mix_Init(MIX_INIT_MP3);
		if (err != MIX_INIT_MP3)
		{
			PLOGE << "Error! Could not initialie MP3 support!\n" 
				  << Mix_GetError();
		}
		PLOGI << "SDL_mixer ok";
		PLOGI << "Audio driver name: " << SDL_GetCurrentAudioDriver();
	}

public:

	AudioManager() {}
	~AudioManager() {}

	AudioManager(MessageBus * mb)
		: message_bus(mb)
	{
		std::function<void(Message)> cb = std::bind(&AudioManager::handle_message, this, std::placeholders::_1);
		message_bus->subscribe("audio", cb);

		audio_channel_id = message_bus->channel_lookup("audio");
		engine_channel_id = message_bus->channel_lookup("engine");
		resource_channel_id = message_bus->channel_lookup("resource");

		init_mix_audio();
	}

	void shutdown()
	{
		Mix_CloseAudio();
	}
}; // !AudioManager

#endif // !AUDIO_MANAGER_H