#include <App/App.hpp>

App::App()
{

}

App::~App()
{

}

int App::run()
{
	if(init())
	{
		mainLoop();
		cleanup();
		return 0;
	}

	return 1;
}

