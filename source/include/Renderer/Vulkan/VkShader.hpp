#pragma once
#include "VulkanConfig.h"

namespace VkUtils
{
    std::vector<char> ReadFile(const std::string& filename);



    VkShaderModule CreateShaderModule(const std::vector<char>& code);

}