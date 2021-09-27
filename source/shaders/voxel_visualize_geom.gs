#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float cubeScale;

void main()
{
	mat4 mvp = projection * view * model;

	const vec4 cubeVertices[8] = vec4[8] 
	(
		vec4( 0.5f,  0.5f,  0.5f, 0.0f),
		vec4( 0.5f,  0.5f, -0.5f, 0.0f),
		vec4( 0.5f, -0.5f,  0.5f, 0.0f),
		vec4( 0.5f, -0.5f, -0.5f, 0.0f),
		vec4(-0.5f,  0.5f,  0.5f, 0.0f),
		vec4(-0.5f,  0.5f, -0.5f, 0.0f),
		vec4(-0.5f, -0.5f,  0.5f, 0.0f),
		vec4(-0.5f, -0.5f, -0.5f, 0.0f)
	);

	const int cubeIndices[24]  = int[24] 
	(
		0, 2, 1, 3, // right
		6, 4, 7, 5, // left
		5, 4, 1, 0, // up
		6, 7, 2, 3, // down
		4, 6, 0, 2, // front
		1, 3, 5, 7  // back
	);

	for(int face = 0; face < 6; face ++)
	{
		for(int vert = 0; vert < 4; vert ++)
		{
			int cube_indices = cubeIndices[face * 4 + vert];
			vec4 vertex = gl_in[0].gl_Position + cubeVertices[cube_indices] * cubeScale;
			gl_Position = mvp * vertex;
			EmitVertex();
		}

		EndPrimitive();
	}
}