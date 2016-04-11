
#include "Window.h"


Window::Window( uint32_t size_x, uint32_t size_y, std::string name )
{
	_surface_size_x		= size_x;
	_surface_size_y		= size_y;
	_window_name		= name;

	_InitOSWindow();
}

Window::~Window()
{
	_DeInitOSWindow();
}


void Window::Close()
{
	_window_should_run		= false;
}

bool Window::Update()
{
	_UpdateOSWindow();
	return _window_should_run;
}
