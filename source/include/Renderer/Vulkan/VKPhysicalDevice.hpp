#pragma once
#include "VulkanConfig.h"

namespace VkInit
{
	vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance);

	int GetScoreFromPhysicalDevice(const vk::PhysicalDevice& physicalDevice);
}
