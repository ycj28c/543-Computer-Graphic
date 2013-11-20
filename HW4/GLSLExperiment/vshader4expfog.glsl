#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 look_matrix;

in  vec4 vPosition;
in  vec4 vColor;
out vec4 interpolatedColor;
varying float fogFactor;
void main() 
{
  gl_Position = projection_matrix*look_matrix*model_matrix*vPosition;

  const float LOG2 = 1.442695;
  float fogDensity = 0.1; 
  float dist = abs(gl_Position.z); 
  fogFactor = exp2( -fogDensity * fogDensity * dist * dist * LOG2 );
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  //fogFactor = 0.2;
  interpolatedColor = vColor;
} 		