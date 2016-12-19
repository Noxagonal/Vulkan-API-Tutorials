/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.

Original file creator:  Niko Kauppi (Code maintenance)
Contributors:
----------------------------------------------------- */

#include <array>
#include <chrono>
#include <iostream>

#include "Renderer.h"
#include "Window.h"

constexpr double PI				= 3.14159265358979323846;
constexpr double CIRCLE_RAD		= PI * 2;
constexpr double CIRCLE_THIRD	= CIRCLE_RAD / 3.0;
constexpr double CIRCLE_THIRD_1	= 0;
constexpr double CIRCLE_THIRD_2	= CIRCLE_THIRD;
constexpr double CIRCLE_THIRD_3	= CIRCLE_THIRD * 2;

int main()
{
	Renderer r;

	auto w = r.OpenWindow( 800, 600, "Vulkan API Tutorial 12" );

	VkCommandPool command_pool			= VK_NULL_HANDLE;
	VkCommandPoolCreateInfo pool_create_info {};
	pool_create_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_create_info.flags				= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_create_info.queueFamilyIndex	= r.GetVulkanGraphicsQueueFamilyIndex();
	vkCreateCommandPool( r.GetVulkanDevice(), &pool_create_info, nullptr, &command_pool );

	VkCommandBuffer command_buffer					= VK_NULL_HANDLE;
	VkCommandBufferAllocateInfo	command_buffer_allocate_info {};
	command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool		= command_pool;
	command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount	= 1;
	vkAllocateCommandBuffers( r.GetVulkanDevice(), &command_buffer_allocate_info, &command_buffer );

	VkSemaphore render_complete_semaphore	= VK_NULL_HANDLE;
	VkSemaphoreCreateInfo semaphore_create_info {};
	semaphore_create_info.sType				= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore( r.GetVulkanDevice(), &semaphore_create_info, nullptr, &render_complete_semaphore );

	float color_rotator		= 0.0f;
	auto timer				= std::chrono::steady_clock();
	auto last_time			= timer.now();
	uint64_t frame_counter	= 0;
	uint64_t fps			= 0;

	while( r.Run() ) {
		// CPU logic calculations

		++frame_counter;
		if( last_time + std::chrono::seconds( 1 ) < timer.now() ) {
			last_time		= timer.now();
			fps				= frame_counter;
			frame_counter	= 0;
			std::cout << "FPS: " << fps << std::endl;
		}

		// Begin render
		w->BeginRender();
		// Record command buffer
		VkCommandBufferBeginInfo command_buffer_begin_info {};
		command_buffer_begin_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_buffer_begin_info.flags				= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info );

		VkRect2D render_area {};
		render_area.offset.x		= 0;
		render_area.offset.y		= 0;
		render_area.extent			= w->GetVulkanSurfaceSize();

		color_rotator += 0.001;

		std::array<VkClearValue, 2> clear_values {};
		clear_values[ 0 ].depthStencil.depth		= 0.0f;
		clear_values[ 0 ].depthStencil.stencil		= 0;
		clear_values[ 1 ].color.float32[ 0 ]		= std::sin( color_rotator + CIRCLE_THIRD_1 ) * 0.5 + 0.5;
		clear_values[ 1 ].color.float32[ 1 ]		= std::sin( color_rotator + CIRCLE_THIRD_2 ) * 0.5 + 0.5;
		clear_values[ 1 ].color.float32[ 2 ]		= std::sin( color_rotator + CIRCLE_THIRD_3 ) * 0.5 + 0.5;
		clear_values[ 1 ].color.float32[ 3 ]		= 1.0f;

		VkRenderPassBeginInfo render_pass_begin_info {};
		render_pass_begin_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info.renderPass			= w->GetVulkanRenderPass();
		render_pass_begin_info.framebuffer			= w->GetVulkanActiveFramebuffer();
		render_pass_begin_info.renderArea			= render_area;
		render_pass_begin_info.clearValueCount		= clear_values.size();
		render_pass_begin_info.pClearValues			= clear_values.data();

		vkCmdBeginRenderPass( command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );

		vkCmdEndRenderPass( command_buffer );

		vkEndCommandBuffer( command_buffer );

		// Submit command buffer
		VkSubmitInfo submit_info {};
		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount		= 0;
		submit_info.pWaitSemaphores			= nullptr;
		submit_info.pWaitDstStageMask		= nullptr;
		submit_info.commandBufferCount		= 1;
		submit_info.pCommandBuffers			= &command_buffer;
		submit_info.signalSemaphoreCount	= 1;
		submit_info.pSignalSemaphores		= &render_complete_semaphore;

		vkQueueSubmit( r.GetVulkanQueue(), 1, &submit_info, VK_NULL_HANDLE );

		// End render
		w->EndRender( { render_complete_semaphore } );
	}

	vkQueueWaitIdle( r.GetVulkanQueue() );

	vkDestroySemaphore( r.GetVulkanDevice(), render_complete_semaphore, nullptr );
	vkDestroyCommandPool( r.GetVulkanDevice(), command_pool, nullptr );

	return 0;
}
