#version 150

uniform mat4 projection_matrix;
uniform mat4 look_matrix;

in  vec4 vPosition;
in  vec4 vTexCoord;
out vec2 texCoord;

void main() 
{
  texCoord = vTexCoord.st;
  gl_Position = projection_matrix*look_matrix*vPosition;
} 
