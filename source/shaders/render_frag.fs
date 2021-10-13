#version 450 core

out vec4 fragColor;

#define TSQRT2 2.828427
#define SQRT2 1.414213
#define ISQRT2 0.707106

#define MIPMAP_HARDCAP 5.4f //Too high mipmap levels => glitchiness, too low mipmap levels => sharpness
#define DIFFUSE_INDIRECT_FACTOR 0.52f //Just changes intensity of diffuse indirect lighting

//light settings
#define DIRECT_LIGHT_INTENSITY 0.96f
#define MAX_LIGHTS 1

//lighting attenuation factors
#define DIST_FACTOR 1.1f
#define CONSTANT 1
#define LINEAR 0
#define QUADRATIC 1

#define GAMMA_CORRECTION 1

struct PointLight
{
    vec3 position;
    vec3 color;
};

struct Material
{
    vec3 diffuseColor;
    vec3 specularColor;
    float specularDiffusion;
    float diffuseReflectivity;
    float specularReflectivity;
    float emissivity;
    float refractiveIndex;
    float transparency;
};

struct Settings
{
    bool indirectSpecularLight;
    bool indirectDiffuseLight;
    bool directLight;
};

uniform Material material;
uniform Settings settings;
uniform PointLight pointLights[MAX_LIGHTS];
uniform vec3 cameraPosition;
uniform int numberOfLights;

uniform vec3 boxMin;
uniform vec3 scale;
uniform float VOXEL_SIZE;
uniform sampler3D texture3D;

in vec3 worldPos;
in vec3 worldNormal;


vec3 normal = normalize(worldNormal);
float MAX_DISTANCE = distance(vec3(abs(worldPos)), vec3(-1));

//return a vector that is orghogonal to u
vec3 orthogonal(vec3 u)
{
    u = normalize(u);
    vec3 v = vec3(0.99146, 0.11664, 0.05832); // pick any normalized vector
    return abs(dot(u, v)) > 0.99999f ? cross(u, vec3(0, 1, 0)) : cross(u, v);
}

//trace a diffuse voxel cone
vec3 traceDiffuseVoxelCone(vec3 from, vec3 direction)
{
    direction = normalize(direction);

    float CONE_SPREAD = 0.325f;
    
    vec4 acc = vec4(0.0f);

    // controls bleeding from close surfaces
    // Low values look rather bad if using shadow cone tracing.
    // Might be a better choice to use shadow maps and lower this value.
    float dist = 0.1953125;

    //trace
    while(dist < SQRT2 && acc.a < 1)
    {
        vec3 c = from + dist * direction;
        vec3 curr = c - boxMin;
        vec3 coord = curr * scale;

        coord.x = clamp(coord.x, 0, 1);
        coord.y = clamp(coord.y, 0, 1);
        coord.z = clamp(coord.z, 0, 1);

        float l = (1 + CONE_SPREAD * dist / VOXEL_SIZE);
        float level = log2(l);
        float ll = (level + 1) * (level + 1);
        vec4 voxel = textureLod(texture3D, coord, min(MIPMAP_HARDCAP, level));
        acc += 0.75 * ll * voxel * pow(1 - voxel.a, 2); 
        dist += ll * VOXEL_SIZE * 2;
    }

    return pow(acc.rgb * 2.0, vec3(1.5));
}

//trace a specular voxel cone
vec3 traceSpecularVoxelCone(vec3 from, vec3 direction)
{
    direction = normalize(direction);

    float OFFSET = 8 * VOXEL_SIZE;
    float STEP = VOXEL_SIZE;

    from += OFFSET * normal;

    vec4 acc = vec4(0.0f);
    float dist = OFFSET;

    //trace
    while(dist < MAX_DISTANCE && acc.a < 1)
    {
        vec3 c = from + dist * direction;
        vec3 curr = c - boxMin;
        vec3 coord = curr * scale;

        coord.x = clamp(coord.x, 0, 1);
        coord.y = clamp(coord.y, 0, 1);
        coord.z = clamp(coord.z, 0, 1);

        float level = 0.1 * material.specularDiffusion * log2(1 + dist / VOXEL_SIZE);
        vec4 voxel = textureLod(texture3D, coord, min(MIPMAP_HARDCAP, level));
        float f = 1 - acc.a;
        acc.rgb += 0.25 * (1 + material.specularDiffusion) * voxel.rgb * voxel.a * f;
        acc.a += 0.25 * voxel.a * f;
        dist += STEP * (1.0f + 0.125f * level);
    }

    return 1.0 * pow(material.specularDiffusion + 1, 0.8) * acc.rgb;
}

