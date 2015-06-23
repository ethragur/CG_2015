#version 330

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

const int numberOfLights = 3;


out vec3 FragPos;
out vec3 Normal;
out vec2 vUV;
out mat3 TBN;


//everything is done in world space
void main() 
{
    gl_Position = ProjectionMatrix* ViewMatrix * ModelMatrix * vec4(Position, 1);

    FragPos = vec3( ModelMatrix * vec4(Position, 1.0f));
    Normal = vec3( ModelMatrix * vec4(Normals, 1.0));
    
    vec3 FragTangent = vec3( ModelMatrix * vec4(normalize(tangents), 1.0));
    vec3 FragBiTangent = vec3( ModelMatrix * vec4(normalize(bitangents), 1.0));
    TBN =  (mat3 (FragTangent, FragBiTangent, Normal));

    vUV = UV;
}