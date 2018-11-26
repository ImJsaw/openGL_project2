#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

flat in int skillImg;

uniform sampler2D twinsflame;
//uniform sampler2D deep_1;
//uniform sampler2D deep_2;


void main()
{    
	
	if(skillImg == 0){
		if(texture(twinsflame, TexCoord).a < 0.1){
			discard;
		}
		FragColor = texture(twinsflame, TexCoord);
	}
	/*else if(deepImg == 2){
		if(texture(deep_2, TexCoord).a < 0.1){
			discard;
		}
		FragColor = texture(deep_2, TexCoord);
	}
	else if(deepImg == 1){
		if(texture(deep_1, TexCoord).a < 0.1){
			discard;
		}
		FragColor = texture(deep_1, TexCoord);
	}*/
	
}