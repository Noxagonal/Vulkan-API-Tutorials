/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Teagan Chouinard (GLFW)
Contributors:
Niko Kauppi (Code maintenance)
----------------------------------------------------- */

#include "Platform.h"

#include <assert.h>

#if USE_FRAMEWORK_GLFW

void InitPlatform()
{
	glfwInit();
	if( glfwVulkanSupported() == GLFW_FALSE ) {
		assert( 0 && " GLFW Failed to initialize with Vulkan." );
		return;
	}
}

void DeInitPlatform()
{
	glfwTerminate();
}

void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions ){
	uint32_t instance_extension_count = 0;
	const char ** instance_extensions_buffer = glfwGetRequiredInstanceExtensions( &instance_extension_count );
	for ( uint32_t i=0; i < instance_extension_count; i++ ){
		instance_extensions->push_back( instance_extensions_buffer[i] );
	}
}

#elif VK_USE_PLATFORM_WIN32_KHR

void InitPlatform()
{
}

void DeInitPlatform()
{
}

void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions )
{
	instance_extensions->push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
}

#elif VK_USE_PLATFORM_XCB_KHR

void InitPlatform()
{
}

void DeInitPlatform()
{
}

void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions )
{
	instance_extensions->push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
}

#endif
