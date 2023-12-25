#include "Renderer/Vulkan/VKPhysicalDevice.hpp"
#include <vulkan/vk_enum_string_helper.h>

vk::PhysicalDevice VkInit::ChoosePhysicalDevice(const vk::Instance& instance)
{
	if (EnableValidationLayers)
	{
		CORE_LOG(" ChoosingPhysicalDevice \n");
	}


	std::vector<vk::PhysicalDevice> availablePhysicalDevices = instance.enumeratePhysicalDevices();
	std::unique_ptr<int[]> physicaldeviceScore = std::make_unique<int[]>(availablePhysicalDevices.size());

	if(availablePhysicalDevices.empty())
	{
		throw std::exception("There is no physicalDevice available ");
	}


	CORE_LOG(" ChoosingPhysicalDevice .... \n");

	for (size_t i = 0; i < availablePhysicalDevices.size(); i++)
	{
		CORE_LOG("Physical Device " << i << '\n');
		physicaldeviceScore[i] = GetScoreFromPhysicalDevice(availablePhysicalDevices[i]);
		CORE_LOG("Physical Device " << i << '\n');

	}
	

	int bestScore = 0;
	int bestScoreIndex = 0;

	for (size_t i = 0; i < availablePhysicalDevices.size(); i++)
	{
		if(physicaldeviceScore[i] > bestScore)
		{
			bestScore = physicaldeviceScore[i];
			bestScoreIndex = i;
		}
		
	}

	return availablePhysicalDevices[bestScoreIndex];
}

int VkInit::GetScoreFromPhysicalDevice(const vk::PhysicalDevice& physicalDevice)
{
	int currentScore = 0;

	vk::PhysicalDeviceProperties propertie = physicalDevice.getProperties();

	CORE_LOG("Device name " << propertie.deviceName << "\n");
	CORE_LOG("ApiVersion " << propertie.apiVersion << "\n");
	CORE_LOG("DriverVersion " << propertie.driverVersion << "\n");

	std::string deviceType = string_VkPhysicalDeviceType((VkPhysicalDeviceType)(propertie.deviceType));

	CORE_LOG("DeviceType " << deviceType << "\n");

	if((VkPhysicalDeviceType)propertie.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		currentScore += 1;
	}


	return currentScore;
}

