#pragma once
#include "config.hpp"


namespace vkUtil
{
	static std::vector<char> ReadFile(std::string _filename) 
	{
		std::ifstream file(_filename, std::ios::ate | std::ios::binary);

		if(!file.is_open()) 
		{
			CORE_LOG_ERROR(_filename + " Is not Open");
			assert(false);

		}

		size_t fileSize{ static_cast<size_t>(file.tellg()) };

		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		
		return buffer;

	}

	static vk::ShaderModule CreateShaderModule(std::string filename, vk::Device _device)
	{
		std::vector<char> source = ReadFile(filename);
		vk::ShaderModuleCreateInfo moduleInfo = { };
		moduleInfo.flags = vk::ShaderModuleCreateFlags();
		moduleInfo.codeSize = source.size();

	}


}
