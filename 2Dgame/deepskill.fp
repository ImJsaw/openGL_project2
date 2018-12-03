#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

flat in int skillImg;
flat in int skill_x;
flat in int skill_y;


//uniform sampler2D twinsflame;
//uniform sampler2D firedragon;
//uniform sampler2D julianColumn;




uniform sampler2DArray skillPic;

void main(){    
	if(skillImg == 0){
		if(texture(skillPic, vec3(TexCoord, 4 * (skill_y-1) + (skill_x-1))).a < 0.1) discard;
		FragColor = texture(skillPic, vec3( TexCoord, 4 * (skill_y-1) + (skill_x-1) ) );
	}
	else if(skillImg == 1){
		if(texture(skillPic, vec3(TexCoord, 2 * (skill_y-1) + (skill_x-1))).a < 0.1) discard;
		FragColor = texture(skillPic, vec3(TexCoord, 2 * (skill_y-1) + (skill_x-1)));
	}
	else if(skillImg == 2){
		if(texture(skillPic, vec3(TexCoord, 4 * (skill_y-1) + (skill_x-1))).a < 0.1) discard;
		FragColor = texture(skillPic, vec3(TexCoord, 4 * (skill_y-1) + (skill_x-1)));
	}
	

}