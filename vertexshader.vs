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
uniform vec3 lightPos[numberOfLights];

//everything in ViewSpace
out vec3 FragPos;
out vec3 Normal;
out vec3 LightPosView[numberOfLights];
out vec2 vUV;
out mat3 TBN;

out vec3 LightDirection[numberOfLights];
out vec3 EyeDirection_tanSpace;

//everything is done in viewSpace
void main() 
{
    gl_Position = ProjectionMatrix* ViewMatrix * ModelMatrix * vec4(Position, 1);

    FragPos = vec3(ViewMatrix * ModelMatrix * vec4(Position, 1.0f));
    Normal = vec3(ViewMatrix * ModelMatrix * vec4(Normals, 1.0));
    
    for(int i = 0; i < numberOfLights; i++)
    {
      LightPosView[i] = vec3(ViewMatrix * vec4(lightPos[i], 1.0f)) ;
    }
    
    vec3 FragTangent = vec3(ViewMatrix * ModelMatrix * vec4(normalize(tangents), 1.0));
    vec3 FragBiTangent = vec3(ViewMatrix * ModelMatrix * vec4(normalize(bitangents), 1.0));
    TBN =  (mat3 (FragTangent, FragBiTangent, Normalsw));
    
    EyeDirection_tanSpace = TBN *( vec3(0,0,0) - FragPos);
    
    for(int i = 0; i < numberOfLights; i++)
    {
      vec3 lightposition_cameraspace = ( ViewMatrix * vec4(lightPos[i],1)).xyz;
      LightDirection[i] = TBN * (lightposition_cameraspace + (vec3(0,0,0) - FragPos));
     }

    vUV = UV;
}