#include "CornellScene.h"

void CornellScene::init(int viewportwidth, int viewportheight)
{
	camera.setPerspectiveProject((float)viewportwidth / (float)viewportheight, 0.1f, 100.0f);

	//Model dragon("source/assets/dragon.obj");
	Model sphere("source/assets/sphere.obj");

	//bb_min = dragon.m_min;
	//bb_max = dragon.m_max;

	bb_min = sphere.m_min;
	bb_max = sphere.m_max;

	//models.push_back(dragon);
	models.push_back(sphere);
}

void CornellScene::draw(Shader& shader)
{
	for (auto& elem : models)
		elem.Draw(shader);
}