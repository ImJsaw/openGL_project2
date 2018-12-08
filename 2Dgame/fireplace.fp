#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform int seq; 

uniform sampler2DArray firePic;

void main(){    
	
	if( texture( firePic, vec3(TexCoord, seq)).a < 0.05 ) discard;
	FragColor = texture(firePic, vec3(TexCoord, seq) );
	

}