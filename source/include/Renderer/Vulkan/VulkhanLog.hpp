#pragma once
#include "Renderer/Vulkan/config.hpp"



namespace vkInit
{

	/*
	* Debug call back:
	*
	*	typedef enum VkDebugUtilsMessageSeverityFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageSeverityFlagBitsEXT;

	*	typedef enum VkDebugUtilsMessageTypeFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT = 0x00000002,
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT = 0x00000004,
			VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageTypeFlagBitsEXT;

	*	typedef struct VkDebugUtilsMessengerCallbackDataEXT {
			VkStructureType                              sType;
			const void*                                  pNext;
			VkDebugUtilsMessengerCallbackDataFlagsEXT    flags;
			const char*                                  pMessageIdName;
			int32_t                                      messageIdNumber;
			const char*                                  pMessage;
			uint32_t                                     queueLabelCount;
			const VkDebugUtilsLabelEXT*                  pQueueLabels;
			uint32_t                                     cmdBufLabelCount;
			const VkDebugUtilsLabelEXT*                  pCmdBufLabels;
			uint32_t                                     objectCount;
			const VkDebugUtilsObjectNameInfoEXT*         pObjects;
		} VkDebugUtilsMessengerCallbackDataEXT;

	*/

	/**
		Logging callback function.

		\param messageSeverity describes the severity level of the message
		\param messageType describes the type of the message
		\param pCallbackData standard data associated with the message
		\param pUserData custom extra data which can be associated with the message
		\returns whether to end program execution
	*/
	
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);


	
	static vk::DebugUtilsMessengerEXT MakeDebugLogger(vk::Instance& _instance, vk::DispatchLoaderDynamic& _dldi)
	{
		vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
			vk::DebugUtilsMessengerCreateFlagsEXT(),
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			debugCallback,
			nullptr
		);

		return _instance.createDebugUtilsMessengerEXT(createInfo, nullptr, _dldi);

	}

	static std::vector<std::string> LogTransformBits(vk::SurfaceTransformFlagsKHR _bits)
	{
		std::vector<std::string> result;

		if (_bits & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
			result.push_back("identity");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eRotate90) {
			result.push_back("90 degree rotation");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eRotate180) {
			result.push_back("180 degree rotation");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eRotate270) {
			result.push_back("270 degree rotation");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror) {
			result.push_back("horizontal mirror");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90) {
			result.push_back("horizontal mirror, then 90 degree rotation");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180) {
			result.push_back("horizontal mirror, then 180 degree rotation");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270) {
			result.push_back("horizontal mirror, then 270 degree rotation");
		}
		if (_bits & vk::SurfaceTransformFlagBitsKHR::eInherit) {
			result.push_back("inherited");
		}

		return result;
	}


	static std::vector<std::string> LogImageUsageBits(vk::ImageUsageFlags _bits) 
	{
		std::vector<std::string> result;


		if (_bits & vk::ImageUsageFlagBits::eTransferSrc) {
			result.push_back("transfer src: image can be used as the source of a transfer command.");
		}
		if (_bits & vk::ImageUsageFlagBits::eTransferDst) {
			result.push_back("transfer dst: image can be used as the destination of a transfer command.");
		}
		if (_bits & vk::ImageUsageFlagBits::eSampled) {
			result.push_back("sampled: image can be used to create a VkImageView suitable for occupying a \VkDescriptorSet slot either of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE or \ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, and be sampled by a shader.");
		}
		if (_bits & vk::ImageUsageFlagBits::eStorage) {
			result.push_back("storage: image can be used to create a VkImageView suitable for occupying a \VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_STORAGE_IMAGE.");
		}
		if (_bits & vk::ImageUsageFlagBits::eColorAttachment) {
			result.push_back("color attachment: image can be used to create a VkImageView suitable for use as \a color or resolve attachment in a VkFramebuffer.");
		}
		if (_bits & vk::ImageUsageFlagBits::eDepthStencilAttachment) {
			result.push_back("depth/stencil attachment: image can be used to create a VkImageView \suitable for use as a depth/stencil or depth/stencil resolve attachment in a VkFramebuffer.");
		}
		if (_bits & vk::ImageUsageFlagBits::eTransientAttachment) {
			result.push_back("transient attachment: implementations may support using memory allocations \with the VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT to back an image with this usage. This \bit can be set for any image that can be used to create a VkImageView suitable for use as \a color, resolve, depth/stencil, or input attachment.");
		}
		if (_bits & vk::ImageUsageFlagBits::eInputAttachment) {
			result.push_back("input attachment: image can be used to create a VkImageView suitable for \occupying VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; be read from \a shader as an input attachment; and be used as an input attachment in a framebuffer.");
		}
		if (_bits & vk::ImageUsageFlagBits::eFragmentDensityMapEXT) {
			result.push_back("fragment density map: image can be used to create a VkImageView suitable \for use as a fragment density map image.");
		}
		if (_bits & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR) {
			result.push_back("fragment shading rate attachment: image can be used to create a VkImageView \suitable for use as a fragment shading rate attachment or shading rate image");
		}
		return result;
	}



	static std::vector<std::string> LogAlphaCompositeBits(vk::CompositeAlphaFlagsKHR _bits) 
	{
		std::vector<std::string> result;

		/*
			typedef enum VkCompositeAlphaFlagBitsKHR {
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR = 0x00000001,
				VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR = 0x00000002,
				VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR = 0x00000004,
				VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR = 0x00000008,
			} VkCompositeAlphaFlagBitsKHR;
		*/
		if (_bits & vk::CompositeAlphaFlagBitsKHR::eOpaque) {
			result.push_back("opaque (alpha ignored)");
		}
		if (_bits & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) {
			result.push_back("pre multiplied (alpha expected to already be multiplied in image)");
		}
		if (_bits & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) {
			result.push_back("post multiplied (alpha will be applied during composition)");
		}
		if (_bits & vk::CompositeAlphaFlagBitsKHR::eInherit) {
			result.push_back("inherited");
		}

		return result;
	}


}