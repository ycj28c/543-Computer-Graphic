#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 look_matrix;

const float iorefr = 0.99;

in vec4 vPosition;
in vec4 vNormal;
out vec3 T;

void main() 
{
  gl_Position = projection_matrix*look_matrix*model_matrix*vPosition;
  //should be pos vPosition, but doesnt work
  vec4 eyePos = vPosition;
  vec4 NN = look_matrix*vNormal;
  vec3 N = normalize(NN.xyz);
  T = refract(eyePos.xyz, N,iorefr);
} 
