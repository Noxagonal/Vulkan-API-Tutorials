/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Niko Kauppi (Code maintenance)
Contributors:
----------------------------------------------------- */

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
