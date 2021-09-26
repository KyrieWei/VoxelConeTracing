#pragma once
#include <string>
#include "../tools/Singleton.h"
#include "CornellScene.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>


class VCTSystem : public Singleton<VCTSystem>
{
private:
	int width, height;
	std::string title;
	GLFWwindow* window;

	CornellScene cornellscene;
	Renderer renderer;

public:
	typedef std::shared_ptr<VCTSystem> ptr;

	static float deltaTime, lastFrame;
	static float lastX, lastY;
	static bool firstMouse;

public:
	VCTSystem() = default;
	~VCTSystem() = default;

	void init();
	void run();
	void setWindowSize(const int _width, const int _height);
	void setTitle(const std::string& _title);

	static VCTSystem::ptr getSingleton();

protected:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void processInput(GLFWwindow* window, float deltaTime);
};

