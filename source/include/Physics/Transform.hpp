#pragma once
#include "Core/ECS/IRegisterComponent.hpp"



class Transform : public IRegisterComponent<Transform>
{
public:

	Vector3 pos = Vector3::Zero();
	Vector3 rotation = Vector3::Zero();
	Vector3 scale = Vector3::One();

	Matrix4X4 Local = Matrix4X4::Identity();
	Matrix4X4 Global = Matrix4X4::Identity();

	Transform() {}
	~Transform() {}


	Quaternion& GetQuatrion() 
	{
		return quat;
	}

	const Quaternion& GetQuatrion() const 
	{
		return quat;
	}

private:
	Quaternion quat = Quaternion::Identity();
};


