#version 410 core

out vec4 fragColor;

in VertexData
{
	vec2 texCoord;
} vertexIn;

uniform sampler2D tex;

void main(void)
{
	fragColor = texture(tex, vertexIn.texCoord);
}