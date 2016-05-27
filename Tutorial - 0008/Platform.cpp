#include "Platform.h"

#if USE_FRAMEWORK_GLFW

void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions ){
	uint32_t instance_extension_count = 0;
	const char ** instance_extensions_buffer = glfwGetRequiredInstanceExtensions( &instance_extension_count );
	for ( uint32_t i=0; i < instance_extension_count; i++ ){
		instance_extensions->push_back( instance_extensions_buffer[i] );
	}
}

#elif VK_USE_PLATFORM_WIN32_KHR

void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions ){
	    instance_extensions->push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
}

#elif VK_USE_PLATFORM_XCB_KHR

void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions ){
	        instance_extensions->push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
}

#endif

