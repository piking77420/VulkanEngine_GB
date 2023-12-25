#pragma once
#include "ResourceManager.hpp"


class Texture : public IRegisterResource<Texture>
{
public:
	virtual ~Texture() override 
	{
		
	}

	virtual void Destroy() override
	{
	}


	// Inherited via IRegisterResource
	void LoadResource(const std::string& path) override;

};

