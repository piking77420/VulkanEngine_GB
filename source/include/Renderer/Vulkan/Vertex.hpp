#pragma once
#include "VulkanConfig.hpp"

struct Vertex
{
	Vector3 pos;
	Vector3 color;

    static VkVertexInputBindingDescription GetBindingDescription();

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
    
};