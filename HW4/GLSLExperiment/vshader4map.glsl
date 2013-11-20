#version 150

uniform mat4 projection;
uniform mat4 ModelView;

in vec4 vPosition;
in vec4 Normal;
out vec3 R;

void main() 
{
  gl_Position = projection*ModelView*vPosition;
  vec3 eyePos = vPosition.xyz;
  vec4 NN = ModelView*Normal;
  vec3 N = normalize(NN.xyz);
  R = reflect(eyePos, N);
} 
