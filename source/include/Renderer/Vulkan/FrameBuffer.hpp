#pragma once
#include "VulkanConfig.h"

class FrameBuffer
{


	VkImageView& GetImageView(std::uint32_t index);

	VkFramebuffer& GetFrammeBuffer(std::uint32_t index);

private:
	std::array<VkFramebuffer, MAX_FRAMES_IN_FLIGHT> m_FrammeBuffer;
	std::array<VkImageView, MAX_FRAMES_IN_FLIGHT> m_ImageView;

};