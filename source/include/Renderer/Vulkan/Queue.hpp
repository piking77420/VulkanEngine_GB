#pragma once
#include "VulkanConfig.h"

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;


    bool IsComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
namespace VkUtils
{
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    static inline bool IsDeviceSuitable(const VkPhysicalDevice& device) {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        return indices.IsComplete();
    }

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtensions);
    
}