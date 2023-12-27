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

    static bool IsDeviceSuitable(const VkPhysicalDevice& device);
    

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    
}