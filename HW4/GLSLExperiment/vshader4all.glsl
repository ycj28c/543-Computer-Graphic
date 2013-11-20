#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 look_matrix;

const float iorefr = 0.59;

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vTexCoord;
in  vec4 vNormal;
out vec4 interpolatedColor;
out vec4 interpolatedColor2;
out vec2 texCoord;
out vec3 R;
out vec3 T;
out vec4 position ;
varying float fogFactor;

void main() 
{ 
  gl_Position =projection_matrix*look_matrix*model_matrix*vPosition;

  vec4 position = gl_Position;
  vec3 eyePos = vPosition.xyz;
  vec4 NN = look_matrix*model_matrix*vNormal;
  vec3 N = normalize(NN.xyz);
  R = reflect(eyePos, N);
  T = refract(eyePos, N,iorefr);
  interpolatedColor = vColor;
  texCoord = vTexCoord.st;

  const float LOG2 = 1.442695;
  float fogDensity = 0.1; 
  float dist = abs(gl_Position.z); 
  fogFactor = exp2( -fogDensity * fogDensity * dist * dist * LOG2 );
  fogFactor = clamp(fogFactor, 0.0, 1.0);

  float Shininess = 100;
  vec4 AmbientProduct = vec4(0.2,0.2,0.2,1.0);
  vec4 DiffuseProduct = vec4(1.0,0.8,0.0,1.0);
  vec4 SpecularProduct = vec4(1.0,1.0,1.0,1.0);

  vec4 LightPosition = vec4(50.0,50.0,0.0,1.0);
  vec3 pos = (look_matrix*vPosition).xyz;

  vec3 L = normalize(LightPosition.xyz - pos);
  vec3 E = normalize(-pos);
  vec3 H = normalize(L+E);
  vec3 NEWN = normalize(look_matrix*vec4(vNormal.xyz,0.0)).xyz;

  vec4 ambient = AmbientProduct;
  vec4 diffuse = max(dot(L,NEWN),0.0)*DiffuseProduct;
  vec4 specular = (pow(max(dot(NEWN,H),0.0),Shininess))*SpecularProduct;
  if(dot(L,NEWN)<0.0) specular = vec4(0.0,0.0,0.0,1.0);

  interpolatedColor2 = ambient+diffuse+specular;
  interpolatedColor2.a = 1.0;
} 

