#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 look_matrix;

//uniform vec4 AmbientProduct,DiffuseProduct,SpecularProduct;
//uniform float Shininess;
//uniform maxDist,maxDist;

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vNormal;
out vec4 interpolatedColor;

varying float fogFactor;
void main() 
{
  gl_Position = projection_matrix*look_matrix*model_matrix*vPosition;

  float Shininess = 100;
  vec4 AmbientProduct = vec4(0.2,0.2,0.2,1.0);
  vec4 DiffuseProduct = vec4(1.0,0.8,0.0,1.0);
  vec4 SpecularProduct = vec4(1.0,1.0,1.0,1.0);

  vec4 LightPosition = vec4(50.0,50.0,0.0,1.0);
  vec3 pos = (look_matrix*vPosition).xyz;

  vec3 L = normalize(LightPosition.xyz - pos);
  vec3 E = normalize(-pos);
  vec3 H = normalize(L+E);
  vec3 N = normalize(look_matrix*vec4(vNormal.xyz,0.0)).xyz;

  vec4 ambient = AmbientProduct;
  vec4 diffuse = max(dot(L,N),0.0)*DiffuseProduct;
  vec4 specular = (pow(max(dot(N,H),0.0),Shininess))*SpecularProduct;
  if(dot(L,N)<0.0) specular = vec4(0.0,0.0,0.0,1.0);

  float dist = abs(gl_Position.z);
  //fogFactor = (Fog.maxDist - dist)/(Fog.maxDist - Fog.minDist);
  fogFactor = (20 - dist)/(20 - 0);
  fogFactor = clamp(fogFactor,0.0,1.0);

  //interpolatedColor = vColor;
  interpolatedColor = ambient+diffuse+specular;
  interpolatedColor.a = 1.0;
} 		