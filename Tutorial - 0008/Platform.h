#pragma once

// Contact me if you're interested about adding platform
// support in these tutorials, contact me on my youtube channel:
// https://www.youtube.com/user/Nigo40
// Biggest missing right now is support for Android.
// others like Xlib and so are also welcome additions.

#include <stdint.h>
#include <vector>

extern void AddRequiredPlatformInstanceExtensions( std::vector<const char *> *instance_extensions );

// GLFW
#if defined( BUILD_GLFW )
// Define as a build option 
#define VK_USE_PLATFORM_GLFW_KHR 1
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#ifdef _WIN32 // for message box
#include <windows.h>
#endif 

// WINDOWS
#if defined( _WIN32 )
// this is always defined on windows platform

#define VK_USE_PLATFORM_WIN32_KHR 1
#include <Windows.h>

// LINUX ( Via XCB library )
#elif defined( __linux )
// there are other ways to create windows on linux,
// requirements might change based on the available libraries and distro used
// for example: xlib, wayland and mir
// xcb seems like a popular and well supported option on X11, until wayland and mir take over

#define VK_USE_PLATFORM_XCB_KHR 1
#include <xcb/xcb.h>

#else
// platform not yet supported
#error Platform not yet supported
#endif

#include <vulkan/vulkan.h>
