#include "Core/ECS/IRegisterComponent.hpp"



class Transform : public IRegisterComponent<Transform>
{
public:

	Vector3 pos;
	Quaternion rotation;
	Vector3 scale;

	Transform(){}
	~Transform(){}

	Matrix4X4 Local;
	Matrix4X4 Global;

private:
	
};

