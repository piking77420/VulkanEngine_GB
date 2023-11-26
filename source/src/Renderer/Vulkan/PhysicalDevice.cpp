#include "Renderer/Vulkan/PhysicalDevice.hpp"
#include "Renderer/Vulkan/QueueFamily.hpp"

QueueFamilyIndices VkInit::FindQueueFamilies(VkPhysicalDevice& device,VkSurfaceKHR& _surface) 
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}


bool VkInit::IsDeviceSuitable(VkPhysicalDevice device,VkSurfaceKHR& _surface)
{
    QueueFamilyIndices indices = VkInit::FindQueueFamilies(device, _surface);

    return indices.isComplete();
}

void VkInit::PickPhysicalDevice(VkInstance instance,VkPhysicalDevice& _physicalDevice, VkSurfaceKHR& _surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (VkInit::IsDeviceSuitable(device, _surface)) {
            _physicalDevice = device;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    CORE_LOG_SUCCESS("PickPhysical Device Succeed \n");
}
