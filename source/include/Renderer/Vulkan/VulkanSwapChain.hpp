#pragma once
#include "Device.hpp"
#include "Renderer/Vulkan/VulkFrame.hpp"

namespace vkInit
{
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};


	struct SwapChainBundle {
		vk::SwapchainKHR swapchain;
		std::vector<vkUtil::SwapChainFrame> frames;
		vk::Format format;
		vk::Extent2D extent;
	};

	static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice _device, vk::SurfaceKHR _surface, bool _debug)
	{
		SwapChainSupportDetails support;
		support.capabilities = _device.getSurfaceCapabilitiesKHR(_surface);


		if (_debug)
		{
			std::cout << "SwapChain Support Capabilities \n";
			std::cout << " minimum image count " << support.capabilities.minImageCount << '\n';
			std::cout << " maximum image count " << support.capabilities.maxImageCount << '\n';


			std::cout << "\tcurrent extent: \n";

			std::cout << "\t\twidth: " << support.capabilities.currentExtent.width << '\n';
			std::cout << "\t\theight: " << support.capabilities.currentExtent.height << '\n';

			std::cout << "\tminimum supported extent: \n";
			std::cout << "\t\twidth: " << support.capabilities.minImageExtent.width << '\n';
			std::cout << "\t\theight: " << support.capabilities.minImageExtent.height << '\n';

			std::cout << "\tmaximum supported extent: \n";
			std::cout << "\t\twidth: " << support.capabilities.maxImageExtent.width << '\n';
			std::cout << "\t\theight: " << support.capabilities.maxImageExtent.height << '\n';

			std::cout << "\tmaximum image array layers: " << support.capabilities.maxImageArrayLayers << '\n';


			std::cout << "\tsupported transforms:\n";
			std::vector<std::string> stringList = LogTransformBits(support.capabilities.supportedTransforms);
			for (std::string line : stringList) {
				std::cout << "\t\t" << line << '\n';
			}


			std::cout << "\tcurrent transform:\n";
			stringList = LogTransformBits(support.capabilities.currentTransform);
			for (std::string line : stringList) {
				std::cout << "\t\t" << line << '\n';
			}



			std::cout << "\tsupported alpha operations:\n";
			stringList = LogAlphaCompositeBits(support.capabilities.supportedCompositeAlpha);
			for (std::string line : stringList) {
				std::cout << "\t\t" << line << '\n';
			}
			/*
			std::cout << "\tsupported image usage:\n";
			stringList = LogImageUsageBits(support.capabilities.supportedUsageFlags);
			for (std::string line : stringList) {
				std::cout << "\t\t" << line << '\n';
			}
			*/
		}


		support.formats = _device.getSurfaceFormatsKHR(_surface);

		if (_debug)
		{

			for (vk::SurfaceFormatKHR supported : support.formats)
			{
				std::cout << "supported pixel format: " << vk::to_string(supported.format) << '\n';
				std::cout << "supported color space: " << vk::to_string(supported.colorSpace) << '\n';
			}


		}

		support.presentModes = _device.getSurfacePresentModesKHR(_surface);

		return support;
	}

	static vk::SurfaceFormatKHR ChooseSwapchainSurfaceFormat(std::vector<vk::SurfaceFormatKHR> formats) {

		for (vk::SurfaceFormatKHR format : formats) {
			if (format.format == vk::Format::eB8G8R8A8Unorm
				&& format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
				return format;
			}
		}

		return formats[0];
	}

	static vk::PresentModeKHR ChooseSwapchainPresentMode(std::vector<vk::PresentModeKHR> presentModes) {

		for (vk::PresentModeKHR presentMode : presentModes) {
			if (presentMode == vk::PresentModeKHR::eMailbox) {
				return presentMode;
			}
		}

		// eFifo is always support by a device 
		return vk::PresentModeKHR::eFifo;
	}

	static vk::Extent2D ChooseSwapchainExtent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities) {

		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			vk::Extent2D extent = { width, height };

			extent.width = std::min(
				capabilities.maxImageExtent.width,
				std::max(capabilities.minImageExtent.width, extent.width)
			);

			extent.height = std::min(
				capabilities.maxImageExtent.height,
				std::max(capabilities.minImageExtent.height, extent.height)
			);

			return extent;
		}
	}
	static SwapChainBundle CreateSwapchain(vk::Device _logicalDevice, vk::PhysicalDevice _physicalDevice, vk::SurfaceKHR _surface, int _width, int _height, bool _debug) {

		SwapChainSupportDetails support = QuerySwapChainSupport(_physicalDevice, _surface, _debug);

		vk::SurfaceFormatKHR format = ChooseSwapchainSurfaceFormat(support.formats);

		vk::PresentModeKHR presentMode = ChooseSwapchainPresentMode(support.presentModes);

		vk::Extent2D extent = ChooseSwapchainExtent(_width, _height, support.capabilities);

		uint32_t imageCount = std::min(
			support.capabilities.maxImageCount,
			support.capabilities.minImageCount + 1
		);


		vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(
			vk::SwapchainCreateFlagsKHR(), _surface, imageCount, format.format, format.colorSpace,
			extent, 1, vk::ImageUsageFlagBits::eColorAttachment
		);


		vkUtil::QueueFamilyIndices indices = vkUtil::FindFamiliesQueue(_physicalDevice, _surface, _debug);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		}

		createInfo.preTransform = support.capabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

		SwapChainBundle bundle{};
		try
		{
			bundle.swapchain = _logicalDevice.createSwapchainKHR(createInfo);
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("failed to create swap chain!");
		}

		std::vector<vk::Image> images = _logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
		bundle.frames.resize(images.size());
		for (size_t i = 0; i < images.size(); i++)
		{

			vk::ImageViewCreateInfo createInfo = {};
			createInfo.image = images[i];
			createInfo.viewType = vk::ImageViewType::e2D;
			// Pick RGB Standart
			createInfo.components.r = vk::ComponentSwizzle::eIdentity;
			createInfo.components.g = vk::ComponentSwizzle::eIdentity;
			createInfo.components.b = vk::ComponentSwizzle::eIdentity;
			createInfo.components.a = vk::ComponentSwizzle::eIdentity;
			createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			createInfo.format = format.format;

			bundle.frames[i].images = images[i];
			bundle.frames[i].imageView = _logicalDevice.createImageView(createInfo);
		}


		bundle.format = format.format;
		bundle.extent = extent;

		return bundle;
	}


}