#pragma once
#include "Scene.h"
#include "Texture3D.h"
#include "../tools/standardModel.h"

class Renderer
{
public:
	enum RenderingMode
	{
		VOXELIZATION_VISUALIZATION = 0,
		VOXEL_CONE_TRACING = 1
	};

	void init(Scene& scene);
	void render(Scene& scene, RenderingMode renderMode = RenderingMode::VOXEL_CONE_TRACING);
	void renderScene(Scene& scene);
	void endFrame();

	void uploadCameraInfo(const Camera& camera, Shader& shader);
	void uploadLightInfo(Scene& scene, Shader& shader);
	void setRenderSetting(Shader& shader);
public:

	//shaders
	Shader render_shader;
	Shader voxelize_shader;
	Shader voxelVisualize_shader;

	//settings
	bool indirectDiffuseLight = true;
	bool indirectSpecularLight = true;
	bool directLight = true;

	//voxelization
	glm::uvec3 voxelTextureSize = glm::uvec3(64, 64, 64);
	std::vector<GLfloat> texture3D;
	Texture3D* voxelTexture = nullptr;
	Camera voxelCamera;

	void initVoxelization(Scene& scene);
	void voxelize(Scene& scene);

	//voxel visualization
	GLuint voxel_VAO, voxel_VBO;
	std::vector<glm::vec3> voxelPos;
	std::vector<glm::mat4> voxelMatrix;
	InstanceCube instanceCube;

	void initVoxelVisualization(Scene& scene);
	void voxelVisualization(Scene& scene);
};

