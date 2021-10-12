#version 450 core

in vec3 WorldPos;
in vec3 WorldNormal;

out vec4 fragColor;

uniform sampler3D texture3D;

uniform vec3 boxMin;
uniform vec3 scale;

void main()
{
    
    vec4 color = vec4(0.0f);

    vec3 curr = WorldPos - boxMin;
    vec3 coord = vec3(curr * scale);

    coord.x = clamp(coord.x, 0, 1);
    coord.y = clamp(coord.y, 0, 1);
    coord.z = clamp(coord.z, 0, 1);
    
    vec4 textureColor = textureLod(texture3D, coord, 0);

    fragColor = textureColor;
}