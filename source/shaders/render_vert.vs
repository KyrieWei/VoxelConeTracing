#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldPos;
out vec3 worldNormal;

void main()
{
	worldPos = vec3(model * vec4(aPos, 1.0));
	worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);

	gl_Position = projection * view * vec4(worldPos, 1.0);
}