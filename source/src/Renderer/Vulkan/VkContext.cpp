#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/VulkanConfig.h"
#include "Renderer/Vulkan/VkLayers.h"
#include "Renderer/Vulkan/VKPhysicalDevice.hpp"
#include "Renderer/Vulkan/Queue.hpp"
#include "Renderer/Vulkan/VkSuface.hpp"

VkInstance VkContext::m_Instance = nullptr;
VkDevice VkContext::m_Device = nullptr;
VkPhysicalDevice VkContext::m_PhysicalDevice = nullptr;


const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


void VkContext::CreateContext(const GLFWwindow const* window)
{

	CreateInstance();
	DebugMessenger();
	InitPhysicalDevice();
	m_Surface = VkUtils::CreateSurface();
	InitDevice();
}

void VkContext::DestroyContext()
{
	vkDestroyDevice(m_Device, nullptr);

	if (EnableValidationLayers) 
	{
		VkInit::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	}
	vkDestroyInstance(m_Instance, nullptr);

}

void VkContext::CreateInstance()
{
	if(EnableValidationLayers)
	{
		CORE_LOG("CreateVkInstance \n");
	}

	uint32_t version{ 0 };
	vkEnumerateInstanceVersion(&version);

	if (EnableValidationLayers) 
	{
		CORE_LOG("System can support vulkan Variant: " << VK_API_VERSION_VARIANT(version) + '\n');
		CORE_LOG(", Major: " << VK_API_VERSION_MAJOR(version) + '\n');
		CORE_LOG(", Minor: " << VK_API_VERSION_MINOR(version) + '\n');
		CORE_LOG(", Patch: " << VK_API_VERSION_PATCH(version) + '\n');
	}

	vk::ApplicationInfo appInfo = vk::ApplicationInfo(
		"Engine",
		version,
		"Engine",
		version,
		version
	);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (EnableValidationLayers)
	{
		extensions.push_back("VK_EXT_debug_utils");
	}

	if (EnableValidationLayers)
	{
		CORE_LOG("extensions to be requested:\n");

		for (const char* extensionName : extensions)
		{
			CORE_LOG("CreateInstance : "<< extensionName << " \n");
		}
	}

	vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
		vk::InstanceCreateFlags(),
		&appInfo,
		0, nullptr, // enabled layers
		static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
	);


	if (EnableValidationLayers)
	{
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	if (!EnumerateSupportedExtension(extensions, layers)) {
		throw std::exception("Failed EnumerateSupportedExtension");
	}


	 m_Instance = vk::createInstance(createInfo);


	if(m_Instance == nullptr && EnableValidationLayers)
	{
		CORE_LOG("Failed To Create VkContextInstance\n");

		throw std::exception("Failed To Create VkContextInstance");
	}
	
	

}

void VkContext::InitPhysicalDevice()
{
	m_PhysicalDevice = VkInit::ChoosePhysicalDevice(m_Instance);

}

void VkContext::InitDevice()
{
	QueueFamilyIndices indices = VkUtils::FindQueueFamilies(m_PhysicalDevice);
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;



	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();


	if (EnableValidationLayers) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		createInfo.ppEnabledLayerNames = layers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	
	if (vkCreateDevice(VkContext::GetPhysicalDevice(), &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
}

bool VkContext::EnumerateSupportedExtension(std::vector<const char*>& extensions, std::vector<const char* >& layers)
{

	//check extension support
	std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

	if (EnableValidationLayers) 
	{
		std::cout << "Device can support the following extensions:\n";
		for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
			std::cout << '\t' << supportedExtension.extensionName << '\n';
		}
	}

	bool found;
	for (const char* extension : extensions) {
		found = false;
		for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
			if (strcmp(extension, supportedExtension.extensionName) == 0) {
				found = true;
				if (EnableValidationLayers)
				{
					std::cout << "Extension \"" << extension << "\" is supported!\n";
				}
			}
		}
		if (!found)
		{
			if (EnableValidationLayers)
			{
				std::cout << "Extension \"" << extension << "\" is not supported!\n";
			}
			return false;
		}
	}

	//check layer support
	std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

	if (EnableValidationLayers)
	{
		std::cout << "Device can support the following layers:\n";
		for (vk::LayerProperties supportedLayer : supportedLayers) {
			std::cout << '\t' << supportedLayer.layerName << '\n';
		}
	}

	for (const char* layer : layers) {
		found = false;
		for (vk::LayerProperties supportedLayer : supportedLayers) {
			if (strcmp(layer, supportedLayer.layerName) == 0) {
				found = true;
				if (EnableValidationLayers)
				{
					std::cout << "Layer \"" << layer << "\" is supported!\n";
				}
			}
		}
		if (!found) {
			if (EnableValidationLayers) {
				std::cout << "Layer \"" << layer << "\" is not supported!\n";
			}
			return false;
		}
	}

	return true;


}



void VkContext::DebugMessenger()
{
	if (!EnableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	VkInit::PopulateDebugMessengerCreateInfo(createInfo);

	if (VkInit::CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}

	CORE_LOG("Succes to Create DebugUtilsMessengerEXT \n");

}


