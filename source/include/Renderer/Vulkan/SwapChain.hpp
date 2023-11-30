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

    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

     VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* _window);

    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);

    void CreateSwapChain(GLFWwindow* _window, VkPhysicalDevice& _physicalDevice, VkSurfaceKHR& surface, VkSwapchainKHR& _swapChain, VkDevice& _device
        , std::vector<VkImage>& _swapChainImages, VkFormat& _swapChainImageFormat, VkExtent2D& _swapChainExtent);
    
}