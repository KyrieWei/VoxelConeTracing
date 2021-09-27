#include "Renderer.h"

void Renderer::init(Scene& scene)
{
	render_shader = Shader("source/shaders/render_vert.vs", "source/shaders/render_frag.fs");
	voxelize_shader = Shader("source/shaders/voxelize_vert.vs", "source/shaders/voxelize_frag.fs");
	voxelVisualize_shader = Shader("source/shaders/voxel_visualize_vert.vs", "source/shaders/voxel_visualize_frag.fs"); //, "source/shaders/voxel_visualize_geom.gs");
}

void Renderer::initVoxelization(Scene& scene)
{
	//voxelize init
	texture3D = std::vector<GLfloat>(4 * voxelTextureSize.x * voxelTextureSize.y * voxelTextureSize.z, 0.0f);
	voxelTexture = new Texture3D(texture3D, voxelTextureSize.x, voxelTextureSize.y, voxelTextureSize.z, true);

	glm::vec3 min = scene.bb_min, max = scene.bb_max;
	glm::vec3 range(max.x - min.x, max.y - min.y, max.z - min.z);

	float offset = 0.2f;
	glm::vec3 cameraPosZ;
	cameraPosZ.x = (min.x + max.x) * 0.5f;
	cameraPosZ.y = (min.y + max.y) * 0.5f;
	cameraPosZ.z = max.z + offset;

	voxelCamera = Camera(cameraPosZ);
	voxelCamera.setOrthographicProject(-range.x * 0.51f, range.x * 0.51f, -range.y * 0.51f, range.y * 0.51f, 0.1f, range.z * 1.2f + offset);
}

void Renderer::voxelize(Scene& scene)
{
	glm::vec3 range(scene.bb_max - scene.bb_min);

	GLfloat clearColor[4] = { 0, 0, 0, 0 };
	voxelTexture->Clear(clearColor);

	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	voxelize_shader.use();
	voxelize_shader.setVec3("boxMin", scene.bb_min);
	voxelize_shader.setMat4("view", voxelCamera.GetViewMatrix());
	voxelize_shader.setMat4("projection", voxelCamera.GetProjectMatrix());

	glm::vec3 scale = glm::vec3((float)voxelTextureSize.x / range.x, (float)voxelTextureSize.y / range.y, (float)voxelTextureSize.z / range.z);
	voxelize_shader.setVec3("scale", scale);

	voxelTexture->Activate(voxelize_shader, "texture3D", 0);
	glBindImageTexture(0, voxelTexture->textureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);

	scene.draw(voxelize_shader);
}

void Renderer::initVoxelVisualization(Scene& scene)
{
	glm::vec3 min_pos = scene.bb_min;
	glm::vec3 range = scene.bb_max - scene.bb_min;
	GLuint size = voxelTextureSize.x * voxelTextureSize.y * voxelTextureSize.z * 4;

	std::unique_ptr<float[]> pixels(new float[size * 4]);
	glActiveTexture(GL_TEXTURE0);
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, pixels.get());

	glm::vec3 scale = glm::vec3(range.x / (float)voxelTextureSize.x, range.y / (float)voxelTextureSize.y, range.z / (float)voxelTextureSize.z);

	for (int i = 3; i < size; i += 4)
	{
		if (pixels[i] != 0.0f)
		{
			int xy = voxelTextureSize.x * voxelTextureSize.y;
			int index = (i + 1) / 4;
			int iz = index / xy;
			int iy = (index % xy) / voxelTextureSize.x;
			int ix = (index % xy) % voxelTextureSize.x;

			glm::vec3 elem = min_pos + glm::vec3(float(ix) / voxelTextureSize.x * range.x, float(iy) / voxelTextureSize.y * range.y, float(iz) / voxelTextureSize.z * range.z);
			glm::mat4 matrix(1.0f);
			matrix = glm::translate(matrix, elem);
			matrix = glm::scale(matrix, scale);
			voxelMatrix.push_back(matrix);
		}
	}

	instanceCube.init();
	instanceCube.setInstanceMatrix(voxelMatrix, GL_STATIC_DRAW);
}

void Renderer::render(Scene& scene, RenderingMode renderMode)
{
	switch (renderMode)
	{
	case RenderingMode::VOXELIZATION_VISUALIZATION:
		voxelVisualization(scene);
		//voxelize(scene);
		break;
	case RenderingMode::VOXEL_CONE_TRACING:
		renderScene(scene);
		break;
	default:
		break;
	}
}

void Renderer::renderScene(Scene& scene)
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uploadCameraInfo(voxelCamera, render_shader);

	scene.draw(render_shader);
}

void Renderer::voxelVisualization(Scene& scene)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0)
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	voxelVisualize_shader.use();
	voxelVisualize_shader.setFloat("cubeScale", 0.01f);
	uploadCameraInfo(scene.camera, voxelVisualize_shader);

	instanceCube.Draw(voxelVisualize_shader, voxelMatrix.size());
}

void Renderer::endFrame()
{
	glDeleteVertexArrays(1, &voxel_VAO);
	glDeleteBuffers(1, &voxel_VBO);
}

void Renderer::uploadCameraInfo(const Camera& camera, Shader& shader)
{
	shader.use();

	shader.setMat4("model", glm::mat4(1.0));
	shader.setMat4("view", camera.GetViewMatrix());
	shader.setMat4("projection", camera.GetProjectMatrix());
}