#pragma once
#include "VulkanConfig.h"

class VkDepth
{
public:
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	void CreateDepthRessource();
	void DestroyDepthRessource();

	VkDepth() {};
	~VkDepth(){}
};


