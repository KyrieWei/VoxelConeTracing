#pragma once
#include "../tools/Model.h"
#include "../tools/Camera.h"


class Scene
{
public:
	Camera camera;
	std::vector<Model> models;
	glm::vec3 bb_min, bb_max;

	virtual void init(int viewportWidth, int viewportHeight) = 0;
	virtual void draw(Shader&) = 0;
};

