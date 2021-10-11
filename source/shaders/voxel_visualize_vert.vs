#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;

out vec3 WorldPos;
out vec3 WorldNormal;

void main()
{
	WorldPos = vec3(instanceMatrix * vec4(aPos, 1.0));
	WorldNormal = normalize(mat3(transpose(inverse(instanceMatrix))) * aNormal);
	gl_Position = projection * view * vec4(WorldPos, 1.0);
}