//calculate indirect diffuse light using voxel cone tracing.
//the current implementation uses 9 cones.
vec3 indirectDiffuseLight()
{
    float ANGLE_MIX = 0.5f; // angle mix (1.0f == orthogonal direction, 0.0f == normal direction)

    float w[3] = {1.0, 1.0, 1.0}; // cone weights

    //find a base for the side cones with the normal as one of its base vectors
    vec3 ortho = normalize(orthogonal(normal));
    vec3 ortho2 = normalize(cross(ortho, normal));

    //find base vectors for the corner cones too
    vec3 corner = 0.5f * (ortho + ortho2);
    vec3 corner2 = 0.5f * (ortho - ortho2);

    //find start position of trace (start with a bit of offset)
    vec3 N_OFFSET = normal * (1 + 4 * ISQRT2) * VOXEL_SIZE;
    vec3 C_ORIGIN = worldPos + N_OFFSET;
    
    //accumulate indirect diffuse light
    vec3 acc = vec3(0.0f);

    // We offset forward in normal direction, and backward in cone direction.
    // Backward in cone direction improves GI, and forward direction removes
    // artifacts.
    float CONE_OFFSET = -0.01;

    //trace front cone
    acc += w[0] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * normal, normal);

    //trace four side cones
    vec3 s1 = mix(normal, ortho, ANGLE_MIX);
    vec3 s2 = mix(normal, -ortho, ANGLE_MIX);
    vec3 s3 = mix(normal, ortho2, ANGLE_MIX);
    vec3 s4 = mix(normal, -ortho2, ANGLE_MIX);

    acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * ortho, s1);
    acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * ortho, s2);
    acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * ortho2, s3);
    acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * ortho2, s4);

    //trace four corner cones
    vec3 c1 = mix(normal, corner, ANGLE_MIX);
    vec3 c2 = mix(normal, -corner, ANGLE_MIX);
    vec3 c3 = mix(normal, corner2, ANGLE_MIX);
    vec3 c4 = mix(normal, -corner2, ANGLE_MIX);

    acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * corner, c1);
    acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * corner, c2);
    acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * corner2, c3);
    acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * corner2, c4);

    //return result
    return DIFFUSE_INDIRECT_FACTOR * material.diffuseReflectivity * acc * (material.diffuseColor + vec3(0.001f));
}

//calculate indirect specular light using voxel cone tracing
vec3 indirectSpecularLight(vec3 viewDirection)
{
    vec3 reflection = normalize(reflect(viewDirection, normal));
    return material.specularReflectivity * material.specularColor * traceSpecularVoxelCone(worldPos, reflection);
}

//calculate refractive light using voxel cone tracing
vec3 indirectRefractiveLight(vec3 viewDirection)
{
    vec3 refraction = refract(viewDirection, normal, 1.0 / material.refractiveIndex);
    vec3 cmix = mix(material.specularColor, 0.5 * (material.specularColor + vec3(1)), material.transparency);
    return cmix * traceSpecularVoxelCone(worldPos, refraction);
}

float attenuate(float dist)
{
    dist *= DIST_FACTOR;
    return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist);
}

vec3 calculateDirectLight(const PointLight light, const vec3 viewDirection)
{
    vec3 lightDirection = light.position - worldPos;
    float distanceToLight = length(lightDirection);
    lightDirection = normalize(lightDirection);
    float lightAngle = dot(normal, lightDirection);

    //diffuse light
    float diffuseAngle = max(lightAngle, 0.0f);
    float diffuse = diffuseAngle;
    vec3 diff = material.diffuseColor * diffuse;

    vec3 total = diff;

    vec3 dirLight = total * attenuate(distanceToLight);

    return dirLight;
}

vec3 directLight(vec3 viewDirection)
{
    vec3 direct = vec3(0.0f);
    int maxLights = min(numberOfLights, MAX_LIGHTS);
    for(int i = 0; i < maxLights; i ++)
        direct += calculateDirectLight(pointLights[i], viewDirection);

    direct *= DIRECT_LIGHT_INTENSITY;
    return direct;
}

void main()
{
    vec4 color = vec4(0, 0, 0, 0);
    vec3 viewDirection = normalize(worldPos - cameraPosition);

    //Indirect diffuse light
    if(settings.indirectDiffuseLight && material.diffuseReflectivity * (1.0f - material.transparency) > 0.01f)
        color.rgb += indirectDiffuseLight();

    //Indirect specular light
    if(settings.indirectSpecularLight && material.specularReflectivity * (1.0f - material.transparency) > 0.01f)
        color.rgb += indirectSpecularLight(viewDirection);

    //Indirect Transparency
    if(material.transparency > 0.01f)
        color.rgb = mix(color.rgb, indirectRefractiveLight(viewDirection), material.transparency);

    //Emissivity
    color.rgb += material.emissivity * material.diffuseColor;

    //Direct light
    if(settings.directLight)
        color.rgb += directLight(viewDirection);

    fragColor = color;
}