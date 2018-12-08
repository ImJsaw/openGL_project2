#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Color; // ¤p¦a¹Ï
uniform sampler2D screenTexture;
uniform sampler2D magic;


uniform float time;
uniform vec4 color; // ¿Ã¹õº¥·t
const float offset = 1.0 / 300.0; // from learnOpenGL


void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0) * color;

	if(Color[0] == 0.0){
		vec4 mask = texture(magic, TexCoords);
		FragColor = mix(FragColor, mask, 0.5);
		if(FragColor.a < 0.503) discard;
		FragColor *= color;
	}
	
	
}