#version 450 core

in vec3 FragPos;

uniform vec3 boxMin;
uniform vec3 scale;

out vec4 fragColor;

layout(RGBA8) uniform image3D texture3D;

void main()
{
    vec3 curr = FragPos - boxMin;
    ivec3 coord = ivec3(curr * scale);

    clamp(coord.x, 0, 64);
    clamp(coord.y, 0, 64);
    clamp(coord.z, 0, 64);

    vec4 res = vec4(0.2, 0.3, 0.5, 1.0);

    imageStore(texture3D, coord, res);

    fragColor = vec4(float(coord.x) / 64, float(coord.y) / 64, float(coord.z) / 64, 1.0);
}
