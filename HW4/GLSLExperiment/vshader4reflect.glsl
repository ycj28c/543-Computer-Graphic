#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 look_matrix;

in vec4 vPosition;
in vec4 vNormal;
out vec3 R;

void main() 
{
  gl_Position = projection_matrix*look_matrix*model_matrix*vPosition;
  vec3 eyePos = vPosition.xyz;
  vec4 NN = look_matrix*vNormal;
  vec3 N = normalize(NN.xyz);
  R = reflect(eyePos, N);
} 
