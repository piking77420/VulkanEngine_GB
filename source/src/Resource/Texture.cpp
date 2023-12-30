#include "Resource/Texture.hpp"
#include "Renderer/Vulkan/VkContext.hpp"

void Texture::Destroy()
{
    const VkDevice& device = VkContext::GetDevice();
    vkDestroyImageView(device, m_TextureImageView, nullptr);
    vkDestroyImage(device, m_TextureImage, nullptr);
    vkFreeMemory(device, m_TextureImageMemory, nullptr);
}

void Texture::LoadResource(const std::string& path)
{
    CreateTextureImage(path.c_str(), m_TextureImage, m_TextureImageMemory);
    m_TextureImageView = CreateImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

}

void Texture::OnChange()
{
}
