#ifndef ENGINE_H
#define ENGINE_H

#include <SDL_ttf.h>

#include "../engine/engine_object.h"

#include "../resource_manager/resource_manager.h"
#include "../render_manager/2d_render_manager.h"

enum class EEngineState : unsigned char
{
	  UNINIT		= 0x01
	, CONSTRUCTED	= 0x02
	, STARTUP		= 0x04
	, RUNNING		= 0x08
	, SHUTDOWN		= 0x10
	// 0x20
	// 0x40
	// 0x80
};

struct EngineInitializer
{
	unsigned int id;
	bool tm_early_exit;
};


class Engine : public qEngineObject
{
private:
protected:

	//std::unique_ptr<cMessageManager> message;
	std::unique_ptr<qResourceManager> resource;
	std::unique_ptr<q2DRenderManager> render;
	//std::unique_ptr<cSceneManager> scene;

	EEngineState current_engine_state = EEngineState::UNINIT;

	// testing modes
	bool tm_early_exit;


public:
	
	void startup();
	void run();
	void shutdown();

	EEngineState get_current_state() const { return current_engine_state; }

	Engine() 
	: current_engine_state(EEngineState::CONSTRUCTED) 
	{}

	Engine(EngineInitializer init) 
	: current_engine_state(EEngineState::CONSTRUCTED)
	{
		id = init.id;
		tm_early_exit = init.tm_early_exit;
	}
};


namespace witchcraft
{
	namespace engine
	{
		static void move_object_by_vector(RenderObject2D * object, int x, int y)
		{
			auto wh = object->get_position();
			auto _x = x + int(std::get<0>(wh));
			auto _y = y + int(std::get<1>(wh));
			object->set_position(static_cast<float>(_x), static_cast<float>(_y));
		}

		static Uint32 get_delta_time()
		{
			float const smoothing = 0.99f;
			
			static Uint32 frame_time_current = 0;
			static Uint32 frame_time_last = 0;
			static Uint32 delta_time = 0;
		
			frame_time_current = SDL_GetTicks();
			delta_time = frame_time_current - frame_time_last;
			delta_time = static_cast<Uint32>(
				floor(
					static_cast<float>(delta_time) * smoothing
				)
			);
		
			return delta_time;
		}
		
		static std::string get_debug_fps_text()
		{
			auto delta_time = get_delta_time();
			std::stringstream out;
			out << "delta time: " << delta_time << " ms";
			out << "\nfps: " << 1.f / delta_time;
			return out.str();
		}
	}

	namespace configuration
	{
		int const screen_fps = 60;
		float const frame_length_ms = 1000/screen_fps;
	}
}

#endif // !ENGINE_H
