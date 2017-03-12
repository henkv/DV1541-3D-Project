#version 440 core
layout(location = 0) in vec3 fragment_position;
layout(location = 1) in vec3 fragment_normal;
layout(location = 2) in vec3 frag_color;
layout(location = 3) in vec4 fragment_posLightSpace;

out vec4 fragment_color;

layout(location = 4) uniform sampler2D depthMap;

vec3 ambient_light = vec3(0.5f, 0.5f, 0.5f);
vec3 light_position = vec3(0.f, 0.f, -100.f);
vec3 light_color = vec3(1.f, 1.f, 1.f);

vec4 ambient()
{
  return vec4(ambient_light, 1.f);
}

vec4 diffuse()
{
  vec4 diffuseColor = vec4(light_color, 1.f);
  vec3 lightDir = normalize(light_position - fragment_position);

  return diffuseColor * max(dot(fragment_normal, lightDir), 0.f);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = fragment_posLightSpace.xyz / fragment_posLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	//bias
	vec3 lightDir = normalize(light_position - fragment_position);
	float bias = max(0.05 * (1.0 - dot(fragment_normal, lightDir)), 0.005);  

    // Check whether current frag pos is in shadow
	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 

	// PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
	
	if(projCoords.z > 1.0)
	{
	   shadow = 0.0;
	}
      

    return shadow;
}

void main()
{
	float shadow = ShadowCalculation(fragment_posLightSpace);   
	
	//hard coded color on bacground 
	fragment_color = vec4(0.5f, 0.5f, 0.5f, 1.0f); //vec4(frag_color, 1.0f); //vec4(abs(normalize(fragment_position)), 1.f);
	fragment_color *= (ambient() + (1.0 - shadow) * diffuse()); //(ambient() + (1.0 - shadow) * (diffuse() + specular))
}


//#version 330 core
//out vec4 FragColor;
//
//in VS_OUT {
//    vec3 FragPos;
//    vec3 Normal;
//    vec2 TexCoords;
//    vec4 FragPosLightSpace;
//} fs_in;
//
//uniform sampler2D diffuseTexture;
//uniform sampler2D shadowMap;
//
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//
//float ShadowCalculation(vec4 fragPosLightSpace)
//{
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // Transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xy).r; 
//    // Get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // Check whether current frag pos is in shadow
//	  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
//	  float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
//
//    return shadow;
//}
//
//void main()
//{           
//    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
//    vec3 normal = normalize(fs_in.Normal);
//    vec3 lightColor = vec3(1.0);
//    // Ambient
//    vec3 ambient = 0.15 * color;
//    // Diffuse
//    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * lightColor;
//    // Specular
//    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//    float spec = 0.0;
//    vec3 halfwayDir = normalize(lightDir + viewDir);  
//    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//    vec3 specular = spec * lightColor;    
//    // Calculate shadow
//    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);       
//    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
//    
//    FragColor = vec4(lighting, 1.0f);
//}