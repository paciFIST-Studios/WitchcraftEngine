#ifndef TWO_D_SPRITE_ANIMATION_H
#define TWO_D_SPRITE_ANIMATION_H

#include <string>
#include <vector>

#include "../uninitialized_values.h"

class Animation2D 
{
private:
protected:
	
	// element 0 = ms until frame change
	// other elements indicate frame index
	std::vector<unsigned int> _animation_vector;

	unsigned int _current_idx;

	std::string _name;

public:

	std::string const & get_name() { return _name; }

	unsigned int get_frame_advance_timing(){return _animation_vector[0];}

	unsigned int get_next_index()
	{
		if (_current_idx == _animation_vector.size() - 1)
		{
			// reset to index 1, b/c index 0 holds timing
			_current_idx = 1;
		}
		else
		{
			_current_idx++;
		}

		return _animation_vector[_current_idx];
	}
	
	Animation2D() 
		: _name(std::string(uninit::CSTRING))
		, _current_idx(uninit::UINT)
		, _animation_vector{uninit::UINT, uninit::UINT}
	{}

	Animation2D
	// args ---
	(
		  std::string const & name
		, std::vector<unsigned int> animation_sequence
		, unsigned int timing_ms
	)
	// initializer ---
		: _name(std::string(name))

		// NOTE: we start at zero, so the first time we call get_next_index,
		// it advances to the correct number.  This is also why we're squishing
		// timing_ms into the frame vector, so we can use it as a -1 position, 
		// for an unsigned int type variable
		, _current_idx(0)

	// fn body ---
	{
		_animation_vector.reserve(animation_sequence.size() + 1);
		_animation_vector.push_back(timing_ms);
		for (auto element : animation_sequence)
			_animation_vector.push_back(element);
	}

};

#endif // !TWO_D_SPRITE_ANIMATION_H
