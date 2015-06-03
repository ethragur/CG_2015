#version 330

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec3 Normals;

out vec3 fragVert;
out vec3 color;
out vec3 fragNormal;
out vec3 world_pos;

void main() {
    
    color = Color;
    
    //fix when translating objects
    fragNormal = mat3(transpose(inverse(ModelMatrix))) * Normals;
    fragVert = Position;
    //calculate world position of vertexes
    world_pos = vec3(ModelMatrix * vec4(Position, 1.0f));
    
    // Apply all matrix transformations to vert
    gl_Position = ProjectionMatrix* ViewMatrix * ModelMatrix * vec4(Position, 1);
    
    
    
}