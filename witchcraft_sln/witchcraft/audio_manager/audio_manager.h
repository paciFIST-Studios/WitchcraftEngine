#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

//#include <array>
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <vector>

// low level audio lib
#include "../miniaudio.h"

#include "../message_bus/message_bus.h"
#include "../resource_manager/engine_resource_base.h"

#include "../string_constants.h"
#include "../utility/utility.h"

///// this namespace is reserved for the audio manager interactions with 
///// the computer's audio device.  DO NOT use these fns, unless you are
///// explicitly working on AudioManager
//namespace reserved_audio
//{
//	/// stores a decoder which is initialized to the associated path
//	struct AudioTrack
//	{
//		ma_decoder decoder;
//		std::string path;
//		bool is_playing_now;
//		bool loop_when_finished;
//		bool allow_prune;
//	};
//
//	struct Mixer
//	{
//		/// number of channels in the output device
//		unsigned channels = 2;
//		/// maximum number of tracks that can run at once time
//		unsigned playing_max = 5;
//		/// maximum number of tracks that can be held in standby at one time
//		unsigned standby_max = 5;
//
//		std::vector<std::unique_ptr<AudioTrack>> tracks;
//	};
//
//	static Mixer mixer;
//
//	static void prune_tracks()
//	{
//		// uses <algorithm> 
//		// erase track fitting critera track->allow_prune == true
//		mixer.tracks.erase(
//			std::remove_if(mixer.tracks.begin(), mixer.tracks.end(),
//				[](auto const & track) { 
//					return (track->allow_prune && !track->is_playing_now); 
//				}
//			) /* remove_if */
//		); /* erase fn */
//	} /* prune */
//
//	static void set_allow_prune(char const * path)
//	{
//		for (auto&& track : mixer.tracks) {
//			if (track->path.compare(path) == 0) {
//				track->allow_prune = true;
//				return;
//			}
//		}
//	}
//
//	static void set_is_looping(char const * path, bool is_looping)
//	{
//		for (auto&& track : mixer.tracks) {
//			if (track->path.compare(path) == 0) {
//				track->loop_when_finished = is_looping;
//				return;
//			}
//		}
//	}
//
//	static void set_is_playing(char const * path, bool is_playing)
//	{
//		for (auto&& track : mixer.tracks) {
//			if (track->path.compare(path) == 0) {
//				track->is_playing_now = is_playing;
//				return;
//			}
//		}
//	}
//	
//	static int load_track(
//		char const * path
//		, bool begin_play = false
//		, bool loop_track = false
//		, bool allow_drop = true
//	)
//	{
//		if (path == ""){
//			return -1;}
//
//		//prune_tracks();
//
//		auto p = std::string(path);
//		auto ext = p.substr(p.find_last_of(".") + 1);
//
//		ma_decoder decoder;
//		ma_result result;
//
//		result = ma_decoder_init_file(path, NULL, &decoder);
//
//		//if (ext == "mp3") {
//		//	result = ma_decoder_init_file_mp3(path, NULL, &decoder);
//		//}
//		//else if (ext == "wav") {
//		//	result = ma_decoder_init_file_wav(path, NULL, &decoder);
//		//}
//
//		if (result != MA_SUCCESS) {
//			// could not load file
//			return -2;
//		}
//
//		AudioTrack track{
//			  decoder
//			, std::string(path)
//			, loop_track
//			, allow_drop
//		};
//
//		mixer.tracks.emplace_back(std::make_unique<AudioTrack>(track));
//
//		return 0;
//	}
//	
//	//static ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder * pDecoder, float * pOutputF32, ma_uint32 frameCount)
//	//{
//	//	int CHANNEL_COUNT = 2;
//	//
//	//	float temp[4096];
//	//	ma_uint32 tempCapInFrames = (sizeof(temp) / sizeof(temp[0])) / CHANNEL_COUNT;
//	//	ma_uint32 totalFramesRead = 0;
//	//
//	//	while (totalFramesRead < frameCount)
//	//	{
//	//		ma_uint32 iSample;
//	//		ma_uint32 framesReadThisIteration;
//	//		ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
//	//		ma_uint32 framesToReadThisIteration = tempCapInFrames;
//	//
//	//		if (framesToReadThisIteration > totalFramesRemaining){
//	//			framesToReadThisIteration = totalFramesRemaining;}
//	//
//	//		framesReadThisIteration = (ma_uint32)ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration);
//	//
//	//		if (framesReadThisIteration == 0){
//	//			break; }
//	//
//	//		// mix frames
//	//		for (iSample = 0; iSample < framesReadThisIteration * CHANNEL_COUNT; ++iSample){
//	//			pOutputF32[totalFramesRead*CHANNEL_COUNT + iSample] = temp[iSample];}
//	//
//	//		totalFramesRead += framesReadThisIteration;
//	//
//	//		if (framesReadThisIteration < framesToReadThisIteration){
//	//			break; /* end of file */ }
//	//	}
//	//
//	//	return totalFramesRead;
//	//}
//
//	// I think we might have to leave this outside of the class, b/c we're working with c-code here
//	static void miniaudio_data_callback(ma_device * pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
//	{
//		float * pOutputF32 = (float*)pOutput;
//		
//		for (auto&& track : mixer.tracks)
//		{
//			if (track->is_playing_now) {
//				//ma_uint32 framesRead = read_and_mix_pcm_frames_f32(&track->decoder, pOutputF32, frameCount);
//				ma_uint32 framesRead = ma_decoder_read_pcm_frames(&track->decoder, pOutputF32, frameCount);
//				if (framesRead < frameCount) {
//					if (track->loop_when_finished) {
//						// seek to beinning of track to restart play
//						ma_decoder_seek_to_pcm_frame(&track->decoder, 0);
//						track->is_playing_now = true;
//					}
//					else { 
//						track->is_playing_now = false; }
//				}
//			}
//		}
//
//		(void)pInput;
//	}
//   
//
//} // !reserved_audio namespace


