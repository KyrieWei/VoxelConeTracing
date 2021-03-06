#include "Renderer.h"

void Renderer::init(Scene& scene)
{
	render_shader = Shader("source/shaders/render_vert.vs", "source/shaders/render_frag.fs");
	voxelize_shader = Shader("source/shaders/voxelize_vert.vs", "source/shaders/voxelize_frag.fs", "source/shaders/voxelize_geom.gs");
	voxelVisualize_shader = Shader("source/shaders/voxel_visualize_vert.vs", "source/shaders/voxel_visualize_frag.fs");
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
	Camera cameraZ(cameraPosZ);
	cameraZ.setOrthographicProject(-range.x * 0.51f, range.x * 0.51f, -range.y * 0.51f, range.y * 0.51f, 0.1f, range.z * 1.2f + offset);

	glm::vec3 cameraPosX;
	cameraPosX.x = max.x + offset;
	cameraPosX.y = (min.y + max.y) * 0.5f;
	cameraPosX.z = (min.z + max.z) * 0.5f;
	Camera cameraX(cameraPosX);
	cameraX.setOrthographicProject(-range.z * 0.51f, range.z * 0.51f, -range.y * 0.51f, range.y * 0.51f, 0.1f, range.x * 1.2f + offset);

	glm::vec3 cameraPosY;
	cameraPosY.x = (min.x + max.x) * 0.5f;
	cameraPosY.y = max.y + offset;
	cameraPosY.z = (min.z + max.z) * 0.5f;
	Camera cameraY(cameraPosY);
	cameraY.setOrthographicProject(-range.x * 0.51f, range.x * 0.51f, -range.z * 0.51f, range.z * 0.51f, 0.1f, range.y * 1.2f + offset);

	voxelize_shader.use();
	voxelize_shader.setMat4("viewProject[0]", cameraX.GetProjectMatrix() * cameraX.GetViewMatrix(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	voxelize_shader.setMat4("viewProject[1]", cameraY.GetProjectMatrix() * cameraY.GetViewMatrix(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.001f)));
	voxelize_shader.setMat4("viewProject[2]", cameraZ.GetProjectMatrix() * cameraZ.GetViewMatrix(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

}

void Renderer::voxelize(Scene& scene)
{
	glm::vec3 range(scene.bb_max - scene.bb_min);

	GLfloat clearColor[4] = { 0, 0, 0, 0 };
	voxelTexture->Clear(clearColor);

	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	voxelize_shader.use();
	voxelize_shader.setVec3("boxMin", scene.bb_min);

	glm::vec3 scale = glm::vec3((float)voxelTextureSize.x / range.x, (float)voxelTextureSize.y / range.y, (float)voxelTextureSize.z / range.z);
	voxelize_shader.setVec3("scale", scale);

	uploadLightInfo(scene, voxelize_shader);

	voxelize_shader.setVec3("cameraPosition", scene.camera.Position);

	voxelTexture->Activate(voxelize_shader, "texture3D", 0);
	glBindImageTexture(0, voxelTexture->textureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);

	scene.draw(voxelize_shader);

	glGenerateTextureMipmap(voxelTexture->textureID);
}

void Renderer::initVoxelVisualization(Scene& scene)
{
	glm::vec3 min_pos = scene.bb_min;
	glm::vec3 range = scene.bb_max - scene.bb_min;
	GLuint size = voxelTextureSize.x * voxelTextureSize.y * voxelTextureSize.z * 4;

	std::unique_ptr<float[]> pixels(new float[size]);
	glActiveTexture(GL_TEXTURE0);
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, pixels.get());

	glm::vec3 scale = glm::vec3(range.x / (float)voxelTextureSize.x, range.y / (float)voxelTextureSize.y, range.z / (float)voxelTextureSize.z);

	//get values of voxelTexture
	for (int i = 3; i < size; i += 4)
	{
		if (pixels[i] != 0.0f)
		{
			//be careful of the linear voxelTexture coord to 3D coord
			int xy = voxelTextureSize.x * voxelTextureSize.y;
			int index = (i + 1) / 4;
			int iz = (index - 1) / xy;
			int xy_num = index - iz * xy;
			int iy = (xy_num - 1) / voxelTextureSize.x;
			int ix = xy_num - iy * voxelTextureSize.x - 1;

			//calculate the instance matrix of 3D coord from voxelTexture
			glm::vec3 elem = min_pos + glm::vec3(float(ix) / voxelTextureSize.x * range.x, float(iy) / voxelTextureSize.y * range.y, float(iz) / voxelTextureSize.z * range.z);
			glm::mat4 matrix(1.0f);
			matrix = glm::translate(matrix, elem);
			matrix = glm::scale(matrix, scale);
			voxelMatrix.push_back(matrix);
		}
	}

	if (voxelMatrix.empty())
	{
		std::cout << "no voxel point!" << std::endl;
		return;
	}
	else
	{
		std::cout << "voxel points: " << voxelMatrix.size() << std::endl;
	}

	instanceCube.init();
	instanceCube.setInstanceMatrix(voxelMatrix, GL_STATIC_DRAW);

	scale = glm::vec3(1.0f / range.x, 1.0f / range.y, 1.0f / range.z);

	voxelVisualize_shader.use();
	voxelVisualize_shader.setVec3("boxMin", scene.bb_min);
	voxelVisualize_shader.setVec3("scale", scale);
	voxelVisualize_shader.setVec3("dim", voxelTextureSize);
}

void Renderer::render(Scene& scene, RenderingMode renderMode)
{
	switch (renderMode)
	{
	case RenderingMode::VOXELIZATION_VISUALIZATION:
		voxelVisualization(scene);
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
	//glEnable(GL_CULL_FACE);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uploadCameraInfo(scene.camera, render_shader);
	uploadLightInfo(scene, render_shader);
	setRenderSetting(render_shader);

	voxelTexture->Activate(render_shader, "texture3D", 0);

	glm::vec3 range(scene.bb_max - scene.bb_min);
	glm::vec3 scale = glm::vec3(1.0f / range.x, 1.0f / range.y, 1.0f / range.z);

	render_shader.use();
	render_shader.setVec3("boxMin", scene.bb_min);
	render_shader.setVec3("scale", scale);
	render_shader.setFloat("VOXEL_SIZE", 1.0f / voxelTextureSize.x);

	scene.draw(render_shader);
}

void Renderer::voxelVisualization(Scene& scene)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0)
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uploadCameraInfo(scene.camera, voxelVisualize_shader);

	voxelTexture->Activate(voxelVisualize_shader, "texture3D", 0);

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

	shader.setMat4("view", camera.GetViewMatrix());
	shader.setMat4("projection", camera.GetProjectMatrix());
	shader.setVec3("cameraPosition", camera.Position);
}

void Renderer::uploadLightInfo(Scene& scene, Shader& shader)
{
	for (int i = 0; i < scene.pointLights.size(); i++)
		scene.pointLights[i].Upload(shader, i);

	shader.setInt("numberOfLights", scene.pointLights.size());
}

void Renderer::setRenderSetting(Shader& shader)
{
	shader.use();
	shader.setBool("settings.indirectDiffuseLight", indirectDiffuseLight);
	shader.setBool("settings.indirectSpecularLight", indirectSpecularLight);
	shader.setBool("settings.directLight", directLight);

}