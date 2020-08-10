#include "sprite_object_2d.h"

UINT_UINT_TUPLE c2DSpriteObject::get_sprite_sheet_dimensions()
{
	return std::make_tuple(_sprite_sheet_width, _sprite_sheet_height);
}

UINT_UINT_TUPLE c2DSpriteObject::get_frame_dimensions()
{
	return std::make_tuple(_frame_width, _frame_height);
}

UINT_UINT_TUPLE c2DSpriteObject::get_frame_position()
{
	return _current_frame_position;
}

Animation2D * c2DSpriteObject::get_current_animation()
{
	return _current_animation;
}

float c2DSpriteObject::get_current_frame_timing()
{
	return _time_until_frame_advance;
}

bool c2DSpriteObject::is_animation_playing()
{
	return _animation_is_playing;
}

void c2DSpriteObject::set_animation(std::string const & name)
{
	if (_animation_map.count(name) > 0)
	{
		_current_animation = &_animation_map[name];
	}
	else
	{
		_current_animation = nullptr;
	}
}

void c2DSpriteObject::update()
{
	if (_animation_is_playing == false)
		return;

	DWORD time_since_last_frame_ms = timeGetTime() - _time_at_last_frame_advance;
	
	if (time_since_last_frame_ms >= _time_until_frame_advance)
	{
		//_current_frame = _current_animation->get_next_index();
		//set_frame_rect(_current_frame);	
		_time_at_last_frame_advance = timeGetTime();
	}
}

void c2DSpriteObject::play()
{
	_animation_is_playing = true;
}

void c2DSpriteObject::stop()
{
	_animation_is_playing = false;
}

void c2DSpriteObject::set_frame_rect(unsigned int FrameNumber)
{
}

c2DSpriteObject::c2DSpriteObject()
	: _sprite_sheet_width(uninit::UINT)
	, _sprite_sheet_height(uninit::UINT)
	, _frame_width(uninit::UINT)
	, _frame_height(uninit::UINT)
	, _current_frame_position{uninit::UINT, uninit::UINT}
	, _current_animation(nullptr)
	, _time_until_frame_advance(uninit::FLOAT)
	, _time_at_last_frame_advance(uninit::ULONG)
	, _animation_is_playing(uninit::BOOL)
{}

c2DSpriteObject::c2DSpriteObject(c2DSpriteObjectInitializer args)
	: _sprite_sheet_width(args.sprite_sheet_width)
	, _sprite_sheet_height(args.sprite_sheet_height)
	, _frame_width(args.frame_width)
	, _frame_height(args.frame_height)
	, _current_frame_position{args.frame_position_x, args.frame_position_y }
	, _current_animation(nullptr)
	, _time_until_frame_advance(uninit::FLOAT)
	, _time_at_last_frame_advance(uninit::ULONG)
	, _animation_is_playing(uninit::BOOL)
{
	stop();

	// manually add the animations to the map.
	for (auto anim : args.animations)
		_animation_map.emplace(anim.name, anim);
}


