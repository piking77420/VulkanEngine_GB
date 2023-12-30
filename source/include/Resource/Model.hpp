#pragma once
//#include "Renderer/Vulkan/Vertex.hpp"
#include "Resource/ResourceManager.hpp" 
#include "Renderer/Vulkan/EBO.hpp"
#include "Renderer/Vulkan/VBO.hpp"
#include "Renderer/Vulkan/Vertex.hpp"

class Model : public IRegisterResource<Model>
{
public:


	virtual void LoadResource(const std::string& path) override;

	virtual void Destroy() override ;

	virtual void OnChange() override;

	Model() {};
	~Model() {};

	EBO Ebo;
	VBO Vbo;


	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;	
private:
	void LoadObj(const std::string& path);
	void LoadGltf(const std::string& path);


};

