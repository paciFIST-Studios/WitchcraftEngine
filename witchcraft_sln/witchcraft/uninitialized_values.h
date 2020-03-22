#ifndef UNINITIALIZED_VALUES_H
#define UNINITIALIZED_VALUES_H

namespace uninit
{
	static bool  constexpr BOOL = false;

	static float constexpr FLOAT = 0.0;

	static unsigned char UCHAR = 0;

	static unsigned int UINT = 0;

	static char const * CSTRING = "un-init";
}

#endif // !UNINITIALIZED_VALUES_H
