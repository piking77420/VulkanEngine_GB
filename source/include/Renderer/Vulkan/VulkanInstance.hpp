#pragma once
#include "VulkanConfig.hpp"


namespace VulkanInstance 
{
    void CreateInstance(VkInstance* _instance, const std::vector<const char*>& _validationLayers);
    
}