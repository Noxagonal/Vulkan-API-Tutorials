#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include "Window.h"
#include "Shared.h"
#include "Renderer.h"

#include <assert.h>
#include <iostream>

#if USE_FRAMEWORK_GLFW

void Window::_InitOSWindow()
{	
	if ( glfwInit() == GLFW_FALSE ) {
		assert ( 0 && "GLFW failed to initialize." );
		return;
	}
	if ( glfwVulkanSupported() == GLFW_FALSE ) {
		glfwTerminate();
		assert( 0 && "GLFW reported Vulkan is not supported." );
		return;
	}

	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	_glfw_window = glfwCreateWindow( _surface_size_x, _surface_size_y, _window_name.c_str(), nullptr, nullptr );
	glfwGetFramebufferSize ( _glfw_window, (int*)&_surface_size_x, (int*)&_surface_size_y );

}

void Window::_DeInitOSWindow()
{
	glfwDestroyWindow( _glfw_window );	
}

void Window::_UpdateOSWindow()
{
	glfwPollEvents();

	if ( glfwWindowShouldClose( _glfw_window ) ){
		Close();
	}
}

void Window::_InitOSSurface()
{
	if ( VK_SUCCESS != glfwCreateWindowSurface( _renderer->GetVulkanInstance(), _glfw_window, nullptr, &_surface ) ){
		glfwTerminate();
		assert ( 0 && "GLFW could not create the window surface." );
		return;
	}
}

#endif // USE_FRAMEWORK_GLFW
