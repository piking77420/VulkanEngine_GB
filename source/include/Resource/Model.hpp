#pragma once
//#include "Renderer/Vulkan/Vertex.hpp"
#include "Resource/ResourceManager.hpp" 

class Model : public IRegisterResource<Model>
{
public:


	virtual void LoadResource(const std::string& path) override;

	virtual void Destroy() override ;


	Model() {};
	~Model() {};

	//std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;



};

