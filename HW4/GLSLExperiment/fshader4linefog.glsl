#version 150

in  vec4  interpolatedColor;
out vec4  fColor;

varying float fogFactor;
void main() 
{ 
//vec4 shaderColor = vec4(0.4,0.6,0.0,1.0);
vec4 fogColor = vec4(1.0,1.0,1.0,1.0);
vec4 color = mix(fogColor,interpolatedColor,fogFactor);
fColor = color;
} 

