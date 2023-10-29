#pragma once
#include "Renderer/Vulkan/config.hpp"
#include "Renderer/Vulkan/QueuFamilies.hpp"

namespace vkInit
{

	static bool CheckDeviceExtensionSupport(vk::PhysicalDevice _PhysicalDevice, const std::vector<const char*>& requestedExtensions, bool _debug)
	{

		std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

		if (_debug) {
			std::cout << "Device can support extensions:\n";
		}

		for (vk::ExtensionProperties& extension : _PhysicalDevice.enumerateDeviceExtensionProperties()) {

			if (_debug) {
				std::cout << "\t\"" << extension.extensionName << "\"\n";
			}

			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	static bool IsDeviceSuitable(vk::PhysicalDevice _PhysicalDevice, bool _debug)
	{
		if (_debug)
		{
			std::cout << "Check Physical Device Is Suitable ...\n";
		}
		// Choose a physical device if he can perform swapChain
		std::vector<const char*> requestExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		if (_debug)
		{
			std::cout << "We are requesting for ...\n";

			for (const char* request : requestExtension)
			{
				std::cout << " [" << request << "] \n";
			}
		}

		if (bool extensionSupported = CheckDeviceExtensionSupport(_PhysicalDevice, requestExtension, _debug))
		{
			if (_debug)
			{
				std::cout << "Device can support the requested extensions!\n";
			}
			else
			{
				if (_debug)
				{
					std::cout << "Device can't support the requested extensions!\n";
				}
				return false;
			}
		}
		return true;
	}

	static void LogPhysicalDevice(vk::PhysicalDevice _PhysicalDevice)
	{
		vk::PhysicalDeviceProperties properties = _PhysicalDevice.getProperties();



		std::cout << "Device name : " << properties.deviceName << '\n';
		std::cout << "Device Type : ";

		switch (properties.deviceType)
		{

		case (vk::PhysicalDeviceType::eCpu):
			std::cout << "CPU\n";
			break;

		case (vk::PhysicalDeviceType::eDiscreteGpu):
			std::cout << "Discrete GPU\n";
			break;

		case (vk::PhysicalDeviceType::eIntegratedGpu):
			std::cout << "Integrated GPU\n";
			break;

		case (vk::PhysicalDeviceType::eVirtualGpu):
			std::cout << "Virtual GPU\n";
			break;

		default:
			std::cout << "Other\n";
		}

	}

	static vk::PhysicalDevice GetPhyscialDevice(vk::Instance& _instance, bool _debug)
	{
		if (_debug)
		{
			std::cout << "Choosing Physical Device... \n";
		}

		std::vector<vk::PhysicalDevice> availablePhysicalDevice = _instance.enumeratePhysicalDevices();
		if (_debug)
		{
			std::cout << "There are " << availablePhysicalDevice.size() << " Physical device available" << '\n';
		}

		for (vk::PhysicalDevice physicDevice : availablePhysicalDevice)
		{
			if (_debug)
			{
				LogPhysicalDevice(physicDevice);
			}
			if (IsDeviceSuitable(physicDevice, _debug))
			{
				return physicDevice;
			}
		}


		return nullptr;
	}

	static vk::Device CreateLogicalDevice(vk::PhysicalDevice _physicalDevice, vk::SurfaceKHR _surface, bool _debug)
	{
		vkUtil::QueueFamilyIndices indicies = vkUtil::FindFamiliesQueue(_physicalDevice, _surface, _debug);

		std::vector<uint32_t> uniqueIndicies;
		uniqueIndicies.push_back(indicies.graphicsFamily.value());

		if (indicies.graphicsFamily.value() != indicies.presentFamily.value())
		{
			uniqueIndicies.push_back(indicies.presentFamily.value());
		}

		float queuPriority = 1.f;



		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
		for (uint32_t queueFamily : uniqueIndicies)
		{
			queueCreateInfo.push_back(vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuPriority));
		}

		std::vector<const char*> deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};





		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

		std::vector<const char*> enableLayers;
		if (_debug)
		{
			enableLayers.push_back("VK_LAYER_KHRONOS_validation");
		}

		vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(),
			queueCreateInfo.size(), queueCreateInfo.data(),
			enableLayers.size(), enableLayers.data(),
			deviceExtensions.size(), deviceExtensions.data(),
			&deviceFeatures
		);



		try {
			vk::Device device = _physicalDevice.createDevice(deviceInfo);
			if (_debug) {
				std::cout << "GPU has been successfully abstracted!\n";
			}
			return device;
		}
		catch (vk::SystemError err) {
			if (_debug) {
				std::cout << "Device creation failed!\n";
				return nullptr;
			}
		}
		return nullptr;


	}

	static std::array<vk::Queue, 2> GetQueue(vk::PhysicalDevice _physicalDevice, const vk::Device& _device, vk::SurfaceKHR _surface, bool _debug)
	{
		vkUtil::QueueFamilyIndices indices = vkUtil::FindFamiliesQueue(_physicalDevice, _surface, _debug);

		return { _device.getQueue(indices.graphicsFamily.value(), 0)
			,_device.getQueue(indices.graphicsFamily.value(), 0)
		};
	}

}