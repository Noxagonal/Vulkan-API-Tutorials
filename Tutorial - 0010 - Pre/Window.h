/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Niko Kauppi (Code maintenance)
Contributors:
Teagan Chouinard (GLFW)
----------------------------------------------------- */

#pragma once

#include "Platform.h"

#include <vector>
#include <string>

class Renderer;

class Window
{
public:
	Window( Renderer * renderer, uint32_t size_x, uint32_t size_y, std::string name );
	~Window();

	void Close();
	bool Update();

private:
	void								_InitOSWindow();
	void								_DeInitOSWindow();
	void								_UpdateOSWindow();
	void								_InitOSSurface();

	void								_InitSurface();
	void								_DeInitSurface();

	void								_InitSwapchain();
	void								_DeInitSwapchain();

	void								_InitSwapchainImages();
	void								_DeInitSwapchainImages();

	void								_InitDepthStencilImage();
	void								_DeInitDepthStencilImage();

	void								_InitRenderPass();				// 2: create functions
	void								_DeInitRenderPass();

	Renderer						*	_renderer						= nullptr;

	VkSurfaceKHR						_surface						= VK_NULL_HANDLE;
	VkSwapchainKHR						_swapchain						= VK_NULL_HANDLE;
	VkRenderPass						_render_pass					= VK_NULL_HANDLE;		// 6: Add _render_pass handle

	uint32_t							_surface_size_x					= 512;
	uint32_t							_surface_size_y					= 512;
	std::string							_window_name;
	uint32_t							_swapchain_image_count			= 2;

	std::vector<VkImage>				_swapchain_images;
	std::vector<VkImageView>			_swapchain_image_views;

	VkImage								_depth_stencil_image			= VK_NULL_HANDLE;
	VkDeviceMemory						_depth_stencil_image_memory		= VK_NULL_HANDLE;
	VkImageView							_depth_stencil_image_view		= VK_NULL_HANDLE;

	VkSurfaceFormatKHR					_surface_format					= {};
	VkSurfaceCapabilitiesKHR			_surface_capabilities			= {};

	VkFormat							_depth_stencil_format			= VK_FORMAT_UNDEFINED;
	bool								_stencil_available				= false;

	bool								_window_should_run				= true;

#if USE_FRAMEWORK_GLFW
	GLFWwindow						*	_glfw_window					= nullptr;
#elif VK_USE_PLATFORM_WIN32_KHR
	HINSTANCE							_win32_instance					= NULL;
	HWND								_win32_window					= NULL;
	std::string							_win32_class_name;
	static uint64_t						_win32_class_id_counter;
#elif VK_USE_PLATFORM_XCB_KHR
	xcb_connection_t				*	_xcb_connection					= nullptr;
	xcb_screen_t					*	_xcb_screen						= nullptr;
	xcb_window_t						_xcb_window						= 0;
	xcb_intern_atom_reply_t			*	_xcb_atom_window_reply			= nullptr;
#endif
};
