#include "Engine.h"

#ifdef main
#undef main //remove SDL's main() hook if it exists
#endif

/* Use nvidia graphics card */
extern "C" {
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

int main(int argc, char** argv)
{
	Engine engine;

	try 
	{
		engine.CreateWindow();
	    engine.Work();
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what();
		std::cin.get();
		return -1;
	}

	return 0;
}