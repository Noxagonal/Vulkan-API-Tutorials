#pragma once

#include "Platform.h"

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

	Renderer						*	_renderer						= nullptr;

	VkSurfaceKHR						_surface						= VK_NULL_HANDLE;

	uint32_t							_surface_size_x					= 512;
	uint32_t							_surface_size_y					= 512;
	std::string							_window_name;

	VkSurfaceFormatKHR					_surface_format					= {};
	VkSurfaceCapabilitiesKHR			_surface_capabilities			= {};

	bool								_window_should_run				= true;

#if VK_USE_PLATFORM_WIN32_KHR
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
