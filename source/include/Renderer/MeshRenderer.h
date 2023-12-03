#pragma once
#include "vulkan/vulkan.hpp"
#include "Core/ECS/IRegisterComponent.hpp"
#include "Resource/Model.hpp"


class MeshRenderer : public IRegisterComponent<MeshRenderer>
{
public:
	Model* model = nullptr;
};

