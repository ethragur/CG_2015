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

out vec3 NormalView;
out vec3 NormalWorld;
out vec3 FragPosView;
out vec3 FragPosWorld;
out vec3 LightPosWorld[numberOfLights];
out vec3 LightPosView[numberOfLights];
out vec2 vUV;
out mat3 TBN;

out vec3 LightDirection[numberOfLights];
out vec3 EyeDirection_tanSpace;

//everything is done in viewSpace
void main() 
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1);

    FragPosView = vec3(ViewMatrix * ModelMatrix * vec4(Position, 1.0f));
    NormalView =  mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * Normals;
    FragPosWorld = vec3(ModelMatrix * vec4(Position, 1.0f));
    NormalWorld = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * Normals;
    vUV = UV;
    
    
    
    //calculate the Light Position in view Space
    for(int i = 0; i < numberOfLights; i++)
    {
      LightPosWorld[i] = vec3(ModelMatrix * vec4(lightPos[i], 1.0f));
      LightPosView[i] = vec3(ViewMatrix * vec4(lightPos[i], 1.0f)) ;
    }
    
    vec3 FragTangentView = vec3(ViewMatrix * ModelMatrix * vec4(normalize(tangents), 1.0));
    
    //bitangents are orthogonal to the Tangent and the Normals
    vec3 FragBiTangentView = vec3(cross(FragTangentView, NormalView));
    
    TBN = (mat3 (FragTangentView, FragBiTangentView, NormalView));
    
    
    /*
    ** Second variant of Normal Mapping not used
    EyeDirection_tanSpace = TBN *( vec3(0,0,0) - FragPos);
    
    for(int i = 0; i < numberOfLights; i++)
    {
      LightDirection[i] = TBN * (LightPosView[i] + (vec3(0,0,0) - FragPosView));
    }
    */

    
}