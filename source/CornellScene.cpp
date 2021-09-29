#include "CornellScene.h"

void CornellScene::init(int viewportwidth, int viewportheight)
{
	camera.setInitialStatus(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 270, 0);
	camera.setPerspectiveProject((float)viewportwidth / (float)viewportheight, 0.1f, 100.0f);
	
	Model cornellBox("source/assets/cornell.obj");

	//Cornel box
	cornellBox.meshes[0].materialSetting = MaterialSetting(glm::vec3(0.35f, 0.38f, 1.0f));   //Green wall
	cornellBox.meshes[1].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //Floor
	cornellBox.meshes[2].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //Roof
	cornellBox.meshes[3].materialSetting = MaterialSetting(glm::vec3(1.0f, 0.26f, 0.27f));   //Red wall
	cornellBox.meshes[4].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //white wall
	cornellBox.meshes[5].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //left box
	cornellBox.meshes[6].materialSetting = MaterialSetting(glm::vec3(0.97f, 0.97f, 0.97f));  //right box
	cornellBox.useMaterialSetting = true;

	models.push_back(cornellBox);

	Model dragon("source/assets/dragon.obj");
	dragon.meshes[0].materialSetting = MaterialSetting(glm::vec3(0.95f, 1.0f, 0.95f));
	dragon.meshes[0].materialSetting.specularColor = glm::vec3(0.95f, 1.0f, 0.95f);
	dragon.meshes[0].materialSetting.emissivity = 0.00f;
	dragon.meshes[0].materialSetting.transparency = 0.00f;
	dragon.meshes[0].materialSetting.refractiveIndex = 1.18f;
	dragon.meshes[0].materialSetting.specularReflectivity = 1.00f;
	dragon.meshes[0].materialSetting.diffuseReflectivity = 0.0f;
	dragon.meshes[0].materialSetting.specularDiffusion = 2.0f;

	dragon.useMaterialSetting = true;
	dragon.transform.rotation = glm::vec3(0.0f, 90.0f, 0.0f);
	dragon.transform.position = glm::vec3(-0.4f, -0.7f, 0.4f);
	dragon.transform.updateTransformMatrix();

	models.push_back(dragon);

	Model lightSphere("source/assets/sphere.obj");
	lightSphere.meshes[0].materialSetting = MaterialSetting(glm::vec3(1.0f, 1.0f, 1.0f), 8.0f, 0.0f, 0.0f);
	lightSphere.useMaterialSetting = true;

	lightSphere.transform.position = glm::vec3(0, 0.5, 0.1);
	lightSphere.transform.position.x *= 4.5f;
	lightSphere.transform.position.z *= 4.5f;

	lightSphere.transform.scale = glm::vec3(0.05f);
	lightSphere.transform.updateTransformMatrix();
	lightSphere.updateBBox();

	models.push_back(lightSphere);

	calculate_bbox();

	PointLight p;
	p.color = normalize(glm::vec3(1.4f, 0.9f, 0.35f));
	p.position = lightSphere.transform.position;
	pointLights.push_back(p);
	
}

void CornellScene::update(float t)
{
	//light sphere
	glm::vec3 r = glm::vec3(sinf(float(t * 0.97)), sinf(float(t * 0.45)), sinf(float(t * 0.32)));

	int index = models.size() - 1;
	models[index].transform.position = glm::vec3(0, 0.5, 0.1) + r * 0.1f;
	models[index].transform.position.x *= 4.5f;
	models[index].transform.position.z *= 4.5f;

	models[index].transform.scale = glm::vec3(0.05f);
	models[index].transform.updateTransformMatrix();
	models[index].updateBBox();

	pointLights[0].position = models[index].transform.position;

	//update bounding box
	calculate_bbox();
}

void CornellScene::draw(Shader& shader)
{
	for (auto& elem : models)
		elem.Draw(shader);
}

void CornellScene::calculate_bbox()
{
	if (models.empty())
		return;

	bb_min = models[0].m_min;
	bb_max = models[0].m_max;

	for (int i = 1; i < models.size(); i++)
	{
		if (bb_min.x > models[i].m_min.x)
			bb_min.x = models[i].m_min.x;
		if (bb_min.y > models[i].m_min.y)
			bb_min.y = models[i].m_min.y;
		if (bb_min.z > models[i].m_min.z)
			bb_min.z = models[i].m_min.z;
	
		if (bb_max.x < models[i].m_max.x)
			bb_max.x = models[i].m_max.x;
		if (bb_max.y < models[i].m_max.y)
			bb_max.y = models[i].m_max.y;
		if (bb_max.z < models[i].m_max.z)
			bb_max.z = models[i].m_max.z;
	}
}

