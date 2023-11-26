#pragma once
#include "VulkanConfig.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace VkUtils
{


    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    static inline VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
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