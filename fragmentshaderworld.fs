#version 330

in vec3 Normal;
in vec3 FragPos;
in vec2 vUV;
in mat3 TBN;

uniform sampler2D texSampler;
uniform sampler2D normTexSampler;
uniform vec3 objectColor;

const int numberOfLights = 3;
uniform vec3 lightPos[numberOfLights];
uniform vec3 lightColor[numberOfLights];
uniform float lightIntensity[numberOfLights];
uniform vec3 cameraPos;

uniform vec3 specColor;
uniform float shininess;

uniform bool disableSpec;
uniform bool disableDiff;
uniform bool disableAmbi;
uniform bool disableNormalMapping;

out vec4 finalColor;

void main() 
{
  
  vec3 ambient = vec3(0,0,0);
  vec3 diffuse = vec3(0,0,0);
  vec3 specular = vec3(0,0,0);
  vec3 result = vec3(0,0,0);
  
  
  vec3 texColor = texture(texSampler, vUV.st).rgb;
  float texAlpha = texture(texSampler, vUV.st).a;
  //texColor = texColor * texAlpha;
  //ambient
  
  if(texAlpha < 0.1)
  {
    discard;
  }
  else
  {
    if(!disableAmbi)
    {
      float ambientIntensity = 0.1f;
      ambient = ambientIntensity * vec3(1,1,1);
    }
    
    
    result = result + (ambient *texColor);
    
    for(int i = 0; i < numberOfLights; i++)
    {
      vec3 n;
      vec3 l;
      
      if(!disableNormalMapping)
      {
	vec3 TextureNormal_tangentspace = normalize( texture2D( normTexSampler, vUV ).rgb*2.0 - 1.0);
	n =  normalize(TBN*TextureNormal_tangentspace);
	l = normalize(lightPos[i] - FragPos);
      }
      else
      {
	n = normalize(Normal);
	l = normalize(lightPos[i] - FragPos);
      }
      
      
      float diff = clamp( dot( n,l ), 0,1 );
      if(!disableDiff)
      {
	//diffuse   
	diffuse = diff * lightColor[i];
      }
      
    
      if(!disableSpec)
      {
	//specular lighting only on surface which face light
	  float specStrength = 8;
	  vec3 viewDir;
	  viewDir = normalize(cameraPos - FragPos);
	  vec3 reflectDir = reflect(-l, n);
	  float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), shininess);
	  specular = specStrength * spec * lightColor[i] * specColor;
	
      }
     // float attenuation = 1.0 / (1.0 + 5.0 * pow(length(LightPosView[i] - FragPos), 2));
      result = result + (diffuse + specular) * texColor * lightIntensity[i];
    
    }
    
    finalColor = vec4(result, texAlpha);
  }
}