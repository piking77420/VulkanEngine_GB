#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	
#include "Core/Core.h"	
#include <vulkan/vulkan.hpp>



#ifdef NDEBUG
static const constinit bool EnableValidationLayers{ false };
#else
static const constinit bool EnableValidationLayers{ true };
#endif