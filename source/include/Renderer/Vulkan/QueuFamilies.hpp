#include "Renderer/Vulkan/config.hpp"


namespace vkUtil
{
	struct QueueFamilyIndices
	{
		std::optional<std::uint32_t> graphicsFamily;
		std::optional<std::uint32_t> presentFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};


	static vkUtil::QueueFamilyIndices FindFamiliesQueue(vk::PhysicalDevice _device, vk::SurfaceKHR _surface, bool _debug)
	{
		vkUtil::QueueFamilyIndices indicies;

		std::vector<vk::QueueFamilyProperties> queueFamilies = _device.getQueueFamilyProperties();



		if (_debug) {
			std::cout << "There are " << queueFamilies.size() << " queue families available on the system.\n";
		}

		/*
			* // Provided by VK_VERSION_1_0
				typedef struct VkQueueFamilyProperties {
				VkQueueFlags    queueFlags;
				uint32_t        queueCount;
				uint32_t        timestampValidBits;
				VkExtent3D      minImageTransferGranularity;
				} VkQueueFamilyProperties;

				queueFlags is a bitmask of VkQueueFlagBits indicating capabilities of the queues in this queue family.

				queueCount is the unsigned integer count of queues in this queue family. Each queue family must support
				at least one queue.

				timestampValidBits is the unsigned integer count of meaningful bits in the timestamps written via
				vkCmdWriteTimestamp. The valid range for the count is 36..64 bits, or a value of 0,
				indicating no support for timestamps. Bits outside the valid range are guaranteed to be zeros.

				minImageTransferGranularity is the minimum granularity supported for image transfer
				operations on the queues in this queue family.
			*/

			/*
			* // Provided by VK_VERSION_1_0
				typedef enum VkQueueFlagBits {
				VK_QUEUE_GRAPHICS_BIT = 0x00000001,
				VK_QUEUE_COMPUTE_BIT = 0x00000002,
				VK_QUEUE_TRANSFER_BIT = 0x00000004,
				VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
				} VkQueueFlagBits;
			*/

		int i = 0;
		for (vk::QueueFamilyProperties queueFamily : queueFamilies) {

			/*
			* // Provided by VK_VERSION_1_0
				typedef struct VkQueueFamilyProperties {
				VkQueueFlags    queueFlags;
				uint32_t        queueCount;
				uint32_t        timestampValidBits;
				VkExtent3D      minImageTransferGranularity;
				} VkQueueFamilyProperties;

				queueFlags is a bitmask of VkQueueFlagBits indicating capabilities of the queues in this queue family.

				queueCount is the unsigned integer count of queues in this queue family. Each queue family must support
				at least one queue.

				timestampValidBits is the unsigned integer count of meaningful bits in the timestamps written via
				vkCmdWriteTimestamp. The valid range for the count is 36..64 bits, or a value of 0,
				indicating no support for timestamps. Bits outside the valid range are guaranteed to be zeros.

				minImageTransferGranularity is the minimum granularity supported for image transfer
				operations on the queues in this queue family.
			*/

			/*
			* // Provided by VK_VERSION_1_0
				typedef enum VkQueueFlagBits {
				VK_QUEUE_GRAPHICS_BIT = 0x00000001,
				VK_QUEUE_COMPUTE_BIT = 0x00000002,
				VK_QUEUE_TRANSFER_BIT = 0x00000004,
				VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
				} VkQueueFlagBits;
			*/

			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
				indicies.graphicsFamily = i;

				if (_debug)
				{
					std::cout << "Queue Family " << i << " is suitable for graphics \n";
				}
			}

			if (_device.getSurfaceSupportKHR(i, _surface))
			{
				indicies.presentFamily = i;
				if (_debug)
				{
					std::cout << "Queue Family " << i << " is suitable for presenting\n";
				}
			}

			if (indicies.IsComplete()) {
				break;
			}

			i++;
		}

		return indicies;
	}


}