#version 150

in  vec4  interpolatedColor;
out vec4  fColor;

varying float fogFactor;
void main() 
{ 
vec3 fogColor    = vec3(1.0,1.0,1.0);
//vec3 finalColor = vec3(1.0,0.4,0.6);
vec3 finalColor = interpolatedColor.xyz;
vec3 ccolor = mix(fogColor, finalColor, fogFactor );
//fColor     = mix(fogColor, interpolatedColor, fogFactor );
fColor = vec4(ccolor,1.0);
//fColor = interpolatedColor;
} 

