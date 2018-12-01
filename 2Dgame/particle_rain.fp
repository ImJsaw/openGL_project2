#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;
uniform float time;
uniform sampler2D sprite;

void main()
{
	
    color = (texture(sprite, TexCoords) * ParticleColor);
	color.rgb += 0.4;
	if((color).a < 0.1){
		discard;
	}
}  
