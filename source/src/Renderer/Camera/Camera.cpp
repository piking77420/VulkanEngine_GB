#include "Renderer/Camera/Camera.hpp"
#include <Imgui/imgui.h>

Camera* Camera::mainCamera = new Camera();

void Camera::UpdateMainCamera()
{
	Camera* cam = SetMainCamera();

	if (!cam)
		return;

	float deltatime = ImGui::GetIO().DeltaTime;

	Vector3& vec = cam->transform.pos;


	if(ImGui::IsKeyDown(ImGuiKey_A))
	{
		cam->transform.pos.x += 1 * deltatime;
	}
	if (ImGui::IsKeyDown(ImGuiKey_D))
	{
		cam->transform.pos.x -= 1 * deltatime;
	}

	if (ImGui::IsKeyDown(ImGuiKey_W))
	{
		cam->transform.pos.y += 1 * deltatime;
	}

	if (ImGui::IsKeyDown(ImGuiKey_S))
	{
		cam->transform.pos.y -= 1 * deltatime;
	}

	if (ImGui::IsKeyDown(ImGuiKey_Space))
	{
		cam->transform.pos.z += 1 * deltatime;
	}

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		cam->transform.pos.z -= 1 * deltatime;
	}




}
