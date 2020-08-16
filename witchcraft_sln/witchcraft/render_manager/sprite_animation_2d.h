#ifndef ANIMATION_TWO_D_H
#define ANIMATION_TWO_D_H

#include <string>
#include <vector>

#include "../uninitialized_values.h"

class Animation2D 
{
private:
protected:
	
	std::vector<unsigned int> const _frame_indicies;
	unsigned int _current_idx;

public:

	std::string  const name;
	unsigned int const ms_per_frame;


	unsigned int get_next_index()
	{
		if (_current_idx == _frame_indicies.size() - 1)
		{
			_current_idx = 0;
		}
		else
		{
			_current_idx++;
		}

		return _frame_indicies[_current_idx];
	}
	
	Animation2D() 
		: name(std::string(uninit::CSTRING))
		, ms_per_frame(uninit::UINT)
		, _frame_indicies{uninit::UINT}
		, _current_idx(uninit::UINT)
	{}

	Animation2D
	// args ---
	(
		  std::string  const & name
		, unsigned int const & ms_per_frame
		, std::vector<unsigned int> const & frame_index_sequence
	)
	// initializer ---
		: name(std::string(name))
		, ms_per_frame(ms_per_frame)
		, _frame_indicies(frame_index_sequence)
	// fn body ---
	{
		// set current index as the last once, b/c when we call
		// next_index it will advance
		_current_idx = _frame_indicies.size() - 1;
	}

};

#endif // !ANIMATION_TWO_D_H
