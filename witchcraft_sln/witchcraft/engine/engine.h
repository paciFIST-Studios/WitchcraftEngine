#ifndef ENGINE_H
#define ENGINE_H

#include <SDL_ttf.h>

#include "../engine/engine_object.h"

#include "../resource_manager/resource_manager.h"
#include "../render_manager/render_manager_2d.h"

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
	bool tm_early_exit; // tm = testing mode
};


class Engine : public qEngineObject
{
private:
protected:

	//std::unique_ptr<MessageManager> message;
	std::unique_ptr<ResourceManager> resource;
	std::unique_ptr<RenderManager2D> render;
	std::unique_ptr<SceneManager2D> scene;

	EEngineState current_engine_state = EEngineState::UNINIT;

	// testing modes
	bool tm_early_exit;


	int const JOYSTICK_DEAD_ZONE = 8000;
	SDL_Joystick * gameController = nullptr;



public:
	
	void startup();
	void run();
	void shutdown();

	EEngineState get_current_state() const { return current_engine_state; }

	Engine() 
	: current_engine_state(EEngineState::CONSTRUCTED) 
	{}

	Engine(EngineInitializer init) 
	: qEngineObject(init.id)
	, tm_early_exit(init.tm_early_exit)
	, current_engine_state(EEngineState::CONSTRUCTED)
	{}
};


namespace witchcraft
{
	namespace engine
	{
		static void move_object_by_vector(RenderObject2D * object, int x, int y)
		{
			auto wh = object->get_position(); // width, height
			auto _x = x + int(std::get<0>(wh));
			auto _y = y + int(std::get<1>(wh));
			object->set_position(static_cast<float>(_x), static_cast<float>(_y));
		}

		static void move_layer_by_vector(Layer2D * layer, int x, int y)
		{
			auto wh = layer->get_offset();  // width, height
			auto _x = x + int(std::get<0>(wh));
			auto _y = y + int(std::get<1>(wh));
			layer->set_offset(static_cast<float>(_x), static_cast<float>(_y));
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

		static int connnected_controller_count()
		{
			return SDL_NumJoysticks();
		}

		static SDL_Joystick * get_controller(int idx)
		{
			if (idx < 0) return nullptr;

			if (connnected_controller_count() > 0)
				return SDL_JoystickOpen(idx);

			return nullptr;
		}


	}

	namespace configuration
	{
		bool const default_window_start_fullscreen = false;
		int const default_window_x_width = 800;
		int const default_window_y_height = 800;
		int const default_window_x_offset = 0;
		int const default_window_y_offset = 0;


		int const screen_fps = 60;
		float const frame_length_ms = static_cast<float>(screen_fps * 0.001f); // same as division by 1000

	}
}

#endif // !ENGINE_H
