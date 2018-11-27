#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D back;

void main()
{    
	FragColor = texture(back, TexCoord);
}
