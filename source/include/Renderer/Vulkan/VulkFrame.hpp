
#include "Renderer/Vulkan/config.hpp"


namespace vkUtil
{
	struct SwapChainFrame
	{
		vk::Image images;
		vk::ImageView imageView;
	};
}