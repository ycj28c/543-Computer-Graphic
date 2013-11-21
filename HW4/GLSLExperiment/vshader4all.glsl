#version 150

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 look_matrix;
uniform bool fogtype;

const float iorefr = 0.59;

in  vec4 vPosition;
in  vec4 vColor;
in  vec4 vTexCoord;
in  vec4 vNormal;
out vec4 interpolatedColor;
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

  if(fogtype){
	float dist = abs(gl_Position.z);
	fogFactor = (10 - dist)/(10 - 0);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
  }
  else{
	const float LOG2 = 1.442695;
	float fogDensity = 0.1; 
	float dist = abs(gl_Position.z); 
	fogFactor = exp2( -fogDensity * fogDensity * dist * dist * LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
  }
} 

