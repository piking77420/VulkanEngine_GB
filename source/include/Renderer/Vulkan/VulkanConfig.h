#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	
#include "Core/Core.h"	
#include <vulkan/vulkan.hpp>

static const constinit std::uint32_t MAX_FRAMES_IN_FLIGHT = 2;


#ifdef NDEBUG
static const constinit bool EnableValidationLayers{ false };
#else
static const constinit bool EnableValidationLayers{ true };
#endif

struct UniformBufferObject {
    //Matrix4X4 model;
    Matrix4X4 view;
    Matrix4X4 proj;
};

