
#include "Renderer.h"

int main()
{
	Renderer r;

	auto device = r._device;
	auto queue	= r._queue;

	VkFence fence;
	VkFenceCreateInfo fence_create_info {};
	fence_create_info.sType				= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkCreateFence( device, &fence_create_info, nullptr, &fence );

	VkSemaphore semaphore;
	VkSemaphoreCreateInfo semaphore_create_info {};
	semaphore_create_info.sType			= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore( device, &semaphore_create_info, nullptr, &semaphore );

	VkCommandPool command_pool;
	VkCommandPoolCreateInfo pool_create_info {};
	pool_create_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_create_info.queueFamilyIndex	= r._graphics_family_index;
	pool_create_info.flags				= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkCreateCommandPool( device, &pool_create_info, nullptr, &command_pool );

	VkCommandBuffer command_buffer[ 2 ];
	VkCommandBufferAllocateInfo command_buffer_allocate_info {};
	command_buffer_allocate_info.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool			= command_pool;
	command_buffer_allocate_info.commandBufferCount		= 2;
	command_buffer_allocate_info.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkAllocateCommandBuffers( device, &command_buffer_allocate_info, command_buffer );

	{
		VkCommandBufferBeginInfo begin_info {};
		begin_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer( command_buffer[ 0 ], &begin_info );

		vkCmdPipelineBarrier( command_buffer[ 0 ],
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			0,
			0, nullptr,
			0, nullptr,
			0, nullptr );

		VkViewport viewport {};
		viewport.maxDepth	= 1.0f;
		viewport.minDepth	= 0.0f;
		viewport.width		= 512;
		viewport.height		= 512;
		viewport.x			= 0;
		viewport.y			= 0;
		vkCmdSetViewport( command_buffer[ 0 ], 0, 1, &viewport );

		vkEndCommandBuffer( command_buffer[ 0 ] );
	}
	{
		VkCommandBufferBeginInfo begin_info {};
		begin_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer( command_buffer[ 1 ], &begin_info );

		VkViewport viewport {};
		viewport.maxDepth	= 1.0f;
		viewport.minDepth	= 0.0f;
		viewport.width		= 512;
		viewport.height		= 512;
		viewport.x			= 0;
		viewport.y			= 0;
		vkCmdSetViewport( command_buffer[ 1 ], 0, 1, &viewport );

		vkEndCommandBuffer( command_buffer[ 1 ] );
	}
	{
		VkSubmitInfo submit_info {};
		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount		= 1;
		submit_info.pCommandBuffers			= &command_buffer[ 0 ];
		submit_info.signalSemaphoreCount	= 1;
		submit_info.pSignalSemaphores		= &semaphore;
		vkQueueSubmit( queue, 1, &submit_info, VK_NULL_HANDLE );
	}
	{
		VkPipelineStageFlags flags[] { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
		VkSubmitInfo submit_info {};
		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount		= 1;
		submit_info.pCommandBuffers			= &command_buffer[ 1 ];
		submit_info.waitSemaphoreCount		= 1;
		submit_info.pWaitSemaphores			= &semaphore;
		submit_info.pWaitDstStageMask		= flags;
		vkQueueSubmit( queue, 1, &submit_info, VK_NULL_HANDLE );
	}

//	auto ret = vkWaitForFences( device, 1, &fence, VK_TRUE, UINT64_MAX );
	vkQueueWaitIdle( queue );

	vkDestroyCommandPool( device, command_pool, nullptr );
	vkDestroyFence( device, fence, nullptr );
	vkDestroySemaphore( device, semaphore, nullptr );

	return 0;
}
