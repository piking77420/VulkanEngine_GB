#pragma once
#include "VulkanConfig.h"
#include "Renderer/Camera/Camera.hpp"




class VulkanRenderer
{
public:
	


	VulkanRenderer();
	~VulkanRenderer();
private:
	Camera m_cam;


	VkSwapchainKHR m_ViewportSwapChain;
	std::vector<VkImage> m_ViewportImages;
	std::vector<VkImageView> m_ViewportImageViews;
	VkDescriptorPool m_ViewportDescriptorPool;
	VkRenderPass m_ViewportRenderPass;
	std::vector<VkFramebuffer> m_ViewportFramebuffers;
	VkCommandPool m_ViewportCommandPool;
	std::vector<VkCommandBuffer> m_ViewportCommandBuffers;


};

