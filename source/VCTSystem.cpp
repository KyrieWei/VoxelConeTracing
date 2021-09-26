#include <iostream>
#include <glad/glad.h>
#include "VCTSystem.h"

float VCTSystem::deltaTime = 0.0f;
float VCTSystem::lastFrame = 0.0f;
float VCTSystem::lastX = 0.0f;
float VCTSystem::lastY = 0.0f;
bool VCTSystem::firstMouse = true;
template<> VCTSystem::ptr Singleton<VCTSystem>::_instance = nullptr;


VCTSystem::ptr VCTSystem::getSingleton()
{
	if (!_instance)
		_instance = std::make_shared<VCTSystem>();
	return _instance;
}

void VCTSystem::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }


}

void VCTSystem::run()
{
    init();

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void VCTSystem::setWindowSize(const int _width, const int _height)
{
    width = _width;
    height = _height;
}

void VCTSystem::setTitle(const std::string& _title)
{
    title = _title;
}

void VCTSystem::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void VCTSystem::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //getSingleton()->getPBRenderer()->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

void VCTSystem::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //getSingleton()->getPBRenderer()->getCamera()->ProcessMouseScroll(yoffset);
}

void VCTSystem::processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera::ptr camera = getSingleton()->getPBRenderer()->getCamera();

    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //    camera->ProcessKeyboard(FORWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera->ProcessKeyboard(BACKWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    camera->ProcessKeyboard(LEFT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    camera->ProcessKeyboard(RIGHT, deltaTime);
}