#pragma once
#include "VulkanConfig.h"

class Vertex;

class VBO
{
public:
	
	VBO() 
	{

	};
	~VBO() 
	{

	};

	VkBuffer GetBuffer()
	{
		return m_VertexBuffer;
	}

	void LoadVBO(const std::vector<Vertex>& verticies);
	void FreeVBO();

	void operator=(const VBO& vbo);

private:

	VkBuffer m_VertexBuffer = nullptr;
	VkDeviceMemory m_VertexBufferMemory = nullptr;
};


