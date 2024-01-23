#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/VulkanConfig.h"
#include "Renderer/Vulkan/VkLayers.h"
#include "Renderer/Vulkan/VKPhysicalDevice.hpp"
#include "Renderer/Vulkan/Queue.hpp"
#include "Renderer/Vulkan/VkSuface.hpp"
#include "Renderer/Vulkan/Queue.hpp"
#include "Renderer/Vulkan/SwapChain.hpp"
#include "Renderer/Vulkan/VkShader.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp" 
#include "Engine.hpp"
#include "Renderer/Vulkan/VkFormat.hpp"
#include "Renderer/Vulkan/Depth.hpp"


VkDepth VkContext::vkDepth;


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
	m_Surface = VkUtils::CreateSurface();
	InitPhysicalDevice();
	InitDevice();
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateSyncObjects();
	CreateCommandPool(m_CommandPool);
	vkDepth.CreateDepthRessource();
	CreateFramebuffers();


	// vIEWpORT
	//CreateCommandPool(m_ViewportCommandPool);
	//CreateViewportImage();
	//CreateViewportImageViews();
	//CreateViewportFrameBuffers();

	CreateTextureSampler();

}

void VkContext::DestroyContext()
{
	vkDepth.DestroyDepthRessource(),
	CleanupSwapChain();

	vkDestroySampler(m_Device, m_TextureSampler, nullptr);

	vkDestroyRenderPass(VkContext::GetDevice(), m_RenderPass, nullptr);
	vkDestroyCommandPool(VkContext::GetDevice(), m_CommandPool, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(VkContext::GetDevice(), m_RenderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(VkContext::GetDevice(), m_ImageAvailableSemaphores[i], nullptr);
		vkDestroyFence(VkContext::GetDevice(), m_InFlightFences[i], nullptr);
	}

	vkDestroyDevice(m_Device, nullptr);

	if (EnableValidationLayers) {
		VkInit::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	vkDestroyInstance(m_Instance, nullptr);

}
void VkContext::CreateImageViews()
{
	m_SwapChainImageViews.resize(m_SwapChainImages.size());


	
	for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
		m_SwapChainImageViews[i] = CreateImageView(m_SwapChainImages[i], m_SwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}





void VkContext::CreateSwapChain()
{
	using namespace VkUtils;


	const VkPhysicalDevice const& physicalDevice = VkContext::GetPhysicalDevice();
	VkSurfaceKHR surface = VkContext::GetSurface();

	const VkDevice const& device = VkContext::GetDevice();


	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice, surface);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = VkUtils::FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_SwapChain);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, nullptr);
	m_SwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, m_SwapChain, &imageCount, m_SwapChainImages.data());

	m_SwapChainImageFormat = surfaceFormat.format;
	m_SwapChainExtent = extent;
}

void VkContext::CreateFramebuffers()
{
	std::uint32_t swapChainImageViewsSize = VkContext::GetSwapChainImageViewsSize();
	std::vector<VkImageView>& swapChainImageViews = VkContext::GetSwapChainImageViews();
	VkExtent2D swapChainVkExtent = VkContext::GetSwapChainVkExtent();



	m_SwapChainFramebuffers.resize(swapChainImageViewsSize);

	for (size_t i = 0; i < swapChainImageViewsSize; i++) 
	{

		std::array<VkImageView, 2> attachments = {
				swapChainImageViews[i],
				vkDepth.depthImageView
		};;

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = VkContext::GetRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainVkExtent.width;
		framebufferInfo.height = swapChainVkExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(VkContext::GetDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void VkContext::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_SwapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = VkUtils::FindDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(VkContext::GetDevice(), &renderPassInfo, nullptr, &VkContext::GetRenderPass()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}


void VkContext::CreateSyncObjects()
{
	m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	
	VkDevice device = VkContext::GetDevice();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {

			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void VkContext::CleanupSwapChain()
{
	const VkDevice& device = VkContext::GetDevice();

	for (size_t i = 0; i < m_SwapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(device, m_SwapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
		vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
}

void VkContext::CreateCommandPool(VkCommandPool& commandPool)
{
	QueueFamilyIndices queueFamilyIndices = VkUtils::FindQueueFamilies(VkContext::GetPhysicalDevice());

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(VkContext::GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void VkContext::CreateTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(m_Device, &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VkContext::CreateViewportImage()
{
	/*
	m_ViewportImages.resize(m_SwapChainImages.size());
	m_DstImageMemory.resize(m_SwapChainImages.size());

	for (uint32_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		// Create the linear tiled destination image to copy to and to read the memory from
		VkImageCreateInfo imageCreateCI{};
		imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
		// Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
		imageCreateCI.format = VK_FORMAT_B8G8R8A8_SRGB;
		imageCreateCI.extent.width = m_SwapChainExtent.width;
		imageCreateCI.extent.height = m_SwapChainExtent.height;
		imageCreateCI.extent.depth = 1;
		imageCreateCI.arrayLayers = 1;
		imageCreateCI.mipLevels = 1;
		imageCreateCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateCI.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		// Create the image
		// VkImage dstImage;
		vkCreateImage(m_Device, &imageCreateCI, nullptr, &m_ViewportImages[i]);
		// Create memory to back up the image
		VkMemoryRequirements memRequirements;
		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		// VkDeviceMemory dstImageMemory;
		vkGetImageMemoryRequirements(m_Device, m_ViewportImages[i], &memRequirements);
		memAllocInfo.allocationSize = memRequirements.size;
		// Memory must be host visible to copy from
		memAllocInfo.memoryTypeIndex = VkUtils::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		vkAllocateMemory(m_Device, &memAllocInfo, nullptr, &m_DstImageMemory[i]);
		vkBindImageMemory(m_Device, m_ViewportImages[i], m_DstImageMemory[i], 0);

		VkCommandBuffer copyCmd = VkUtils::BeginSingleTimeCommands(m_ViewportCommandPool);

		insertImageMemoryBarrier(
			copyCmd,
			m_ViewportImages[i],
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		endSingleTimeCommands(copyCmd, m_ViewportCommandPool);
	}
	*/
}

void VkContext::CreateViewportImageViews()
{
	/*
	m_ViewportImageViews.resize(m_ViewportImages.size());

	for (uint32_t i = 0; i < m_ViewportImages.size(); i++)
	{
		m_ViewportImageViews[i] = CreateImageView(m_ViewportImages[i], VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}
	*/
}

void VkContext::CreateViewportFrameBuffers()
{
	/*
	m_ViewportFramebuffers.resize(m_ViewportImageViews.size());

	for (size_t i = 0; i < m_ViewportImageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
				m_ViewportImageViews[i],
				m_DepthImageView };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_ViewportRenderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = m_SwapChainExtent.width;
		framebufferInfo.height = m_SwapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_ViewportFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
	*/
}



void VkContext::NextFrame()
{
	m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


void VkContext::RecreateSwapChain()
{
	const constexpr int nullSize = 0;

	int width = nullSize, height = nullSize;
	glfwGetFramebufferSize(Engine::GetWindow(), &width, &height);
	while (width == nullSize || height == nullSize) {
		glfwGetFramebufferSize(Engine::GetWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(VkContext::GetDevice());

	CleanupSwapChain();
	CreateSwapChain();

	CreateImageViews();
	vkDepth.CreateDepthRessource();
	CreateFramebuffers();
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

	deviceFeatures.samplerAnisotropy = VK_TRUE;

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
	vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);

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


