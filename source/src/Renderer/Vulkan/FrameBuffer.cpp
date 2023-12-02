#include "Renderer/Vulkan/FrameBuffer.hpp"



void VkUtils::CreateFramebuffers(VulkanRendererData& vkdata)
{
	vkdata.swapChainFramebuffers.resize(vkdata.swapChainImageViews.size());

	for (size_t i = 0; i < vkdata.swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			vkdata.swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = vkdata.renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = vkdata.swapChainExtent.width;
		framebufferInfo.height = vkdata.swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(vkdata.device, &framebufferInfo, nullptr, &vkdata.swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
	CORE_LOG_SUCCESS("Create Frame Buffers");
}
