#pragma once
#include "Scene.h"

class CornellScene : public Scene
{
public:
	virtual void init(int viewportWidth, int viewportHeight);
	virtual void draw(Shader& shader);
};

