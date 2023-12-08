#pragma once
#include<Core/Core.h>
#include "Physics/Transform.hpp"

class Camera
{
public : 


	static  const Camera* GetMainCamera() { return mainCamera; }
	static  Camera* SetMainCamera() { return mainCamera; }

	static void UpdateMainCamera();

	Transform& SetTramsform() { return transform; }
	const Transform& GetTramsform() const { return transform; }




	Camera() { transform.pos = Vector3(2, 2, 2); };
	~Camera() {};
private : 
	static Camera* mainCamera;

	Transform transform;



};

