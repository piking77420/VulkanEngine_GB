#pragma once
#include "vulkan/vulkan.hpp"
#include "Core/ECS/IRegisterComponent.hpp"
#include "Resource/Model.hpp"
#include "Resource/Material.hpp"

class MeshRenderer : public IRegisterComponent<MeshRenderer>
{
public:
	const Model const* model = nullptr;
	const Material const* material = nullptr;

	MeshRenderer()
	{

	}

};

