#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

flat in int skillImg;

uniform sampler2D twinsflame;


void main(){    
	if(skillImg == 0){
		if(texture(twinsflame, TexCoord).a < 0.1) discard;
		FragColor = texture(twinsflame, TexCoord);
	}
}