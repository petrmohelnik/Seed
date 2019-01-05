#include "Engine.h"

#ifdef main
#undef main //remove SDL's main() hook if it exists
#endif

int main(int argc, char** argv)
{
	Engine engine;

	try 
	{
		engine.CreateWindow();
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what();
		std::cin.get();
		return -1;
	}

	engine.Work();

	return 0;
}