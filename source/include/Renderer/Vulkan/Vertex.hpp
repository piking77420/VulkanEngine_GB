#pragma once
#include "VulkanConfig.h"


	class Vertex
	{
	public:
		Vector3 pos;
		Vector3 color;

		static VkVertexInputBindingDescription GetBindingDescription();

		constexpr Vertex(const Vector3& position, const Vector3& color)
			: pos(position), color(color)
		{
		}
		constexpr Vertex() = default;

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

	};





