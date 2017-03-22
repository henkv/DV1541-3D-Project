#version 440 core
layout (triangles, equal_spacing, ccw) in;

layout(location = 0) in vec3 PatchWorldPosition[];
layout(location = 1) in vec2 PatchTexCoord[];
layout(location = 2) in vec3 PatchNormal[];

layout(location = 0) out vec3 WorldPosition;
layout(location = 1) out vec2 TexCoord;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 ViewPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform sampler2D diffuseMap;

vec2 interpolate(vec2 i, vec2 j, vec2 k)
{
  return vec2(gl_TessCoord.x) * i + vec2(gl_TessCoord.y) * j + vec2(gl_TessCoord.z) * k;
}

vec3 interpolate(vec3 i, vec3 j, vec3 k)
{
  return vec3(gl_TessCoord.x) * i + vec3(gl_TessCoord.y) * j + vec3(gl_TessCoord.z) * k;
}

float rgb2luma(vec3 rgb){
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

void main()
{
  WorldPosition = interpolate(PatchWorldPosition[0], PatchWorldPosition[1], PatchWorldPosition[2]);
  TexCoord = interpolate(PatchTexCoord[0], PatchTexCoord[1], PatchTexCoord[2]);
  Normal = interpolate(PatchNormal[0], PatchNormal[1], PatchNormal[2]);

  float displacement = rgb2luma(texture(diffuseMap, TexCoord).rgb);
  WorldPosition += Normal * displacement * 0.05;


  ViewPosition = vec3(viewMatrix * vec4(WorldPosition, 1));
  gl_Position = projectionMatrix * vec4(ViewPosition, 1);
}
