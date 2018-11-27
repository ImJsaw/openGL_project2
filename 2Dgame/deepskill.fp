#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

flat in int skillImg;

uniform sampler2D twinsflame;
uniform sampler2D firedragon;
uniform sampler2D julianColumn;

void main(){    
	if(skillImg == 0){
		if(texture(twinsflame, TexCoord).a < 0.1) discard;
		FragColor = texture(twinsflame, TexCoord);
	}
	else if(skillImg == 1){
		if(texture(firedragon, TexCoord).a < 0.1) discard;
		FragColor = texture(firedragon, TexCoord);
	}
	else if(skillImg == 2){
		if(texture(julianColumn, TexCoord).a < 0.1) discard;
		FragColor = texture(julianColumn, TexCoord);
	}
}