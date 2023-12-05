#include "Renderer/Vulkan/FrameBuffer.hpp"



void VkUtils::CreateFramebuffers(VulkanRendererData& vkdata)
{
    vkdata.swapChainFramebuffers.resize(vkdata.swapChainImageViews.size());

    for (size_t i = 0; i < vkdata.swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            vkdata.swapChainImageViews[i],
            vkdata.depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkdata.renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = vkdata.swapChainExtent.width;
        framebufferInfo.height = vkdata.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vkdata.device, &framebufferInfo, nullptr, &vkdata.swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
	CORE_LOG_SUCCESS("Create Frame Buffers");
}
