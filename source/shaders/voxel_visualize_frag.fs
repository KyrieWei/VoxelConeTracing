#version 450 core

in vec3 Normal;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(Normal);
    
    fragColor = vec4(normal, 1.0);
}