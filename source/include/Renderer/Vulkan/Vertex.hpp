#pragma once
#include "VulkanConfig.hpp"

struct Vertex
{
	Vector3 pos;
	Vector3 color;
	Vector2 texCoord;

    static VkVertexInputBindingDescription GetBindingDescription();

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
    
};