#pragma once

#include "Mesh.h"
#include "Shader.h"

class Quad
{
public:
	std::vector<Mesh> meshes;

	Quad()
	{
		std::vector<Vertex> vertexs;

		Vertex v;

		v.Position = { -1, -1, 1 };
		v.TexCoords = { 0, 1 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		v.Position = { 1, -1, 1 };
		v.TexCoords = { 0, 0 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		v.Position = { 1, 1, 1 };
		v.TexCoords = { 1, 1 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		v.Position = { -1, 1, 1 };
		v.TexCoords = { 1, 0 };
		v.Normal = { 0, 0, 1 };
		vertexs.push_back(v);

		std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
		std::vector<Texture> textures;

		meshes.push_back(Mesh(vertexs, indices, textures));
	}

	void Draw(Shader& shader)
	{
		for (int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}
};


class InstanceCube
{
public:
	std::vector<Mesh> meshes;

	InstanceCube()
	{

	}

	void init(float width = 1.0f, float height = 1.0f, float depth = 1.0f)
	{
		float halfW = width * 0.5f;
		float halfH = height * 0.5f;
		float halfD = depth * 0.5f;

		std::vector<Vertex> vertexs;

		Vertex v;
		//v0
		v.Position = { -halfW, -halfH, -halfD };
		v.TexCoords = { 0.0f, 1.0f };
		v.Normal = { 0.0f, 0.0f, -1.0f };
		vertexs.push_back(v);

		//v1
		v.Position = { -halfW, +halfH, -halfD };
		v.TexCoords = { 0.0f, 0.0f };
		v.Normal = { 0.0f, 0.0f, -1.0f };
		vertexs.push_back(v);

		//v2
		v.Position = { +halfW, +halfH, -halfD };
		v.TexCoords = { 1.0f, 0.0f };
		v.Normal = { 0.0f, 0.0f, -1.0f };
		vertexs.push_back(v);

		//v3
		v.Position = { +halfW, -halfH, -halfD };
		v.TexCoords = { 1.0f, 1.0f };
		v.Normal = { 0.0f, 0.0f, -1.0f };
		vertexs.push_back(v);

		//v4
		v.Position = { -halfW, -halfH, +halfD };
		v.TexCoords = { 0.0f, 1.0f };
		v.Normal = { -1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v5
		v.Position = { -halfW, +halfH, +halfD };
		v.TexCoords = { 0.0f, 0.0f };
		v.Normal = { -1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v6
		v.Position = { -halfW, +halfH, -halfD };
		v.TexCoords = { 1.0f, 0.0f };
		v.Normal = { -1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v7
		v.Position = { -halfW, -halfH, -halfD };
		v.TexCoords = { 1.0f, 1.0f };
		v.Normal = { -1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v8
		v.Position = { +halfW, -halfH, +halfD };
		v.TexCoords = { 0.0f, 1.0f };
		v.Normal = { 0.0f, 0.0f, 1.0f };
		vertexs.push_back(v);

		//v9
		v.Position = { +halfW, +halfH, +halfD };
		v.TexCoords = { 0.0f, 0.0f };
		v.Normal = { 0.0f, 0.0f, 1.0f };
		vertexs.push_back(v);

		//v10
		v.Position = { -halfW, +halfH, +halfD };
		v.TexCoords = { 1.0f, 0.0f };
		v.Normal = { 0.0f, 0.0f, 1.0f };
		vertexs.push_back(v);

		//v11
		v.Position = { -halfW, -halfH, +halfD };
		v.TexCoords = { 1.0f, 1.0f };
		v.Normal = { 0.0f, 0.0f, 1.0f };
		vertexs.push_back(v);

		//v12
		v.Position = { +halfW, -halfH, -halfD };
		v.TexCoords = { 0.0f, 1.0f };
		v.Normal = { 1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v13
		v.Position = { +halfW, +halfH, -halfD };
		v.TexCoords = { 0.0f, 0.0f };
		v.Normal = { 1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v14
		v.Position = { +halfW, +halfH, +halfD };
		v.TexCoords = { 1.0f, 0.0f };
		v.Normal = { 1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v15
		v.Position = { +halfW, -halfH, +halfD };
		v.TexCoords = { 1.0f, 1.0f };
		v.Normal = { 1.0f, 0.0f, 0.0f };
		vertexs.push_back(v);

		//v16
		v.Position = { -halfW, +halfH, -halfD };
		v.TexCoords = { 0.0f, 1.0f };
		v.Normal = { 0.0f, 1.0f, 0.0f };
		vertexs.push_back(v);

		//v17
		v.Position = { -halfW, +halfH, +halfD };
		v.TexCoords = { 0.0f, 0.0f };
		v.Normal = { 0.0f, 1.0f, 0.0f };
		vertexs.push_back(v);

		//v18
		v.Position = { +halfW, +halfH, +halfD };
		v.TexCoords = { 1.0f, 0.0f };
		v.Normal = { 0.0f, 1.0f, 0.0f };
		vertexs.push_back(v);

		//v19
		v.Position = { +halfW, +halfH, -halfD };
		v.TexCoords = { 1.0f, 1.0f };
		v.Normal = { 0.0f, 1.0f, 0.0f };
		vertexs.push_back(v);

		//v20
		v.Position = { -halfW, -halfH, +halfD };
		v.TexCoords = { 0.0f, 1.0f };
		v.Normal = { 0.0f, -1.0f, 0.0f };
		vertexs.push_back(v);

		//v21
		v.Position = { -halfW, -halfH, -halfD };
		v.TexCoords = { 0.0f, 0.0f };
		v.Normal = { 0.0f, -1.0f, 0.0f };
		vertexs.push_back(v);

		//v22
		v.Position = { +halfW, -halfH, -halfD };
		v.TexCoords = { 1.0f, 0.0f };
		v.Normal = { 0.0f, -1.0f, 0.0f };
		vertexs.push_back(v);

		//v23
		v.Position = { +halfW, -halfH, +halfD };
		v.TexCoords = { 1.0f, 1.0f };
		v.Normal = { 0.0f, -1.0f, 0.0f };
		vertexs.push_back(v);

		std::vector<unsigned int> indices =
		{
			//! front.
			 0, 1, 2,
			 0, 2, 3,

			 //! back.
			 4, 5, 6,
			 4, 6, 7,

			 //! left.
			 8, 9,10,
			 8,10,11,

			 //! right.
			 12,13,14,
			 12,14,15,

			 //! up.
			 16,17,18,
			 16,18,19,

			 //! down.
			 20,21,22,
			 20,22,23
		};

		std::vector<Texture> textures;

		meshes.push_back(Mesh(vertexs, indices, textures));

		glGenBuffers(1, &instance_VBO);
	}

	void setInstanceMatrix(const std::vector<glm::mat4>& instanceMatrix, GLenum drawWay)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instance_VBO);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrix.size() * sizeof(glm::mat4), &instanceMatrix[0], drawWay);

		for (auto elem : meshes)
		{
			glBindVertexArray(elem.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, instance_VBO);

			GLsizei vec4Size = sizeof(glm::vec4);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	void Draw(Shader& shader, int amount)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			GLenum VAO = meshes[i].VAO;
			glBindVertexArray(VAO);
			glDrawElementsInstanced(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}
	}

private:
	unsigned int instance_VBO;

};