#pragma once



class IResource
{
public:
	virtual ~IResource() {}
	virtual void Destroy() = 0 ;
	virtual void LoadResource(const std::string& path) = 0;

};


