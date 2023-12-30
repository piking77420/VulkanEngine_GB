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

	VkBuffer GetBuffer() const
	{
		return m_VertexBuffer;
	}

	void BindVBO() const;
	


	void LoadVBO(const std::vector<Vertex>& verticies);
	void FreeVBO();

	void operator=(const VBO& vbo);

private:

	VkBuffer m_VertexBuffer = nullptr;
	VkDeviceMemory m_VertexBufferMemory = nullptr;
};


