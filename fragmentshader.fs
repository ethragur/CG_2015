#version 330

in vec3 fragNormal;
in vec3 world_pos;
in vec3 world_posM;
out vec4 finalColor;

uniform vec3 objectColor;

const int numberOfLights = 4;
uniform vec3 lightPos[numberOfLights];
uniform vec3 lightColor[numberOfLights];
uniform float lightIntensity[numberOfLights];

uniform vec3 cameraPos;
uniform vec3 specColor;
uniform float shininess;


uniform bool disableSpec;
uniform bool disableDiff;
uniform bool disableAmbi;


void main() 
{
  vec3 ambient = vec3(0,0,0);
  vec3 diffuse = vec3(0,0,0);
  vec3 specular = vec3(0,0,0);
  vec3 result = vec3(0,0,0);
  
  
  //ambient
  if(!disableAmbi)
  {
    float ambientIntensity = 0.2f;
    ambient = ambientIntensity * vec3(1,1,1);
  }
  result = result + (ambient *objectColor);
  
  vec3 norm = normalize(fragNormal);
  for(int i = 0; i < numberOfLights; i++)
  {
    vec3 lightDirDiff = normalize(lightPos[i] - world_posM); 
    vec3 lightDirSpec = normalize(lightPos[i] - world_pos); 
    //normalize Normals and distance from LightDirection
    
    
    float diff = max(dot(norm, lightDirDiff), 0.0);
    if(!disableDiff)
    {
      //diffuse   
      diffuse = diff * lightColor[i];
    }
  
    if(!disableSpec)
    { 
      //specular intensity
      float specularStrength = 16.0f;
    
      float spec;
      //fix so that specular is only shown on the right side 
      if(diff > 0.0)
      {
	//distance between object and camera
	vec3 viewDir = normalize(  world_pos - cameraPos);
	//get reflection vector, negate lightDir cause we need the light the other way around
	vec3 reflectDir = reflect(-lightDirSpec, norm);
	//dot product between viewDir and ReflectDir shouldn't be negative, then take it to the power of the shininess factor
	spec = pow(clamp(dot(viewDir, reflectDir),0.0,1.0), shininess);
      }
      else
      {
	spec = 0.0;
      }
      
      specular = specularStrength *  spec * specColor * lightColor[i]  ;  
      
    }
    result = result + (specular + diffuse) * objectColor * lightIntensity[i];
  }

  
     finalColor = vec4(result, 1.0f);
  //see normals for debug
 //     finalColor = vec4(norm, 1.0f);
    
}