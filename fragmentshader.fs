#version 330

in vec3 NormalView;
in vec3 NormalWorld;
in vec3 FragPosView;
in vec3 FragPosWorld;
in vec2 vUV;
in mat3 TBN;

uniform sampler2D texSampler;
uniform sampler2D normTexSampler;
uniform vec3 objectColor;

const int numberOfLights = 3;
in vec3 LightPosView[numberOfLights];
in vec3 LightPosWorld[numberOfLights];
uniform vec3 lightColor[numberOfLights];
uniform float lightIntensity[numberOfLights];

in vec3 LightDirection[numberOfLights];
in vec3 EyeDirection_tanSpace;

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

  //stop fragment shader when alpha is too low --> faster and texture not shown
  if(texAlpha < 0.1)
  {
    discard;
  }
  else
  {
    //ambient Lighting calculation
    if(!disableAmbi)
    {
      float ambientIntensity = 0.1f;
      ambient = ambientIntensity * vec3(1,1,1);
    }
    
    result = result + (ambient *texColor);
    
    vec3 n;
    vec3 l;
    
    if(!disableNormalMapping)
    {
	vec3 TextureNormal_tangentspace = normalize( texture2D( normTexSampler, vUV ).rgb*2.0 - 1.0);
	n =  normalize(TBN * TextureNormal_tangentspace);
    }
    
    //go through all the Lights and Calculate the Diffuse, Specualar and Normal
    for(int i = 0; i < numberOfLights; i++)
    {      
      l = normalize(LightPosView[i] - FragPosView);
      
      //when Normal Mapping is disabled just take the normal normals
      if(disableNormalMapping)
      {
	n = normalize(NormalView);
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
	  if(diff > 0)
	  {
	    float specStrength = 32;
	    vec3 viewDir = normalize(-FragPosView);
	    vec3 reflectDir = reflect(-l, n);
	    float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), shininess);
	    specular = specStrength * spec * lightColor[i] * specColor;
	  }
	
      }
      //light Distance Factor
      float attenuation = 1.0 / (1.0 + 1.0 * pow(length(LightPosWorld[i] - FragPosWorld), 2));
      result = result + attenuation * (diffuse + specular) * texColor * lightIntensity[i];
    
    }
 
    finalColor = vec4(result, texAlpha);
  }
}