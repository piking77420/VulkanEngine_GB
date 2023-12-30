#pragma once
#include<Core/Core.h>
#include "Physics/Transform.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Camera
{
public : 


	static  const Camera* GetMainCamera() { return mainCamera; }
	static  Camera* SetMainCamera() { return mainCamera; }

	static void UpdateMainCamera();

	Transform& SetTramsform() { return transform; }
	const Transform& GetTramsform() const { return transform; }


	static void MouseCallback(GLFWwindow* context, double _xpos, double _ypos);

	Camera() { transform.pos = Vector3(2, 2, 2); };
	~Camera() {};
		
	static inline float lastX;
	static inline float lastY;


	Vector3 front = Vector3::Forward();
	Vector3 right = Vector3::Right();
	Vector3 up = Vector3::Up();

	float mouseSesitivity = 0.5f;

	inline Matrix4X4 GetLookMatrix() const
	{
		Vector3 eye = transform.pos;
		Vector3 at = (transform.pos + Vector3::Forward());

		return Matrix4X4::LookAt(eye, transform.pos + front, up);
	}

	Matrix4X4 GetProjectionMatrix() const;
	


private : 
	static Camera* mainCamera;

	Transform transform;

	void CameraGetInput(float xInput, float yInput);

	void CameraRotation();
	
	float fov;
};

