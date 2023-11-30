#pragma once
#include "VulkanConfig.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace VkUtils
{


    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
    {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

     VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* _window);

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);

    void CreateSwapChain(GLFWwindow* _window, VkPhysicalDevice& _physicalDevice, VkSurfaceKHR& surface, VkSwapchainKHR& _swapChain, VkDevice& _device
        , std::vector<VkImage>& _swapChainImages, VkFormat& _swapChainImageFormat, VkExtent2D& _swapChainExtent);
    
}