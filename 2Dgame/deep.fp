#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

flat in int mariocontroller;
flat in int deepImg;
flat in int deep_x;
flat in int deep_y;


//uniform sampler2D deep_0;
//uniform sampler2D deep_1;
//uniform sampler2D deep_2;

uniform sampler2DArray tex;

uniform float time;
vec2 iResolution = vec2(800., 600.);
void main()
{    
	
	/*if(deepImg == 0){
		if(texture(deep_0, TexCoord).a < 0.1){
			discard;
		}
		FragColor = texture(deep_0, TexCoord);
	}
	else if(deepImg == 2){
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
	if(texture(tex, vec3(TexCoord, (deep_y-1) * 10 + (deep_x-1))).a < 0.1){
		discard;
	}
	FragColor = texture(tex, vec3(TexCoord, (deep_y-1) * 10 + (deep_x-1)));
	
}