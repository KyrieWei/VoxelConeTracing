#pragma once
#include "../tools/Model.h"
#include "../tools/Camera.h"
#include "PointLight.h"


class Scene
{
public:
	Camera camera;
	std::vector<Model> models;
	std::vector<PointLight> pointLights;
	glm::vec3 bb_min, bb_max;

	virtual void init(int viewportWidth, int viewportHeight) = 0;
	virtual void draw(Shader&) = 0;
	virtual void calculate_bbox() = 0;
	virtual void update(float t) = 0;
};

