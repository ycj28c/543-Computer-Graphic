#version 150

in  vec4 interpolatedColor;
in  vec4 interpolatedColor2;
in  vec2 texCoord;
in  vec3 R;
in  vec3 T;
in  vec4 position;
out vec4 fColor;

uniform samplerCube texMap;
uniform bool groundOn;
uniform bool refleOn;
uniform bool refraOn;
uniform bool fogtype;
uniform bool drawshadow;
uniform sampler2D texture;

varying float fogFactor;

void main() 
{ 
    vec4 refractcolor;
    vec4 texColor;

	if(refleOn){
		texColor = textureCube(texMap, R);
	    //gl_FragColor = texColor;
		fColor = texColor;
	}
	else if(refraOn){
		vec4 texColor = textureCube(texMap, T);
	    vec4 WHITE = vec4(1.0,1.0,1.0,1.0);
	    vec4 refractcolor = mix(texColor,WHITE,0.3);
	    //gl_FragColor = refractcolor;
		fColor = refractcolor;
	}
	else if(groundOn){
	    fColor = texture2D( texture, texCoord );
	}
	else{
	    fColor = interpolatedColor;
	}

	if(fogtype){
		vec4 fogColor = vec4(1.0,1.0,1.0,1.0);
        vec4 color = mix(fogColor,interpolatedColor2,fogFactor);
        fColor = color;
	}
	else{
	    if(drawshadow){
	        fColor = vec4(0.0,0.0,0.0,1.0);
	    }
        vec3 fogColor = vec3(1.0,1.0,1.0);
	    vec3 finalColor = fColor.xyz;
	    vec3 ccolor = mix(fogColor, finalColor, fogFactor );
	    fColor = vec4(ccolor,1.0);
	}	

} 

