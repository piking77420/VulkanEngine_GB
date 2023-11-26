#pragma once
#include "VulkanConfig.hpp"


struct QueueFamilyIndices;

namespace VkInit
{
    bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& _surface);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& _surface);

    void PickPhysicalDevice(VkInstance instance, VkPhysicalDevice& _physicalDevice, VkSurfaceKHR& _surface);

}