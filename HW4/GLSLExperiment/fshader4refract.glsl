#version 150

in vec3 T;

uniform samplerCube texMap;
uniform vec4 fcolor;

void main() 
{ 
	vec4 texColor = textureCube(texMap, T);
	vec4 WHITE = vec4(1.0,1.0,1.0,1.0);
	vec4 refractcolor = mix(texColor,WHITE,0.3);
	gl_FragColor = refractcolor;
} 

