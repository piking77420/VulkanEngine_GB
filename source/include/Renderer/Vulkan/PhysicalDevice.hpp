#pragma once
#include <Core/Core.h>
#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices;

namespace PhysicalDevice
{
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    void PickPhysicalDevice(VkInstance instance, VkPhysicalDevice& _physicalDevice);

}