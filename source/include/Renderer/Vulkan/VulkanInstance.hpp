#pragma once
#include <vulkan/vulkan.hpp>
#include "Core/Core.h"

namespace VulkanInstance 
{
    void CreateInstance(VkInstance* _instance, const std::vector<const char*>& _validationLayers);
    
}