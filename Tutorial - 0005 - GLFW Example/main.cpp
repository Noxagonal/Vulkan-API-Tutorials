
/*
This example is using GLFW for window creation and surfaces Using Vulkan API.
GLFW will handle a few things for us automatically, we still need to
Create our Vulkan instance and device ourselves but GLFW will help us
with the OS specific stuff, this code should run on all OSes without modification.

Before running though, you will need to direct your include and library
directories to your GLFW 3.2 locations in project settings.
You will need GLFW version 3.2 or newer.

Provided as-is without any quarantee that it'll work
but I will likely fix any errors reported.

Code licence: CC0
*/

// if you have a non-standard path to vulkan, you need to include the vulkan header before GLFW,
// if vulkan header is in a standard location you don't have to do this step.
#include <vulkan/vulkan.h>

// You tell GLFW to use vulkan by defining GLFW_INCLUDE_VULKAN, GLFW automatically looks for
// vulkan header in <vulkan/vulkan.h> if not included already.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

int main()
{
	// regular instance and device layers and extensions
	std::vector<const char*> instance_layers;
	std::vector<const char*> device_layers;
	std::vector<const char*> instance_extensions;
	std::vector<const char*> device_extensions;

	// if using debugging, push back debug layers and extensions
	instance_layers.push_back( "VK_LAYER_LUNARG_standard_validation" );
	instance_extensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
	device_layers.push_back( "VK_LAYER_LUNARG_standard_validation" );

	// push back extensions and layers you need
	// We'll need the swapchain for sure if we want to display anything
	device_extensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

	// don't forget to initialize glfw!
	glfwInit();

	// check for vulkan support
	if( GLFW_FALSE == glfwVulkanSupported() ) {
		// not supported
		glfwTerminate();
		return -1;
	}

	// Get required instance extensions to create the window.
	// These instance extensions change from OS to OS.
	// For example on Windows we'd get back "VK_KHR_surface" and "VK_KHR_win32_surface"
	// and on Linux XCB window library we'd get back "VK_KHR_surface" and "VK_KHR_xcb_surface"
	uint32_t instance_extension_count				= 0;
	const char ** instance_extensions_buffer		= glfwGetRequiredInstanceExtensions( &instance_extension_count );
	for( uint32_t i=0; i < instance_extension_count; ++i ) {
		// Push back required instance extensions as well
		instance_extensions.push_back( instance_extensions_buffer[ i ] );
	}

	// create instance
	VkApplicationInfo application_info {};
	application_info.sType							= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.apiVersion						= VK_MAKE_VERSION( 1, 0, 2 );
	application_info.applicationVersion				= VK_MAKE_VERSION( 0, 0, 1 );
	application_info.engineVersion					= VK_MAKE_VERSION( 0, 0, 1 );
	application_info.pApplicationName				= "GLFW with Vulkan";
	application_info.pEngineName					= "GLFW with Vulkan";

	VkInstanceCreateInfo instance_create_info {};
	instance_create_info.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo			= &application_info;
	instance_create_info.enabledLayerCount			= instance_layers.size();
	instance_create_info.ppEnabledLayerNames		= instance_layers.data();
	instance_create_info.enabledExtensionCount		= instance_extensions.size();
	instance_create_info.ppEnabledExtensionNames	= instance_extensions.data();

	VkInstance instance								= VK_NULL_HANDLE;
	vkCreateInstance( &instance_create_info, nullptr, &instance );

	// Get GPUs
	uint32_t GPU_count;
	vkEnumeratePhysicalDevices( instance, &GPU_count, nullptr );
	std::vector<VkPhysicalDevice> GPUs( GPU_count );
	vkEnumeratePhysicalDevices( instance, &GPU_count, GPUs.data( ) );

	// select graphics queue family
	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties( GPUs[ 0 ], &queue_family_count, nullptr );
	std::vector<VkQueueFamilyProperties> family_properties( queue_family_count );
	vkGetPhysicalDeviceQueueFamilyProperties( GPUs[ 0 ], &queue_family_count, family_properties.data() );

	uint32_t graphics_queue_family					= UINT32_MAX;
	for( uint32_t i=0; i < queue_family_count; ++i ) {
		if( family_properties[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
			graphics_queue_family = i;
		}
	}
	if( graphics_queue_family == UINT32_MAX ) {
		// queue family not found
		glfwTerminate();
		return -1;
	}
	// graphics_queue_family now contains queue family ID which supports graphics

	// Create Vulkan device
	const float priorities[] { 1.0f };
	VkDeviceQueueCreateInfo queue_create_info {};
	queue_create_info.sType							= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueCount					= 1;
	queue_create_info.queueFamilyIndex				= graphics_queue_family;
	queue_create_info.pQueuePriorities				= priorities;

	VkDeviceCreateInfo device_create_info {};
	device_create_info.sType						= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount			= 1;
	device_create_info.pQueueCreateInfos			= &queue_create_info;
	device_create_info.enabledLayerCount			= device_layers.size();
	device_create_info.ppEnabledLayerNames			= device_layers.data();
	device_create_info.enabledLayerCount			= device_extensions.size();
	device_create_info.ppEnabledExtensionNames		= device_extensions.data();

	VkDevice device									= VK_NULL_HANDLE;
	vkCreateDevice( GPUs[ 0 ], &device_create_info, nullptr, &device );

	// create window
	int width		= 800;
	int height		= 600;
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );		// This tells GLFW to not create an OpenGL context with the window
	auto window = glfwCreateWindow( width, height, application_info.pApplicationName, nullptr, nullptr );

	// make sure we indeed get the surface size we want.
	glfwGetFramebufferSize( window, &width, &height );

	// Create window surface, looks a lot like a Vulkan function ( and not GLFW function )
	// This is a one function solution for all operating systems. No need to hassle with the OS specifics.
	// For windows this would be vkCreateWin32SurfaceKHR() or on linux XCB window library this would be vkCreateXcbSurfaceKHR()
	VkSurfaceKHR surface							= VK_NULL_HANDLE;
	VkResult ret = glfwCreateWindowSurface( instance, window, nullptr, &surface );
	if( VK_SUCCESS != ret ) {
		// couldn't create surface, exit
		glfwTerminate();
		return -1;
	}

	/*
	All regular Vulkan API stuff goes here, no more GLFW commands needed for the window.
	We still need to initialize the swapchain, it's images and all the rest
	just like we would have done with OS native windows.
	*/

	// Destroy window surface, Note that this is a native Vulkan API function
	// ( surface was created with GLFW function )
	vkDestroySurfaceKHR( instance, surface, nullptr );

	// destroy window using GLFW function
	glfwDestroyWindow( window );

	// destroy Vulkan device and instance normally
	vkDestroyDevice( device, nullptr );
	vkDestroyInstance( instance, nullptr );

	// don't forget to terminate glfw either
	glfwTerminate();

	return 0;
}
