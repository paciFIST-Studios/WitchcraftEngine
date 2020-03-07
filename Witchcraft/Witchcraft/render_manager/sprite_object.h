#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include <windows.h>

#include "SDL_render_object.h"

// Encapsulates some animation, sprite anims are 
// to be packed into a single texture;
class cSpriteObject : public cSDLRenderObject
{
private:
protected:
	DWORD _time_at_last_frame;

public:
	unsigned int _total_animation_frames;
	unsigned int _anim_frames_per_row;
	unsigned int _anim_frames_per_column;
	unsigned int _current_frame;
	unsigned int _start_frame;

	float _speed;

	unsigned int _frame_width;
	unsigned int _frame_height;

	void update();
	void play();
	void stop();
	void set_frame_rect(unsigned int FrameNumber);
	cSpriteObject();
		


};

#endif
