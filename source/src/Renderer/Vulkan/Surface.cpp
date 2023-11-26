#include "Renderer/Vulkan/Surface.hpp"

void VkInit::CreateSurface(VkInstance* _instance,GLFWwindow* _window, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(*_instance, _window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
	CORE_LOG_SUCCESS("Succeed to CreateSurface \n");

}
