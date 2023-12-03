#include "Texture.hpp"

using namespace VkClass;

void Texture::LoadResource(const std::string& path, VulkanRendererData& _vulkanRendererData)
{
	LoadImage(path,_vulkanRendererData);
}
