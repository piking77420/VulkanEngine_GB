#include "Renderer/Camera/Camera.hpp"


Camera* Camera::mainCamera = new Camera();

bool IskeyPress(const int& GLFWInput)
{
	/*
	static GLFWwindow* currentContext = VulkanRendererData::window;

	if (!currentContext)
		return false;

	if (glfwGetKey(currentContext, GLFWInput) == GLFW_PRESS)
	{
		return true;
	}
	return false;
	*/
	return false;

}


void Camera::UpdateMainCamera()
{
	Camera* cam = SetMainCamera();

	if (!cam)
		return;

	constexpr float deltatime = 0.003;

	Vector3& vec = cam->transform.pos;

	
	if(IskeyPress(GLFW_KEY_A))
	{
		cam->transform.pos -= mainCamera->right * deltatime;
	}
	if (IskeyPress(GLFW_KEY_D))
	{
		cam->transform.pos += mainCamera->right * deltatime;
	}

	if (IskeyPress(GLFW_KEY_W))
	{
		cam->transform.pos += mainCamera->front * deltatime;
	}

	if (IskeyPress(GLFW_KEY_S))
	{
		cam->transform.pos -= mainCamera->front * deltatime;
	}

	if (IskeyPress(GLFW_KEY_SPACE))
	{
		cam->transform.pos += mainCamera->up * deltatime;
	}

	if (IskeyPress(GLFW_KEY_LEFT_CONTROL))
	{
		cam->transform.pos -= mainCamera->up * deltatime;
	}




}

void Camera::MouseCallback(GLFWwindow* context, double _xpos, double _ypos)
{
		static bool firstmove = false;

		float xpos = static_cast<float>(_xpos);
		float ypos = static_cast<float>(_ypos);


		if (!firstmove)
		{
			lastX = xpos;
			lastY = ypos;
			firstmove = true;
		}

		float xoffset =  xpos - lastX;
		float yoffset = lastY - ypos ;



		lastX = xpos;
		lastY = ypos;

		if (glfwGetMouseButton(context, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			Camera::SetMainCamera()->CameraGetInput(xoffset, yoffset);
}

void Camera::CameraGetInput(float xInput, float yInput)
{
	xInput *= mouseSesitivity;
	yInput *= mouseSesitivity;

	transform.rotation.x += xInput;
	transform.rotation.y += yInput;



	if (transform.rotation.y > 89.0f)
		transform.rotation.y = 89.0f;
	if (transform.rotation.y < -89.0f)
		transform.rotation.y = -89.0f;

	CameraRotation();
}

void Camera::CameraRotation()
{

	Vector3 frontVector = Vector3::Zero();
	Vector3& rot = transform.rotation;

	frontVector.x = std::cos(Math::Deg2Rad * rot.x) * std::cos(Math::Deg2Rad * rot.y);
	frontVector.y = std::sin(Math::Deg2Rad * rot.y);
	frontVector.z = std::sin(Math::Deg2Rad * rot.x) * std::cos(Math::Deg2Rad * rot.y);

	front = frontVector.Normalize();

	right = Vector3::CrossProduct(front, Vector3::Up()).Normalize();
	up = Vector3::CrossProduct(right, front);


}