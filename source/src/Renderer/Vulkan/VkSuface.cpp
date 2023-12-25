#include "Renderer/Vulkan/VkSuface.hpp"
#include "Engine.hpp"
#include "Renderer/Vulkan/VkContext.hpp"

VkSurfaceKHR VkUtils::CreateSurface()
{
	VkSurfaceKHR surface;

	if (glfwCreateWindowSurface(VkContext::GetInstance(), Engine::GetWindow(), nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}


	return surface;
}