class AudioManager : public EngineObjectBase
{
private:
protected:

	MessageBus * message_bus = nullptr;

	void handle_message(Message m)
	{
		message_bus->log_message(m);

		if (m.type == MessageType::REQUEST__AUDIO_STOP_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
			//reserved_audio::set_is_playing(path, false);
		}
		else if (m.type == MessageType::REQUEST__AUDIO_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
			//reserved_audio::set_is_playing(path, true);
		}
		else if (m.type == MessageType::REQUEST__AUDIO_PLAY_LOOP)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
			//reserved_audio::set_is_looping(path, true);
			//reserved_audio::set_is_playing(path, true);
		}
		else if (m.type == MessageType::REQUEST__AUDIO_LOAD)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
			//auto err = reserved_audio::load_track(path);
			//if (err == -2)
			//{
			//	PLOGE << "ERROR: Could not load file! Supported formats: <.mp3>, <.wav>"
			//		  << "\n\tfile: \"" << path << "\"";
			//}
			//else if (err == -1)
			//{
			//	PLOGE << "ERROR: Too many files loaded! (which use allow_drop_from_standby=false)"
			//		<< "Unload some files!";
			//}
		}
		else if (m.type == MessageType::REQUEST__AUDIO_LOAD_AND_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
			//auto err = reserved_audio::load_track(path, true);
			//if (err == -2)
			//{
			//	PLOGE << "ERROR: Could not load file! Supported formats: <.mp3>, <.wav>"
			//		<< "\n\tfile: \"" << path << "\"";
			//}
			//else if (err == -1)
			//{
			//	PLOGE << "ERROR: Too many files loaded! (which use allow_drop_from_standby=false)"
			//		<< "Unload some files!";
			//}
		}
	}

	unsigned int audio_channel_id = 0;
	unsigned int engine_channel_id = 0;
	unsigned int resource_channel_id = 0;

	// - miniaudio --------------------------------------

	unsigned int const TRACK_MAX = 8;
	unsigned int const SAMPLE_CHANNELS = 2;
	unsigned int const SAMPLE_RATE = 44100;
	ma_format const SAMPLE_FORMAT = ma_format_f32;
	ma_device_type const OUTPUT_DEVICE_TYPE = ma_device_type::ma_device_type_playback;

	ma_event start_event;
	ma_event stop_event;
		
	ma_device_config device_config;
	ma_device		 audio_device;

	// - miniaudio --------------------------------------

	void init_device()
	{
		PLOGI << witchcraft::log_strings::audio_device_init;

		device_config = ma_device_config_init(OUTPUT_DEVICE_TYPE);
		device_config.playback.format	= SAMPLE_FORMAT;
		device_config.playback.channels = SAMPLE_CHANNELS;
		device_config.sampleRate		= SAMPLE_RATE;
		//device_config.dataCallback		= &reserved_audio::miniaudio_data_callback;

		device_config.pUserData = nullptr;

		if (ma_device_init(NULL, &device_config, &audio_device) != MA_SUCCESS)
		{
			auto cfg = device_config;
			PLOGF << "FAILURE! Could not initialize audio device!"
				<< "\n\tattempted init{"
				<< "\n\ttype: "			<< "ma_device_type_playback"
				<< "\n\tformat: "		<< "ma_format_f32"
				<< "\n\tchannels: "		<< device_config.playback.channels
				<< "\n\tsample rate: "	<< device_config.sampleRate
				<< "\n};"
				;

			ma_device_uninit(&audio_device);
			return;
		}

		start_device();

		PLOGI << witchcraft::log_strings::audio_device_ok;
	}


	void start_device()
	{
		auto result = ma_device_start(&audio_device);
		if (result != MA_SUCCESS)
		{
			PLOGF << "FAILURE! could not start audio device!";
			ma_device_uninit(&audio_device);
		}
		PLOGI << "audio device start";
	}

	void stop_device()
	{
		auto result = ma_device_stop(&audio_device);
		if (result != MA_SUCCESS)
		{
			PLOGF << "FAILURE! could not stop audio device!  The machince has become conscious!";
			ma_device_uninit(&audio_device);
		}

		PLOGI << "audio device stop";
	}

public:

	AudioManager() {}
	~AudioManager() 
	{
		stop_device();

		//// uninit running
		//for (auto&& track : reserved_audio::mixer.tracks){
		//	ma_decoder_uninit(&track->decoder);
		//}
		//// uninit standby
		//for (auto&& track : reserved_audio::mixer.tracks){
		//	ma_decoder_uninit(&track->decoder);
		//}

		// todo: cleanup success message
	}

	AudioManager(MessageBus * mb)
		: message_bus(mb)
	{
		std::function<void(Message)> cb = std::bind(&AudioManager::handle_message, this, std::placeholders::_1);
		message_bus->subscribe("audio", cb);

		audio_channel_id = message_bus->channel_lookup("audio");
		engine_channel_id = message_bus->channel_lookup("engine");
		resource_channel_id = message_bus->channel_lookup("resource");

		init_device();
	}
}; // !AudioManager

#endif // !AUDIO_MANAGER_H