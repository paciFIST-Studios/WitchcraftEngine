///#include <windows.h>
///
///
/////#define LINKER_ERROR 0
///
///#ifdef LINKER_ERROR
///// just a function decleration allows the compiler to finish
///// the linker will throw an error if an undefined fn is called
///void oranges(void);
///#endif
///
///void apples(void)
///{
///	OutputDebugString("\n\nTest String: C-style\n\n");
///}
///
///
///int CALLBACK WinMain(
///	// refers to our running exe, in memory
///	HINSTANCE hInstance,
///
///	// legacy support
///	HINSTANCE hPrevInstance,
///	
///	// what was sent to use when this exe was run
///	// , allows access for command line arguments
///	LPSTR     lpCmdLine,
///	
///	// legacy; window state you should use (min/max/etc)
///	int       nShowCmd
///)
///{
///	apples();
///
///#ifdef LINKER_ERROR
///	oranges();
///#endif
///}

