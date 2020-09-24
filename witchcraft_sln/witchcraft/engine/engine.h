#ifndef ENGINE_H
#define ENGINE_H

#include <functional>

#include "../engine/engine_object.h"

#include "../project_loader/project_loader.h"
#include "../message_bus/message_bus.h"
#include "../resource_manager/resource_manager.h"
#include "../render_manager/render_manager.h"
#include "../console/console.h"


struct TestMode
{
	// steps through the function calls involved in running the
	// game engine, but early-outs as soon as the fn is called
	bool early_exit;
};

struct DebugOptions
{
	// prints the processing time for current frame, in ms
	bool emit_frame_length;

	// shows the number of connected controllers
	bool emit_controller_count;

	// shows the current state of controller-0
	bool emit_controller_state;
};


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
	TestMode test_mode;
	std::string project_file_path;
};

class Engine : public qEngineObject
{
private:
	std::string string_buffer;

protected:
	std::string project_file_path;
	ProjectSettings project_settings;
	
	std::unique_ptr<ProjectLoader> project_loader;

	std::unique_ptr<MessageBus> message;
	unsigned int engine_channel_id		= 0;
	unsigned int resource_channel_id	= 0;
	unsigned int render_channel_id		= 0;
	unsigned int scene_channel_id		= 0;
	unsigned int console_channel_id		= 0;

	std::unique_ptr<ResourceManager> resource;
	std::unique_ptr<RenderManager> render;
	std::unique_ptr<SceneManager2D> scene;
	std::unique_ptr<Console> console;


	EEngineState current_engine_state = EEngineState::UNINIT;
	TestMode test_mode;


	int const JOYSTICK_DEAD_ZONE = 8000;
	SDL_GameController * gameController = nullptr;

	bool continue_gameplay_loop(SDL_Event const & e);
	void process_window_event(SDL_Event const & e);

	// sends a command over the message bus.
	void send_console_command(char const * command, bool send_direct = true);
	void send_render_command(char const * command, bool send_direct = true);

	void send_message(unsigned int sendTo, unsigned int sendFrom, MessageType type, void* data, bool send_direct=true);

	void handle_message(Message m);

public:
	
	void startup();
	void run();
	void shutdown();

	EEngineState const get_current_state() const { return current_engine_state; }

	Engine() 
		: qEngineObject()
		, current_engine_state(EEngineState::CONSTRUCTED) 
		, test_mode({ false })
	{}

	Engine(EngineInitializer init) 
		: qEngineObject()
		, test_mode(init.test_mode)
		, project_file_path(init.project_file_path)
		, current_engine_state(EEngineState::CONSTRUCTED)
	{}
};


namespace witchcraft
{
	namespace engine
	{
		static void move_object_by_vector(RenderObject2D * object, float x, float y)
		{
			auto pos = object->get_position();
			auto _x = x + std::get<0>(pos);
			auto _y = y + std::get<1>(pos);
			// HACK: 20200802 - EB - clamp movement to visible screen area, 
			_x = utility::clamp_to_range(_x, 0.0f, 768.f);
			_y = utility::clamp_to_range(_y, 0.0f, 768.f);
			// HACK: 20200802 - EB - clamp movement to visible screen area, 
			object->set_position(_x, _y);
		}

		static void move_layer_by_vector(Layer2D * layer, float x, float y)
		{
			auto pos = layer->get_offset();
			auto _x = x + std::get<0>(pos);
			auto _y = y + std::get<1>(pos);
			// HACK: 20200802 - EB - clamp movement to visible screen area, 
			_x = utility::clamp_to_range(_x, -768.0f, 768.f);
			_y = utility::clamp_to_range(_y, -768.0f, 768.f);
			// HACK: 20200802 - EB - clamp movement to visible screen area, 
			layer->set_offset(_x, _y);
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

		static SDL_GameController * get_controller(int idx)
		{
			if (idx < 0) return nullptr;

			if (connnected_controller_count() > 0)
				return SDL_GameControllerOpen(idx);

			return nullptr;
		}

		static bool is_keyboard_event(SDL_Event const & e)
		{
			if (e.type == SDL_KEYDOWN ||
				e.type == SDL_KEYUP
			)
			{
				return true;
			}

			return false;
		}

		static bool is_gamepad_event(SDL_Event const & e)
		{			
			if (e.type == SDL_CONTROLLERBUTTONDOWN		||
				e.type == SDL_CONTROLLERBUTTONUP		||
				e.type == SDL_CONTROLLERAXISMOTION		||
				e.type == SDL_CONTROLLERDEVICEADDED		||
				e.type == SDL_CONTROLLERDEVICEREMOVED	||
				e.type == SDL_CONTROLLERDEVICEREMAPPED
				)
			{
				return true;
			}

			return false;
		}

		static void toggle_layer_visibility(Layer2D * layer)
		{
			bool is_visible = layer->get_is_visible();
			is_visible = !is_visible;
			layer->set_is_visible(is_visible);
		}
	}
	

	namespace configuration
	{
		bool const default_window_start_fullscreen = false;
		int const default_window_x_width = 800;
		int const default_window_y_height = 800;
		int const default_window_x_offset = 32;
		int const default_window_y_offset = 48;

		float const default_world_boundary_top = 0.0f;
		float const default_world_boundary_right = 768.0f;
		float const default_world_boundary_bottom = 768.0f;
		float const default_world_boundary_left = 0.0f;


		float const screen_fps = 60.0f;
		float const frame_length_ms = (screen_fps * 0.001f); // same as division by 1000
	}
}

#endif // !ENGINE_H
