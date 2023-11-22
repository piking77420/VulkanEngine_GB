#pragma once
#include "Renderer/Vulkan/config.hpp"

namespace vkInit
{
	// Check if extensions are supported
	static bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool _debug)
	{

		std::vector<vk::ExtensionProperties> supportedExtension = vk::enumerateInstanceExtensionProperties();

		if (_debug)
		{
			std::cout << "this device can support the follow extension \n";
			for (vk::ExtensionProperties supported : supportedExtension)
			{
				std::cout << " [" << supported.extensionName << "] " << '\n';
			}
		}

		bool found;

		for (const char* ext : extensions)
		{
			found = false;

			for (vk::ExtensionProperties prop : supportedExtension)
			{
				if (strcmp(ext, prop.extensionName) == 0)
				{
					found = true;
					if (_debug)
					{
						std::cout << "This extension is supported " << ext << '\n';
					}
				}
			}

			if (!found)
			{
				if (_debug)
				{
					std::cout << "This extension is not supported " << ext << '\n';
				}
				return false;
			}



		}
		std::vector<vk::LayerProperties> supportedLayer = vk::enumerateInstanceLayerProperties();

		if (_debug)
		{
			std::cout << " Device can support the following layers : \n";
			for (vk::LayerProperties supported : supportedLayer)
			{
				std::cout << " [" << supported.layerName << "] \n";
			}

		}

		for (const char* lay : layers)
		{
			found = false;

			for (vk::LayerProperties prop : supportedLayer)
			{
				if (strcmp(lay, prop.layerName) == 0)
				{
					found = true;
					if (_debug)
					{
						std::cout << "This layers is supported " << lay << '\n';
					}
				}
			}

			if (!found)
			{
				if (_debug)
				{
					std::cout << "This layers is not supported " << lay << '\n';
				}
				return false;
			}



		}


		return true;
	}



	static vk::Instance MakeInstance(bool _debug, const char* appName)
	{
		std::uint32_t version{ 0 };
		vkEnumerateInstanceVersion(&version);

		if (_debug)
		{
			std::cout << VK_API_VERSION_VARIANT(version);
			std::cout << VK_API_VERSION_MAJOR(version);
			std::cout << VK_API_VERSION_MINOR(version);
			std::cout << VK_API_VERSION_PATCH(version);

		}

		version &= ~(0xFFFU);

		vk::ApplicationInfo appInfo = vk::ApplicationInfo(appName, version, appName, version, version);

		uint32_t glfwExtensioncount = 0;
		const char** glfwExtension;
		glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensioncount);

		std::vector<const char*> extension(glfwExtension, glfwExtension + glfwExtensioncount);
		if (_debug)
		{
			extension.push_back("VK_EXT_debug_utils");
		}


		if (_debug)
		{
			std::cout << "extension request : \n";

			for (const char* extensionname : extension)
			{
				std::cout << extensionname << '\n';
			}
		}

		std::vector<const char*> layers;

		if (_debug)
		{
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}

		if (!supported(extension, layers, _debug))
			return nullptr;


		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, 0, nullptr, static_cast<uint32_t>(extension.size()), extension.data());

		try
		{
			return vk::createInstance(createInfo, nullptr);
		}
		catch (vk::SystemError err)
		{
			if (_debug)
			{
				CORE_LOG_ERROR("Failed to Create Instance " << '\n');
			}
			return nullptr;
		}

		return nullptr;
	}





}