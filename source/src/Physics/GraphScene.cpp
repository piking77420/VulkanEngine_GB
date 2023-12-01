#include "Physics/GraphScene.h"
#include "Core/ECS/Scene.hpp"

void GraphScene::Begin()
{

}

void GraphScene::Update(Scene* scene)
{
	std::vector<Transform>* data = scene->GetComponentData<Transform>();


	for (std::vector<Transform>::iterator it = data->begin(); it != data->end(); it++)
	{
		it->Local = Matrix4X4::TRS(it->pos, it->rotation, it->scale);
		it->Global = it->Local;
	}

}


void GraphScene::FixedUpdate()
{

}
