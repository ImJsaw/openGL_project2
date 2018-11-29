#version 410 core

layout(location = 0) out vec4 fragColor;

uniform vec4 drawColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main()
{
	fragColor = drawColor;
}