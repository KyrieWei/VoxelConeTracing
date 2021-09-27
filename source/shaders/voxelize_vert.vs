#version 450 core

layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;

void main()
{
	FragPos = aPos;
	gl_Position = projection * view * vec4(aPos, 1.0);
}