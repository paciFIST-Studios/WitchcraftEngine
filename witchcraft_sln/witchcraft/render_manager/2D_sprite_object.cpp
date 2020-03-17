#include "2D_sprite_object.h"

void c2DSpriteObject::update()
{
	DWORD time_since_last_frame_ms = get_time() - _time_at_last_frame;

	if (time_since_last_frame_ms >= _speed)
	{
		_current_frame++;

		if (_current_frame >= _total_animation_frames)
			_current_frame = _start_frame;

		set_frame_rect(_current_frame);

		_time_at_last_frame = get_time();
	}
}

void c2DSpriteObject::play()
{
	// calculate frame dimension
	auto * surface = _render_resource->_sdl_surface;
	_frame_width = surface->w / _animation_frames_per_row;
	_frame_height = surface->h / _animation_frames_per_column;
	_current_frame = _start_frame;
	set_frame_rect(_current_frame);
	_time_at_last_frame = get_time();
	
}

void c2DSpriteObject::set_frame_rect(unsigned int FrameNumber)
{
	unsigned int row_number = static_cast<int>(std::floor(FrameNumber / _animation_frames_per_row));
	unsigned int column_number = FrameNumber;

	if (row_number > 0)
	{
		column_number = FrameNumber - (row_number * _animation_frames_per_row);
		_render_rect.x = column_number * _frame_width;
		_render_rect.y = row_number * _frame_height;
		_render_rect.w = _frame_width;
		_render_rect.h = _frame_height;
	}

}
