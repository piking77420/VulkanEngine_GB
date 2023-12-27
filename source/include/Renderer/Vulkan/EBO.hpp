#pragma once
#include "VulkanConfig.h"

class Vertex;

class EBO
{
public:



	EBO() {};
	~EBO() {};


	VkBuffer GetBuffer()
	{
		return m_IndexBuffer;
	}

	void LoadEBO(const std::vector<std::uint32_t>& verticies);
	void FreeEBO();

	void operator=(const EBO& vbo);


private:
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
};

