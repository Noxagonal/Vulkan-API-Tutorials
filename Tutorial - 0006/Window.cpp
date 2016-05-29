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
#include "Renderer.h"

#include <assert.h>

Window::Window( Renderer * renderer, uint32_t size_x, uint32_t size_y, std::string name )
{
	_renderer			= renderer;
	_surface_size_x		= size_x;
	_surface_size_y		= size_y;
	_window_name		= name;

	_InitOSWindow();
	_InitSurface();
}

Window::~Window()
{
	_DeInitSurface();
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

void Window::_InitSurface()
{
	_InitOSSurface();

	auto gpu = _renderer->GetVulkanPhysicalDevice();

	VkBool32 WSI_supported = false;
	vkGetPhysicalDeviceSurfaceSupportKHR( gpu, _renderer->GetVulkanGraphicsQueueFamilyIndex(), _surface, &WSI_supported );
	if( !WSI_supported ) {
		assert( 0 && "WSI not supported" );
		std::exit( -1 );
	}

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR( gpu, _surface, &_surface_capabilities );
	{
		uint32_t format_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR( gpu, _surface, &format_count, nullptr );
		if( format_count == 0 ) {
			assert( 0 && "Surface formats missing." );
			std::exit( -1 );
		}
		std::vector<VkSurfaceFormatKHR> formats( format_count );
		vkGetPhysicalDeviceSurfaceFormatsKHR( gpu, _surface, &format_count, formats.data() );
		if( formats[ 0 ].format == VK_FORMAT_UNDEFINED ) {
			_surface_format.format		= VK_FORMAT_B8G8R8A8_UNORM;
			_surface_format.colorSpace	= VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		} else {
			_surface_format				= formats[ 0 ];
		}
	}
}

void Window::_DeInitSurface()
{
	vkDestroySurfaceKHR( _renderer->GetVulkanInstance(), _surface, nullptr );
}