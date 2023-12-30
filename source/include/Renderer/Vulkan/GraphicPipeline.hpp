#pragma once
#include "VulkanConfig.h"


void CreateGraphicsPipeline(const VkDescriptorSetLayout& descriptorSetLayout, 
	const VkRenderPass& renderPass, 
	VkPipelineLayout* pipelineLayout, 
	VkPipeline* graphicpipiline);

