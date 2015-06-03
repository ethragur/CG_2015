#version 330

in vec3 color;
in vec3 fragNormal;
in vec3 world_pos;
out vec4 finalColor;

//uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() 
{
  float ambientIntensity = 0.3f;
  vec3 ambient = ambientIntensity * lightColor;
  
  
  vec3 norm = normalize(fragNormal);
  vec3 lightDir = normalize(lightPos - world_pos);  
  
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  
  
  vec3 result = (ambient + diffuse) * color;
  finalColor = vec4(result, 1.0f);
  
  
  //see normals for debug
  //finalColor = vec4(fragNormal, 1.0f);
    
}