#version 330

in vec3 color;
in vec3 fragNormal;
in vec3 world_pos;
out vec4 finalColor;

//uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() 
{
  //ambient
  float ambientIntensity = 0.2f;
  vec3 ambient = ambientIntensity * lightColor;
  
  
  //normalize Normals and distance from LightDirection
  vec3 norm = normalize(fragNormal);
  vec3 lightDir = normalize(lightPos - world_pos);  
  
  
  //diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  //specular intensity
  float specularStrength = 0.6f;
  
  //distance between object and camera
  vec3 viewDir = normalize(cameraPos - world_pos);
  //get reflection vector, negate lightDir cause we need the light the other way around
  vec3 reflectDir = reflect(-lightDir, norm);  
  
  //dot product between viewDir and ReflectDir shouldn't be negate, then take it to the bower of the shininess factor
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
  vec3 specular = specularStrength * spec * lightColor;  
  
  
  vec3 result = (ambient + diffuse + specular) * color;
  finalColor = vec4(result, 1.0f);
  
  
  //see normals for debug
  //finalColor = vec4(fragNormal, 1.0f);
    
}