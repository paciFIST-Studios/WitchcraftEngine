#ifndef TWO_D_SPRITE_ANIMATION_H
#define TWO_D_SPRITE_ANIMATION_H

#include <string>
#include <vector>

#include "../uninitialized_values.h"

class c2DSpriteAnimation 
{
private:
protected:
	
	// element 0 = ms until frame change
	// other elements indicate frame index
	std::vector<unsigned int> _animation_vector;

	unsigned int _current_frame;

	std::string _name;

public:

	std::string const & get_name() { return _name; }

	unsigned int get_frame_advance_timing(){return _animation_vector[0];}

	unsigned int get_next_frame()
	{
		if (_current_frame == _animation_vector.size() - 1)
		{
			// reset to index 1, b/c index 0 holds timing
			_current_frame = 1;
		}
		else
		{
			_current_frame++;
		}

		return _animation_vector[_current_frame];
	}
	
	c2DSpriteAnimation() 
		: _name(std::string(uninit::CSTRING))
		, _current_frame(uninit::UINT)
		, _animation_vector{ uninit::UINT, uninit::UINT}
	{}

	c2DSpriteAnimation
	(
		  std::string const & name
		, std::vector<unsigned int> animation_sequence
		, unsigned int timing_ms
	)
		: _name(std::string(name))

		// NOTE: we start at zero, so the first time we call get_next_frame,
		// it advances to the correct number.  This is also why we're squishing
		// timing_ms into the frame vector, so we can use it as a -1 position, 
		// for an unsigned int type variable
		, _current_frame(0)
	{
		_animation_vector.reserve(animation_sequence.size() + 1);
		_animation_vector.push_back(timing_ms);
		for (auto element : animation_sequence)
			_animation_vector.push_back(element);
	}

};

#endif // !TWO_D_SPRITE_ANIMATION_H
