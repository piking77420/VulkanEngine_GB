#include "Renderer/Vulkan/FrameBuffer.hpp"

VkImageView& FrameBuffer::GetImageView(std::uint32_t index)
{
	return m_ImageView.at(index);
}

VkFramebuffer& FrameBuffer::GetFrammeBuffer(std::uint32_t index)
{
	
	return m_FrammeBuffer.at(index);
}
