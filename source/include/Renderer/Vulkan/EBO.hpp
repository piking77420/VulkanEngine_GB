#pragma once
#include "VulkanConfig.h"

class Vertex;

class EBO
{
public:



	EBO() {};
	~EBO() {};


	VkBuffer GetBuffer() const
	{
		return m_IndexBuffer;
	}

	void BindEBO() const;

	void LoadEBO(const std::vector<std::uint32_t>& verticies);
	void LoadEBO(const std::vector<std::uint16_t>& verticies);

	void FreeEBO();

	void operator=(const EBO& vbo);


private:
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
};

