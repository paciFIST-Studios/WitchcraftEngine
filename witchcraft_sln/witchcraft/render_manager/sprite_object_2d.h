#ifndef SPRITE_OBJECT_TWO_D_H
#define SPRITE_OBJECT_TWO_D_H

#include <map>

#include "../utility/utility.h"
#include "render_object.h"

#include "sprite_animation_2d.h"

#define UINT_UINT_TUPLE std::tuple<unsigned int, unsigned int>
#define ANIMATION_MAP std::map<std::string, Animation2D>


struct c2DSpriteObjectInitializer
{
	unsigned int sprite_sheet_width;
	unsigned int sprite_sheet_height;
	unsigned int frame_width;
	unsigned int frame_height;
	unsigned int frame_position_x;
	unsigned int frame_position_y;
	std::vector<Animation2D> animations;
};

class c2DSpriteObject : public RenderObject
{
private:
protected:
	DWORD _time_at_last_frame_advance;

	// relating to the sprite sheet, a 4x4 square of sprites
	unsigned int const _sprite_sheet_width;
	unsigned int const _sprite_sheet_height;
	
	// the w/h of a single frame/sprite in a sprite sheet
	unsigned int const _frame_width;
	unsigned int const _frame_height;

	UINT_UINT_TUPLE _current_frame_position;

	ANIMATION_MAP _animation_map;

	Animation2D * _current_animation;

	float _time_until_frame_advance;

	bool _animation_is_playing;

public:
	UINT_UINT_TUPLE get_sprite_sheet_dimensions();
	UINT_UINT_TUPLE get_frame_dimensions();
	UINT_UINT_TUPLE get_frame_position();
	Animation2D * get_current_animation();
	float get_current_frame_timing();
	bool is_animation_playing();

	void set_animation(std::string const & name);

	void update();
	void play();
	void stop();
	void set_frame_rect(unsigned int FrameNumber);

	c2DSpriteObject();
	c2DSpriteObject(c2DSpriteObjectInitializer args);

};

#endif // !SPRITE_OBJECT_TWO_D_H
