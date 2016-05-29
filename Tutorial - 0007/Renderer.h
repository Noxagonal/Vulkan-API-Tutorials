/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Niko Kauppi (Code maintenance)
Contributors:
----------------------------------------------------- */

#pragma once

#include "Platform.h"

#include <vector>
#include <string>

class Window;

class Renderer
{
public:
	Renderer();
	~Renderer();

	Window								*	OpenWindow( uint32_t size_x, uint32_t size_y, std::string name );

	bool									Run();

	const VkInstance						GetVulkanInstance()	const;
	const VkPhysicalDevice					GetVulkanPhysicalDevice() const;
	const VkDevice							GetVulkanDevice() const;
	const VkQueue							GetVulkanQueue() const;
	const uint32_t							GetVulkanGraphicsQueueFamilyIndex() const;
	const VkPhysicalDeviceProperties	&	GetVulkanPhysicalDeviceProperties() const;

private:
	void _SetupLayersAndExtensions();

	void _InitInstance();
	void _DeInitInstance();

	void _InitDevice();
	void _DeInitDevice();

	void _SetupDebug();
	void _InitDebug();
	void _DeInitDebug();

	VkInstance								_instance						= VK_NULL_HANDLE;
	VkPhysicalDevice						_gpu							= VK_NULL_HANDLE;
	VkDevice								_device							= VK_NULL_HANDLE;
	VkQueue									_queue							= VK_NULL_HANDLE;
	VkPhysicalDeviceProperties				_gpu_properties					= {};

	uint32_t								_graphics_family_index			= 0;

	Window								*	_window							= nullptr;

	std::vector<const char*>				_instance_layers;
	std::vector<const char*>				_instance_extensions;
//	std::vector<const char*>				_device_layers;					// depricated
	std::vector<const char*>				_device_extensions;

	VkDebugReportCallbackEXT				_debug_report					= VK_NULL_HANDLE;
	VkDebugReportCallbackCreateInfoEXT		_debug_callback_create_info		= {};
};

